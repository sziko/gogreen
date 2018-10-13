using System;
using System.Net;
using System.Net.Sockets;

namespace Server
{
    public class ServerSocket
    {
        private Socket _socket;

        protected void CreateSocket(int port)
        {
            if (_socket != null)
            {
                throw new Exception("Socket already exists!");
            }
            
            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                _socket.Bind(new IPEndPoint(IPAddress.Any, port));
                _socket.Listen(10);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }

        protected Socket AcceptConnection()
        {
            return _socket != null ? _socket.Accept() : null;
        }

        protected void CloseSocket()
        {
            try
            {
                _socket.Close();
                _socket = null;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }
    }
}