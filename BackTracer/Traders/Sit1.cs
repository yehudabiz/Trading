using System;
using System.Collections.Generic;

namespace PFY
{
    public class Sit1 : ITrader
    {
        public string Name { get { return "SIT1"; } }
        public string Description { get { return ""; } }
        public short Id { get { return 1; } }
        public Symbol Symbol { get; set; }
        public TradeResolution TradeResolution { get; set; }
        public Dictionary<string, double> Parameters { get { return m_parameters; } }

        private Dictionary<string, double> m_parameters = new Dictionary<string, double>();

        public Sit1()
        {
            m_parameters.Add("SL", 200);
            m_parameters.Add("TP", 30);
            m_parameters.Add("FrequencyHrs", 1);
            m_parameters.Add("", 0);
        }
    }
}