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
        public TransformComponent m_TransformComponent;

        void OnCreate()
        {
            Console.WriteLine("OnCreate Called");
            m_TransformComponent = GetComponent<TransformComponent>();
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine($"OnUpdateCalled: {ts}");

            float Speed = 1.0f;
            Vector3 Velocity = Vector3.Zero;

            if(Input.IsKeyDown(KeyCode.W))
                Velocity.Y = 1.0f;
            
            else if(Input.IsKeyDown(KeyCode.S))
                Velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.D))
                Velocity.X = 1.0f;

            else if (Input.IsKeyDown(KeyCode.A))
                Velocity.X = -1.0f;

            Velocity *= Speed;

            Vector3 translation = Translation;
            translation += Velocity * ts;
            Translation = translation;
        }
    }
}
  