using System.Diagnostics.CodeAnalysis;

namespace SCEngine
{
	public struct Mathf
	{
		public const float PI = 3.14159274F;
		public const float Rad2Deg = 57.29578F;
		public const float Deg2Rad = 0.0174532924F;


		public static float Sqrt(float n) {
			return System.MathF.Sqrt(n);
		}
		public static float Pow(float n, float p) {
			return System.MathF.Pow(n, p);
		}

		public static float Sin(float n) {
			return System.MathF.Sin(n);
		}
		public static float Cos(float n) {
			return System.MathF.Cos(n);
		}
		public static float Tan(float n) {
			return System.MathF.Tan(n);
		}

		public static float Asin(float n) {
			return System.MathF.Asin(n);
		}
		public static float Acos(float n) {
			return System.MathF.Acos(n);
		}
		public static float Atan(float n) {
			return System.MathF.Atan(n);
		}
		public static float Atan2(float x, float y) {
			return System.MathF.Atan2(x, y);
		}
	}
}