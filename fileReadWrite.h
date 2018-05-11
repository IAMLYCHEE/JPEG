#pragma once
//author : Li Yicheng 8/27/2017
#ifndef FILEREADWRITE_H_
#define FILEREADWRITE_H_

#include<iostream>
#include<string>
#include<vector>
class FileReadWrite {
public:
	FileReadWrite(std::string filename);
	std::string filename;
	char* content;
	long length;
private:
	char * fileRead(const std::string fileName);
	char * fileReadRaw(const std::string fileName, int channal);
	//unsigned char * fileRead(const std::string fileName, bool isRaw);
};

#endif
