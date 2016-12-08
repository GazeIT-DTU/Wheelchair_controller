using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WheelchairLink;
using CommunicationTester;
//using WheelchairLink.CommunicationLink;


namespace CommunicationTester
{
    class Program : CommunicationLink.CommandListener
    {
        public Program()
        {
            CommunicationLink cl = CommunicationLink.GetInstance();
            cl.AddCommandListener(this);
            Console.ReadKey();
            cl.Close();
        }

        

        public void onIncomingCommand(CommunicationLink.Command cmd)
        {
            Console.WriteLine("Test program got command: " + cmd.ToString());
        }



        static void Main(string[] args)
        {
            new Program();

        }
    }
}
