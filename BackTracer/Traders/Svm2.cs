using System;
using System.Collections.Generic;

namespace PFY
{
    public class Svm2 : ITrader
    {
        public string Name { get { return "Svm 2"; } }
        public string Description { get { return "Based on 3 MA (13, 34, 100) and stochastic"; } }
        public short Id { get { return 9; } }
        public Symbol Symbol { get; set; }
        public TradeResolution TradeResolution { get; set; }
        public Dictionary<string, double> Parameters { get { return m_parameters; } }

        private Dictionary<string, double> m_parameters = new Dictionary<string, double>();

        public Svm2()
        {
            m_parameters.Add("SL", 10);
            m_parameters.Add("TP", 30);
            m_parameters.Add("TrainDays", 1);
            m_parameters.Add("", 0);
        }
    }
}