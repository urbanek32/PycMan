#ifndef TYPEDEFS_H	//nothing here
#define TYPEDEFS_H

#include <basetsd.h>

namespace moje
{
	typedef signed char Int8;
	typedef unsigned char UInt8;

	typedef signed short Int16;
	typedef unsigned short UInt16;

	typedef signed int Int32;
	typedef unsigned int UInt32;


	//kochany int 64bitowy dla visuala
#if defined(_MSC_VER)
	typedef signed __int64 Int64;
	typedef unsigned __int64 UInt64;
#else
	typedef signed long long Int64;
	typedef unsigned long long UInt64;
#endif

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib,"ws2_32.lib")
	typedef UINT_PTR SocketHandle;
#else
	typedef int SocketHandle;
#endif

} // namespace moje

#endif