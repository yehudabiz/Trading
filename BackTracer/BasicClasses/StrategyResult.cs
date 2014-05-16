using System;

namespace PFY
{
    public class StrategyResult
    {
        public int TradeCount { get; set; }
        public int TradeLongCount { get; set; }
        public int TradeShortCount { get; set; }
        public double ResultBalance { get; set; }
        public double MinBalance { get; set; }
        public double MaxBalance { get; set; }
        public int LongProfitCount { get; set; }
        public int LongLossCount { get; set; }
        public int ShortProfitCount { get; set; }
        public int ShortLossCount { get; set; }
        public int ForcedLongProfit { get; set; }
        public int ForcedLongLoss { get; set; }
        public int ForcedShortProfit { get; set; }
        public int ForcedShortLoss { get; set; }
        public TimeSpan AveragePositionTime { get; set; }
        public TimeSpan ShortestPosition { get; set; }
        public TimeSpan LongestPosition { get; set; }
        public double SharpRatio { get; set; }
    }
}