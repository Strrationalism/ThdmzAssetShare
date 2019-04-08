using System;
using System.IO;
using System.Windows.Forms;

namespace Config2
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        /// 

        public static string ConfigPath =
                Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) +
                "\\Strrationalism\\Thdmz\\config.sav";

        [STAThread]
        static void Main()
        {
            ConfigData.Data data = new ConfigData.Data();

            try
            {
                FileStream configData = new FileStream(ConfigPath, FileMode.Open);
                byte[] blob = new byte[8];
                configData.Read(blob, 0, 8);
                data.Load(blob);

                configData.Close();

            }
            catch(Exception)
            {

            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Form form = new Form();
            form.SetToData(data);

            Application.Run(form);
        }
    }
}
