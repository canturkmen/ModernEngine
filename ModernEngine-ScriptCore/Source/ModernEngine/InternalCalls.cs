using System;
using System.Runtime.CompilerServices;

namespace ModernEngine
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void Entity_GetTranslation(ulong uuid, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetTranslation(ulong uuid, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsKeyDown(KeyCode keycode);
    }
}
