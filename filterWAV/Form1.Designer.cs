namespace filterWAV
{
    partial class Form1
    {
        /// <summary>
        /// Wymagana zmienna projektanta.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Wyczyść wszystkie używane zasoby.
        /// </summary>
        /// <param name="disposing">prawda, jeżeli zarządzane zasoby powinny zostać zlikwidowane; Fałsz w przeciwnym wypadku.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Kod generowany przez Projektanta formularzy systemu Windows

        /// <summary>
        /// Metoda wymagana do obsługi projektanta — nie należy modyfikować
        /// jej zawartości w edytorze kodu.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.optionCpp = new System.Windows.Forms.RadioButton();
            this.optionAsm = new System.Windows.Forms.RadioButton();
            this.groupBoxLib = new System.Windows.Forms.GroupBox();
            this.dialogOpenWAV = new System.Windows.Forms.OpenFileDialog();
            this.wavForm = new System.Windows.Forms.TextBox();
            this.browseButton = new System.Windows.Forms.Button();
            this.groupBoxInput = new System.Windows.Forms.GroupBox();
            this.waveChartLeft = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.waveChartRight = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.threads16 = new System.Windows.Forms.RadioButton();
            this.threads8 = new System.Windows.Forms.RadioButton();
            this.threads4 = new System.Windows.Forms.RadioButton();
            this.threads2 = new System.Windows.Forms.RadioButton();
            this.threads1 = new System.Windows.Forms.RadioButton();
            this.runButton = new System.Windows.Forms.Button();
            this.groupBoxLib.SuspendLayout();
            this.groupBoxInput.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.waveChartLeft)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.waveChartRight)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(22, 427);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(596, 23);
            this.progressBar.TabIndex = 0;
            // 
            // optionCpp
            // 
            this.optionCpp.AutoSize = true;
            this.optionCpp.Location = new System.Drawing.Point(94, 19);
            this.optionCpp.Name = "optionCpp";
            this.optionCpp.Size = new System.Drawing.Size(78, 17);
            this.optionCpp.TabIndex = 1;
            this.optionCpp.TabStop = true;
            this.optionCpp.Text = "Run in C++";
            this.optionCpp.UseVisualStyleBackColor = true;
            this.optionCpp.CheckedChanged += new System.EventHandler(this.optionCpp_CheckedChanged);
            // 
            // optionAsm
            // 
            this.optionAsm.AutoSize = true;
            this.optionAsm.Location = new System.Drawing.Point(6, 19);
            this.optionAsm.Name = "optionAsm";
            this.optionAsm.Size = new System.Drawing.Size(82, 17);
            this.optionAsm.TabIndex = 2;
            this.optionAsm.TabStop = true;
            this.optionAsm.Text = "Run in ASM";
            this.optionAsm.UseVisualStyleBackColor = true;
            this.optionAsm.CheckedChanged += new System.EventHandler(this.optionAsm_CheckedChanged);
            // 
            // groupBoxLib
            // 
            this.groupBoxLib.Controls.Add(this.optionAsm);
            this.groupBoxLib.Controls.Add(this.optionCpp);
            this.groupBoxLib.Location = new System.Drawing.Point(19, 68);
            this.groupBoxLib.Name = "groupBoxLib";
            this.groupBoxLib.Size = new System.Drawing.Size(196, 46);
            this.groupBoxLib.TabIndex = 3;
            this.groupBoxLib.TabStop = false;
            this.groupBoxLib.Text = "Pick a library";
            // 
            // dialogOpenWAV
            // 
            this.dialogOpenWAV.FileName = "Select a WAV file";
            this.dialogOpenWAV.Filter = "WAV files (*.wav)|*.wav";
            this.dialogOpenWAV.Title = "Open WAV file";
            // 
            // wavForm
            // 
            this.wavForm.Location = new System.Drawing.Point(6, 19);
            this.wavForm.Name = "wavForm";
            this.wavForm.Size = new System.Drawing.Size(503, 20);
            this.wavForm.TabIndex = 4;
            // 
            // browseButton
            // 
            this.browseButton.Location = new System.Drawing.Point(515, 19);
            this.browseButton.Name = "browseButton";
            this.browseButton.Size = new System.Drawing.Size(75, 23);
            this.browseButton.TabIndex = 5;
            this.browseButton.Text = "Browse";
            this.browseButton.UseVisualStyleBackColor = true;
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // groupBoxInput
            // 
            this.groupBoxInput.Controls.Add(this.wavForm);
            this.groupBoxInput.Controls.Add(this.browseButton);
            this.groupBoxInput.Location = new System.Drawing.Point(19, 4);
            this.groupBoxInput.Name = "groupBoxInput";
            this.groupBoxInput.Size = new System.Drawing.Size(596, 58);
            this.groupBoxInput.TabIndex = 6;
            this.groupBoxInput.TabStop = false;
            this.groupBoxInput.Text = "Pick a WAV file";
            this.groupBoxInput.Enter += new System.EventHandler(this.groupBoxInput_Enter);
            // 
            // waveChartLeft
            // 
            chartArea1.Name = "ChartArea1";
            this.waveChartLeft.ChartAreas.Add(chartArea1);
            this.waveChartLeft.Location = new System.Drawing.Point(19, 120);
            this.waveChartLeft.Name = "waveChartLeft";
            series1.ChartArea = "ChartArea1";
            series1.Name = "Series1";
            this.waveChartLeft.Series.Add(series1);
            this.waveChartLeft.Size = new System.Drawing.Size(596, 132);
            this.waveChartLeft.TabIndex = 7;
            this.waveChartLeft.Text = "chart1";
            // 
            // waveChartRight
            // 
            chartArea2.Name = "ChartArea1";
            this.waveChartRight.ChartAreas.Add(chartArea2);
            this.waveChartRight.Location = new System.Drawing.Point(19, 258);
            this.waveChartRight.Name = "waveChartRight";
            series2.ChartArea = "ChartArea1";
            series2.Name = "Series1";
            this.waveChartRight.Series.Add(series2);
            this.waveChartRight.Size = new System.Drawing.Size(596, 132);
            this.waveChartRight.TabIndex = 8;
            this.waveChartRight.Text = "chart1";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.threads16);
            this.groupBox1.Controls.Add(this.threads8);
            this.groupBox1.Controls.Add(this.threads4);
            this.groupBox1.Controls.Add(this.threads2);
            this.groupBox1.Controls.Add(this.threads1);
            this.groupBox1.Controls.Add(this.runButton);
            this.groupBox1.Location = new System.Drawing.Point(221, 68);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(397, 46);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Number of threads";
            // 
            // threads16
            // 
            this.threads16.AutoSize = true;
            this.threads16.Location = new System.Drawing.Point(154, 19);
            this.threads16.Name = "threads16";
            this.threads16.Size = new System.Drawing.Size(37, 17);
            this.threads16.TabIndex = 9;
            this.threads16.TabStop = true;
            this.threads16.Text = "16";
            this.threads16.UseVisualStyleBackColor = true;
            this.threads16.CheckedChanged += new System.EventHandler(this.threads16_CheckedChanged);
            // 
            // threads8
            // 
            this.threads8.AutoSize = true;
            this.threads8.Location = new System.Drawing.Point(117, 19);
            this.threads8.Name = "threads8";
            this.threads8.Size = new System.Drawing.Size(31, 17);
            this.threads8.TabIndex = 8;
            this.threads8.TabStop = true;
            this.threads8.Text = "8";
            this.threads8.UseVisualStyleBackColor = true;
            this.threads8.CheckedChanged += new System.EventHandler(this.threads8_CheckedChanged);
            // 
            // threads4
            // 
            this.threads4.AutoSize = true;
            this.threads4.Location = new System.Drawing.Point(80, 19);
            this.threads4.Name = "threads4";
            this.threads4.Size = new System.Drawing.Size(31, 17);
            this.threads4.TabIndex = 7;
            this.threads4.TabStop = true;
            this.threads4.Text = "4";
            this.threads4.UseVisualStyleBackColor = true;
            this.threads4.CheckedChanged += new System.EventHandler(this.threads4_CheckedChanged);
            // 
            // threads2
            // 
            this.threads2.AutoSize = true;
            this.threads2.Location = new System.Drawing.Point(43, 19);
            this.threads2.Name = "threads2";
            this.threads2.Size = new System.Drawing.Size(31, 17);
            this.threads2.TabIndex = 6;
            this.threads2.TabStop = true;
            this.threads2.Text = "2";
            this.threads2.UseVisualStyleBackColor = true;
            this.threads2.CheckedChanged += new System.EventHandler(this.threads2_CheckedChanged);
            // 
            // threads1
            // 
            this.threads1.AutoSize = true;
            this.threads1.Location = new System.Drawing.Point(6, 19);
            this.threads1.Name = "threads1";
            this.threads1.Size = new System.Drawing.Size(31, 17);
            this.threads1.TabIndex = 5;
            this.threads1.TabStop = true;
            this.threads1.Text = "1";
            this.threads1.UseVisualStyleBackColor = true;
            this.threads1.CheckedChanged += new System.EventHandler(this.threads1_CheckedChanged);
            // 
            // runButton
            // 
            this.runButton.Location = new System.Drawing.Point(313, 13);
            this.runButton.Name = "runButton";
            this.runButton.Size = new System.Drawing.Size(75, 23);
            this.runButton.TabIndex = 4;
            this.runButton.Text = "Run";
            this.runButton.UseVisualStyleBackColor = true;
            this.runButton.Click += new System.EventHandler(this.runButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(630, 462);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.waveChartRight);
            this.Controls.Add(this.waveChartLeft);
            this.Controls.Add(this.groupBoxInput);
            this.Controls.Add(this.groupBoxLib);
            this.Controls.Add(this.progressBar);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBoxLib.ResumeLayout(false);
            this.groupBoxLib.PerformLayout();
            this.groupBoxInput.ResumeLayout(false);
            this.groupBoxInput.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.waveChartLeft)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.waveChartRight)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.RadioButton optionCpp;
        private System.Windows.Forms.RadioButton optionAsm;
        private System.Windows.Forms.GroupBox groupBoxLib;
        private System.Windows.Forms.OpenFileDialog dialogOpenWAV;
        private System.Windows.Forms.TextBox wavForm;
        private System.Windows.Forms.Button browseButton;
        private System.Windows.Forms.GroupBox groupBoxInput;
        private System.Windows.Forms.DataVisualization.Charting.Chart waveChartLeft;
        private System.Windows.Forms.DataVisualization.Charting.Chart waveChartRight;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton threads1;
        private System.Windows.Forms.Button runButton;
        private System.Windows.Forms.RadioButton threads2;
        private System.Windows.Forms.RadioButton threads8;
        private System.Windows.Forms.RadioButton threads4;
        private System.Windows.Forms.RadioButton threads16;
    }
}

