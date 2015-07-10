#pragma once

#include <Windows.h>
#include <vector>

#include "tstring.h"
#include <iostream>

struct CompressionInfo
{
	std::tstring compressorName;
	DWORD originalLength;
	DWORD compressedLength;
	LONGLONG compressingTime;

	void print(void)
	{
		TCHAR tchBuf[1024] = {};
		wsprintf(
			tchBuf,
			L"compressor: %s\n"
			L"original length: %d\n"
			L"compressed length: %d\n"
			L"compressing time: %l\n",
			compressorName,
			originalLength,
			compressedLength,
			compressingTime);

		std::cout << tchBuf << std::endl;
	}
};

void zipFile(__in const std::tstring& strFilePath,
			 __inout CompressionInfo& compresInfo);

void snappyFile(__in const std::tstring& strFilePath,
				__inout CompressionInfo& compresInfo);

void printComparision(__in const std::vector<CompressionInfo>&);