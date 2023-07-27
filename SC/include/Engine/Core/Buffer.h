#pragma once

#include <cstring>
#include <inttypes.h>
#include <cstddef>
#include <sys/types.h>

namespace SC {
	
	struct Buffer {
		uint8_t* Data = nullptr;
		uint64_t Size = 0;

		Buffer() = default;

		Buffer(uint64_t size)
		{
			Allocate(size);
		}

		Buffer(const Buffer&) = default;

		static Buffer Copy(Buffer other)
		{
			Buffer result(other.Size);
			memcpy(result.Data, other.Data, other.Size);
			return result;
		}

		void Allocate(uint64_t size)
		{
			Release();

			Data = new uint8_t[size];
			Size = size;
		}

		void Release()
		{
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		operator bool() const
		{
			return (bool)Data;
		}
	};

	class ScopedBuffer {
		public:
			ScopedBuffer(Buffer buffer)
			:buffer(buffer) 
			{ }

			ScopedBuffer(uint64_t size)
			:buffer(size)
			{ }

			~ScopedBuffer() {
				buffer.Release();
			}

			uint8_t* Data() { return buffer.Data; }
			uint64_t Size() { return buffer.Size; }

			template<typename T>
			T* As() {
				return buffer.As<T>();
			}
		private:
			Buffer buffer;
	};
}