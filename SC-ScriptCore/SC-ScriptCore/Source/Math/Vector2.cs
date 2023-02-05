using System.Diagnostics.CodeAnalysis;

namespace SCEngine
{
	public struct Vector2
	{
		public float x, y;
		public float sqMagnitude {
			get => (x*x) + (y*y);
		}
		public float magnitude {
			get => Mathf.Sqrt(sqMagnitude);
		}

		public Vector2 normalised {
			get => SCEngineInternal.InternalCalls.Vector2_Normalise(ref this);
		}

		public Vector2(float scaler)
		{
			this.x = scaler;
			this.y = scaler;
		}

		public Vector2(float x = 0, float y = 0)
		{
			this.x = x;
			this.y = y;
		}

		public static Vector2 operator +(Vector2 a, Vector2 b) {
			return new Vector2(a.x + b.x, a.y + b.y);
		}

		public static Vector2 operator -(Vector2 a, Vector2 b) {
			return new Vector2(a.x - b.x, a.y - b.y);
		}

		public static Vector2 operator *(Vector2 a, float scaler) {
			return new Vector2(a.x * scaler, a.y * scaler);
		}

		public static Vector2 operator /(Vector2 a, float scaler) {
			return new Vector2(a.x / scaler, a.y / scaler);
		}

		static float Distance(Vector2 a, Vector2 b) {
			return (b - a).magnitude;
		}
		
		static float GetAngle(Vector2 a, Vector2 b) {
			Vector2 v = (b - a).normalised;
			return Mathf.Atan2(v.x, v.y) * Mathf.Rad2Deg;
		}

		void Normalise() => this = SCEngineInternal.InternalCalls.Vector2_Normalise(ref this);
	}
}