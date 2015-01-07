using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using System.Xml.Linq;

namespace PFY
{
    public partial class MainForm : Form
    {
        private CancellationTokenSource m_cancelHandle;
        private ITrader m_trader;
        private List<string> m_filesList;
        private double m_balance = 0;
        private Bar m_lastBar = null;
        private List<Order> m_orders = new List<Order>();
        private double m_slippage;
        private int m_totalIterations;
        private int m_iterationsCounter = 0;
        private StreamWriter m_reportWriter;

        public MainForm()
        {
            InitializeComponent();

            this.FormClosing += SaveParams;
            this.Load += LoadParams;

            Assembly asm = Assembly.GetAssembly(this.GetType());
            Type[] types = asm.GetTypes();
            foreach (Type tp in types)
            {
                if (typeof(ITrader).IsAssignableFrom(tp) && !tp.IsInterface)
                {
                    cbxTraderSelector.Items.Add(tp.Name);
                }
            }

            cbxSymbol.DataSource = Enum.GetValues(typeof(Symbol));
            cbxSymbol.SelectedIndex = 0;
            cbxPeriod.DataSource = Enum.GetValues(typeof(TradeResolution));
            cbxPeriod.SelectedIndex = 0;
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            if (folderSelector.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                tbxPath.Text = folderSelector.SelectedPath;
            }
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (!String.IsNullOrEmpty(tbxPath.Text) && Directory.Exists(tbxPath.Text))
            {
                m_filesList = Directory.GetFiles(tbxPath.Text, "*.dat").ToList();
            }
            m_iterationsCounter = 0;
            if (m_filesList != null && m_filesList.Count != 0)
            {
                m_cancelHandle = new CancellationTokenSource();
                Task.Factory.StartNew(() => IterateOnStrategy(), m_cancelHandle.Token);
            }
        }

        private void cbx_traderSelector_SelectedIndexChanged(object sender, EventArgs e)
        {
            lbl_par1.Text = "";
            lbl_par2.Text = "";
            lbl_par3.Text = "";
            lbl_par4.Text = "";

            if (CreateTrader() && m_trader.Parameters != null)
            {
                if (m_trader.Parameters.Count > 0) lbl_par1.Text = m_trader.Parameters.Keys.ToList()[0] + ":";
                if (m_trader.Parameters.Count > 1) lbl_par2.Text = m_trader.Parameters.Keys.ToList()[1] + ":";
                if (m_trader.Parameters.Count > 2) lbl_par3.Text = m_trader.Parameters.Keys.ToList()[2] + ":";
                if (m_trader.Parameters.Count > 3) lbl_par4.Text = m_trader.Parameters.Keys.ToList()[3] + ":";
            }
        }

        private bool InitTrader()
        {
            try
            {
                Symbol sym = Symbol.EURUSD;
                Enum.TryParse<Symbol>(cbxSymbol.Text, out sym);
                m_trader.Symbol = sym;

                TradeResolution per = TradeResolution.Min_5;
                Enum.TryParse<TradeResolution>(cbxPeriod.Text, out per);
                m_trader.TradeResolution = per;
            }
            catch (Exception e)
            {
                MessageBox.Show("Trader instance creation failed with error: " + e.Message);
                return false;
            }

            return true;
        }

        private bool CreateTrader()
        {
            try
            {
                Assembly asm = Assembly.GetAssembly(this.GetType());
                m_trader = asm.CreateInstance("PFY." + cbxTraderSelector.SelectedItem.ToString()) as ITrader;
            }
            catch (Exception e)
            {
                MessageBox.Show("Trader instance creation failed with error: " + e.Message);
                return false;
            }

            return true;
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            m_cancelHandle.Cancel();
        }

        private void IterateOnStrategy()
        {
            this.Invoke(new MethodInvoker(() => InitTrader()));
            m_totalIterations = CalculateAllIterationsCount();
            ShowProgressBar();
            FileStream fs = File.Open(m_trader.Name + "_" + m_trader.Symbol.ToString() + "_" + m_trader.TradeResolution.ToString() +
                "_" + numMinPar1.Value + "_" + numMinPar2.Value + "_" + numMinPar3.Value + "_" + numMinPar4.Value + ".csv", FileMode.Create);
            StreamWriter writer = new StreamWriter(fs);
            writer.WriteLine("From, To, " + m_trader.Parameters.ToList()[0].Key + ", " + m_trader.Parameters.ToList()[1].Key + ", " +
                             m_trader.Parameters.ToList()[2].Key + ", " + m_trader.Parameters.ToList()[3].Key + ", " +
                             "ResultBalance,  MinBalance,  MaxBalance,  LongProfitCount,  LongLossCount, ShortProfitCount, ShortLossCount,   " +
                            "ForcedLongProfit,  ForcedLongLoss, ForcedShortProfit,  ForcedShortLoss, TotalLongCount,  TotalShortCount,  TotalOrdersCount, AveragePositionTime, " +
                            "ShortestPosition,  LongestPosition,  Sharp");

            FileStream fsr = File.Open("Orders_" + m_trader.Name + "_" + m_trader.Symbol.ToString() + "_" + m_trader.TradeResolution.ToString() +
                "_" + numMinPar1.Value + "_" + numMinPar2.Value + "_" + numMinPar3.Value + "_" + numMinPar4.Value + ".csv", FileMode.Create);
            m_reportWriter = new StreamWriter(fsr);
            m_reportWriter.WriteLine("OpenTime, CloseTime, Type, OpenPrice, ClosePrice, Volume, TakeProfit, StopLoss, Profit, IsForced");

            DateTime start = startTimePicker.Value;
            DateTime end = endTimePicker.Value;
            do
            {
                if (m_cancelHandle.Token.IsCancellationRequested) break;
                if (cbUseWindow.Checked) end = start.AddDays((int)numWindowWidth.Value);

                decimal p1Start = m_trader.Parameters.Count > 0 ? numMinPar1.Value : 1;
                decimal p1End = m_trader.Parameters.Count > 0 ? numMaxPar1.Value : 1;
                decimal p1Step = m_trader.Parameters.Count > 0 ? numStepPar1.Value : 1;

                for (decimal p1 = p1Start; p1 <= p1End; p1 += p1Step)
                {
                    if (m_cancelHandle.Token.IsCancellationRequested) break;
                    m_trader.Parameters[m_trader.Parameters.ToList()[0].Key] = (double)p1;

                    decimal p2Start = m_trader.Parameters.Count > 1 ? numMinPar2.Value : 1;
                    decimal p2End = m_trader.Parameters.Count > 1 ? numMaxPar2.Value : 1;
                    decimal p2Step = m_trader.Parameters.Count > 1 ? numStepPar2.Value : 1;

                    for (decimal p2 = p2Start; p2 <= p2End; p2 += p2Step)
                    {
                        if (m_cancelHandle.Token.IsCancellationRequested) break;
                        m_trader.Parameters[m_trader.Parameters.ToList()[1].Key] = (double)p2;

                        decimal p3Start = m_trader.Parameters.Count > 2 ? numMinPar3.Value : 1;
                        decimal p3End = m_trader.Parameters.Count > 2 ? numMaxPar3.Value : 1;
                        decimal p3Step = m_trader.Parameters.Count > 2 ? numStepPar3.Value : 1;

                        for (decimal p3 = p3Start; p3 <= p3End; p3 += p3Step)
                        {
                            if (m_cancelHandle.Token.IsCancellationRequested) break;
                            m_trader.Parameters[m_trader.Parameters.ToList()[2].Key] = (double)p3;

                            decimal p4Start = m_trader.Parameters.Count > 3 ? numMinPar4.Value : 1;
                            decimal p4End = m_trader.Parameters.Count > 3 ? numMaxPar4.Value : 1;
                            decimal p4Step = m_trader.Parameters.Count > 3 ? numStepPar4.Value : 1;

                            for (decimal p4 = p4Start; p4 <= p4End; p4 += p4Step)
                            {
                                if (m_cancelHandle.Token.IsCancellationRequested) break;
                                List<StrategyResult> results = new List<StrategyResult>();
                                m_trader.Parameters[m_trader.Parameters.ToList()[3].Key] = (double)p4;
                                InitProgressBar(start, end);
                                for (int i = 0; i < numRepeat.Value; i++)
                                {
                                    if (m_cancelHandle.Token.IsCancellationRequested) break;
                                    StrategyResult res = IterateOnDays(start, end);
                                    writer.WriteLine(start.ToString("yyyy-MM-dd") + ", " + end.ToString("yyyy-MM-dd") + ", " + 
                                        p1 + "," + p2 + "," + p3 + "," + p4 + "," + res.ResultBalance + "," + res.MinBalance +
                                        "," + res.MaxBalance + "," + res.LongProfitCount + "," + res.LongLossCount + "," + res.ShortProfitCount + "," +
                                        res.ShortLossCount + "," + res.ForcedLongProfit + "," + res.ForcedLongLoss + "," + res.ForcedShortProfit + "," + 
                                        res.ForcedShortLoss + "," + res.TradeLongCount + "," + res.TradeShortCount + "," + res.TradeCount + "," +
                                        res.AveragePositionTime.ToString(@"dd\:hh\:mm\:ss") + "," + res.ShortestPosition.ToString(@"dd\:hh\:mm\:ss") + "," +
                                        res.LongestPosition.ToString(@"dd\:hh\:mm\:ss") + "," + res.SharpRatio);
                                    results.Add(res);
                                }

                                if (numRepeat.Value > 1)
                                {
                                    StrategyResult avrgRes = AverageStrategies(results);
                                    writer.WriteLine("Average:,," + p1 + "," + p2 + ",,," + avrgRes.ResultBalance + "," + avrgRes.MinBalance +
                                        "," + avrgRes.MaxBalance + "," + avrgRes.LongProfitCount + "," + avrgRes.LongLossCount + "," + 
                                        avrgRes.ShortProfitCount + "," + avrgRes.ShortLossCount + "," + avrgRes.ForcedLongProfit + "," +
                                        avrgRes.ForcedLongLoss + "," + avrgRes.ForcedShortProfit + "," +
                                        avrgRes.ForcedShortLoss + "," + avrgRes.TradeLongCount + "," + avrgRes.TradeShortCount + "," + avrgRes.TradeCount + "," +
                                        avrgRes.AveragePositionTime + "," + avrgRes.ShortestPosition + "," + avrgRes.LongestPosition + "," + avrgRes.SharpRatio);
                                    writer.WriteLine("");
                                }
                            }
                        }
                    }
                }
                start = start.AddDays((int)numWindowStep.Value);
                end = end.AddDays((int)numWindowStep.Value);
            }
            while (cbUseWindow.Checked && end.CompareTo(endTimePicker.Value) < 0);
            HideProgressBar();
            writer.Close();
            m_reportWriter.Close();
        }

        private StrategyResult IterateOnDays(DateTime fromDay, DateTime toDay)
        {
            try
            {
                TraderWrapper.InitTrader(m_trader.Id, m_trader.Symbol, m_trader.TradeResolution, (double)numPipSize.Value, m_trader.Parameters);
            }
            catch (Exception e)
            {
                MessageBox.Show("Trader " + m_trader.Name + " initialization failed with error: " + e.Message);
                return null;
            }
            logWindow.Invoke(new MethodInvoker(() => logWindow.Items.Clear()));

            m_iterationsCounter++;
            lblCurrIteration.Invoke(new MethodInvoker(() => lblCurrIteration.Text = m_iterationsCounter + " of " + m_totalIterations));

            m_orders.Clear();
            m_balance = 0;
            m_slippage = (double)numSpread.Value;
            StrategyResult result = new StrategyResult();
            DateTime processDay = fromDay;
            List<double> dailyProfit = new List<double>();
            
            do
            {
                PerformProgressBarStep();
                if (m_cancelHandle.Token.IsCancellationRequested) break;

                string file = String.Empty;

                file = m_filesList.Find(fn => fn.Contains(processDay.ToString("yyyy.MM.dd")));

                double startDayBalance = m_balance;
                if (!String.IsNullOrEmpty(file))
                {
                    ProcessDay(file);
                }
                dailyProfit.Add(m_balance - startDayBalance);

                processDay = processDay.AddDays(1);
            }
            while (processDay.CompareTo(toDay) <= 0);
            ResetProgressBar();

            foreach (Order o in m_orders)
            {
                if (o.Status == OrderStatus.Open)
                {
                    Close(o, m_lastBar, true);
                    if (o.Profit > 0)
                    {
                        if (o.Type == OrderType.Long) result.ForcedLongProfit++;
                        if (o.Type == OrderType.Short) result.ForcedShortProfit++;
                    }
                    else
                    {
                        if (o.Type == OrderType.Long) result.ForcedLongLoss++;
                        if (o.Type == OrderType.Short) result.ForcedShortLoss++;
                    }
                }
            }

            TraderWrapper.DeinitTrader();
            if (m_orders.Count > 0) CalculateResult(ref result, dailyProfit);

            return result;
        }

        private void ProcessDay(string file)
        {
            try
            {
                StreamReader srcReader = new StreamReader(file);
                string line;

                while ((line = srcReader.ReadLine()) != null)
                {
                    if (m_cancelHandle.Token.IsCancellationRequested)
                    {
                        srcReader.Close();
                        break;
                    }

                    m_lastBar = Bar.FromCSV_String(line);
                    if (m_lastBar != null)
                    {
                        foreach (Order o in m_orders)
                        {
                            if (o.Status == OrderStatus.Open && o.IsToClose(m_lastBar.Close))
                            {
                                Close(o, m_lastBar);
                            }
                        }

                        TraderWrapper.UpdateTraderWithPrice(m_lastBar);
                        double sl, tp;
                        Advise advise = TraderWrapper.AskTrader(out sl, out tp);

                        if ((advise & Advise.Buy) == Advise.Buy) OpenOrder(OrderType.Long, m_lastBar, sl, tp);
                        //if ((advise & Advise.Sell) == Advise.Sell) OpenOrder(OrderType.Short, m_lastBar, sl, tp);
                        //if ((advise & Advise.CloseLong) == Advise.CloseLong) CloseByType(OrderType.Long, m_lastBar);
                        //if ((advise & Advise.CloseShort) == Advise.CloseShort) CloseByType(OrderType.Short, m_lastBar);
                    }
                }

                srcReader.Close();
            }
            catch (Exception er)
            {
                WriteLog("Error: " + er.Message);
            }
        }

        private void OpenOrder(OrderType type, Bar bar, double sl, double tp)
        {
            double price = bar.Close + m_slippage * (int)type;
            Order newOrder = new Order(type, bar.TimeStamp, price, 10000, sl, tp);
            m_orders.Add(newOrder);
            TraderWrapper.UpdateTraderWithOrder(newOrder);

            WriteLog("Order " + m_orders.IndexOf(newOrder) + " opened. Time=" + bar.TimeStamp.ToString("dd/MM/yyyy HH:mm:ss") + ", Price=" + price + ", Type=" + type.ToString());
        }

        private void CloseByType(OrderType type, Bar b)
        {
            foreach (Order o in m_orders)
            {
                if (o.Status == OrderStatus.Open && o.Type == type) Close(o, b);
            }
        }

        private void Close(Order order, Bar b, bool isForced = false)
        {
            order.CloseTime = b.TimeStamp;
            order.ClosePrice = b.Close;
            order.Status = OrderStatus.Closed;
            if (!isForced) TraderWrapper.UpdateTraderWithOrder(order);
            WriteLog("Order " + m_orders.IndexOf(order) + " closed. Time=" + b.TimeStamp.ToString("dd/MM/yyyy HH:mm:ss") + ", Price=" + b.Close + ", PL=" +
                                        order.Profit + ", Ballance: " + m_balance);
            m_balance += (double)Math.Round((double)order.Profit, 2);
            m_reportWriter.WriteLine(order.OpenTime.ToString("yyyy-MM-dd_HH:mm:ss") + "," + order.CloseTime.ToString("yyyy-MM-dd_HH:mm:ss") + "," +
                order.Type.ToString() + "," + order.OpenPrice + "," + order.ClosePrice + "," + order.Volume + "," + order.TakeProfit + "," +
                order.StopLoss + "," + order.Profit + "," + isForced.ToString());
        }

        private void CalculateResult(ref StrategyResult result, List<double> dailyProfit)
        {
            double mean = m_balance / m_orders.Count;
            long ticks = 0;
            long shortest = 0;
            long longest = 0;

            foreach (Order o in m_orders)
            {
                if (o.Profit > 0)
                {
                    if (o.Type == OrderType.Long) result.LongProfitCount++;
                    if (o.Type == OrderType.Short) result.ShortProfitCount++;
                }
                else
                {
                    if (o.Type == OrderType.Long) result.LongLossCount++;
                    if (o.Type == OrderType.Short) result.ShortLossCount++;
                }

                result.ResultBalance = result.ResultBalance + o.Profit;
                if (result.ResultBalance > result.MaxBalance || result.MaxBalance == 0) result.MaxBalance = result.ResultBalance;
                if (result.ResultBalance < result.MinBalance || result.MinBalance == 0) result.MinBalance = result.ResultBalance;
                if (o.Type == OrderType.Long) result.TradeLongCount++;
                if (o.Type == OrderType.Short) result.TradeShortCount++;

                long orderTIcks = o.CloseTime.Ticks - o.OpenTime.Ticks;
                ticks += orderTIcks;
                if (orderTIcks > longest || longest == 0) longest = orderTIcks;
                if (orderTIcks < shortest || shortest == 0) shortest = orderTIcks;
            }

            double sigma = 0;
            foreach (double dp in dailyProfit)
            {
                sigma += (double)Math.Pow(dp - mean, 2);
            }

            sigma = (double)Math.Sqrt(sigma);

            result.SharpRatio = mean / sigma;
            result.TradeCount = m_orders.Count;
            result.AveragePositionTime = new TimeSpan(ticks / result.TradeCount);
            result.ShortestPosition = new TimeSpan(shortest);
            result.LongestPosition = new TimeSpan(longest);
        }

        private StrategyResult AverageStrategies(List<StrategyResult> list)
        {
            StrategyResult avrgRes = new StrategyResult();
            foreach (StrategyResult sr in list)
            {
                avrgRes.ResultBalance += sr.ResultBalance;
                avrgRes.MinBalance += sr.MinBalance;
                avrgRes.MaxBalance += sr.MaxBalance;
                avrgRes.LongProfitCount += sr.LongProfitCount;
                avrgRes.ShortProfitCount += sr.ShortProfitCount;
                avrgRes.LongLossCount += sr.LongLossCount;
                avrgRes.ShortLossCount += sr.ShortLossCount;
                avrgRes.ForcedLongProfit += sr.ForcedLongProfit;
                avrgRes.ForcedShortProfit += sr.ForcedShortProfit;
                avrgRes.ForcedLongLoss += sr.ForcedLongLoss;
                avrgRes.ForcedShortLoss += sr.ForcedShortLoss;
                avrgRes.TradeLongCount += sr.TradeLongCount;
                avrgRes.TradeShortCount += sr.TradeShortCount;
                avrgRes.TradeCount += sr.TradeCount;
                avrgRes.AveragePositionTime += sr.AveragePositionTime;
                avrgRes.ShortestPosition += sr.ShortestPosition;
                avrgRes.LongestPosition += sr.LongestPosition;
                avrgRes.SharpRatio += sr.SharpRatio;
            }

            avrgRes.ResultBalance /= (double)list.Count;
            avrgRes.MinBalance /= (double)list.Count;
            avrgRes.MaxBalance /= (double)list.Count;
            avrgRes.LongProfitCount /= (int)list.Count;
            avrgRes.ShortProfitCount /= (int)list.Count;
            avrgRes.LongLossCount /= (int)list.Count;
            avrgRes.ShortLossCount /= (int)list.Count;
            avrgRes.ForcedLongProfit /= (int)list.Count;
            avrgRes.ForcedShortProfit /= (int)list.Count;
            avrgRes.ForcedLongLoss /= (int)list.Count;
            avrgRes.ForcedShortLoss /= (int)list.Count;
            avrgRes.TradeLongCount /= (int)list.Count;
            avrgRes.TradeShortCount /= (int)list.Count;
            avrgRes.TradeCount /= (int)list.Count;
            avrgRes.AveragePositionTime = new TimeSpan(avrgRes.AveragePositionTime.Ticks / (int)list.Count);
            avrgRes.ShortestPosition = new TimeSpan(avrgRes.ShortestPosition.Ticks / (int)list.Count);
            avrgRes.LongestPosition = new TimeSpan(avrgRes.LongestPosition.Ticks / (int)list.Count);
            avrgRes.SharpRatio /= (double)list.Count;

            return avrgRes;
        }

        private void SaveParams(object sender, FormClosingEventArgs e)
        {
            XElement config = new XElement("settings",
                                    new XElement("sourceFolder", tbxPath.Text),
                                    new XElement("symbol", cbxSymbol.SelectedIndex),
                                    new XElement("period", cbxPeriod.SelectedIndex),
                                    new XElement("spread", numSpread.Value),
                                    new XElement("pipSize", numPipSize.Value),
                                    new XElement("startTime", startTimePicker.Value.Ticks),
                                    new XElement("endTime", endTimePicker.Value.Ticks),
                                    new XElement("useWindow", cbUseWindow.Checked),
                                    new XElement("windowWidth", numWindowWidth.Value),
                                    new XElement("windowStep", numWindowStep.Value),
                                    new XElement("trader", cbxTraderSelector.SelectedIndex),
                                    new XElement("repeat", numRepeat.Value),
                                    new XElement("minPar1", numMinPar1.Value),
                                    new XElement("maxPar1", numMaxPar1.Value),
                                    new XElement("stepPar1", numStepPar1.Value),
                                    new XElement("minPar2", numMinPar2.Value),
                                    new XElement("maxPar2", numMaxPar2.Value),
                                    new XElement("stepPar2", numStepPar2.Value),
                                    new XElement("minPar3", numMinPar3.Value),
                                    new XElement("maxPar3", numMaxPar3.Value),
                                    new XElement("stepPar3", numStepPar3.Value),
                                    new XElement("minPar4", numMinPar4.Value),
                                    new XElement("maxPar4", numMaxPar4.Value),
                                    new XElement("stepPar4", numStepPar4.Value)
                                );
            config.Save("settings.xml");
        }

        private void LoadParams(object sender, EventArgs e)
        {
            XElement config = null;
            try
            {
                config = XElement.Load("settings.xml");
            }
            catch { }
            if (config == null) return;

            try { tbxPath.Text = config.Element("sourceFolder").Value; } catch { }
            try { cbxSymbol.SelectedIndex = Convert.ToInt32(config.Element("symbol").Value); } catch { }
            try { cbxPeriod.SelectedIndex = Convert.ToInt32(config.Element("period").Value); } catch { }
            try { numSpread.Value = Convert.ToInt32(config.Element("spread").Value); } catch { }
            try { numPipSize.Value = Convert.ToInt32(config.Element("pipSize").Value); } catch { }
            try { startTimePicker.Value = new DateTime(Convert.ToInt64(config.Element("startTime").Value)); } catch { }
            try { endTimePicker.Value = new DateTime(Convert.ToInt64(config.Element("endTime").Value)); } catch { }
            try { cbUseWindow.Checked = Convert.ToBoolean(config.Element("useWindow").Value); } catch { }
            try { numWindowWidth.Value = Convert.ToInt32(config.Element("windowWidth").Value); } catch { }
            try { numWindowStep.Value = Convert.ToInt32(config.Element("windowStep").Value); } catch { }
            try { cbxTraderSelector.SelectedIndex = Convert.ToInt32(config.Element("trader").Value); } catch { }
            try { numRepeat.Value = Convert.ToInt32(config.Element("repeat").Value); } catch { }
            try { numMinPar1.Value = Convert.ToInt32(config.Element("minPar1").Value); } catch { }
            try { numMaxPar1.Value = Convert.ToInt32(config.Element("maxPar1").Value); } catch { }
            try { numStepPar1.Value = Convert.ToInt32(config.Element("stepPar1").Value); } catch { }
            try { numMinPar2.Value = Convert.ToInt32(config.Element("minPar2").Value); } catch { }
            try { numMaxPar2.Value = Convert.ToInt32(config.Element("maxPar2").Value); } catch { }
            try { numStepPar2.Value = Convert.ToInt32(config.Element("stepPar2").Value); } catch { }
            try { numMinPar3.Value = Convert.ToInt32(config.Element("minPar3").Value); } catch { }
            try { numMaxPar3.Value = Convert.ToInt32(config.Element("maxPar3").Value); } catch { }
            try { numStepPar3.Value = Convert.ToInt32(config.Element("stepPar3").Value); } catch { }
            try { numMinPar4.Value = Convert.ToInt32(config.Element("minPar4").Value); } catch { }
            try { numMaxPar4.Value = Convert.ToInt32(config.Element("maxPar4").Value); } catch { }
            try { numStepPar4.Value = Convert.ToInt32(config.Element("stepPar4").Value); } catch { }
        }

        private int CalculateAllIterationsCount()
        {
            int p1Count = (int)((numMaxPar1.Value - numMinPar1.Value) / numStepPar1.Value) + 1;
            int p2Count = (int)((numMaxPar2.Value - numMinPar2.Value) / numStepPar2.Value) + 1;
            int p3Count = (int)((numMaxPar3.Value - numMinPar3.Value) / numStepPar3.Value) + 1;
            int p4Count = (int)((numMaxPar4.Value - numMinPar4.Value) / numStepPar4.Value) + 1;
            int winCount = 1;
            if (cbUseWindow.Checked)
            {
                int totalDays = (int)(new TimeSpan(endTimePicker.Value.Ticks - startTimePicker.Value.Ticks)).TotalDays;
                int stepsInRange = (int)(totalDays / numWindowStep.Value) + 1;
                int stepsInWindow = (int)(numWindowWidth.Value / numWindowStep.Value) + 1;
                winCount = stepsInRange - stepsInWindow;
            }

            return p1Count * p2Count * p3Count * p4Count * winCount * (int)numRepeat.Value;
        }
    }
}
