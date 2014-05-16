using System;
using System.Collections.Generic;

namespace PFY
{
    public class Random : ITrader
    {
        public string Name { get { return "Random"; } }
        public string Description { get { return "Random entry in random direction every defined time interval (hours)./n/rExit on defined TP or SL"; } }
        public short Id { get { return 0; } }
        public Symbol Symbol { get; set; }
        public TradeResolution TradeResolution { get; set; }
        public Dictionary<string, double> Parameters { get { return m_parameters; } }

        private Dictionary<string, double> m_parameters = new Dictionary<string, double>();

        public Random()
        {
            m_parameters.Add("SL", 200);
            m_parameters.Add("TP", 30);
            m_parameters.Add("FrequencyHrs", 1);
            m_parameters.Add("", 0);
        }
    }
}