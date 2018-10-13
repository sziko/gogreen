using System;
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Server
{
    public class ConnectionHandler
    {
        private Socket _connection;
        private const int BufferSize = 1024;
        private Thread _thread;
        private readonly String _remoteEndPoint;

        private MemoryStream _buffer = new MemoryStream();

        public ConnectionHandler(Socket connection)
        {
            _connection = connection;
            _remoteEndPoint = _connection.RemoteEndPoint.ToString();
        }

        public void InitializeConnection()
        {
            if (_thread != null)
                return;

            _thread = new Thread(Listen);
            _thread.Start();
        }

        public void ProcessMessage(byte[] message)
        {
            //message contine msajul complet
        }

        //asa vor fi trimise datele si de pe server si de pe arduino
        private void SendMessage(byte[] data)
        {
            int dataLength = data.Length;
            byte[] totalBytes = new byte[4 + dataLength];
            totalBytes[0] = (byte)dataLength;
            totalBytes[1] = (byte)(dataLength >> 8);
            totalBytes[2] = (byte)(dataLength >> 16);
            totalBytes[3] = (byte)(dataLength >> 24);

            for (int i = 0; i < dataLength; i++)
            {
                totalBytes[i + 4] = data[i];
            }

            _connection.Send(totalBytes, 4 + dataLength, SocketFlags.None);
        }

        private void Listen()
        {
            while (IsConnected())
            {
                //4 bytes lungimea mesajului
                //the message bytes
                byte[] rawMessage = new byte[BufferSize];
                int byteCount = _connection.Receive(rawMessage);

                _buffer.Position = _buffer.Length;
                _buffer.Write(rawMessage, 0, byteCount);

                if (_buffer.Length > 4)//daca avem cel putin 4 bytes adica lungimea mesajului
                {
                    _buffer.Position = 0;
                    byte[] lengthBuffer = new byte[4];
                    _buffer.Read(lengthBuffer, 0, 4);//copiem primii 4 bytes din buffer cei care contin lungimea mesajului
                    uint messageLength = (uint)(lengthBuffer[0] | lengthBuffer[1] << 8 | lengthBuffer[2] << 16 | lengthBuffer[3] << 24);
                    //verificam daca am primit tot mesajul
                    if (_buffer.Length - 4 >= messageLength)//-4 pentru ca sarim lungimea mesajului
                    {
                        byte[] message = new byte[messageLength];
                        _buffer.Read(message, 0, (int)messageLength);
                        ProcessMessage(message);
                        int leftBytes = (int)(_buffer.Length - _buffer.Position);
                        byte[] remainingBytes = new byte[leftBytes];
                        _buffer.Read(remainingBytes, 0, leftBytes);
                        _buffer.Position = 0;
                        _buffer.SetLength(0);
                        _buffer.Write(remainingBytes, 0, leftBytes);
                    }//altfel mai trebuie sa primim
                }

                //if (byteCount > 0)
                //{
                //    String msg = DecodeMessage(rawMessage).Substring(0, byteCount);
                //    Console.WriteLine("{0} : {1}", _connection.RemoteEndPoint, msg);

                //    SendResponse(String.Format("Response: {0}", msg));
                //}
            }

            Console.WriteLine("Server: {0} disconnected!", _remoteEndPoint);
            _connection.Close();
            _connection = null;
        }

        private void SendResponse(string response)
        {
            if (_connection != null)
                _connection.Send(EncodeMessage(response));
        }

        private String DecodeMessage(byte[] rawMessage)
        {
            return Encoding.ASCII.GetString(rawMessage);
        }

        private byte[] EncodeMessage(String message)
        {
            return Encoding.ASCII.GetBytes(message);
        }

        private bool IsConnected()
        {
            try
            {
                return !(_connection.Poll(1, SelectMode.SelectRead) && _connection.Available == 0);
            }
            catch (SocketException)
            {
                return false;
            }
        }
    }
}