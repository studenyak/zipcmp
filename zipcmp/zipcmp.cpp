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
#include "lz4\lz4.h"

namespace std
{
	#ifdef UNICODE
		typedef wifstream tifstream;
		typedef wios tios;
		typedef wostringstream tostringstream;
	#else
		typedef ifstream tifstream;
		typedef ios tios;
		typedef ostringstream tostringstream;
	#endif
}

CompressionInfo::CompressionInfo()
	: compressorName(TEXT(""))
	, originalLength(0)
	, compressedLength(0)
	, compressingTime(0)
{}

void CompressionInfo::print(void) const
{
	int const size = 512;
	TCHAR tchBuf[size] = {0};
	StringCbPrintf(
		tchBuf,
		size*sizeof(TCHAR),
		TEXT("compressor: %s\n")
		TEXT("original length: %d Bytes\n")
		TEXT("compressed length: %d Bytes\n")
		TEXT("compress rate: %.3f\n")
		TEXT("compressing time: %d msec\n"),
		compressorName.c_str(),
		originalLength,
		compressedLength,
		(float)originalLength/compressedLength,
		compressingTime);

	std::wcout << tchBuf << std::endl;
}


DWORD getFileSize(__in const std::tstring strFilePath)
{ 
	std::tifstream zipFileStream;
	std::streamoff length;
	zipFileStream.open(strFilePath, std::tios::binary | std::tios::in);          // open input file
	if(!zipFileStream.is_open())
		return 0;
	zipFileStream.seekg(0, std::tios::end);    // go to the end
	length = zipFileStream.tellg();
	zipFileStream.close();
	return length;
}

void zipFile(__in const FileList& fileList,
			 __inout CompressionInfo& compresInfo)
{
	compresInfo.compressorName.assign(TEXT("zip"));
	std::tstring strCompressedFile = compresInfo.compressorName;
	strCompressedFile.append(TEXT(".cmp"));

	SYSTEMTIME startSysTime, endSysTime;
	FILETIME startFileTime, endFileTime;

	HZIP hZip = CreateZip(strCompressedFile.c_str(), 0);
	for(auto strFilePath : fileList)
	{
		compresInfo.originalLength += getFileSize(strFilePath);
	
		GetSystemTime(&startSysTime);
		ZipAdd(hZip, strCompressedFile.c_str(), strFilePath.c_str());
		GetSystemTime(&endSysTime);

		SystemTimeToFileTime(&startSysTime, &startFileTime);
		SystemTimeToFileTime(&endSysTime, &endFileTime);

		compresInfo.compressingTime += TimeHelper::subtruct(endFileTime, startFileTime);
	}

	CloseZip(hZip);
	compresInfo.compressedLength = getFileSize(strCompressedFile);
}

void snappyFile(__in const FileList& fileList,
				__inout CompressionInfo& compresInfo,
				__in const std::tstring& strCompressorName)
{
	compresInfo.compressorName = strCompressorName;
	std::tstring strCompressedFile = compresInfo.compressorName;
	strCompressedFile.append(TEXT(".cmp"));

	std::ostringstream oStringStream;
	for(auto strFilePath : fileList)
	{
		compresInfo.originalLength += getFileSize(strFilePath);

		std::ifstream inputFileStream;
		//std::wcout << TEXT("Open file: ") << strFilePath << std::endl;
		inputFileStream.open(strFilePath, std::ios::binary | std::ios::in);
		if(!inputFileStream.is_open())
		{
			std::wcout << TEXT("Could not open file to for reading: ") << strFilePath << std::endl;
			return;
		}
		//std::cout << TEXT("Add data to output stream") << inputFileStream.rdbuf() << std::endl;
		oStringStream << inputFileStream.rdbuf();
		inputFileStream.close();
	}

	std::string outString;
	size_t originalDataSize = oStringStream.str().size();

	//std::wcout << TEXT("Size of original data: ") << originalDataSize << std::endl;
	char* chCompressedData = new char[snappy::MaxCompressedLength(originalDataSize)];
	size_t compressedDataSize = 0;
	SYSTEMTIME startSysTime, endSysTime;
	FILETIME startFileTime, endFileTime;
	//std::wcout << TEXT("Run compression") << std::endl;
	GetSystemTime(&startSysTime);
	if (compresInfo.compressorName == std::tstring(TEXT("snappy")))
	{
		snappy::RawCompress(
			oStringStream.str().c_str(),
			originalDataSize,
			chCompressedData,
			&compressedDataSize);
	}
	else
	{
		compressedDataSize = LZ4_compress(
			oStringStream.str().c_str(),
			chCompressedData,
			originalDataSize);
	}

	GetSystemTime(&endSysTime);
	//std::wcout << TEXT("Size of compressed data: ") << compressedDataSize << std::endl;

	SystemTimeToFileTime(&startSysTime, &startFileTime);
	SystemTimeToFileTime(&endSysTime, &endFileTime);
	
	std::ofstream compressedFile;
	
	//std::wcout << TEXT("Write output stream to file: ") << strCompressedFile << std::endl;
	compressedFile.open(strCompressedFile, std::ios::binary | std::ios::out);
	if(!compressedFile.is_open())
	{
		std::cout << "Could not open file to write: " << strCompressedFile.c_str() << std::endl;
		return;
	}

	compressedFile.write(chCompressedData, compressedDataSize);
	compressedFile.close();
	delete[] chCompressedData;

	compresInfo.compressedLength = getFileSize(strCompressedFile);
	compresInfo.compressingTime = TimeHelper::subtruct(endFileTime, startFileTime);
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

		FileList fileList;
		unsigned int i = 0;
		while(argv[++i] != NULL)
			fileList.push_back(std::tstring(argv[i]));

		CompressionInfo zipinfo;
		zipFile(fileList, zipinfo);
		
		CompressionInfo snappyInfo;
		snappyFile(fileList, snappyInfo);

		CompressionInfo lz4Info;
		snappyFile(fileList, lz4Info, TEXT("lz4"));

		std::vector<CompressionInfo> infoList;
		infoList.push_back(zipinfo);
		infoList.push_back(snappyInfo);
		infoList.push_back(lz4Info);
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
		std::cerr << "Error: Panic" << std::endl;
		return -1;
	}
}



