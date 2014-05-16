using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace DataConvertor
{
    class Program
    {
        static void Main(string[] args)
        {
            string src = @"C:\TEMP\EURUSD_1999-2013.csv";
            string dest = @"C:\TEMP\EURUSD_1999-2013";
            int counter = 0;
            int day = 0;

            StreamReader srcReader = new StreamReader(src);
            StreamWriter writer = null;

            string line;
            while ((line = srcReader.ReadLine()) != null)
            {
                counter++;
                string[] values = line.Split(',');

                string res = counter.ToString() + ";";

                DateTime time = DateTime.ParseExact(values[0] + "_" + values[1], "yyyy.MM.dd_HH:mm", null);
                time = time.AddHours(1);
                long sec = (long)(new TimeSpan(time.Ticks - new DateTime(1970, 1, 1).Ticks).TotalSeconds);
                res += sec.ToString() + ";" + values[3] + ";" + values[4] + ";" + values[2] + ";" + values[5];

                if (day != time.Day && time.DayOfWeek != DayOfWeek.Saturday)
                {
                    if (writer != null) writer.Close();
                    writer = new StreamWriter(Path.Combine(dest, time.ToString("yyyy.MM.dd") + ".dat"));
                    day = time.Day;
                }

                writer.WriteLine(res);
                writer.Flush();
            }

            try
            {
                writer.Close();
            }
            catch { }
            srcReader.Close();

        }
    }
}
