using System;
using System.Collections.Generic;
using System.IO;

namespace Config2
{
    namespace ConfigData
    {
        public enum VideoMode
        {
            VM2560X1440 = 0,
            VM1920X1080 = 1,
            VM1820X1024 = 2,
            VM1440X810 = 3,
            VM1280X720 = 4,
            VM1024X576 = 5,
            VM800X450 = 6,

            VMFSDesktop = 0,
            VMFS1280x720 = 1
        }

        public enum FPSMode
        {
            FPSNoLimit = 0,
            FPS120 = 1,
            FPS60 = 2,
            FPS30 = 3,
            FPS20 = 4
        }

        public enum TextureFliter
        {
            Linear = 0,
            Fast = 1
        }

        public enum MSAAMode
        {
            MSAA16X = 0,
            MSAA8X = 1,
            MSAA4X = 2,
            MSAA2X = 3,
            MSAAOff = 4
        }

        public class Data
        {
            public VideoMode VideoModeSelect { get; set; } = VideoMode.VM1280X720;
            public bool Fullscreen { get; set; } = false;
            public FPSMode FPSModeSelect { get; set; } = FPSMode.FPS60;
            public TextureFliter TextureFliterMode { get; set; } = TextureFliter.Linear;
            public bool VSyncEnabled { get; set; } = true;
            public MSAAMode MSAAModeSelect { get; set; } = MSAAMode.MSAA4X;
            public bool BeautifulBackground { get; set; } = true;
            public bool PreloadBGM { get; set; } = true;


            private void DataLimit(byte val,int max)
            {
                if (val > max) throw new Exception("Bad Config Data.");
            }

            public void Load(byte[] bytes)
            {
                if (bytes.Length != 8) throw new Exception("Bad Config Data.");

                DataLimit(bytes[0], bytes[1] > 0 ? 1 : 6);
                DataLimit(bytes[1], 1);
                DataLimit(bytes[2], 4);
                DataLimit(bytes[3], 2);
                DataLimit(bytes[4], 1);
                DataLimit(bytes[5], 4);
                DataLimit(bytes[6], 1);
                DataLimit(bytes[7], 8);

                VideoModeSelect = (VideoMode)bytes[0];
                Fullscreen = bytes[1] > 0;
                FPSModeSelect = (FPSMode)bytes[2];
                TextureFliterMode = (TextureFliter)bytes[3];
                VSyncEnabled = bytes[4] > 0;
                MSAAModeSelect = (MSAAMode)bytes[5];
                BeautifulBackground = bytes[6] > 0;
                PreloadBGM = bytes[7] > 0;
            }

            public byte[] Save()
            {
                byte[] b = new byte[8];

                b[0] = (byte)VideoModeSelect;
                b[1] = Fullscreen ? (byte)1 : (byte)0;
                b[2] = (byte)FPSModeSelect;
                b[3] = (byte)TextureFliterMode;
                b[4] = VSyncEnabled ? (byte)1 : (byte)0;
                b[5] = (byte)MSAAModeSelect;
                b[6] = BeautifulBackground ? (byte)1 : (byte)0;
                b[7] = PreloadBGM ? (byte)1 : (byte)0;

                return b;
            }
        }

        
    }
}
