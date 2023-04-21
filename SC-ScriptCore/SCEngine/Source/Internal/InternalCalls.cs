using System;
using System.Runtime.CompilerServices;

namespace SCEngineInternal
{
	public static class InternalCalls
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Debug_Log(string text);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static SCEngine.Vector2 Vector2_Normalise(ref SCEngine.Vector2 vec);
		
		// Entity Methods
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Entity_GetTransform(ulong eid, out SCEngine.Transform transform);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Entity_SetTransform(ulong eid, ref SCEngine.Transform transform);


		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static object Entity_GetComponent(ulong eid);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static object Entity_AddComponent(ulong eid);

		// Transform methods
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Transform_GetPosition(ulong eid, ref SCEngine.Vector2 pos);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Transform_SetPosition(ulong eid, ref SCEngine.Vector2 pos);
	}
}
