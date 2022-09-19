using System;
using System.Runtime.CompilerServices;

namespace ModernEngine
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_HasComponent(ulong uuid, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern ulong Entity_FindEntityByName(string name);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern object Entity_GetScriptInstance(ulong entityId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetTranslation(ulong uuid, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_SetTranslation(ulong uuid, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void Rigidbody2DComponent_ApplyLinearImpulse(ulong uuid, ref Vector2 impulse, ref Vector2 point, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern void Rigidbody2DComponent_ApplyLinearImpulseToCenter(ulong uuid, ref Vector2 impulse, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsKeyDown(KeyCode keycode);
    }
}
