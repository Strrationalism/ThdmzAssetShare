using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Config2.ConfigData;

namespace Config2
{
    public partial class Form : System.Windows.Forms.Form
    {
        public Form()
        {
            InitializeComponent();
        }

        private void UpdateFPSComboByVSync()
        {
            FPSMode.Enabled = !VSync.Checked;
            if (VSync.Checked)
                FPSMode.SelectedIndex = 2;  //60FPS
        }
        private void ResetScreenComboByFullscreen()
        {
            bool full = Fullscreen.Checked;

            VideoModeSelect.Items.Clear();

            if(full)
            {
                VideoModeSelect.Items.Add("和桌面相同的分辨率");
                VideoModeSelect.Items.Add("1280x720（推荐）");
                VideoModeSelect.SelectedIndex = 1;
            }
            else
            {
                VideoModeSelect.Items.Add("2560x1440");
                VideoModeSelect.Items.Add("1920x1080");
                VideoModeSelect.Items.Add("1820x1024");
                VideoModeSelect.Items.Add("1440x810");
                VideoModeSelect.Items.Add("1280x720（推荐）");
                VideoModeSelect.Items.Add("1024x576");
                VideoModeSelect.Items.Add("800x450");
                VideoModeSelect.SelectedIndex = 4;
            }
        }

        public void SetToData(Data d)
        {
            Beautiful.Checked = d.BeautifulBackground;
            MSAA.SelectedIndex = (int)d.MSAAModeSelect;
            TexFliter.SelectedIndex = (int)d.TextureFliterMode;

            VSync.Checked = d.VSyncEnabled;
            FPSMode.SelectedIndex = (int)d.FPSModeSelect;
            UpdateFPSComboByVSync();

            Fullscreen.Checked = d.Fullscreen;
            ResetScreenComboByFullscreen();
            VideoModeSelect.SelectedIndex = (int)d.VideoModeSelect;

            preloadBGM.Checked = d.PreloadBGM;
        }


        private void OK_Click(object sender, EventArgs e)
        {
            var blob = new Data()
            {
                BeautifulBackground = Beautiful.Checked,
                MSAAModeSelect = (MSAAMode)MSAA.SelectedIndex,
                TextureFliterMode = (TextureFliter)TexFliter.SelectedIndex,
                VSyncEnabled = VSync.Checked,
                FPSModeSelect = (ConfigData.FPSMode)FPSMode.SelectedIndex,
                Fullscreen = Fullscreen.Checked,
                VideoModeSelect = (VideoMode)VideoModeSelect.SelectedIndex,
                PreloadBGM = preloadBGM.Checked
            }.Save();



            if(!System.IO.Directory.Exists(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\Strrationalism"))
            {
                System.IO.Directory.CreateDirectory(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\Strrationalism");
            }

            if(!System.IO.Directory.Exists(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\Strrationalism\\Thdmz"))
            {
                System.IO.Directory.CreateDirectory(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\Strrationalism\\Thdmz");
            }

            var fs = new System.IO.FileStream(Program.ConfigPath, System.IO.FileMode.Create);
            fs.Write(blob, 0, 8);
            fs.Close();

            Close();
        }

        private void VSync_CheckedChanged(object sender, EventArgs e)
        {
            UpdateFPSComboByVSync();
        }

        private void Fullscreen_CheckedChanged(object sender, EventArgs e)
        {
            ResetScreenComboByFullscreen();
        }

        private void Cance_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
