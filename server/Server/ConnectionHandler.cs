using System;
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

        private void Listen()
        {
            while (IsConnected())
            {
                byte[] rawMessage = new byte[BufferSize];
                int byteCount = _connection.Receive(rawMessage);

                if (byteCount > 0)
                {
                    String msg = DecodeMessage(rawMessage).Substring(0, byteCount);
                    Console.WriteLine("{0} : {1}", _connection.RemoteEndPoint, msg);

                    SendResponse(String.Format("Response: {0}", msg));
                }
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