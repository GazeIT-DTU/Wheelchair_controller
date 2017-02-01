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
    class WheelchairControl : CommunicationLink.CommandListener
    {
        private CommunicationLink cl;
        public WheelchairControl()
        {
            // Create serial link and listen
            cl = CommunicationLink.GetInstance();
            cl.AddCommandListener(this);

            // Create and listen. Input from console. Should support multiple input sources at the same time via events. 
            // Fx. Create a class for the eye tribe.
            IControlActionInput cae = new ConsoleInput();
            cae.ControlActionRecieved += OnIncomingControlAction;
            ((ConsoleInput) cae).Begin();
        }

        

        public void onIncomingCommand(CommunicationLink.Command cmd)
        {
            Console.WriteLine("Test program got command: " + cmd.ToString());
        }

        public void OnIncomingControlAction(object sender, EventArgs e)
        {
            var args = (ControlActionEventArgs) e;
            cl.DriveTowards(args.X, args.Y);
        }


        static void Main(string[] args)
        {
            new WheelchairControl();

        }

    }
}
