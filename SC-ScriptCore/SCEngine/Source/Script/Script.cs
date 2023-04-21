using System.Diagnostics.CodeAnalysis;

namespace SCEngine
{
	public class Script {
		Entity entity { get; set; }
		Transform transform { get => entity.transform; set => entity.transform = value; }
	}	
}