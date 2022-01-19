using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace vfddesigner
{
    public partial class Form1 : Form
    {

        UInt32 thirtytwobitone = 1;
        UInt32 code = 0;
        public int asciicode = 48;
        public Form1()
        {
            InitializeComponent();
            updateCode();
        }

        public void updateCode()
        {
            String binary = (Convert.ToString(code, 2)).PadLeft(20,'0');

            this.outTextBox.Text = "0b" +(binary) + ",  //" + Convert.ToChar(asciicode).ToString();          
            this.charTextBox.Text = Convert.ToChar(asciicode).ToString();
            this.asciiTextBox.Text = Convert.ToString(asciicode);
 
        }

        private void nextbtn_Click(object sender, EventArgs e)
        {
            asciicode = Convert.ToInt16(asciiTextBox.Text);
            String binary = (Convert.ToString(code, 2)).PadLeft(20, '0');
            this.outLines.AppendText("0b" + (binary) + ",  //" + Convert.ToChar(asciicode).ToString() + "\r\n");
            asciicode++; //inc ascii code to next CHAR
            updateCode();

        }
        private void toggleColor(object sent, int index)
        {
            Button clickedButton = (Button)sent;
            if (clickedButton.BackColor.Equals(Color.Yellow))
            {
                clickedButton.BackColor = Color.Transparent;
                code = code ^ (thirtytwobitone << index); //xor to get rid of bit
            }
            else
            {
                clickedButton.BackColor = Color.Yellow;
                code = code | (thirtytwobitone << index);   //or to get bit
            }

            updateCode();
        }

        private void segbut_a_Click(object sender, EventArgs e)
        {
            toggleColor(sender,0);

        }

        private void segbut_b_Click(object sender, EventArgs e)
        {
            toggleColor(sender,1);
        }

        private void segbut_c_Click(object sender, EventArgs e)
        {
            toggleColor(sender,2);
        }

        private void segbut_d_Click(object sender, EventArgs e)
        {
            toggleColor(sender,3);
        }

        private void segbut_e_Click(object sender, EventArgs e)
        {
            toggleColor(sender,4);
        }

        private void segbut_f_Click(object sender, EventArgs e)
        {
            toggleColor(sender,5);
        }

        private void segbut_g_Click(object sender, EventArgs e)
        {
            toggleColor(sender,6);
        }

        private void segbut_h_Click(object sender, EventArgs e)
        {
            toggleColor(sender,7);
        }

        private void segbut_i_Click(object sender, EventArgs e)
        {
            toggleColor(sender,8);
        }

        private void segbut_j_Click(object sender, EventArgs e)
        {
            toggleColor(sender,9);
        }

        private void segbut_k_Click(object sender, EventArgs e)
        {
            toggleColor(sender,10);
        }

        private void segbut_l_Click(object sender, EventArgs e)
        {
            toggleColor(sender,11);
        }

        private void segbut_m_Click(object sender, EventArgs e)
        {
            toggleColor(sender,12);
        }

        private void segbut_n_Click(object sender, EventArgs e)
        {
            toggleColor(sender,13);
        }

        private void segbut_o_Click(object sender, EventArgs e)
        {
            toggleColor(sender,14);
        }

        private void segbut_p_Click(object sender, EventArgs e)
        {
            toggleColor(sender,15);
        }
        private void segbut_17_Click(object sender, EventArgs e)
        {
            toggleColor(sender,16);
        }

        private void segbut_18_Click(object sender, EventArgs e)
        {
            toggleColor(sender,17);
        }

    }
}
