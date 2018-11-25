#include <log_stream.h>

#include <limits>
#include <algorithm>
#include <assert.h>

#ifdef _MSC_VER
#define	snprintf	sprintf_s
#endif

using namespace muduo;
using namespace muduo::detail;

namespace muduo
{
namespace detail
{
	const char digits[] = "9876543210123456789";
	const char* zero = digits + 9;
	const char digitsHex[] = "0123456789ABCDEF";
	template<typename T>
	size_t convert(char buf[], T value)
	{
		T i = value;
		char* p = buf;
		do 
		{
			int lsd = static_cast<int>(i % 10);
			i /= 10;
			*p++ = zero[lsd];
		} while (i!=0);
		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';
		std::reverse(buf, p);
		return p - buf;
	}
	size_t convertHex(char buf[], uintptr_t value)
	{
		uintptr_t i = value;
		char* p = buf;
		do 
		{
			int lsd = static_cast<int>(i % 16);
			i /= 16;
			*p++ = digitsHex[lsd];
		} while (i != 0);
		*p = '\0';
		std::reverse(buf, p);
		return p - buf;
	}
	template class FixedBuffer<kSmallBuffer>;
	template class FixedBuffer<kLargeBuffer>;
}// end namespace detail
}// end namespace muduo

template<typename T>
void LogStream::_formatInteger(T v)
{
	if (buffer_.avail() >= kMaxNumericSize)
	{
		size_t len = convert(buffer_.current(), v);
		buffer_.add(len);
	}
}
LogStream& LogStream::operator<<(short v)
{
	*this << static_cast<int>(v);
	return *this;
}
LogStream& LogStream::operator<<(unsigned short v)
{
	*this << static_cast<unsigned int>(v);
	return *this;
}
LogStream& LogStream::operator<<(int v)
{
	_formatInteger(v);
	return *this;
}
LogStream& LogStream::operator<<(unsigned int v)
{
	_formatInteger(v);
	return *this;
}

LogStream& LogStream::operator << (long v)
{
	_formatInteger(v);
	return *this;
}
LogStream& LogStream::operator<<(unsigned long v)
{
	_formatInteger(v);
	return *this;
}
LogStream& LogStream::operator<<(long long v)
{
	_formatInteger(v);
	return *this;
}
LogStream& LogStream::operator<<(unsigned long long v)
{
	_formatInteger(v);
	return *this;
}
LogStream& LogStream::operator<<(const void* p)
{
	// convert to number
	uintptr_t i = reinterpret_cast<uintptr_t>(p);
	if (buffer_.avail() > kMaxNumericSize)
	{
		char * buf = buffer_.current();
		buf[0] = '0';
		buf[1] = 'x';
		size_t len = convertHex(buf + 2, i);
		buffer_.add(len);
	}
	return *this;
}
LogStream& LogStream::operator<<(double v)
{
	if (buffer_.avail() > kMaxNumericSize)
	{
		int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
		buffer_.add(len);
	}
	return *this;
}
template<typename T>
Fmt::Fmt(const char * fmt, T val)
{
	length_ = snprintf(buf_, sizeof buf_, fmt, val);
}


// Explicit instantiations

template Fmt::Fmt(const char* fmt, char);

template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);
