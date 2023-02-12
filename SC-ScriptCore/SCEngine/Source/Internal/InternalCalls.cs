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
		internal extern static SCEngine.Transform Entity_GetTransform();

		// Transform methods
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static SCEngine.Vector2 Transform_GetPosition();
	}
}
