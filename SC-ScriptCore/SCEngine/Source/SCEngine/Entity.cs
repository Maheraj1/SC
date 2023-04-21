namespace SCEngine
{
	public class Entity
	{
		public readonly ulong id;
		protected Entity() => id = 0;

		internal Entity(ulong id) => this.id = id;

		public Transform transform {
			get {
					SCEngineInternal.InternalCalls.Entity_GetTransform(id, out Transform trans);
					return trans;
				}
			set {
				SCEngineInternal.InternalCalls.Entity_SetTransform(id, ref value);
			}
		}

		public T GetComponent<T>() where T : Script {
			// SCEngineInternal.InternalCalls.Entity_GetComponent();

			return null;
		}
	}
}