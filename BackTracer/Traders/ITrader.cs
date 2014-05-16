using System;
using System.Collections.Generic;

namespace PFY
{
    public interface ITrader
    {
        string Name { get; }
        string Description { get; }
        short Id { get; }
        Symbol Symbol { get; set; }
        TradeResolution TradeResolution { get; set; }
        Dictionary<string, double> Parameters { get; }
    }
}