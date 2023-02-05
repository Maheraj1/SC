namespace SCEngine
{
	public struct Debug {
		public static void Log(string text) => SCEngineInternal.InternalCalls.Debug_Log(text);
	}	
}