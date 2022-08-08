using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ModernEngine;

namespace Sandbox
{
    internal class Camera : Entity
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

            float Speed = 0.1f;
            Vector3 Velocity = Vector3.Zero;

            if(Input.IsKeyDown(KeyCode.Up))
                Velocity.Y = 1.0f;
            
            else if(Input.IsKeyDown(KeyCode.Down))
                Velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.Right))
                Velocity.X = 1.0f;

            else if (Input.IsKeyDown(KeyCode.Left))
                Velocity.X = -1.0f;

            Velocity *= Speed;

            Vector3 translation = m_TransformComponent.Translation;
            translation += Velocity * ts;
            m_TransformComponent.Translation = translation;
        }
    }
}
  