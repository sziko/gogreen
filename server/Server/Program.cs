using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Server
{
    internal class Program
    {
        private static void ServerBeacon()
        {
            UdpClient client = new UdpClient();
            IPEndPoint ip = new IPEndPoint(IPAddress.Broadcast, 4210);
            byte[] bytes = { 0, 1, 2 };

            while (true)
            {
                client.Send(bytes, bytes.Length, ip);
                Console.WriteLine("Sent server package!");
                Thread.Sleep(5000);
            }

            client.Close();
        }

        public static void Main(string[] args)
        {
            ServerCore.Instance.CreateServer(6655);
            ServerBeacon();
        }
    }
}