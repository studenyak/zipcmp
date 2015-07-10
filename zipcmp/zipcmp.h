#pragma once

#include <Windows.h>
struct CompressionInfo
{
	DWORD originalLength;
	DWORD compressedLength;
	LONGLONG compressingTime;
};