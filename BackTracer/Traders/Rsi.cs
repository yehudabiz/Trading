using System;
using System.Collections.Generic;

namespace PFY
{
    public class Rsi : ITrader
    {
        public string Name { get { return "RSI"; } }
        public string Description { get { return "RSI indicator based trader"; } }
        public short Id { get { return 11; } }
        public Symbol Symbol { get; set; }
        public TradeResolution TradeResolution { get; set; }
        public Dictionary<string, double> Parameters { get { return m_parameters; } }

        private Dictionary<string, double> m_parameters = new Dictionary<string, double>();

        public Rsi()
        {
            m_parameters.Add("SL", 200);
            m_parameters.Add("TP", 30);
            m_parameters.Add("Period", 14);
            m_parameters.Add("", 0);
        }
    }
}