namespace Config2
{
    partial class Form
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.Beautiful = new System.Windows.Forms.CheckBox();
            this.VSync = new System.Windows.Forms.CheckBox();
            this.MSAA = new System.Windows.Forms.ComboBox();
            this.TexFliter = new System.Windows.Forms.ComboBox();
            this.FPSMode = new System.Windows.Forms.ComboBox();
            this.Fullscreen = new System.Windows.Forms.CheckBox();
            this.VideoModeSelect = new System.Windows.Forms.ComboBox();
            this.OK = new System.Windows.Forms.Button();
            this.Cance = new System.Windows.Forms.Button();
            this.preloadBGM = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.preloadBGM);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.Beautiful);
            this.groupBox1.Controls.Add(this.VSync);
            this.groupBox1.Controls.Add(this.MSAA);
            this.groupBox1.Controls.Add(this.TexFliter);
            this.groupBox1.Controls.Add(this.FPSMode);
            this.groupBox1.Controls.Add(this.Fullscreen);
            this.groupBox1.Controls.Add(this.VideoModeSelect);
            this.groupBox1.Location = new System.Drawing.Point(13, 13);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(210, 247);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "游戏设置";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(5, 156);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 12);
            this.label4.TabIndex = 10;
            this.label4.Text = "抗锯齿：";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 118);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 9;
            this.label3.Text = "图像质量：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 80);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "帧率：";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 39);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "分辨率：";
            // 
            // Beautiful
            // 
            this.Beautiful.AutoSize = true;
            this.Beautiful.Location = new System.Drawing.Point(6, 197);
            this.Beautiful.Name = "Beautiful";
            this.Beautiful.Size = new System.Drawing.Size(144, 16);
            this.Beautiful.TabIndex = 6;
            this.Beautiful.Text = "使用更华丽的动画效果";
            this.Beautiful.UseVisualStyleBackColor = true;
            // 
            // VSync
            // 
            this.VSync.AutoSize = true;
            this.VSync.Location = new System.Drawing.Point(135, 97);
            this.VSync.Name = "VSync";
            this.VSync.Size = new System.Drawing.Size(72, 16);
            this.VSync.TabIndex = 5;
            this.VSync.Text = "垂直同步";
            this.VSync.UseVisualStyleBackColor = true;
            this.VSync.CheckedChanged += new System.EventHandler(this.VSync_CheckedChanged);
            // 
            // MSAA
            // 
            this.MSAA.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.MSAA.FormattingEnabled = true;
            this.MSAA.Items.AddRange(new object[] {
            "MSAA 16X",
            "MSAA 8X",
            "MSAA 4X",
            "MSAA 2X",
            "关闭"});
            this.MSAA.Location = new System.Drawing.Point(6, 171);
            this.MSAA.Name = "MSAA";
            this.MSAA.Size = new System.Drawing.Size(121, 20);
            this.MSAA.TabIndex = 4;
            // 
            // TexFliter
            // 
            this.TexFliter.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.TexFliter.FormattingEnabled = true;
            this.TexFliter.Items.AddRange(new object[] {
            "平滑（推荐）",
            "快速"});
            this.TexFliter.Location = new System.Drawing.Point(6, 133);
            this.TexFliter.Name = "TexFliter";
            this.TexFliter.Size = new System.Drawing.Size(121, 20);
            this.TexFliter.TabIndex = 3;
            // 
            // FPSMode
            // 
            this.FPSMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.FPSMode.FormattingEnabled = true;
            this.FPSMode.Items.AddRange(new object[] {
            "无限制",
            "120FPS",
            "60FPS",
            "30FPS",
            "20FPS"});
            this.FPSMode.Location = new System.Drawing.Point(7, 95);
            this.FPSMode.Name = "FPSMode";
            this.FPSMode.Size = new System.Drawing.Size(121, 20);
            this.FPSMode.TabIndex = 2;
            // 
            // Fullscreen
            // 
            this.Fullscreen.AutoSize = true;
            this.Fullscreen.Location = new System.Drawing.Point(135, 57);
            this.Fullscreen.Name = "Fullscreen";
            this.Fullscreen.Size = new System.Drawing.Size(48, 16);
            this.Fullscreen.TabIndex = 1;
            this.Fullscreen.Text = "全屏";
            this.Fullscreen.UseVisualStyleBackColor = true;
            this.Fullscreen.CheckedChanged += new System.EventHandler(this.Fullscreen_CheckedChanged);
            // 
            // VideoModeSelect
            // 
            this.VideoModeSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.VideoModeSelect.FormattingEnabled = true;
            this.VideoModeSelect.Location = new System.Drawing.Point(7, 57);
            this.VideoModeSelect.Name = "VideoModeSelect";
            this.VideoModeSelect.Size = new System.Drawing.Size(121, 20);
            this.VideoModeSelect.TabIndex = 0;
            // 
            // OK
            // 
            this.OK.Location = new System.Drawing.Point(12, 277);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(84, 34);
            this.OK.TabIndex = 1;
            this.OK.Text = "确定";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // Cance
            // 
            this.Cance.Location = new System.Drawing.Point(139, 277);
            this.Cance.Name = "Cance";
            this.Cance.Size = new System.Drawing.Size(84, 34);
            this.Cance.TabIndex = 2;
            this.Cance.Text = "取消";
            this.Cance.UseVisualStyleBackColor = true;
            this.Cance.Click += new System.EventHandler(this.Cance_Click);
            // 
            // preloadBGM
            // 
            this.preloadBGM.AutoSize = true;
            this.preloadBGM.Location = new System.Drawing.Point(6, 221);
            this.preloadBGM.Name = "preloadBGM";
            this.preloadBGM.Size = new System.Drawing.Size(78, 16);
            this.preloadBGM.TabIndex = 11;
            this.preloadBGM.Text = "预加载BGM";
            this.preloadBGM.UseVisualStyleBackColor = true;
            // 
            // Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(236, 323);
            this.Controls.Add(this.Cance);
            this.Controls.Add(this.OK);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "东方蝶梦志 游戏设置";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox Beautiful;
        private System.Windows.Forms.CheckBox VSync;
        private System.Windows.Forms.ComboBox MSAA;
        private System.Windows.Forms.ComboBox TexFliter;
        private System.Windows.Forms.ComboBox FPSMode;
        private System.Windows.Forms.CheckBox Fullscreen;
        private System.Windows.Forms.ComboBox VideoModeSelect;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.Button Cance;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox preloadBGM;
    }
}

