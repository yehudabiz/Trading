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


namespace PFY
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.folderSelector = new System.Windows.Forms.FolderBrowserDialog();
            this.tbxPath = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.btnStart = new System.Windows.Forms.Button();
            this.startTimePicker = new System.Windows.Forms.DateTimePicker();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.endTimePicker = new System.Windows.Forms.DateTimePicker();
            this.logWindow = new System.Windows.Forms.ListBox();
            this.numSpread = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.btnStop = new System.Windows.Forms.Button();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.label5 = new System.Windows.Forms.Label();
            this.numPipSize = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lbl_winSt = new System.Windows.Forms.Label();
            this.numWindowStep = new System.Windows.Forms.NumericUpDown();
            this.lbl_winWid = new System.Windows.Forms.Label();
            this.numWindowWidth = new System.Windows.Forms.NumericUpDown();
            this.cbUseWindow = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.cbxSymbol = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.cbxPeriod = new System.Windows.Forms.ComboBox();
            this.cbxTraderSelector = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.numStepPar1 = new System.Windows.Forms.NumericUpDown();
            this.numMaxPar1 = new System.Windows.Forms.NumericUpDown();
            this.numMinPar1 = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.numStepPar2 = new System.Windows.Forms.NumericUpDown();
            this.numMaxPar2 = new System.Windows.Forms.NumericUpDown();
            this.numMinPar2 = new System.Windows.Forms.NumericUpDown();
            this.numStepPar3 = new System.Windows.Forms.NumericUpDown();
            this.numMaxPar3 = new System.Windows.Forms.NumericUpDown();
            this.numMinPar3 = new System.Windows.Forms.NumericUpDown();
            this.numMinPar4 = new System.Windows.Forms.NumericUpDown();
            this.numStepPar4 = new System.Windows.Forms.NumericUpDown();
            this.numMaxPar4 = new System.Windows.Forms.NumericUpDown();
            this.lbl_par1 = new System.Windows.Forms.Label();
            this.lbl_par2 = new System.Windows.Forms.Label();
            this.lbl_par3 = new System.Windows.Forms.Label();
            this.lbl_par4 = new System.Windows.Forms.Label();
            this.numRepeat = new System.Windows.Forms.NumericUpDown();
            this.label10 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.lblCurrIteration = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numSpread)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPipSize)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numWindowStep)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numWindowWidth)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numRepeat)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbxPath
            // 
            this.tbxPath.Location = new System.Drawing.Point(79, 21);
            this.tbxPath.Name = "tbxPath";
            this.tbxPath.Size = new System.Drawing.Size(173, 20);
            this.tbxPath.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(76, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Source Folder:";
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(258, 19);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(27, 23);
            this.btnBrowse.TabIndex = 2;
            this.btnBrowse.Text = "...";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(11, 144);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(62, 23);
            this.btnStart.TabIndex = 3;
            this.btnStart.Text = "Start";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // startTimePicker
            // 
            this.startTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.startTimePicker.Location = new System.Drawing.Point(42, 21);
            this.startTimePicker.Name = "startTimePicker";
            this.startTimePicker.Size = new System.Drawing.Size(95, 20);
            this.startTimePicker.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "From:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 50);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(23, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "To:";
            // 
            // endTimePicker
            // 
            this.endTimePicker.CustomFormat = "";
            this.endTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.endTimePicker.Location = new System.Drawing.Point(42, 47);
            this.endTimePicker.Name = "endTimePicker";
            this.endTimePicker.Size = new System.Drawing.Size(95, 20);
            this.endTimePicker.TabIndex = 6;
            // 
            // logWindow
            // 
            this.logWindow.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.logWindow.FormattingEnabled = true;
            this.logWindow.Location = new System.Drawing.Point(12, 253);
            this.logWindow.Name = "logWindow";
            this.logWindow.Size = new System.Drawing.Size(550, 160);
            this.logWindow.TabIndex = 8;
            // 
            // numSpread
            // 
            this.numSpread.Location = new System.Drawing.Point(85, 21);
            this.numSpread.Name = "numSpread";
            this.numSpread.Size = new System.Drawing.Size(39, 20);
            this.numSpread.TabIndex = 9;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 24);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(72, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Spread (pips):";
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(79, 144);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(66, 23);
            this.btnStop.TabIndex = 11;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(11, 238);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(552, 13);
            this.progressBar.TabIndex = 12;
            this.progressBar.Visible = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(8, 50);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(48, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Pip Size:";
            // 
            // numPipSize
            // 
            this.numPipSize.DecimalPlaces = 5;
            this.numPipSize.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.numPipSize.Location = new System.Drawing.Point(58, 47);
            this.numPipSize.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numPipSize.Name = "numPipSize";
            this.numPipSize.Size = new System.Drawing.Size(66, 20);
            this.numPipSize.TabIndex = 13;
            this.numPipSize.Value = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lbl_winSt);
            this.groupBox1.Controls.Add(this.numWindowStep);
            this.groupBox1.Controls.Add(this.lbl_winWid);
            this.groupBox1.Controls.Add(this.numWindowWidth);
            this.groupBox1.Controls.Add(this.cbUseWindow);
            this.groupBox1.Controls.Add(this.endTimePicker);
            this.groupBox1.Controls.Add(this.startTimePicker);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Location = new System.Drawing.Point(161, 56);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(148, 175);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Date Range";
            // 
            // lbl_winSt
            // 
            this.lbl_winSt.AutoSize = true;
            this.lbl_winSt.Enabled = false;
            this.lbl_winSt.Location = new System.Drawing.Point(10, 146);
            this.lbl_winSt.Name = "lbl_winSt";
            this.lbl_winSt.Size = new System.Drawing.Size(74, 13);
            this.lbl_winSt.TabIndex = 12;
            this.lbl_winSt.Text = "Window Step:";
            // 
            // numWindowStep
            // 
            this.numWindowStep.Enabled = false;
            this.numWindowStep.Location = new System.Drawing.Point(96, 140);
            this.numWindowStep.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numWindowStep.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numWindowStep.Name = "numWindowStep";
            this.numWindowStep.Size = new System.Drawing.Size(40, 20);
            this.numWindowStep.TabIndex = 11;
            this.numWindowStep.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // lbl_winWid
            // 
            this.lbl_winWid.AutoSize = true;
            this.lbl_winWid.Enabled = false;
            this.lbl_winWid.Location = new System.Drawing.Point(11, 120);
            this.lbl_winWid.Name = "lbl_winWid";
            this.lbl_winWid.Size = new System.Drawing.Size(80, 13);
            this.lbl_winWid.TabIndex = 10;
            this.lbl_winWid.Text = "Window Width:";
            // 
            // numWindowWidth
            // 
            this.numWindowWidth.Enabled = false;
            this.numWindowWidth.Location = new System.Drawing.Point(97, 114);
            this.numWindowWidth.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.numWindowWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numWindowWidth.Name = "numWindowWidth";
            this.numWindowWidth.Size = new System.Drawing.Size(40, 20);
            this.numWindowWidth.TabIndex = 9;
            this.numWindowWidth.Value = new decimal(new int[] {
            30,
            0,
            0,
            0});
            // 
            // cbUseWindow
            // 
            this.cbUseWindow.AutoSize = true;
            this.cbUseWindow.Location = new System.Drawing.Point(11, 91);
            this.cbUseWindow.Name = "cbUseWindow";
            this.cbUseWindow.Size = new System.Drawing.Size(125, 17);
            this.cbUseWindow.TabIndex = 8;
            this.cbUseWindow.Text = "Use Moving Window";
            this.cbUseWindow.UseVisualStyleBackColor = true;
            this.cbUseWindow.CheckedChanged += new System.EventHandler(this.cb_useWindow_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.numPipSize);
            this.groupBox2.Controls.Add(this.numSpread);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(11, 56);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(134, 76);
            this.groupBox2.TabIndex = 17;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Market Data";
            // 
            // cbxSymbol
            // 
            this.cbxSymbol.FormattingEnabled = true;
            this.cbxSymbol.Location = new System.Drawing.Point(345, 21);
            this.cbxSymbol.Name = "cbxSymbol";
            this.cbxSymbol.Size = new System.Drawing.Size(81, 21);
            this.cbxSymbol.TabIndex = 20;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(301, 25);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(44, 13);
            this.label11.TabIndex = 21;
            this.label11.Text = "Symbol:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(437, 26);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(40, 13);
            this.label12.TabIndex = 23;
            this.label12.Text = "Period:";
            // 
            // cbxPeriod
            // 
            this.cbxPeriod.FormattingEnabled = true;
            this.cbxPeriod.Location = new System.Drawing.Point(477, 22);
            this.cbxPeriod.Name = "cbxPeriod";
            this.cbxPeriod.Size = new System.Drawing.Size(83, 21);
            this.cbxPeriod.TabIndex = 22;
            // 
            // cbxTraderSelector
            // 
            this.cbxTraderSelector.FormattingEnabled = true;
            this.cbxTraderSelector.Location = new System.Drawing.Point(54, 18);
            this.cbxTraderSelector.Name = "cbxTraderSelector";
            this.cbxTraderSelector.Size = new System.Drawing.Size(78, 21);
            this.cbxTraderSelector.TabIndex = 15;
            this.cbxTraderSelector.SelectedIndexChanged += new System.EventHandler(this.cbx_traderSelector_SelectedIndexChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(7, 22);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(41, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "Trader:";
            // 
            // numStepPar1
            // 
            this.numStepPar1.Location = new System.Drawing.Point(184, 68);
            this.numStepPar1.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numStepPar1.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numStepPar1.Name = "numStepPar1";
            this.numStepPar1.Size = new System.Drawing.Size(43, 20);
            this.numStepPar1.TabIndex = 17;
            this.numStepPar1.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numMaxPar1
            // 
            this.numMaxPar1.Location = new System.Drawing.Point(135, 68);
            this.numMaxPar1.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMaxPar1.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMaxPar1.Name = "numMaxPar1";
            this.numMaxPar1.Size = new System.Drawing.Size(43, 20);
            this.numMaxPar1.TabIndex = 18;
            // 
            // numMinPar1
            // 
            this.numMinPar1.Location = new System.Drawing.Point(84, 68);
            this.numMinPar1.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMinPar1.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMinPar1.Name = "numMinPar1";
            this.numMinPar1.Size = new System.Drawing.Size(43, 20);
            this.numMinPar1.TabIndex = 19;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(86, 51);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(24, 13);
            this.label7.TabIndex = 20;
            this.label7.Text = "Min";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(136, 51);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(27, 13);
            this.label8.TabIndex = 21;
            this.label8.Text = "Max";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(184, 51);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(29, 13);
            this.label9.TabIndex = 22;
            this.label9.Text = "Step";
            // 
            // numStepPar2
            // 
            this.numStepPar2.Location = new System.Drawing.Point(184, 94);
            this.numStepPar2.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numStepPar2.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numStepPar2.Name = "numStepPar2";
            this.numStepPar2.Size = new System.Drawing.Size(43, 20);
            this.numStepPar2.TabIndex = 23;
            this.numStepPar2.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numMaxPar2
            // 
            this.numMaxPar2.Location = new System.Drawing.Point(135, 94);
            this.numMaxPar2.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMaxPar2.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMaxPar2.Name = "numMaxPar2";
            this.numMaxPar2.Size = new System.Drawing.Size(43, 20);
            this.numMaxPar2.TabIndex = 24;
            // 
            // numMinPar2
            // 
            this.numMinPar2.Location = new System.Drawing.Point(84, 94);
            this.numMinPar2.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMinPar2.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMinPar2.Name = "numMinPar2";
            this.numMinPar2.Size = new System.Drawing.Size(43, 20);
            this.numMinPar2.TabIndex = 25;
            // 
            // numStepPar3
            // 
            this.numStepPar3.Location = new System.Drawing.Point(184, 120);
            this.numStepPar3.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numStepPar3.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numStepPar3.Name = "numStepPar3";
            this.numStepPar3.Size = new System.Drawing.Size(43, 20);
            this.numStepPar3.TabIndex = 26;
            this.numStepPar3.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numMaxPar3
            // 
            this.numMaxPar3.Location = new System.Drawing.Point(135, 120);
            this.numMaxPar3.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMaxPar3.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMaxPar3.Name = "numMaxPar3";
            this.numMaxPar3.Size = new System.Drawing.Size(43, 20);
            this.numMaxPar3.TabIndex = 27;
            // 
            // numMinPar3
            // 
            this.numMinPar3.Location = new System.Drawing.Point(84, 120);
            this.numMinPar3.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMinPar3.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMinPar3.Name = "numMinPar3";
            this.numMinPar3.Size = new System.Drawing.Size(43, 20);
            this.numMinPar3.TabIndex = 28;
            // 
            // numMinPar4
            // 
            this.numMinPar4.Location = new System.Drawing.Point(84, 146);
            this.numMinPar4.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMinPar4.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMinPar4.Name = "numMinPar4";
            this.numMinPar4.Size = new System.Drawing.Size(43, 20);
            this.numMinPar4.TabIndex = 28;
            // 
            // numStepPar4
            // 
            this.numStepPar4.Location = new System.Drawing.Point(184, 147);
            this.numStepPar4.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numStepPar4.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numStepPar4.Name = "numStepPar4";
            this.numStepPar4.Size = new System.Drawing.Size(43, 20);
            this.numStepPar4.TabIndex = 29;
            this.numStepPar4.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numMaxPar4
            // 
            this.numMaxPar4.Location = new System.Drawing.Point(135, 147);
            this.numMaxPar4.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.numMaxPar4.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.numMaxPar4.Name = "numMaxPar4";
            this.numMaxPar4.Size = new System.Drawing.Size(43, 20);
            this.numMaxPar4.TabIndex = 30;
            // 
            // lbl_par1
            // 
            this.lbl_par1.AutoSize = true;
            this.lbl_par1.Location = new System.Drawing.Point(7, 71);
            this.lbl_par1.Name = "lbl_par1";
            this.lbl_par1.Size = new System.Drawing.Size(0, 13);
            this.lbl_par1.TabIndex = 32;
            // 
            // lbl_par2
            // 
            this.lbl_par2.AutoSize = true;
            this.lbl_par2.Location = new System.Drawing.Point(7, 96);
            this.lbl_par2.Name = "lbl_par2";
            this.lbl_par2.Size = new System.Drawing.Size(0, 13);
            this.lbl_par2.TabIndex = 33;
            // 
            // lbl_par3
            // 
            this.lbl_par3.AutoSize = true;
            this.lbl_par3.Location = new System.Drawing.Point(8, 122);
            this.lbl_par3.Name = "lbl_par3";
            this.lbl_par3.Size = new System.Drawing.Size(0, 13);
            this.lbl_par3.TabIndex = 34;
            // 
            // lbl_par4
            // 
            this.lbl_par4.AutoSize = true;
            this.lbl_par4.Location = new System.Drawing.Point(8, 149);
            this.lbl_par4.Name = "lbl_par4";
            this.lbl_par4.Size = new System.Drawing.Size(0, 13);
            this.lbl_par4.TabIndex = 35;
            // 
            // numRepeat
            // 
            this.numRepeat.Location = new System.Drawing.Point(189, 19);
            this.numRepeat.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numRepeat.Name = "numRepeat";
            this.numRepeat.Size = new System.Drawing.Size(39, 20);
            this.numRepeat.TabIndex = 36;
            this.numRepeat.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(144, 22);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(45, 13);
            this.label10.TabIndex = 37;
            this.label10.Text = "Repeat:";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.numRepeat);
            this.groupBox3.Controls.Add(this.lbl_par4);
            this.groupBox3.Controls.Add(this.lbl_par3);
            this.groupBox3.Controls.Add(this.lbl_par2);
            this.groupBox3.Controls.Add(this.lbl_par1);
            this.groupBox3.Controls.Add(this.numMaxPar4);
            this.groupBox3.Controls.Add(this.numStepPar4);
            this.groupBox3.Controls.Add(this.numMinPar4);
            this.groupBox3.Controls.Add(this.numMinPar3);
            this.groupBox3.Controls.Add(this.numMaxPar3);
            this.groupBox3.Controls.Add(this.numStepPar3);
            this.groupBox3.Controls.Add(this.numMinPar2);
            this.groupBox3.Controls.Add(this.numMaxPar2);
            this.groupBox3.Controls.Add(this.numStepPar2);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.numMinPar1);
            this.groupBox3.Controls.Add(this.numMaxPar1);
            this.groupBox3.Controls.Add(this.numStepPar1);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.cbxTraderSelector);
            this.groupBox3.Location = new System.Drawing.Point(325, 56);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(238, 175);
            this.groupBox3.TabIndex = 18;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Strategy";
            // 
            // lblCurrIteration
            // 
            this.lblCurrIteration.AutoSize = true;
            this.lblCurrIteration.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCurrIteration.Location = new System.Drawing.Point(21, 194);
            this.lblCurrIteration.Name = "lblCurrIteration";
            this.lblCurrIteration.Size = new System.Drawing.Size(0, 20);
            this.lblCurrIteration.TabIndex = 24;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(574, 426);
            this.Controls.Add(this.lblCurrIteration);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.cbxPeriod);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.cbxSymbol);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnStart);
            this.Controls.Add(this.btnBrowse);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbxPath);
            this.Controls.Add(this.logWindow);
            this.Name = "MainForm";
            this.Text = "Back Tracer";
            ((System.ComponentModel.ISupportInitialize)(this.numSpread)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPipSize)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numWindowStep)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numWindowWidth)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinPar4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numStepPar4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMaxPar4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numRepeat)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.FolderBrowserDialog folderSelector;
        private System.Windows.Forms.TextBox tbxPath;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.DateTimePicker startTimePicker;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.DateTimePicker endTimePicker;
        private System.Windows.Forms.ListBox logWindow;
        private System.Windows.Forms.NumericUpDown numSpread;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown numPipSize;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label lbl_winSt;
        private System.Windows.Forms.NumericUpDown numWindowStep;
        private System.Windows.Forms.Label lbl_winWid;
        private System.Windows.Forms.NumericUpDown numWindowWidth;
        private System.Windows.Forms.CheckBox cbUseWindow;
        private System.Windows.Forms.GroupBox groupBox2;

        private void InitProgressBar(DateTime start, DateTime end)
        {
            progressBar.Minimum = 0;
            int daysNumber = (int)(end.Subtract(start).Duration().TotalDays * 6 / 7);
            progressBar.Invoke(new MethodInvoker(() => progressBar.Maximum = daysNumber + 1));
            progressBar.Invoke(new MethodInvoker(() => progressBar.Step = 1));
            progressBar.Invoke(new MethodInvoker(() => progressBar.Value = 0));
        }

        private void ResetProgressBar()
        {
            progressBar.Invoke(new MethodInvoker(() => progressBar.Value = 0));
        }

        private void ShowProgressBar()
        {
            if (progressBar.InvokeRequired)
            {
                progressBar.Invoke(new MethodInvoker(() => progressBar.Show()));
            }
            else
            {
                progressBar.Show();
            }
        }

        private void HideProgressBar()
        {
            if (progressBar.InvokeRequired)
            {
                progressBar.Invoke(new MethodInvoker(() => progressBar.Hide()));
            }
            else
            {
                progressBar.Hide();
            }
        }

        private void PerformProgressBarStep()
        {
            if (progressBar.InvokeRequired)
            {
                progressBar.Invoke(new MethodInvoker(() => progressBar.PerformStep()));
            }
            else
            {
                progressBar.PerformStep();
            }
        }

        private void WriteLog(string msg)
        {
            if (logWindow.InvokeRequired)
            {
                logWindow.Invoke(new MethodInvoker(() =>
                {
                    logWindow.Items.Add(msg);
                    logWindow.Items.Add("");
                }));
            }
            else
            {
                logWindow.Items.Add(msg);
                logWindow.Items.Add("");
            }
        }

        private void cb_useWindow_CheckedChanged(object sender, EventArgs e)
        {
            if (cbUseWindow.Checked)
            {
                numWindowStep.Enabled = true;
                numWindowWidth.Enabled = true;
                lbl_winSt.Enabled = true;
                lbl_winWid.Enabled = true;
            }
            else
            {
                numWindowStep.Enabled = false;
                numWindowWidth.Enabled = false;
                lbl_winSt.Enabled = false;
                lbl_winWid.Enabled = false;
            }
        }

        private ComboBox cbxSymbol;
        private Label label11;
        private Label label12;
        private ComboBox cbxPeriod;
        private ComboBox cbxTraderSelector;
        private Label label6;
        private NumericUpDown numStepPar1;
        private NumericUpDown numMaxPar1;
        private NumericUpDown numMinPar1;
        private Label label7;
        private Label label8;
        private Label label9;
        private NumericUpDown numStepPar2;
        private NumericUpDown numMaxPar2;
        private NumericUpDown numMinPar2;
        private NumericUpDown numStepPar3;
        private NumericUpDown numMaxPar3;
        private NumericUpDown numMinPar3;
        private NumericUpDown numMinPar4;
        private NumericUpDown numStepPar4;
        private NumericUpDown numMaxPar4;
        private Label lbl_par1;
        private Label lbl_par2;
        private Label lbl_par3;
        private Label lbl_par4;
        private NumericUpDown numRepeat;
        private Label label10;
        private GroupBox groupBox3;
        private Label lblCurrIteration;

    }
}

