using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SetupHelperLib;

namespace WindowsForms_SetupHelper
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SetupHelperLib.ShareFun obj = new SetupHelperLib.ShareFun();
            textBox1.Text = obj.GetSystemPath();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            SetupHelperLib.ShareFun obj = new SetupHelperLib.ShareFun();
            textBox2.Text = obj.GetProgramFilesPath();
        }
    }
}
