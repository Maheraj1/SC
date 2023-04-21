using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;


// Marks a Field in a class to be serialized
[System.AttributeUsage(System.AttributeTargets.Field, Inherited = false, AllowMultiple = false)]
public class SerializeFieldAttribute : System.Attribute
{
	
}