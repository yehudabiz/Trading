using System;
using System.Collections.Generic;
using System.Drawing;

namespace PFY
{
    public enum TradeResolution
    {
        Ticks =	0,
        Min_1 =	1,
        Min_5 =	5,
        Min_15 = 15,
        Min_30 = 30,
        Hour_1 = 60,
        Hour_4 = 240,
        Day_1 =	1440,
        Week_1 = 10080,
        Month_1 = 43200
    }

    [Flags]
    public enum Advise
    {
        DoNothing = 0,
        Sell = 1,
        Buy = 2,
        CloseLong = 4,
        CloseShort = 8,
        SellContinue = 16,
        BuyContinue = 32,
        TrainingOrder = 64
    }

    public enum OrderType
    {
        Long = 1,
        Short = -1
    }

    public enum OrderStatus
    {
        Open = 1,
        Closed = 2
    }

    public enum Symbol
    {
        EURUSD = 1,
        USDCHF = 2,
        USDJPY = 3
    }
}