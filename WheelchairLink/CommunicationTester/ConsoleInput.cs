using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommunicationTester
{
    class ConsoleInput : IControlActionInput
    {
        public ConsoleInput()
        {
            
        }

        /// <summary>
        /// //TODO Maybe start on a new thread
        /// </summary>
        public void Begin()
        {
            Console.WriteLine("Awaiting coordinates. ");
            while (true)
            {
                string input = Console.ReadLine();
                OnControlActionRecieved(input);
            }
        }

        // Event implementation and raising
        event EventHandler ControlActionRecieved;

        event EventHandler IControlActionInput.ControlActionRecieved
        {
            add { ControlActionRecieved += value; }
            remove { ControlActionRecieved -= value; }
        }

        /// <summary>
        /// Raise the event if proper input format
        /// </summary>
        protected virtual void OnControlActionRecieved(string input)
        {
            var res = input.Split(' ');
            if (res.Length != 2)
            {
                Console.WriteLine("Wrong format. Should be \"<int> <int>\" only. Example \"30 50\", with no whitespaces");
                return;
            }

            int x = 0;
            int y = 0;
            bool isParseSucces = int.TryParse(res[0], out x) && int.TryParse(res[1], out y);
            if (!isParseSucces)
            {
                Console.WriteLine("Wrong format. Should be \"<int> <int>\" only. Example \"30 50\", with no whitespaces");
                return;
            }
            EventArgs args = new ControlActionEventArgs( x, y );
            ControlActionRecieved?.Invoke(this, args);
            Console.WriteLine("Input accepted. Raised an event");
        }


        

    }
}
