#pragma once

#include <Windows.h>
#include <vector>

#include "tstring.h"
#include <strsafe.h>
#include <iostream>

struct CompressionInfo
{
	std::tstring compressorName;
	DWORD originalLength;
	DWORD compressedLength;
	LONGLONG compressingTime;

	void print(void) const;
};

void zipFile(__in const std::tstring& strFilePath,
			 __inout CompressionInfo& compresInfo);

void snappyFile(__in const std::tstring& strFilePath,
				__inout CompressionInfo& compresInfo);

void printComparision(__in const std::vector<CompressionInfo>& infoList);