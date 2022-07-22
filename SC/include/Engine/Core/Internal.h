#pragma once

#include <functional>

namespace Internal 
{
	class ScriptFuncList
	{
	public:
		ScriptFuncList() { }
		~ScriptFuncList() { }

		std::function<void(void)> funcs[4];
	};
	template<typename T>
	ScriptFuncList GetScriptFunctions(T& script)
	{
		ScriptFuncList l;
		l.funcs[0] = [&](){script.Start();};
		l.funcs[1] = [&](){script.Update();};
		l.funcs[2] = [&](){script.Awake();};
		l.funcs[3] = [&](){script.OnDestroy();};
		return l;
	}
}