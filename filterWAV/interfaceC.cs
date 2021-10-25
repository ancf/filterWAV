using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace filterWAV
{
    unsafe class interfaceC
    {
        [DllImport("LIBRARYC.dll", EntryPoint = "FilterC", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl )]
        private static extern double FilterC(double a, double b);
   

        public double callCFunction(double a, double b)
        {
            double result = FilterC(a, b);
            return result;
        }
    }
}
