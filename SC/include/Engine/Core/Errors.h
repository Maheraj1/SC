#pragma once

#include <stdexcept>

namespace SC::Errors {
	class ScriptInstanceNotFound: public std::exception {};
	class SceneIndexOutOfRange:   public std::exception {};
	class InvalidSceneFile:       public std::exception {};
}
