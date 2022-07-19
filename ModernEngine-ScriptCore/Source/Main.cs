using System;

namespace ModernEngine
{
    public class Main
    {
        public float FloatVar { get; set; }
    
        public Main()
        {
            Console.WriteLine("Main Constructor");
        }
    
        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#");
        }
    
        public void PrintMessage(string message)
        {
            Console.WriteLine($"C# Says: {message}");
        }
    }
}
