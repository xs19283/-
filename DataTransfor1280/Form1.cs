using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DataTransfor
{
    public partial class Form1 : Form
    {
        int[] ModBusData = new int[6];
        Random random = new Random();

        public Form1()
        {
            InitializeComponent();
        }

        ////////亂數測試函式
        private float[] GetRandomValueByCount(int count, float min, float max)
        {
            float[] data = new float[count];
            for (int i = 0; i < data.Length; i++)
            {
                data[i] = (float)random.NextDouble() * (max - min) + min;
            }
            return data;
        }

        ////////程式一執行做藍芽serialPort初始化, 以及曲線圖初始化
        private void Form1_Load(object sender, EventArgs e)
        {
            SerialPortInit(sender, e);
            InitGraph(sender, e);
        }

        ////////SerialPort初始化  (確認 COM PORT 有無正確)
        private void SerialPortInit(object sender, EventArgs e)
        {
            serialPort1.PortName = "COM7";
            serialPort1.BaudRate = 115200;
            serialPort1.Parity = System.IO.Ports.Parity.None;
            serialPort1.DataBits = 8;
            serialPort1.StopBits = System.IO.Ports.StopBits.One;
            serialPort1.Encoding = Encoding.GetEncoding("big5");
            serialPort1.Open();
        }

        ////////曲線圖初始化
        private void InitGraph(object sender, EventArgs e)
        {
            xAxis.SetLeftCurve("A", new float[] { }, Color.Tomato);
            zAxis.SetLeftCurve("B", new float[] { }, Color.DodgerBlue);
            hallValue.SetLeftCurve("C", new float[] { }, Color.Orchid);
            DisView.SetLeftCurve("F", new float[] { }, Color.Black);
        }

        ////////X軸資料顯示
        private void XAxisValueShow(object sender, EventArgs e)
        {
            Timer timer = new Timer
            {
                Interval = 1  //10毫秒更新一次
            };
            timer.Tick += (sender1, e1) =>
            {
                xAxis.AddCurveData("A", ModBusData[0]);
            };
            timer.Start();
        }

        ////////Z軸資料顯示
        private void ZAxisValueShow(object sender, EventArgs e)
        {
            Timer timer = new Timer
            {
                Interval = 1  //10毫秒更新一次
            };
            timer.Tick += (sender1, e1) =>
            {
                zAxis.AddCurveData("B", ModBusData[1]);
            };
            timer.Start();
        }

        ////////霍爾資料顯示函式
        private void HallValueShow(object sender, EventArgs e)
        {
            Timer timer = new Timer
            {
                Interval = 1  //10毫秒更新一次
            };
            timer.Tick += (sender1, e1) =>
            {
                hallValue.AddCurveData("C", ModBusData[2]);
            };
            timer.Start();
        }

        ////////距離顯示函式
        private void DisView_Show(object sender, EventArgs e)
        {
            Timer timer = new Timer
            {
                Interval = 1  //10毫秒更新一次
            };
            timer.Tick += (sender1, e1) =>
            {
                DisView.AddCurveData("F", ModBusData[5]);
            };
            timer.Start();
        }

        private void Mode_Show(object sender, EventArgs e)
        {
            Timer timer = new Timer
            {
                Interval = 1  //10毫秒更新一次
            };
            timer.Tick += (sender1, e1) =>
            {
                if (ModBusData[4] == 1)
                {
                    ModeBox.Image = Properties.Resources._1;
                }
                if (ModBusData[4] == 2)
                {
                    ModeBox.Image = Properties.Resources._2;
                }
                if (ModBusData[4] == 3)
                {
                    ModeBox.Image = Properties.Resources._3;
                }
                if (ModBusData[4] == 4)
                {
                    ModeBox.Image = Properties.Resources._4;
                }
                if (ModBusData[4] == 5)
                {
                    ModeBox.Image = Properties.Resources._5;
                }
            };
            timer.Start();
        }

        private void Elevation_Show(object sender, EventArgs e)
        {
            Timer timer = new Timer
            {
                Interval = 1  //10毫秒更新一次
            };
            timer.Tick += (sender1, e1) =>
            {
                elevationGaugeChart.Value = ModBusData[3];
            };
            timer.Start();
        }

        ////////開始觀測
        private void StartObservation_Click(object sender, EventArgs e)
        {
            XAxisValueShow(sender, e);
            ZAxisValueShow(sender, e);
            HallValueShow(sender, e);
            DisView_Show(sender, e);
            Mode_Show(sender, e);
            Elevation_Show(sender, e);
        }
        

        ////////收serialport資料函示
        private void SerialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            int FirstData = Convert.ToInt32(serialPort1.ReadByte());
            if(FirstData == 85)
            {
                for (int i = 0; i < 6; i++)
                {
                    ModBusData[i] = Convert.ToInt32(serialPort1.ReadByte());
                }
            }
        }
    }
}
