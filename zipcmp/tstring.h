#pragma once

#include <string>

namespace std
{
	#ifdef UNICODE
		typedef wstring tstring;
	#else
		typedef string tstring;
	#endif
}


