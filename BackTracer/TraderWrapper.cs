using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace PFY
{
    public class TraderWrapper
    {
        private const string dllPath = @"TradeAdvisor.dll";

        [DllImport(dllPath, CharSet = CharSet.None)]
        extern private static void Init(short trader_id, short symbol, short period, double pip, double p1, double p2, double p3, double p4);

        public static void InitTrader(short trader_id, Symbol symbol, TradeResolution period, double pip, Dictionary<string,double> parameters)
        {
            Init(trader_id, (short)symbol, (short)period, pip, parameters.Values.ToList()[0], parameters.Values.ToList()[1],
                parameters.Values.ToList()[2], parameters.Values.ToList()[3]);
        }

        [DllImport(dllPath, CharSet = CharSet.None)]
        extern private static void Deinit();

        public static void DeinitTrader()
        {
            Deinit();
        }

        [DllImport(dllPath, CharSet = CharSet.None)]
        extern private static void UpdatePrice(int time, double high, double low, double open, double close);

        public static void UpdateTraderWithPrice(Bar b)
        {
            int secondsSince1970 = (int)((b.TimeStamp.Ticks - (new DateTime(1970, 1, 1)).Ticks) / 10000000);
            double h = b.High;
            double l = b.Low;
            double o = b.Open;
            double c = b.Close;
            UpdatePrice(secondsSince1970, h, l, o, c);
        }

        [DllImport(dllPath, CharSet = CharSet.None)]
        extern private static void UpdateOrder(int id, short symbol, int open_time, int close_time,
            short order_type, short order_state, double entry_price, double close_price, double pl);
        
        public static void UpdateTraderWithOrder(Order ord)
        {
            int openTime = (int)((ord.OpenTime.Ticks - (new DateTime(1970, 1, 1)).Ticks) / 10000000);
            int closeTime = (int)((ord.CloseTime.Ticks - (new DateTime(1970, 1, 1)).Ticks) / 10000000);
            if (closeTime < 0) closeTime = 0;

            UpdateOrder(ord.GetHashCode(), (short)ord.Symbol, openTime, closeTime, (short)ord.Type, 
                (short)ord.Status, ord.OpenPrice, ord.ClosePrice, ord.Profit);
        }

        [DllImport(dllPath, CharSet = CharSet.None)]
        extern private static void AskAdvise(out int advise, out double sl, out double tp);

        public static Advise AskTrader(out double stopLoss, out double takeProfit)
        {
            int adv = 0;
            AskAdvise(out adv, out stopLoss, out takeProfit);
            return (Advise)adv;
        }
    }
}
