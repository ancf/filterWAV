using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace filterWAV
{
    
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            waveChartLeft.Series.Clear();
            waveChartRight.Series.Clear();
        }

        
        private static double bytesToDouble(byte firstByte, byte secondByte)
        {
          
            short s = (short)((secondByte << 8) | firstByte);
            return s / 32768.0;
        }

      
        public void openWav(string filename, out double[] left, out double[] right)
        {
            byte[] wav = File.ReadAllBytes(filename);

            char[] rawRIFF = new char[4]{ (char)wav[0], (char)wav[1], (char)wav[2], (char)wav[3] };
            string riff = new string(rawRIFF);

            int fileSize = BitConverter.ToInt32(wav, 4);

            char[] rawWAVE = new char[4] { (char)wav[8], (char)wav[9], (char)wav[10], (char)wav[11] };
            string wave = new string(rawWAVE);
            
            byte[] fmt = new byte[4]{ wav[12], wav[13], wav[14], wav[15]};

            byte[] formatByteSize = new byte[4] { wav[16], wav[17], wav[18], wav[19] };

            int channels = BitConverter.ToInt16(wav,22);    
            
            int sampleRate = BitConverter.ToInt32(wav, 24);

            int bytesPerSecond = BitConverter.ToInt32(wav, 28);

            int bytesPerSample = BitConverter.ToInt16(wav, 32);

            int bitsPerSample = BitConverter.ToInt16(wav, 34);

            char[] rawData = new char[4] { (char)wav[36], (char)wav[37], (char)wav[38], (char)wav[39] };

            string data = new string(rawData);

            int dataSize = BitConverter.ToInt32(wav, 40);

            string msg = "Format: " + riff + "\n File size: " + fileSize + "\n File type: " + wave + "\n Sample Rate: "  + sampleRate + "\n Number of channels: " + channels 
                + "\n Bytes Per Second: " + bytesPerSecond + "\n Bytes per Sample: " + bytesPerSample + "\n Bits per Sample: " 
                + bitsPerSample + "\n Data: " + data + "\n Data size: " + dataSize;

            MessageBox.Show(msg);
           
            
            int pos = 44;
           
            int samples = dataSize / bytesPerSample;
      

            
            left = new double[samples];
            if (channels == 2) right = new double[samples];
            else right = null; // if mono then right array is null

            int i = 0;
           while (pos < samples)
            {
               



                left[i] = bytesToDouble(wav[pos], wav[pos + 1]);
                pos += 2;
                if (channels == 2)
                {
                    right[i] = bytesToDouble(wav[pos], wav[pos + 1]);
                    pos += 2;
                }
                i++;
            }
        }

        private void optionCpp_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void optionAsm_CheckedChanged(object sender, EventArgs e)
        {

        }

        private unsafe void browseButton_Click(object sender, EventArgs e)
        {
            if (dialogOpenWAV.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    var filePath = dialogOpenWAV.FileName;
                   
                     double[] left = new double[] { };
                      double[] right = new double[] { };
                      this.openWav(filePath, out left, out right);

                    waveChartLeft.Series.Clear();
                    waveChartRight.Series.Clear();
                    Series seriesL = waveChartLeft.Series.Add("WaveLeft");
                    Series seriesR = waveChartRight.Series.Add("WaveRight");

                    seriesL.ChartType = SeriesChartType.Spline;
                    seriesR.ChartType = SeriesChartType.Spline;
                    int samplesPerPixel = left.Length / (2 * (int)waveChartLeft.ChartAreas[0].Position.Width);  
                    int step = 750;
                    for(int i = 0; i< left.Length - step; i+=step)
                    {
                        double leftLow = left[i];
                        double leftHigh = left[i];
                        double leftLowX = i;
                        double leftHighX = i;

                        double rightLow = left[i];
                        double rightHigh = left[i];
                        double rightLowX = i;
                        double rightHighX = i;

                        for (int j = 0; j < step; j++)
                        {
                            
                            if (left[i + j] < leftLow)
                            {
                                leftLow = left[i + j];
                                leftLowX = i + j;
                            }
                            if (left[i + j] > leftHigh)
                            {
                                leftHigh = left[i + j];
                                leftHighX = i + j;
                            }
                            if (right[i + j] < rightLow)
                            {
                                rightLow = right[i + j];
                                rightLowX = i + j;
                            }
                            if (right[i + j] > rightHigh)
                            {
                                rightHigh = right[i + j];
                                rightHighX = i + j;
                            }

                        }
                        
                          seriesL.Points.AddXY(leftLowX, leftLow);
                          seriesL.Points.AddXY(leftHighX, leftHigh);

                          seriesR.Points.AddXY(rightLowX, rightLow);
                          seriesR.Points.AddXY(rightHighX, rightHigh);


                    }
                  //  waveChart.ChartAreas[0].AxisX.Maximum = 1900;

                    string msg = "Array is: " + samplesPerPixel;
                     MessageBox.Show(msg); 
                }
                catch (SecurityException ex)
                {
                    MessageBox.Show($"Security error.\n\nError message: {ex.Message}\n\n" +
                    $"Details:\n\n{ex.StackTrace}");
                }
            }
        }
    

        private void groupBoxInput_Enter(object sender, EventArgs e)
        {

        }

        private void runButton_Click(object sender, EventArgs e)
        {
            interfaceC cppInterface = new interfaceC();
            interfaceASM asmInterface = new interfaceASM();
            string result = "";
            double x = 5.25;
            double y = 2.5;
            if (optionCpp.Checked)
            {
                result += "Cpp picked. Result: ";
                double additionResult = cppInterface.callCFunction(x, y);
                result += additionResult;



            }
            else if (optionAsm.Checked)
            {
                result += "ASM picked. Result: ";
                double substractionResult = asmInterface.callAsmFunction(x, y);
                result += substractionResult;
            }
            MessageBox.Show(result);
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void handleThreads(int numberOfThreads, int sampleCount)
        {
            int size = sampleCount;
           
           

            
            float sectionSizeFloat = size / numberOfThreads;
            uint sectionSize = (uint)Math.Ceiling(sectionSizeFloat);
            uint lowerBoundry;
            uint upperBoundry;

           

            for (int i = 0; i < numberOfThreads; i++)
            {
               // divide sample table(s) for threads
            }





            for (int i = 0; i < numberOfThreads; i++)
            {
               //join threds
            }

        }
    }


}
