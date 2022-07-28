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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void Entity_GetTranslation(ulong uuid, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetTranslation(ulong uuid, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsKeyDown(KeyCode keycode);
    }
    public struct Vector3
    {
        public float X, Y, Z;
        public static Vector3 Zero = new Vector3(0.0f);

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar; 
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y; 
            Z = z;
        }

        public static Vector3 operator * (Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }
        public static Vector3 operator + (Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
        }
    }
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

        readonly ulong ID;

        public Vector3 Translation
        {
            get
            {
                InternalCalls.Entity_GetTranslation(ID, out Vector3 translation); 
                return translation;
            }

            set
            {
                InternalCalls.Entity_SetTranslation(ID, ref value);
            }
        }
    }
}
