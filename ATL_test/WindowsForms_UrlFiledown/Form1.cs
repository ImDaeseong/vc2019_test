using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using UrlFileDownLib;

namespace WindowsForms_UrlFiledown
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string sUrl = "http://127.0.0.1/test.zip";
            string sFile = "c:\\test.zip";

            UrlFileDownLib.HttpDown obj = new UrlFileDownLib.HttpDown();
            bool bDown = obj.GetDownloadFile(sUrl, sFile);
            if (bDown)
            {
                MessageBox.Show("download successed");
            }
            else
            {
                MessageBox.Show("download failed");
            }
        }
    }
}
