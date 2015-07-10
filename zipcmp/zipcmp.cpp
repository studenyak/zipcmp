// zipcmp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zipcmp.h"
#include <iostream>

void zipFile(__in const std::tstring& strFilePath,
			 __inout CompressionInfo& compresInfo)
{
}

void snappyFile(__in const std::tstring& strFilePath,
				__inout CompressionInfo& compresInfo)
{
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



