using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WheelchairLink
{
    public class CommunicationLink
    {

        private static CommunicationLink _instance;
        private IncomingDataListener _incomingDataListener;

        private CommunicationLink()
        {
            //Honour the singleton pattern      
            this._incomingDataListener = new IncomingDataListener();
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


        public static CommunicationLink GetInstance()
        {
            if (_instance == null)
                _instance = new CommunicationLink();
            return _instance;
        }

    }
}
