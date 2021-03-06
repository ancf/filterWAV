#pragma once
#include <stdio.h>
#include <windows.h>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <stdint.h>
#include <variant>
#include <cassert>


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
	int sampleCount;
public:
	FileWAV(byte * wav);
	~FileWAV();
	int getFileSize();
	int getDataSize();
	short * getLeft();
	int getSampleCount();
	int getBytesPerSample();
	int getBitsPerSample();
	int getChannels();
	long long processInThreads(unsigned int numberOfThreads, bool useAsm, LPWSTR filepath, bool writeSamples, bool alternativeMode);
	void writeTestResultsToTxt(short * tab, short * copy, int size, std::wstring filepath, bool isMono, bool isRight);
	std::wstring getDirectory(LPCWSTR filepath);
	void write(std::wstring workspace);
private:
	static short bytesToInteger(byte firstByte, byte secondByte);
	static int bytesToInteger4(byte firstByte, byte secondByte, byte thirdByte, byte fourthByte);
};

