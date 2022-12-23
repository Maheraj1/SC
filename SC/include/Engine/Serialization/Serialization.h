#pragma once

#include <string>
namespace SC {
	class Entity;
}
namespace SC::Serialization {
	class Serialization {
		using CreateFunc = void(*)(Entity*);
		public:
			std::string GetComponentNameByTypeName(std::string name);
			CreateFunc GetComponentCreateFuncByTypeName(std::string name);
	};
}
