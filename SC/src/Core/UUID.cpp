#include "Engine/Core/UUID.h"

#include <random>

namespace SC
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformInt_Distribution;

	UUID::UUID()
	:m_uuid(s_UniformInt_Distribution(s_Engine))
	{ }

	UUID::UUID(uint64_t uuid)
	:m_uuid(uuid) { }

	bool UUID::operator==(const UUID& other) const
	{
		return other.m_uuid == this->m_uuid;
	}

	bool UUID::operator==(const uint64_t other) const
	{
		return other == this->m_uuid;
	}

	bool UUID::operator==(const uint32_t other) const
	{
		return (uint64_t)other == this->m_uuid;
	}
}
