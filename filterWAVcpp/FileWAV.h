#pragma once
#include <stdio.h>
#include <windows.h>
//#include "resource.h"
//#include <shobjidl.h> 
#include <string>
#include <thread>
#include <vector>
//#include <sstream>
//#include <mutex>
#include <chrono>
#include <fstream>
#include <stdint.h>


typedef void(__stdcall * FILTERASM)(short * tab, short * copy, unsigned int min, unsigned int max);
typedef void(*FILTERC)(short * tab, short * copy, unsigned int min, unsigned int max);

class FileWAV {
protected:
	char riff[4];
	int fileSize;
	char wave[4];
	byte fmt[4];
	byte formatByteSize[4];
	int formatType;
	int channels;
	int sampleRate;
	int bytesPerSecond;
	int bytesPerSample;
	int bitsPerSample;
	char data[4];
	int dataSize;
	short * left;
	short * right;
	short * leftOriginal;
	short * rightOriginal;
	int * rawData;
	int sampleCount;
public:
	FileWAV(byte * wav);
	int getFileSize();
	int getDataSize();
	short * getLeft();
	int getSampleCount();
	int getBytesPerSample();
	int getBitsPerSample();
	int getChannels();
	void processInThreads(unsigned int numberOfThreads, bool useAsm, LPWSTR filepath);
	void writeTestResultsToTxt(short * tab, short * copy, int size, int testTime, std::wstring filepath);
	std::wstring getDirectory(LPCWSTR filepath);
	void write(std::wstring workspace);
private:
	static short bytesToInteger(byte firstByte, byte secondByte);
	static int bytesToInteger4(byte firstByte, byte secondByte, byte thirdByte, byte fourthByte);
};

