#pragma once

#include <stdexcept>

namespace SC::Errors {
	class ScriptInstanceNotFound: public std::exception {};
}
