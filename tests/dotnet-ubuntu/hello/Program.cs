using System;
using System.Diagnostics;

namespace hello
{
    class Program
    {
        static void nullstrlen(string s)
        {
            try {
                int len = s.Length;
            } catch (NullReferenceException e) {
                Console.WriteLine("Exception caught: {0}", e);
            }
        }
        static void Main(string[] args)
        {
            int q, a = 5, b = 0;
            try {
                q = a / b;
            } catch (DivideByZeroException e) {
                Console.WriteLine("Exception caught: {0}", e);
            }

            string s = null;
            nullstrlen(s);

            string s1 = null;
            nullstrlen(s1);

            // unreachable
            //Environment.Exit(1);
        }  
    }
}
