namespace DataTransfor
{
    partial class Form1
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置受控資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.xAxis = new HslCommunication.Controls.UserCurve();
            this.hallValue = new HslCommunication.Controls.UserCurve();
            this.zAxis = new HslCommunication.Controls.UserCurve();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.startObservation = new HslCommunication.Controls.UserButton();
            this.label4 = new System.Windows.Forms.Label();
            this.仰俯角 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.DisView = new HslCommunication.Controls.UserCurve();
            this.ModeBox = new System.Windows.Forms.PictureBox();
            this.elevationGaugeChart = new HslCommunication.Controls.UserGaugeChart();
            ((System.ComponentModel.ISupportInitialize)(this.ModeBox)).BeginInit();
            this.SuspendLayout();
            // 
            // xAxis
            // 
            this.xAxis.BackColor = System.Drawing.Color.Transparent;
            this.xAxis.Location = new System.Drawing.Point(12, 28);
            this.xAxis.Name = "xAxis";
            this.xAxis.Size = new System.Drawing.Size(589, 197);
            this.xAxis.TabIndex = 0;
            this.xAxis.ValueMaxLeft = 200F;
            this.xAxis.ValueMaxRight = 200F;
            // 
            // hallValue
            // 
            this.hallValue.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.hallValue.BackColor = System.Drawing.Color.Transparent;
            this.hallValue.Location = new System.Drawing.Point(12, 296);
            this.hallValue.Name = "hallValue";
            this.hallValue.Size = new System.Drawing.Size(589, 197);
            this.hallValue.TabIndex = 1;
            // 
            // zAxis
            // 
            this.zAxis.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.zAxis.BackColor = System.Drawing.Color.Transparent;
            this.zAxis.Location = new System.Drawing.Point(633, 28);
            this.zAxis.Name = "zAxis";
            this.zAxis.Size = new System.Drawing.Size(589, 197);
            this.zAxis.TabIndex = 2;
            this.zAxis.ValueMaxLeft = 300F;
            this.zAxis.ValueMaxRight = 300F;
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort1_DataReceived);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.label1.Location = new System.Drawing.Point(36, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(116, 19);
            this.label1.TabIndex = 5;
            this.label1.Text = "X軸數值標準差";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.label2.Location = new System.Drawing.Point(656, 6);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(114, 19);
            this.label2.TabIndex = 6;
            this.label2.Text = "Z軸數值標準差";
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.label3.Location = new System.Drawing.Point(36, 240);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(73, 19);
            this.label3.TabIndex = 7;
            this.label3.Text = "霍爾數值";
            // 
            // startObservation
            // 
            this.startObservation.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.startObservation.BackColor = System.Drawing.Color.Transparent;
            this.startObservation.CustomerInformation = "";
            this.startObservation.EnableColor = System.Drawing.Color.FromArgb(((int)(((byte)(190)))), ((int)(((byte)(190)))), ((int)(((byte)(190)))));
            this.startObservation.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.startObservation.Location = new System.Drawing.Point(570, 485);
            this.startObservation.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.startObservation.Name = "startObservation";
            this.startObservation.Size = new System.Drawing.Size(78, 25);
            this.startObservation.TabIndex = 8;
            this.startObservation.UIText = "開始觀測";
            this.startObservation.Click += new System.EventHandler(this.StartObservation_Click);
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.label4.Location = new System.Drawing.Point(656, 240);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 19);
            this.label4.TabIndex = 10;
            this.label4.Text = "模式";
            // 
            // 仰俯角
            // 
            this.仰俯角.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.仰俯角.AutoSize = true;
            this.仰俯角.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.仰俯角.Location = new System.Drawing.Point(36, 528);
            this.仰俯角.Name = "仰俯角";
            this.仰俯角.Size = new System.Drawing.Size(57, 19);
            this.仰俯角.TabIndex = 12;
            this.仰俯角.Text = "仰俯角";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("新細明體", 12F);
            this.label5.Location = new System.Drawing.Point(657, 530);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(73, 16);
            this.label5.TabIndex = 13;
            this.label5.Text = "距離(CM)";
            // 
            // DisView
            // 
            this.DisView.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.DisView.BackColor = System.Drawing.Color.Transparent;
            this.DisView.Location = new System.Drawing.Point(633, 551);
            this.DisView.Name = "DisView";
            this.DisView.Size = new System.Drawing.Size(589, 197);
            this.DisView.TabIndex = 14;
            this.DisView.ValueMaxLeft = 5F;
            this.DisView.ValueMaxRight = 5F;
            // 
            // ModeBox
            // 
            this.ModeBox.Location = new System.Drawing.Point(663, 296);
            this.ModeBox.Name = "ModeBox";
            this.ModeBox.Size = new System.Drawing.Size(589, 197);
            this.ModeBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.ModeBox.TabIndex = 15;
            this.ModeBox.TabStop = false;
            // 
            // elevationGaugeChart
            // 
            this.elevationGaugeChart.BackColor = System.Drawing.Color.Transparent;
            this.elevationGaugeChart.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.elevationGaugeChart.Location = new System.Drawing.Point(24, 518);
            this.elevationGaugeChart.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.elevationGaugeChart.Name = "elevationGaugeChart";
            this.elevationGaugeChart.Size = new System.Drawing.Size(589, 197);
            this.elevationGaugeChart.TabIndex = 16;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.ClientSize = new System.Drawing.Size(1264, 761);
            this.Controls.Add(this.elevationGaugeChart);
            this.Controls.Add(this.ModeBox);
            this.Controls.Add(this.DisView);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.仰俯角);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.startObservation);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.zAxis);
            this.Controls.Add(this.hallValue);
            this.Controls.Add(this.xAxis);
            this.Name = "Form1";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.Text = "數值監控";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.ModeBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private HslCommunication.Controls.UserCurve xAxis;
        private HslCommunication.Controls.UserCurve hallValue;
        private HslCommunication.Controls.UserCurve zAxis;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private HslCommunication.Controls.UserButton startObservation;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label 仰俯角;
        private System.Windows.Forms.Label label5;
        private HslCommunication.Controls.UserCurve DisView;
        private System.Windows.Forms.PictureBox ModeBox;
        private HslCommunication.Controls.UserGaugeChart elevationGaugeChart;
    }
}

