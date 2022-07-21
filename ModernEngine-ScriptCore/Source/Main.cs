using System;
using System.Runtime.CompilerServices;

namespace ModernEngine
{   
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void NativeLog(string text, int value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void NativeLog_Vector(ref Vector3 parameter, out Vector3 output);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern float NativeLog_VectorDot(ref Vector3 parameter);
    }
    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y; 
            Z = z;
        }
    }
    public class Entity 
    {
        public float FloatVar { get; set; }
    
        public Entity()
        {
            Console.WriteLine("Main Constructor");

            Log("Can T", 20);

            Vector3 pos = new Vector3(2.5f, 1.0f, 4.0f);
            Vector3 result = Log(pos);
            Console.WriteLine($"{result.X} {result.Y} {result.Z}");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C# Says: {value}"); 
        }

        public void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"C# Says: {value1} and {value2}");
        }

        public void PrintMessage(string message)
        {
            Console.WriteLine($"C# Says: {message}");
        }

        private void Log(string text, int value)
        {
            InternalCalls.NativeLog(text, value);
        }

        private Vector3 Log(Vector3 parameter)
        {
            InternalCalls.NativeLog_Vector(ref parameter, out Vector3 result);
            return result;
        }
    }
}
