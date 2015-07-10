// zipcmp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zipcmp.h"
#include <iostream>
#include "zip\zip.h"
#include <fstream>
#include <Shlwapi.h>
#include "TimeHelper.h"

void CompressionInfo::print(void) const
{
	int const size = 512;
	TCHAR tchBuf[size] = {0};
	StringCbPrintf(
		tchBuf,
		size*sizeof(TCHAR),
		TEXT("compressor: %s\n")
		TEXT("original length: \t%d \tBytes\n")
		TEXT("compressed length: \t%d \tBytes\n")
		TEXT("compressing time: \t%.3f \tsec\n"),
		compressorName.c_str(),
		originalLength,
		compressedLength,
		compressingTime / 1000.0);

	std::wcout << tchBuf << std::endl;
}


DWORD getFileSize(__in const std::tstring strFilePath)
{ 
	std::ifstream zipFileStream;
	std::streamoff length;
	zipFileStream.open(strFilePath, std::ios::binary | std::ios::in);          // open input file
	zipFileStream.seekg(0, std::ios::end);    // go to the end
	length = zipFileStream.tellg();  
	return length;
}

void zipFile(__in const std::tstring& strFilePath,
			 __inout CompressionInfo& compresInfo)
{

	compresInfo.compressorName.assign(TEXT("zip"));

	std::tstring strZippedFile = strFilePath;
	strZippedFile.append(TEXT(".cmp"));
	strZippedFile.append(compresInfo.compressorName);

	HZIP hZip = CreateZip(strZippedFile.c_str(), 0);
	SYSTEMTIME startSysTime, endSysTime;
	GetSystemTime(&startSysTime);
	ZipAdd(hZip, strZippedFile.c_str(), strFilePath.c_str());
	GetSystemTime(&endSysTime);
	CloseZip(hZip);

	
	FILETIME startFileTime, endFileTime;
	SystemTimeToFileTime(&startSysTime, &startFileTime);
	SystemTimeToFileTime(&endSysTime, &endFileTime);

	compresInfo.originalLength = getFileSize(strFilePath);
	compresInfo.compressedLength = getFileSize(strZippedFile);
	compresInfo.compressingTime = TimeHelper::subtruct(endFileTime, startFileTime);
}

void snappyFile(__in const std::tstring& strFilePath,
				__inout CompressionInfo& compresInfo)
{
	compresInfo.compressorName.assign(TEXT("snappy"));
}

void printComparision(__in const std::vector<CompressionInfo>& infoList)
{
	for(auto info : infoList)
		info.print();
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argv[1] == NULL)
	{
		std::cout << " Please enter the file for compressing." << std::endl;
		return -1;
	}

	std::tstring strFileName(argv[1]);
	CompressionInfo zipinfo;
	zipFile(strFileName, zipinfo);

	CompressionInfo snappyInfo;
	snappyFile(strFileName, snappyInfo);

	std::vector<CompressionInfo> infoList;
	infoList.push_back(zipinfo);
	infoList.push_back(snappyInfo);
	printComparision(infoList);

	return 0;
}



