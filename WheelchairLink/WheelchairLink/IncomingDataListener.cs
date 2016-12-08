using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace WheelchairLink
{
    public class IncomingDataListener
    {
        public interface DataListener
        {
            void onIncomingData(String str);
        }

        private const char END_CHAR = ';';
        private const char HANDSHAKE_REQUEST = '?';
        private const char HANDSHAKE_RESPONSE = '!';

        /*private Dictionary<char, Delegate> commands = new Dictionary<char, Delegate>() {
            { 'd', {Console.WriteLine("ASD")} 
             }; 
             */
        private SerialPort _serialPort;
        private volatile bool _keepReading = false;
        private Thread _readThread;
        private HashSet<DataListener> listeners = new HashSet<DataListener>();
        
        public IncomingDataListener()
        {
            _readThread = new Thread(Read);
            //Find correct port
            if (!EstablishComminucation_Auto())
                Console.WriteLine("Uable to auto-detect COM port... ");
            /*if (!EstablishComminucation("COM5", 9600, Parity.None, 8, StopBits.One))
                Console.WriteLine("Not that port...");
            else
                Console.WriteLine("Yaaah");*/


            //TODO Ask wheelchair to stop?
        }


        private bool EstablishComminucation_Auto()
        {
            String[] portNames = SerialPort.GetPortNames();
            foreach (String name in portNames)
                if (EstablishComminucation(name, 9600, Parity.None, 8, StopBits.One))
                    return true;

            return false;
        }

        private bool EstablishComminucation(String portname, int baudRate, Parity parity, int dataBits, StopBits stopBits)
        {
            Console.WriteLine("Establishing link to: " + portname);
            if (_serialPort != null && _serialPort.IsOpen)
                _serialPort.Close();

            this._serialPort = new SerialPort(portname, baudRate, parity, dataBits, stopBits);

            //this._serialPort.ReadTimeout = 500;
            //this._serialPort.WriteTimeout = 500;

            this._serialPort.Open();
            this._serialPort.ReadExisting(); //Empty queue...
            this._serialPort.Write(HANDSHAKE_REQUEST + "" + END_CHAR); //Request handshake

            Stopwatch sw = new Stopwatch();
            sw.Start();
            String response = "";
            while(sw.ElapsedMilliseconds < 3000) //Wait maximum 3 seconds for the response
            {
                String temp = _serialPort.ReadExisting();
                response += temp;
                if(temp.IndexOf(END_CHAR) > -1){
                    //int i = response.IndexOf(END_CHAR);
                    String msg = getNextMsg(ref response);
                    Console.Out.WriteLine("Got response: " + msg);
                    if (msg.Equals(HANDSHAKE_RESPONSE + ""))
                    {
                        Console.WriteLine("Found Wheelchar controller - Starting ReadThread...");
                        _keepReading = true;
                        _readThread.Start();
                        return true;
                    }

                }
            }
            Console.WriteLine("Response: " + response);

            return false;
        }

        public void Close()
        {
            this._keepReading = false;
        }

        private String getNextMsg(ref String str)
        {
            int i = str.IndexOf(END_CHAR);
            if (i < 0)
            {
                return null;
            }

            String res = str.Substring(0, i);
            str = str.Substring(i);


            return res;
        }

        private void OnIncomingData(String data)
        {
            foreach (DataListener dl in listeners)
                dl.onIncomingData(data);
        }

        public bool AddDataListener(DataListener dl)
        {
            return listeners.Add(dl);
        }

        public bool RemoveDataListener(DataListener dl)
        {
            return listeners.Remove(dl);
        }

        private void Read()
        {
            if (this._serialPort != null && this._serialPort.IsOpen)
            {
                Console.WriteLine("Listening for incoming data...");

                String str = "";
                String temp;
                while (_keepReading)
                {
                    temp = this._serialPort.ReadExisting();
                    if (!String.IsNullOrEmpty(temp)) {
                        str += temp;
                        int index = temp.IndexOf(END_CHAR);
                        if (index > -1)
                        {
                            OnIncomingData(str.Substring(0, index));
                            str = str.Substring(index + 1);
                        }


                    }
                    
                   // Console.Out.WriteLine(this._serialPort.ReadExisting());
                }
            }
        }




    }






}
