using System.Collections.Generic;

namespace DebugSkipper
{
    public struct StartPoint
    {
        public string Info;
        public int Time;
    }

    public struct StagePoints
    {
        public List<StartPoint> Reimu, Marisa;
    }
}
