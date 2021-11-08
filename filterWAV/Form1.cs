using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;
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
    public class Data
    {
        int a, b;

             public Data(int a, int b)
        {
            this.a = a;
            this.b = b;
        }
    }
    public partial class Form1 : Form
    {

        private double[] left = new double[] { };
        private double[] right = new double[] { };
        private int threadCount = 1;
        private uint sampleCount;
        Series lSeries, rSeries;
        private interfaceC cppInterface = new interfaceC();
        private interfaceASM asmInterface = new interfaceASM();
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

           
            
            int pos = 44;
           
            uint samples =  (uint)dataSize / (uint)bytesPerSample;

            sampleCount = samples;


            string msg = "Format: " + riff + "\n File size: " + fileSize + "\n File type: " + wave + "\n Sample Rate: " + sampleRate + "\n Number of channels: " + channels
                + "\n Bytes Per Second: " + bytesPerSecond + "\n Bytes per Sample: " + bytesPerSample + "\n Bits per Sample: "
                + bitsPerSample + "\n Data: " + data + "\n Data size: " + dataSize + "\n Sample count" + sampleCount;



            MessageBox.Show(msg);

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
                    this.left = left;
                    this.right = right;
                    waveChartLeft.Series.Clear();
                    waveChartRight.Series.Clear();

                    Series seriesL = waveChartLeft.Series.Add("WaveLeft");
                    Series seriesR = waveChartRight.Series.Add("WaveRight");

                    lSeries = seriesL;
                    rSeries = seriesR;


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
            
            string result = "";
            double x = 5.25;
            double y = 2.5;
            if (optionCpp.Checked)
            {
                result += "Cpp picked. Result: ";
             //   double additionResult = cppInterface.callCFunction(x, y);
              //  result += additionResult;



            }
            else if (optionAsm.Checked)
            {
                result += "ASM picked. Result: ";
             //   double substractionResult = asmInterface.callAsmFunction(x, y);
               // result += substractionResult;
            }
            MessageBox.Show(result);

            handleThreads(sampleCount);
        }

        private void threads1_CheckedChanged(object sender, EventArgs e)
        {
            
        }

        private void threads2_CheckedChanged(object sender, EventArgs e)
        {
            
        }

        private void threads4_CheckedChanged(object sender, EventArgs e)
        {
            
        }

        private void threads8_CheckedChanged(object sender, EventArgs e)
        {
           
        }


        private void threads16_CheckedChanged(object sender, EventArgs e)
        {
         
        }


        private void handleThreads(uint sampleCount)
        {

               
           if(threads1.Checked)
           {
                threadCount = 1;
           } else if(threads2.Checked)
           {
                threadCount = 2;
           } else if (threads4.Checked)
           {
                threadCount = 4;
            }
            else if (threads8.Checked)
            {
                threadCount = 8;
            }
            else if (threads16.Checked)
            {
                threadCount = 16;
            }



            float sectionSizeFloat = sampleCount / threadCount;
            uint sectionSize = (uint)Math.Ceiling(sectionSizeFloat);

            string msg = "";

            Thread[] threadsArray = new Thread[threadCount];
            object[] value = new object[threadCount];
            for(int i = 0; i<threadCount-1;i++) {
                    long bottomLimit = sectionSize * i;
                    long upperToCheck = bottomLimit + sectionSize;
                    long upperLimit = upperToCheck<sampleCount ? upperToCheck : sampleCount; 
                    threadsArray[i] = new Thread(() => cppInterface.callCFunction(ref right, bottomLimit, upperLimit));

                threadsArray[i].Start();


            };
          

            for (int j = 0; j < threadCount-1; j++)
            {
                
                threadsArray[j].Join();
               
            }


            waveChartRight.Series.Clear();
            int samplesPerPixel = left.Length / (2 * (int)waveChartLeft.ChartAreas[0].Position.Width);
            int step = 750;
            for (int i = 0; i < left.Length - step; i += step)
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

             //   seriesL.Points.AddXY(leftLowX, leftLow);
             //   seriesL.Points.AddXY(leftHighX, leftHigh);

                rSeries.Points.AddXY(rightLowX, rightLow);
                rSeries.Points.AddXY(rightHighX, rightHigh);


            }

            for (int k=0; k< threadCount-1; k++)
            {
                msg += "\n Thread: " + value[k];
                MessageBox.Show(msg);
            }

           

       




           

        }
    }


}
