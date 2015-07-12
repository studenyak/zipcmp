#pragma once

#include <Windows.h>
#include <vector>

#include "tstring.h"
#include <strsafe.h>
#include <iostream>

struct CompressionInfo
{
	CompressionInfo();
	std::tstring compressorName;
	DWORD originalLength;
	DWORD compressedLength;
	LONGLONG compressingTime;

	void print(void) const;
};

typedef std::vector<std::tstring> FileList;

void zipFile(__in const FileList& fileList,
			 __inout CompressionInfo& compresInfo);

void snappyFile(__in const FileList& fileList,
				__inout CompressionInfo& compresInfo);

void printComparision(__in const std::vector<CompressionInfo>& infoList);