using System;
using System.Runtime.CompilerServices;

namespace ModernEngine
{   
    public class Entity
    {
        protected Entity()
        { 
            ID = 0; 
        }   
        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 translation);
                return translation;
            }

            set
            {
                InternalCalls.TransformComponent_SetTranslation(ID, ref value);
            }
        }

        public bool HasComponent<T>()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }
        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = this };   
            return component;
        }
        public Entity FindEntityByName(string name)
        {
            ulong entityId = InternalCalls.Entity_FindEntityByName(name);
            if (entityId == 0)
                return null;

            return new Entity(entityId);
        }
        public T As<T>() where T : Entity, new()
        {
            object scriptInstance = InternalCalls.Entity_GetScriptInstance(ID);
            return scriptInstance as T;
        }
    }
}
