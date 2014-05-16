using System;
using System.Collections.Generic;

namespace PFY
{
    public class Svm1 : ITrader
    {
        public string Name { get { return "Svm 1"; } }
        public string Description { get { return "Based on 3 MA (13, 34, 100) and stochastic"; } }
        public short Id { get { return 8; } }
        public Symbol Symbol { get; set; }
        public TradeResolution TradeResolution { get; set; }
        public Dictionary<string, double> Parameters { get { return m_parameters; } }

        private Dictionary<string, double> m_parameters = new Dictionary<string, double>();

        public Svm1()
        {
            m_parameters.Add("SL", 200);
            m_parameters.Add("TP", 30);
            m_parameters.Add("Svm Mode", 1);
            m_parameters.Add("1-Long, -1-Short", 1);
        }
    }
}