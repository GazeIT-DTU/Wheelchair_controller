using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace WheelchairLink
{
    public class IncomingDataListener
    {
        private const char END_CHAR = ';';

        /*private Dictionary<char, Delegate> commands = new Dictionary<char, Delegate>() {
            { 'd', {Console.WriteLine("ASD")} 
             }; 
             */
        private SerialPort _serialPort;
        private bool _keepReading = false;
        private Thread _readThread;

        
        public IncomingDataListener()
        {
            _readThread = new Thread(Read);
            //Find correct port
            //EstablishComminucation_Auto();
            EstablishComminucation("COM3", 9600, Parity.None, 8, StopBits.One);
            //Ask wheelchair to stop
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

            Console.WriteLine("Starting ReadThread");

            _keepReading = true;
            _readThread.Start();


            return false;
        }

        private void Read()
        {
            if (this._serialPort != null && this._serialPort.IsOpen)
            {
                Console.WriteLine("Listening for incoming data...");
                while (_keepReading)
                    Console.Out.WriteLine(this._serialPort.ReadLine());
            }
        }




    }






}
