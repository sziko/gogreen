using System;
using System.IO;
using System.Net.Sockets;
using System.Threading;

namespace Server
{
    public class ConnectionHandler
    {
        private Socket _connection;
        private Thread _thread;
        private readonly String _remoteEndPoint;

        private MemoryStream _buffer;

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
                byte[] buffer = new byte[1024];
                int received = _connection.Receive(buffer, 1024, SocketFlags.None);

                if (received > 0)
                {
                    Console.WriteLine("Sending back " + received);


                    byte[] servoMessage = new byte[2];
                    servoMessage[0] = 1;
                    servoMessage[1] = 2;
                    _connection.Send(servoMessage, servoMessage.Length, SocketFlags.None);


                    //byte[] servoMessage = new byte[7];
                    //servoMessage[0] = 2;
                    //for (byte i = 0; i < 6; i++)
                    //{
                    //    servoMessage[i + 1] = (byte)(i * 10);
                    //}
                    //_connection.Send(servoMessage, servoMessage.Length, SocketFlags.None);
                }

            }

            Console.WriteLine("Server: {0} disconnected!", _remoteEndPoint);
            _connection.Close();
            _connection = null;
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