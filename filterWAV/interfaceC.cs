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
        private static extern void FilterC(ref double[] tab, ref double[] copy, long min, long max);
   

        public void callCFunction(ref double[] tab, ref double[] copy, long min, long max)
        {
            Console.WriteLine(tab[0]);
            Console.WriteLine(copy[1]);
            Console.WriteLine(tab[2]);
            FilterC(ref tab, ref copy, min, max);

            Console.WriteLine(tab[0]);
            Console.WriteLine(tab[1]);
            Console.WriteLine(tab[2]);
            // return result;
        }
    }
}
