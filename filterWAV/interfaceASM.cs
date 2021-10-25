using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace filterWAV
{
    unsafe class interfaceASM
    {
        [DllImport("libraryASM.dll", CallingConvention = CallingConvention.StdCall)]
        private static extern double filterASM(double a, double b);      


        public double callAsmFunction(double a, double b)  
        {
            double result = filterASM(a, b);
            return result;
        }

    }
}

