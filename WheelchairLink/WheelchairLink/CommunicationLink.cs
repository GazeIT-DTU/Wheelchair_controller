using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WheelchairLink
{
    public class CommunicationLink : IncomingDataListener.DataListener
    {
        public enum Command
        {
            Dummy,
            Dummy2
        }

        private Dictionary<char, Command> char2Command = new Dictionary<char, Command>() {
            { 'd', Command.Dummy },
            { 'e', Command.Dummy2 },
        };

        public interface CommandListener
        {
            void onIncomingCommand(Command cmd);
        }
        HashSet<CommandListener> cmdListeners = new HashSet<CommandListener>();

        private static CommunicationLink _instance;
        private IncomingDataListener _incomingDataListener;

        public event EventHandler IncomingCommand;

        private CommunicationLink()
        {
            //Honour the singleton pattern      
            this._incomingDataListener = new IncomingDataListener();
            _incomingDataListener.AddDataListener(this);
        }


        public void Drive(int leftSpeed, int rightSpeed)
        {

        }

        public void Stop()
        {

        }

        public void Brake(int brakePower)
        {

        }


        public void Close()
        {
            _incomingDataListener.Close();
        }

        void IncomingDataListener.DataListener.onIncomingData(String str)
        {
            Command cmd;
            bool found = char2Command.TryGetValue(str[0], out cmd);
            if (found)
            {
                foreach (CommandListener cl in cmdListeners)
                    cl.onIncomingCommand(cmd);
            }
        }

        public bool AddCommandListener(CommandListener cl)
        {
            return this.cmdListeners.Add(cl);
        }

        public bool RemoveCommandListener(CommandListener cl)
        {
            return this.cmdListeners.Remove(cl);
        }

        public static CommunicationLink GetInstance()
        {
            if (_instance == null)
                _instance = new CommunicationLink();
            return _instance;
        }

    }
}
