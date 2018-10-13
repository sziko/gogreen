namespace Server
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            ServerCore.Instance.CreateServer(80);   
        }
    }
}