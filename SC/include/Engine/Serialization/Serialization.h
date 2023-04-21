#pragma once

#include <functional>
#include <string>
namespace SC {
	class Entity;
}
namespace SC::Serialization {
	class Serialization {
		using CreateFunc = std::function<void*()>;
		public:
			std::string GetComponentNameByTypeName(std::string name);
			CreateFunc GetComponentCreateFuncByTypeName(std::string name);
	};
}
