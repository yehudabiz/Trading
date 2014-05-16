using System;
using System.Collections.Generic;

namespace PFY
{
    public class Balanced : ITrader
    {
        public string Name { get { return "Balanced"; } }
        public string Description { get { return "Holds two oposite open orders. The third entry is in more profit direction every defined time interval (hours)./n/rOnly 3 open orders permitted. Exit on defined TP or SL"; } }
        public short Id { get { return 7; } }
        public Symbol Symbol { get; set; }
        public TradeResolution TradeResolution { get; set; }
        public Dictionary<string, double> Parameters { get { return m_parameters; } }

        private Dictionary<string, double> m_parameters = new Dictionary<string, double>();

        public Balanced()
        {
            m_parameters.Add("SL", 200);
            m_parameters.Add("TP", 30);
            m_parameters.Add("FrequencyHrs", 1);
            m_parameters.Add("", 0);
        }
    }
}