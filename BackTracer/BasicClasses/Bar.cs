using System;
using System.Collections.Generic;
using System.Drawing;

namespace PFY
{
    public class Bar
    {
        public DateTime TimeStamp { get; set; }
        public double Open { get; set; }
        public double Close { get; set; }
        public double High { get; set; }
        public double Low { get; set; }

        public Bar()
        {
        }

        public Bar(long t, double h, double l, double o, double c)
        {
            // t - seconds number since 1/1/1970
            DateTime timeOrigin = new DateTime(1970,1,1);
            TimeStamp = new DateTime(timeOrigin.AddSeconds(t).Ticks);
            High = h;
            Low = l;
            Open = o;
            Close = c;
        }

        public static Bar FromCSV_String(string data)
        {
            string[] ar = data.Split(';');

            long ts = 0;
            long.TryParse(ar[1], out ts);
            if (ts == 0) return null;

            double h = 0;
            double.TryParse(ar[2], out h);
            if (h == 0) return null;

            double l = 0;
            double.TryParse(ar[3], out l);
            if (l == 0) return null;

            double o = 0;
            double.TryParse(ar[4], out o);
            if (o == 0) return null;

            double c = 0;
            double.TryParse(ar[5], out c);
            if (c == 0) return null;

            return new Bar(ts,h,l,o,c);
        }
    }
}