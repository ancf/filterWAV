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
        private static extern void FilterC(ref double[] tab, long min, long max);
   

        public void callCFunction(ref double[] tab, long min, long max)
        {
            FilterC(ref tab, min, max);
           // return result;
        }
    }
}
