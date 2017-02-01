using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WheelchairLink
{
    public class CommunicationLink : SerialLink.DataListener
    {
        public enum Command
        {
            Dummy,
            Dummy2
        }

        private Dictionary<char, Command> char2Command = new Dictionary<char, Command>() {
            { 'g', Command.Dummy },
            { 'e', Command.Dummy2 },
        };

        public interface CommandListener
        {
            void onIncomingCommand(Command cmd);
        }
        HashSet<CommandListener> cmdListeners = new HashSet<CommandListener>();

        private static CommunicationLink _instance;
        private SerialLink _serialLinkDataListener;

        public event EventHandler IncomingCommand;

        private CommunicationLink()
        {
            //Honor the singleton pattern      
            this._serialLinkDataListener = new SerialLink();
            _serialLinkDataListener.AddDataListener(this);
        }

        /// <summary>
        /// Write a drive command to the serial port for the wheelchair to perform. The input should emulate a joystick position in a local coordinate system with the joystick in 0,0
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        public void DriveTowards(int x, int y)
        {
            var command = "d" + x.ToString() + "," + y.ToString() + ";";   // Ex. "10,20;"
            _serialLinkDataListener.Write(command);
        }


        public void Stop()
        {
            //TODO implement special stop command
            var command = "0,0;";   // Ex. "10,20;"
            _serialLinkDataListener.Write(command);
        }

        public void Brake(int brakePower)
        {
            throw new NotImplementedException();
            // Originally there was no brake active on the chair. The brake on there is a "parking brake". 
            // There is a lot of "drag" from the motors when not applied power, that slows it down
        }


        public void Close()
        {
            _serialLinkDataListener.Close();
        }

        void SerialLink.DataListener.onIncomingData(String str)
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
