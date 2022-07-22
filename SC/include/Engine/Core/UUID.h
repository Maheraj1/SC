#pragma once

#include <iostream>

namespace SC
{
	class UUID
	{
		public:
			UUID();
			UUID(uint64_t uuid);
			UUID(const UUID&) = default;

			operator uint64_t() const {return m_uuid;}
			bool operator==(const UUID& other) const;
			bool operator==(const uint64_t other) const;
			bool operator+=(const UUID& other) const = delete;
			bool operator-=(const UUID& other) const = delete;

			~UUID();
		private:
			uint64_t m_uuid;
	};
}

namespace std
{
	template<>
	struct hash<SC::UUID>
	{
		std::size_t operator()(const SC::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
