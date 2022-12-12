#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SCObject.h"
#include <sys/types.h>

namespace SC {
	class SC_API Resource: public SCObject
	{
		public:
			const char* path;
			virtual uint64_t GetID() const = 0;
			virtual void Delete() = 0;

			virtual ~Resource() = default;
	};
}
