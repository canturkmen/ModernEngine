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
        private TransformComponent m_TransformComponent;
        private Rigidbody2DComponent m_RigidbodyComponent;

        public float Speed;

        void OnCreate()
        {
            m_TransformComponent = GetComponent<TransformComponent>();
            m_RigidbodyComponent = GetComponent<Rigidbody2DComponent>();
        }

        void OnUpdate(float ts)
        {
            float speed = Speed;
            Vector3 Velocity = Vector3.Zero;

            if(Input.IsKeyDown(KeyCode.W))
                Velocity.Y = 1.0f;
            
            else if(Input.IsKeyDown(KeyCode.S))
                Velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.D))
                Velocity.X = 1.0f;

            else if (Input.IsKeyDown(KeyCode.A))
                Velocity.X = -1.0f;

            Entity cameraEntity = FindEntityByName("Camera");
            if(cameraEntity != null)
            {
                Camera camera = cameraEntity.As<Camera>();

                if (Input.IsKeyDown(KeyCode.Q))
                    camera.DistanceFromCamera -= speed * 5 * ts;

                else if (Input.IsKeyDown(KeyCode.E))
                    camera.DistanceFromCamera += speed * 5 * ts;
            }

            Velocity *= speed;

            m_RigidbodyComponent.ApplyLinearImpulse(Velocity.XY, true);

            // Vector3 translation = m_TransformComponent.Translation;
            // translation += Velocity * ts;
            // m_TransformComponent.Translation = translation;
        }
    }
}
  