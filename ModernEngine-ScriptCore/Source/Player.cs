using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ModernEngine;

namespace Sandbox
{
    internal class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine("OnCreate Called");
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine($"OnUpdateCalled: {ts}");
        }
    }
}
