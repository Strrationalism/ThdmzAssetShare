using System;
using System.Collections.Generic;
using System.Text;

namespace DebugSkipper
{
    public static class StartTimeData
    {
        static StartTimeData()
        {
            Data = new Dictionary<string, StagePoints>
            {
                ["Stage1"] = LoadStage1(),
                ["Stage2"] = LoadStage2(),
                ["Stage3"] = LoadStage3(),
                ["Stage4"] = LoadStage4(),
                ["Stage5"] = LoadStage5(),
                ["Stage6"] = LoadStage6(),
                ["StageEx"] = LoadStageEx(),
                ["Random"] = LoadEmpty()
            };

        }

        static StagePoints LoadEmpty()
        {
            var sp = new StagePoints
            {
                Marisa = new List<StartPoint>(),
                Reimu = new List<StartPoint>()
            };
            return sp;
        }

        static StagePoints LoadStageEx()
        {
            var sp = LoadEmpty();

            sp.Reimu.Add(new StartPoint { Info = "道中一非", Time = 8815 });
            sp.Reimu.Add(new StartPoint { Info = "道中一符", Time = 11349 });
            sp.Reimu.Add(new StartPoint { Info = "道中二符", Time = 13849 });
            sp.Reimu.Add(new StartPoint { Info = "关底一非", Time = 43178 });
            sp.Reimu.Add(new StartPoint { Info = "关底一符", Time = 45700 });
            sp.Reimu.Add(new StartPoint { Info = "关底二非", Time = 50246 });

            sp.Marisa.Add(new StartPoint { Info = "道中一非", Time = 8815 });
            sp.Marisa.Add(new StartPoint { Info = "道中一符", Time = 11349 });
            sp.Marisa.Add(new StartPoint { Info = "道中二符", Time = 13849 });

            return sp;
        }

        static StagePoints LoadStage4()
        {
            var sp = LoadEmpty();

            sp.Reimu.Add(new StartPoint { Info = "关底一非", Time = 30426 });
            sp.Reimu.Add(new StartPoint { Info = "关底一符", Time = 35106 });
            sp.Reimu.Add(new StartPoint { Info = "关底二非", Time = 39606 });
            sp.Reimu.Add(new StartPoint { Info = "关底二符", Time = 44329 });
            sp.Reimu.Add(new StartPoint { Info = "关底三非", Time = 48854 });
            sp.Reimu.Add(new StartPoint { Info = "关底三符", Time = 53600 });
            sp.Reimu.Add(new StartPoint { Info = "关底四非", Time = 58074 });
            sp.Reimu.Add(new StartPoint { Info = "关底四符", Time = 62770 });

            sp.Marisa.Add(new StartPoint { Info = "关底一非", Time = 26325 });
            sp.Marisa.Add(new StartPoint { Info = "关底一符", Time = 30990 });
            sp.Marisa.Add(new StartPoint { Info = "关底二非", Time = 35525 });
            sp.Marisa.Add(new StartPoint { Info = "关底二符", Time = 40027 });
            sp.Marisa.Add(new StartPoint { Info = "关底三非", Time = 44715 });
            sp.Marisa.Add(new StartPoint { Info = "关底三符", Time = 49458 });
            sp.Marisa.Add(new StartPoint { Info = "关底四非", Time = 53965 });
            sp.Marisa.Add(new StartPoint { Info = "关底四符", Time = 58634 });

            return sp;
        }

        static StagePoints LoadStage5()
        {
            var sp = LoadEmpty();

            var move = 65400 - 57294 + 10000 - 4600 - 3000 - 300 -100;

            sp.Reimu.Add(new StartPoint { Info = "道中一非", Time = 8008 });
            sp.Reimu.Add(new StartPoint { Info = "道中二非", Time = 12658 });
            sp.Reimu.Add(new StartPoint { Info = "关底一非", Time = 57294+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底一符", Time = 62030+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底二非", Time = 66674+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底二符", Time = 69250+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底三非", Time = 73881+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底三符", Time = 76468+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底四非", Time = 81105+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底四符", Time = 83720+ move });
            sp.Reimu.Add(new StartPoint { Info = "关底五符", Time = 88329+ move });

            sp.Marisa.Add(new StartPoint { Info = "道中一非", Time = 8008 });
            sp.Marisa.Add(new StartPoint { Info = "道中二非", Time = 12658 });
            sp.Marisa.Add(new StartPoint { Info = "关底一非", Time = 67479+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底一符", Time = 72172+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底二非", Time = 76761+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底二符", Time = 79367+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底三非", Time = 84022+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底三符", Time = 86627+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底四非", Time = 91222+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底四符", Time = 93834+ move });
            sp.Marisa.Add(new StartPoint { Info = "关底五符", Time = 98593+ move });

            return sp;
        }

        static StagePoints LoadStage1()
        {
            var sp = LoadEmpty();
 
            sp.Reimu.Add(new StartPoint { Info = "道中大妖精", Time = 5261 });
            sp.Reimu.Add(new StartPoint { Info = "道中琪露诺 一非", Time = 10227 });
            sp.Reimu.Add(new StartPoint { Info = "道中琪露诺 一符", Time = 12788 });
            sp.Reimu.Add(new StartPoint { Info = "关底一非", Time = 20700 });
            sp.Reimu.Add(new StartPoint { Info = "关底一符", Time = 25255 });
            sp.Reimu.Add(new StartPoint { Info = "关底二非", Time = 29814 });
            sp.Reimu.Add(new StartPoint { Info = "关底二符", Time = 34374 });
            sp.Reimu.Add(new StartPoint { Info = "关底三符", Time = 39084 });

            sp.Marisa.Add(new StartPoint { Info = "道中大妖精", Time = 5261 });
            sp.Marisa.Add(new StartPoint { Info = "道中琪露诺 一非", Time = 10227 });
            sp.Marisa.Add(new StartPoint { Info = "道中琪露诺 一符", Time = 12788 });
            sp.Marisa.Add(new StartPoint { Info = "关底一非", Time = 19663 });
            sp.Marisa.Add(new StartPoint { Info = "关底一符", Time = 24166 });
            sp.Marisa.Add(new StartPoint { Info = "关底二非", Time = 28711 });
            sp.Marisa.Add(new StartPoint { Info = "关底二符", Time = 33284 });
            sp.Marisa.Add(new StartPoint { Info = "关底三符", Time = 37904 });

            return sp;
        }

        static StagePoints LoadStage2()
        {
            var sp = LoadEmpty();

            sp.Reimu.Add(new StartPoint { Info = "道中一非", Time = 9104 });
            sp.Reimu.Add(new StartPoint { Info = "道中一符", Time = 11694 });
            sp.Reimu.Add(new StartPoint { Info = "道中二符", Time = 14289 });
            sp.Reimu.Add(new StartPoint { Info = "关底一非", Time = 22692 });
            sp.Reimu.Add(new StartPoint { Info = "关底一符", Time = 27238 });
            sp.Reimu.Add(new StartPoint { Info = "关底二非", Time = 31739 });
            sp.Reimu.Add(new StartPoint { Info = "关底二符", Time = 36346 });
            sp.Reimu.Add(new StartPoint { Info = "关底三符", Time = 40967 });

            sp.Marisa.Add(new StartPoint { Info = "道中一非", Time = 9104 });
            sp.Marisa.Add(new StartPoint { Info = "道中一符", Time = 11694 });
            sp.Marisa.Add(new StartPoint { Info = "道中二符", Time = 14289 });
            sp.Marisa.Add(new StartPoint { Info = "关底一非", Time = 25700 });
            sp.Marisa.Add(new StartPoint { Info = "关底一符", Time = 30383 });
            sp.Marisa.Add(new StartPoint { Info = "关底二非", Time = 34858 });
            sp.Marisa.Add(new StartPoint { Info = "关底二符", Time = 39472 });
            sp.Marisa.Add(new StartPoint { Info = "关底三符", Time = 44117 });

            return sp;
        }

        static StagePoints LoadStage3()
        {
            var sp = LoadEmpty();

            sp.Reimu.Add(new StartPoint { Info = "道中一非", Time = 12753 });
            sp.Reimu.Add(new StartPoint { Info = "关底一非", Time = 41318 });
            sp.Reimu.Add(new StartPoint { Info = "关底一符", Time = 45821 });
            sp.Reimu.Add(new StartPoint { Info = "关底二非", Time = 50340 });
            sp.Reimu.Add(new StartPoint { Info = "关底二符", Time = 54963 });
            sp.Reimu.Add(new StartPoint { Info = "关底三非", Time = 59581 });
            sp.Reimu.Add(new StartPoint { Info = "关底三符", Time = 64092 });
            sp.Reimu.Add(new StartPoint { Info = "关底四符", Time = 66190 });

            sp.Marisa.Add(new StartPoint { Info = "道中一非", Time = 12753 });
            sp.Marisa.Add(new StartPoint { Info = "关底一非", Time = 40207 });
            sp.Marisa.Add(new StartPoint { Info = "关底一符", Time = 44813 });
            sp.Marisa.Add(new StartPoint { Info = "关底二非", Time = 49393 });
            sp.Marisa.Add(new StartPoint { Info = "关底二符", Time = 53920 });
            sp.Marisa.Add(new StartPoint { Info = "关底三非", Time = 58407 });
            sp.Marisa.Add(new StartPoint { Info = "关底三符", Time = 63062 });
            sp.Marisa.Add(new StartPoint { Info = "关底四符", Time = 65210 });

            return sp;
        }

        static StagePoints LoadStage6()
        {
            var sp = LoadEmpty();

            var baseTime = 3900;

            sp.Reimu.Add(new StartPoint { Info = "一非", Time = 21690 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "一符", Time = 26241 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "二非", Time = 30734 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "二符", Time = 35362 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "三非", Time = 39767 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "三符", Time = 44547 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "四非", Time = 49007 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "四符", Time = 53638 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "五非", Time = 58125 + baseTime });
            sp.Reimu.Add(new StartPoint { Info = "五符", Time = 62744 + baseTime });

            sp.Marisa.Add(new StartPoint { Info = "一非", Time = 17566 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "一符", Time = 22144 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "二非", Time = 26652 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "二符", Time = 31260 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "三非", Time = 35766 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "三符", Time = 40382 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "四非", Time = 44889 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "四符", Time = 49507 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "五非", Time = 54035 + baseTime });
            sp.Marisa.Add(new StartPoint { Info = "五符", Time = 58676 + baseTime });


            return sp;
        }

        public readonly static IDictionary<string, StagePoints> Data;
    }
}
