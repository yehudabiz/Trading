using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace RightPercent
{
    class Program
    {
        static void Main(string[] args)
        {
            string f1 = args[0];
            string f2 = args[1];
            int allAdvises = 0;
            int rightAdvises = 0;

            Stream s1 = File.Open(f1, FileMode.Open);
            StreamReader r1 = new StreamReader(s1);
            Stream s2 = File.Open(f2, FileMode.Open);
            StreamReader r2 = new StreamReader(s2);
            string l1, l2;
            while ((l1 = r1.ReadLine()) != null)
            {
                if ((l2 = r2.ReadLine()) != null)
                {
                    int cl1 = Convert.ToInt32(l1.Split(' ')[0]);
                    int cl2 = Convert.ToInt32(l2.Split(' ')[0]);
                    if (cl1 == 1)
                    {
                        allAdvises++;
                        if (cl2 == 1) rightAdvises++;
                    }
                }
            }

            r1.Close();
            r2.Close();

            Console.WriteLine("All advises: " + allAdvises);
            Console.WriteLine("Right advises: " + rightAdvises);
            Console.WriteLine("Success percentage: " + (int)(((float)rightAdvises / (float)allAdvises) * 100) + "%");
        }
    }
}
