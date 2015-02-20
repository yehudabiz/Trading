using System;
using System.Collections.Generic;
using System.Drawing;

namespace PFY
{
    public class Order
    {
        public DateTime OpenTime { get; set; }
        public DateTime CloseTime { get; set; }
        public OrderType Type { get; set; }
        public Symbol Symbol { get; set; }
        public double OpenPrice { get; set; }
        public double ClosePrice { get; set; }
        public double Volume { get; set; }
        public double TakeProfit { get; set; }
        public double StopLoss { get; set; }
        public int Timeout { get; set; } //seconds
        public OrderStatus Status { get; set; }

        public Order()
        {
            Status = OrderStatus.Open;
        }

        public Order(OrderType typ, DateTime time, double price, double vol, double sl, double tp, int to)
        {
            OpenTime = time;
            Type = typ;
            OpenPrice = price;
            Volume = vol;
            TakeProfit = tp;
            StopLoss = sl;
            Timeout = to;
            Status = OrderStatus.Open;
        }

        public double Profit
        {
            get
            {
                if (ClosePrice <= 0)
                {
                    return 0;
                }
                else
                {
                    return (ClosePrice - OpenPrice) * Volume * (int)Type;
                }
            }
        }

        public bool IsToClose(double price, DateTime now)
        {
            if ((now.Ticks - OpenTime.Ticks) / 10000000 > Timeout) return true;
            if (StopLoss != 0 && price * (int)Type < StopLoss * (int)Type)
            {
                return true;
            }
            if (TakeProfit != 0 && price * (int)Type > TakeProfit * (int)Type)
            {
                return true;
            }
            return false;
        }
    }
}