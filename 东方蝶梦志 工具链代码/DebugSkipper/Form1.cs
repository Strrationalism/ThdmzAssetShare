using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace DebugSkipper
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        void Apply()
        {
            var dbgs = new string[9];
            dbgs[0] = "INT START_FRAME = " + numericUpDown3.Value;
            dbgs[1] = "STR STAGE = \"" + comboBox2.Text + "\"";
            dbgs[2] = "INT LEVEL = " + comboBox3.SelectedIndex;
            dbgs[3] = "INT DEBUG_SPEED = " + numericUpDown1.Value;
            dbgs[4] = "STR CHAR = \"" + comboBox1.Text + "\"";
            dbgs[5] = "FLOAT POWER = " + numericUpDown2.Value;
            dbgs[6] = "INT AUTO_CHANGE_STAGE = " + (AutoChangeStage.Checked ? 1 : 0);
            dbgs[7] = "INT MIRROR = " + (mirror.Checked ? 1 : 0);
            dbgs[8] = "";

            System.IO.File.WriteAllLines("assets\\debug.rv", dbgs);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Apply();
            Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            var rvData = SnowRVReader.ReadRV("assets\\debug.rv");

            switch(rvData["STAGE"].Value)
            {
                case "Stage1":
                    comboBox2.SelectedIndex = 0;
                    break;
                case "Stage2":
                    comboBox2.SelectedIndex = 0;
                    break;
                case "Stage3":
                    comboBox2.SelectedIndex = 0;
                    break;
                case "Stage4":
                    comboBox2.SelectedIndex = 0;
                    break;
                case "Stage5":
                    comboBox2.SelectedIndex = 0;
                    break;
                case "Stage6":
                    comboBox2.SelectedIndex = 0;
                    break;
                case "StageEx":
                    comboBox2.SelectedIndex = 0;
                    break;
                case "Random":
                    comboBox2.SelectedIndex = 0;
                    break;
            }

            switch (rvData["CHAR"].Value)
            {
                case "REIMU":
                    comboBox1.SelectedIndex = 0;
                    break;
                case "MARISA":
                    comboBox1.SelectedIndex = 1;
                    break;
            }

            comboBox3.SelectedIndex = int.Parse(rvData["LEVEL"].Value);

            numericUpDown1.Value = int.Parse(rvData["DEBUG_SPEED"].Value);
            numericUpDown2.Value = decimal.Parse(rvData["POWER"].Value);
            numericUpDown3.Value = decimal.Parse(rvData["START_FRAME"].Value);
            AutoChangeStage.Checked = int.Parse(rvData["AUTO_CHANGE_STAGE"].Value) > 0;
            mirror.Checked = int.Parse(rvData["MIRROR"].Value) > 0;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Apply();
            System.Diagnostics.Process.Start("start.cmd");
            Close();
        }

        void LoadStartTimeData()
        {
            string chr = comboBox1.Text;
            string stg = comboBox2.Text;

            if (chr != "REIMU" && chr != "MARISA") return;

            if (chr == "REIMU")
                LoadStartTimeData(StartTimeData.Data[stg].Reimu);
            else
                LoadStartTimeData(StartTimeData.Data[stg].Marisa);

        }

        void LoadStartTimeData(List<StartPoint> sp)
        {
            listBox1.Items.Clear();
            startPoints.Clear();

            listBox1.Items.Add("起始");
            startPoints.Add(0);

            foreach (var i in sp)
            {
                listBox1.Items.Add(i.Info);
                startPoints.Add(i.Time);
            }

        }

        List<int> startPoints = new List<int>();

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            LoadStartTimeData();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            LoadStartTimeData();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                numericUpDown3.Value = startPoints[listBox1.SelectedIndex];
            }
            catch(Exception)
            {

            }
        }
    }
}
