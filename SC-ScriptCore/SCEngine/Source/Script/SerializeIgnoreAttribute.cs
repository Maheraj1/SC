using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

[System.AttributeUsage(System.AttributeTargets.Field, Inherited = false, AllowMultiple = false)]
public class SerializeIgnoreAttribute: Attribute
{
	
}