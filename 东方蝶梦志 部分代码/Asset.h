#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include <vector>
#include <exception>

namespace Basic {
	class Asset final
	{
	private:
		std::shared_ptr<std::vector<uint8_t> > m_buf;
	public:
		Asset(const std::string& path);
		inline Asset(const Asset& a) { m_buf = a.m_buf; }
		inline ~Asset() {};
		inline operator const void*() const { return (void*)&(*m_buf)[0]; }
		inline operator const char* () const { return (char*)&(*m_buf)[0]; }
		inline uint32_t Size() { return (uint32_t)m_buf->size()-1; }
		inline std::shared_ptr<std::vector<uint8_t> > Ptr() { return m_buf; }

		class FileNotFound : public std::exception
		{
		public:
			using std::exception::exception;
		};
	};

}