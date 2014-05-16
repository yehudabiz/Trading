using System;
using System.Collections.Generic;

namespace PFY
{
    public class RsiStochSvm : ITrader
    {
        public string Name { get { return "RsiStochSvm"; } }
        public string Description { get { return ""; } }
        public short Id { get { return 12; } }
        public Symbol Symbol { get; set; }
        public TradeResolution TradeResolution { get; set; }
        public Dictionary<string, double> Parameters { get { return m_parameters; } }

        private Dictionary<string, double> m_parameters = new Dictionary<string, double>();

        public RsiStochSvm()
        {
            m_parameters.Add("SL", 10);
            m_parameters.Add("TP", 30);
            m_parameters.Add("TrainDays", 1);
            m_parameters.Add("", 0);
        }
    }
}