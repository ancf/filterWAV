using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace filterWAV
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
           
        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            interfaceC cppInterface = new interfaceC();
            interfaceASM asmInterface = new interfaceASM();
            string result = "";
            double x = 5.25;
            double y = 2.5;
            if (radioButton1.Checked)
            {
                result += "Cpp picked. Result: ";
                double additionResult = cppInterface.callCFunction(x, y);
                result += additionResult;



            }
            else if (radioButton2.Checked)
            {
                result += "ASM picked. Result: ";
                double substractionResult = asmInterface.callAsmFunction(x, y);
                result += substractionResult;
            }
            MessageBox.Show(result);
        }
    }
}
