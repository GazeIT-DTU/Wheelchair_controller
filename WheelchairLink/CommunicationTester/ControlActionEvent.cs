using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommunicationTester
{
    public interface IControlActionInput
    {
        event EventHandler ControlActionRecieved;
    }

    class ControlActionEventArgs : EventArgs
    {
        public ControlActionEventArgs(int x, int y)
        {
            X = x;
            Y = y;
        }
        public int X { get; set; }
        public int Y { get; set; }
    }

    //class ControlActionEvent
    //{
    //    public event EventHandler ControlActionRecieved;

    //    protected virtual void OnControlActionRecieved(EventArgs e)
    //    {
    //        ControlActionRecieved?.Invoke(this, e);
    //    }

    //}
}
