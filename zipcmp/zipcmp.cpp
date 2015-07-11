// zipcmp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zipcmp.h"
#include <iostream>
#include "zip\zip.h"
#include <fstream>
#include <Shlwapi.h>
#include "TimeHelper.h"
#include <sstream>
#include "snappy\snappy.h"

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
		TEXT("compressing time: \t%d \tmsec\n"),
		compressorName.c_str(),
		originalLength,
		compressedLength,
		compressingTime);

	std::wcout << tchBuf << std::endl;
}


DWORD getFileSize(__in const std::tstring strFilePath)
{ 
	std::ifstream zipFileStream;
	std::streamoff length;
	zipFileStream.open(strFilePath, std::ios::binary | std::ios::in);          // open input file
	if(!zipFileStream.is_open())
		return 0;
	zipFileStream.seekg(0, std::ios::end);    // go to the end
	length = zipFileStream.tellg();
	zipFileStream.close();
	return length;
}

void zipFile(__in const std::tstring& strFilePath,
			 __inout CompressionInfo& compresInfo)
{
	compresInfo.originalLength = getFileSize(strFilePath);
	compresInfo.compressorName.assign(TEXT("zip"));

	std::tstring strCompressedFile = strFilePath;
	strCompressedFile.append(TEXT(".cmp"));
	strCompressedFile.append(compresInfo.compressorName);

	HZIP hZip = CreateZip(strCompressedFile.c_str(), 0);
	SYSTEMTIME startSysTime, endSysTime;
	GetSystemTime(&startSysTime);
	ZipAdd(hZip, strCompressedFile.c_str(), strFilePath.c_str());
	GetSystemTime(&endSysTime);

	FILETIME startFileTime, endFileTime;
	SystemTimeToFileTime(&startSysTime, &startFileTime);
	SystemTimeToFileTime(&endSysTime, &endFileTime);
	CloseZip(hZip);

	compresInfo.compressedLength = getFileSize(strCompressedFile);
	compresInfo.compressingTime = TimeHelper::subtruct(endFileTime, startFileTime);
}

void snappyFile(__in const std::tstring& strFilePath,
				__inout CompressionInfo& compresInfo)
{
	compresInfo.originalLength = getFileSize(strFilePath);

	std::ifstream inputFileStream;
	inputFileStream.open(strFilePath, std::ios::binary | std::ios::in);

	std::ostringstream oStringStream;
	oStringStream << inputFileStream;

	std::string compressedString;
	SYSTEMTIME startSysTime, endSysTime;
	GetSystemTime(&startSysTime);
	snappy::Compress(oStringStream.str().c_str(), compresInfo.originalLength, &compressedString);
	GetSystemTime(&endSysTime);

	FILETIME startFileTime, endFileTime;
	SystemTimeToFileTime(&startSysTime, &startFileTime);
	SystemTimeToFileTime(&endSysTime, &endFileTime);

	compresInfo.compressorName.assign(TEXT("snappy"));
	std::tstring strCompressedFile = strFilePath;
	strCompressedFile.append(TEXT(".cmp"));
	strCompressedFile.append(compresInfo.compressorName);
	
	std::ofstream compressedFile;
	compressedFile.open(strCompressedFile, std::ios::binary | std::ios::out);
	compressedFile << compressedString;
	
	compresInfo.compressedLength = getFileSize(strCompressedFile);
	compresInfo.compressingTime = TimeHelper::subtruct(endFileTime, startFileTime);

	inputFileStream.close();
	compressedFile.close();
	
}

void printComparision(__in const std::vector<CompressionInfo>& infoList)
{
	for(auto info : infoList)
		info.print();
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
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
	catch(std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}
	catch(...)
	{
		std::cerr << "Error: " << std::endl;
		return -1;
	}
}



