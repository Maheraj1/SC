namespace SCEngine
{
	public class Entity
	{
		public readonly ulong id;
		protected Entity() => id = 0;

		internal Entity(ulong id) => this.id = id;
	}
}