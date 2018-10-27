using System;
using System.Net.Sockets;
using System.Threading;

namespace Server
{
    public class ServerCore : ServerSocket
    {
        private Thread _thread;
        private static readonly ServerCore instance = new ServerCore();

        public static ServerCore Instance
        {
            get { return instance; }
        }

        private ServerCore()
        {
        }

        public void CreateServer(int port)
        {
            CreateSocket(port);
            _thread = new Thread(ListenForConnections);
            _thread.Start();
        }

        public void StopServer()
        {
            CloseSocket();
        }

        private void ListenForConnections()
        {
            Console.WriteLine("Server: Waiting for a connection...");
            while (true)
            {
                try
                {
                    Socket socket = AcceptConnection();
                    
                    if (socket == null)
                    {
                        break;
                    }

                    Console.WriteLine("Server: Accepted connection from " + socket.RemoteEndPoint);
                    
                    ConnectionHandler connection = new ConnectionHandler(socket);
                    connection.InitializeConnection();


                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
            }
            
            Console.WriteLine("Server stopped!");
        }
    }
}