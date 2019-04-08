using System;
using System.Collections.Generic;
using System.Text;

namespace DebugSkipper
{
    public static class SnowRVReader
    {
        public struct Line
        {
            public enum DataType
            {
                String,
                Int,
                Float
            }

            public DataType Type;
            public String Value;
        }

        public static Dictionary<string,Line> ReadRV(string file)
        {
            var rvData = new Dictionary<string, Line>();

            var lines = System.IO.File.ReadAllLines(file);

            foreach(var lineOrg in lines)
            {
                Line lineData;
                if (lineOrg.Trim() == string.Empty) continue;
                var lineA = lineOrg.Trim().Split('=');
                var lineB = lineA[0].Trim().ToUpper().Split(' ');

                lineData.Value = lineA[1].Trim().Trim('\"');
                var type = lineB[0].Trim().ToUpper();

                switch(type)
                {
                    case "INT":
                        lineData.Type = Line.DataType.Int;
                        break;
                    case "FLOAT":
                        lineData.Type = Line.DataType.Float;
                        break;
                    case "STR":
                        lineData.Type = Line.DataType.String;
                        break;
                    default:
                        throw new Exception("Unknown type:" + type);
                        
                }

                rvData[lineB[1].Trim().ToUpper()] = lineData;

            }

            return rvData;
        }
    }
}
