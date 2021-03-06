/*
	Temat projektu: Filtr u?redniaj?cy WAV (?rednia krocz?ca)
	Semestr: 5
	Rok akademicki: 2021/2022
	Opis algorytmu:
		Prosta ?rednia ruchoma - ?rednia arytmetyczna z ostatnich n pomiar?w
	Autor: Krystian Stebel
*/

#include "FileWAV.h"


FileWAV::FileWAV(byte * wav) {

		for (int i = 0; i < 4; i++) {
			riff[i] = (char)wav[i];
		};
		byte fileSizeArray[4];
		for (int i = 0; i < 4; i++) {
			fileSizeArray[i] = wav[4 + i];
		};
		memcpy(&fileSize, fileSizeArray, sizeof(int));
		for (int i = 0; i < 4; i++) {
			wave[i] = (char)wav[8 + i];
		};
		for (int i = 0; i < 4; i++) {
			fmt[i] = wav[12 + i];
		};
		for (int i = 0; i < 4; i++) {
			formatByteSize[i] = wav[16 + i];
		};
		byte formatTypeArray[2] = {
		  wav[20],
		  wav[21]
		};
		memcpy(&formatType, formatTypeArray, sizeof(short));
		byte channelsArray[2] = {
		  wav[22],
		  wav[23]
		};
		memcpy(&channels, channelsArray, sizeof(short));
		byte sampleRateArray[4];
		for (int i = 0; i < 4; i++) {
			sampleRateArray[i] = wav[24 + i];
		};
		memcpy(&sampleRate, sampleRateArray, sizeof(int));
		byte bytesPerSecondArray[4];
		for (int i = 0; i < 4; i++) {
			bytesPerSecondArray[i] = wav[28 + i];
		};
		memcpy(&bytesPerSecond, bytesPerSecondArray, sizeof(int));
		byte bytesPerSampleArray[2] = {
		  wav[32],
		  wav[33]
		};
		memcpy(&bytesPerSample, bytesPerSampleArray, sizeof(short));
		byte bitsPerSampleArray[2] = {
		  wav[34],
		  wav[35]
		};
		memcpy(&bitsPerSample, bitsPerSampleArray, sizeof(short));
		for (int i = 0; i < 4; i++) {
			data[i] = (char)wav[36 + i];
		}; 
		unsigned int offset = 0;
		if (data[0] != 'd' || data[1] != 'a' || data[2] != 't' || data[3] != 'a' ) {
			offset = 4;
			while (!((char)wav[36 + offset] == 'd' && (char)wav[36 + offset + 1] == 'a' && (char)wav[36 + offset + 2] == 't' && (char)wav[36 + offset + 3] == 'a')) {
				offset++;
			}
			data[0] = (char)wav[36 + offset];
			data[1] = (char)wav[36 + offset + 1];
			data[2] = (char)wav[36 + offset + 2];
			data[3] = (char)wav[36 + offset + 3];

		}
		byte dataSizeArray[4];
		for (int i = 0; i < 4; i++) {
			dataSizeArray[i] = wav[40 + i + offset];
		};
		memcpy(&dataSize, dataSizeArray, sizeof(int));

		int pos = 44 + offset;
		unsigned int samples = (unsigned int)dataSize / ((unsigned int)channels * ((unsigned int)bitsPerSample / 8));
		sampleCount = samples;

		left = new short[samples];
		leftOriginal = new short[samples];
		if (channels == 2) { // should also check if more than 2 channels
			right = new short[samples];
			rightOriginal = new short[samples];
		}
		else {
			right = NULL;
			rightOriginal = NULL;
		}
		int i = 0;
		while (pos < samples * 2 * channels) {
			left[i] = bytesToInteger(wav[pos], wav[pos + 1]);
			leftOriginal[i] = bytesToInteger(wav[pos], wav[pos + 1]);

			pos += 2;
			if ((this->channels) == 2) {
				right[i] = bytesToInteger(wav[pos], wav[pos + 1]);
				rightOriginal[i] = bytesToInteger(wav[pos], wav[pos + 1]);
				pos += 2;
			}
			i++;
		}
	}
FileWAV::~FileWAV() {
	delete[] rightOriginal;
	delete[] right;
	delete[] leftOriginal;
	delete[] left;
}
int FileWAV::getFileSize() {
		return this->fileSize;
	}
int FileWAV::getDataSize() {
		return this->dataSize;
	}
short* FileWAV::getLeft() {
		return this->left;
	}
int FileWAV::getSampleCount() {
		return this->sampleCount;
	}
int FileWAV::getBytesPerSample() {
		return this->bytesPerSample;
	}
int FileWAV::getBitsPerSample() {
		return this->bitsPerSample;
	}
int FileWAV::getChannels() {
		return this->channels;
	}
long long FileWAV::processInThreads(unsigned int numberOfThreads, bool useAsm, LPWSTR filepath, bool writeSamples, bool alternativeMode) {
		std::wstring temp(filepath);
		size_t index = temp.rfind('\\');
		std::wstring workspace = temp.substr(0, index);

		HINSTANCE dllHandleC = NULL;
		HINSTANCE dllHandleAsm = NULL;
		dllHandleC = LoadLibrary(L"libraryC.dll");
		dllHandleAsm = LoadLibrary(L"libraryASM.dll");
		unsigned int size = ceil(sampleCount / 2);

		auto time_begin = std::chrono::high_resolution_clock::now();
		
		float sectionSizeFloat;
		std::vector < std::thread > threads;
		if (numberOfThreads >= this->channels && this->channels == 2) {
			sectionSizeFloat = size / (numberOfThreads / 2);
		}
		else {
			sectionSizeFloat = size;
		}
		
		
		unsigned int sectionSize = ceil(sectionSizeFloat);
		unsigned int lowerBoundry;
		unsigned int upperBoundry;
		
		

		
		if (useAsm) {
			FILTERASM filter;
			if (alternativeMode) {
				filter = (FILTERASM)GetProcAddress(dllHandleAsm, "altFilterASM");
			}
			else {
				filter = (FILTERASM)GetProcAddress(dllHandleAsm, "filterASM");
			}
		
			if (numberOfThreads == 1) {
				if (this->channels == 2) {
					threads.push_back(std::thread(filter, leftOriginal, left, 0, sectionSize));
					threads[0].join();
					threads.pop_back();
					threads.push_back(std::thread(filter, rightOriginal, right, 0, sectionSize));
					threads[0].join();
					threads.pop_back();
				}
				else {
					threads.push_back(std::thread(filter, leftOriginal, left, 0, sectionSize));
					threads[0].join();
				}
			}
			else {
				if (this->channels == 2) {
					for (int i = 0; i < (numberOfThreads / 2); i++) {
						lowerBoundry = i * sectionSize;
						upperBoundry = min((i + 1) * sectionSize - 1, size);
						printf("%d", upperBoundry);
						threads.push_back(std::thread(filter, leftOriginal, left, lowerBoundry, upperBoundry));
					}
					for (int i = 0; i < (numberOfThreads / 2); i++) {
						lowerBoundry = i * sectionSize;
						upperBoundry = min((i + 1) * sectionSize - 1, size);
						threads.push_back(std::thread(filter, rightOriginal, right, lowerBoundry, upperBoundry));
					}
					for (int i = 0; i < numberOfThreads; i++) {
						threads[i].join();
					}
				}
				else {
					for (int i = 0; i < numberOfThreads ; i++) {
						lowerBoundry = i * sectionSize;
						upperBoundry = min((i + 1) * sectionSize - 1, size);
						threads.push_back(std::thread(filter, leftOriginal, left, lowerBoundry, upperBoundry));
					}
					for (int i = 0; i < numberOfThreads; i++) {
						threads[i].join();
					}
				}
			}
		}
		else {
			FILTERC filter;
			if (alternativeMode) {
				filter = (FILTERC)GetProcAddress(dllHandleC, "altFilterC");
			}
			else {
				filter = (FILTERC)GetProcAddress(dllHandleC, "filterC");
			}
			if (numberOfThreads == 1) {
				if (this->channels == 2) {
					threads.push_back(std::thread(filter, leftOriginal, left, 0, sectionSize));
					threads[0].join();
					threads.pop_back();
					threads.push_back(std::thread(filter, rightOriginal, right, 0, sectionSize));
					threads[0].join();
					threads.pop_back();
				}
				else {
					threads.push_back(std::thread(filter, leftOriginal, left, 0, sectionSize));
					threads[0].join();
				}
			}
			else {
				if (this->channels == 2) {
					for (int i = 0; i < (numberOfThreads / 2); i++) {
						lowerBoundry = i * sectionSize;
						upperBoundry = min((i + 1) * sectionSize - 1, size);
						threads.push_back(std::thread(filter, leftOriginal, left, lowerBoundry, upperBoundry));
					}
					for (int i = 0; i < (numberOfThreads / 2); i++) {
						lowerBoundry = i * sectionSize;
						upperBoundry = min((i + 1) * sectionSize - 1, size);
						threads.push_back(std::thread(filter, rightOriginal, right, lowerBoundry, upperBoundry));
					}
					for (int i = 0; i < numberOfThreads; i++) {
						threads[i].join();
					}
				}
				else {
					for (int i = 0; i < numberOfThreads ; i++) {
						lowerBoundry = i * sectionSize;
						upperBoundry = min((i + 1) * sectionSize - 1, size);
						threads.push_back(std::thread(filter, leftOriginal, left, lowerBoundry, upperBoundry));
					}
					for (int i = 0; i < numberOfThreads; i++) {
						threads[i].join();
					}
				}
			}
		}
		
		auto time_end = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin);
		
		if (writeSamples) {
			if (this->channels == 2) {
				writeTestResultsToTxt(left, leftOriginal, size, workspace, 0, 0);
				writeTestResultsToTxt(right, rightOriginal, size, workspace, 0, 1);
			}
			else {
				writeTestResultsToTxt(right, rightOriginal, size, workspace, 1, 0);
			}
		}
		
		
	
		write(workspace);
		return elapsedTime.count();
	}

void FileWAV::writeTestResultsToTxt(short * tab, short * copy, int size, std::wstring filepath, bool isMono, bool isRight) {
		std::wstring pathWstring;
		std::wstring pathModifiedWstring;

		if (isMono) {
			pathWstring = filepath + L"\\original.txt";
			pathModifiedWstring = filepath + L"\\modified.txt";
		}
		else {
			if (isRight) {
				pathWstring = filepath + L"\\originalRight.txt";
				pathModifiedWstring = filepath + L"\\modifiedRight.txt";
			}
			else {
				pathWstring = filepath + L"\\originalLeft.txt";
				pathModifiedWstring = filepath + L"\\modifiedLeft.txt";
			}
		}
	
		
		LPWSTR path = const_cast <LPWSTR> (pathWstring.c_str());
		LPWSTR pathModified = const_cast <LPWSTR> (pathModifiedWstring.c_str());

		HANDLE originalFile = CreateFile(
			path,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (originalFile == INVALID_HANDLE_VALUE) {
			// TODO: Invalid filepath warning
		}
		unsigned int offset = 0;

		std::string strTextOriginal = "";
		for (int i = 0; i < size; i++) {
			strTextOriginal += std::to_string(copy[offset + i]) + "\n";
		}
		DWORD bytesWrittenOriginal;
		WriteFile(
			originalFile,
			strTextOriginal.c_str(),
			strTextOriginal.size(), &
			bytesWrittenOriginal,
			nullptr);

		CloseHandle(originalFile);

		HANDLE modifiedFile = CreateFile(
			pathModified,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (originalFile == INVALID_HANDLE_VALUE) {
			// TODO: wrong filepath warning
		}

		std::string strTextModified = "";
		for (int i = 0; i < size; i++) {
			strTextModified += std::to_string(tab[offset + i]) + "\n";
		}
		DWORD bytesWrittenModified;
		WriteFile(
			modifiedFile,
			strTextModified.c_str(),
			strTextModified.size(), &
			bytesWrittenModified,
			nullptr);

		CloseHandle(modifiedFile);
	}
	std::wstring getDirectory(LPCWSTR filepath) {
		std::wstring temp(filepath);
		size_t index = temp.rfind('\\');
		std::wstring workspace = temp.substr(0, index);
		return workspace;
	}

void FileWAV::write(std::wstring workspace) {
		std::wstring nameAndExt = L"\\output.wav";
		std::ofstream file(workspace + nameAndExt, std::ios::binary);
		const uint32_t size_ui32 = sizeof(uint32_t);
		const uint32_t size_ui16 = sizeof(uint16_t);

		

		if (file.good()) {
			file.flush();

			file.write(this->riff, 4 * sizeof(char));
			file.write(reinterpret_cast <
				const char *> (&this->fileSize), size_ui32);
			file.write(this->wave, 4 * sizeof(char));
			file.write(reinterpret_cast <
				const char *> (&this->fmt), 4 * sizeof(char));
			file.write(reinterpret_cast <
				const char *> (&this->formatByteSize), size_ui32);
			file.write(reinterpret_cast <
				const char *> (&this->formatType), size_ui16);
			file.write(reinterpret_cast <
				const char *> (&this->channels), size_ui16);
			file.write(reinterpret_cast <
				const char *> (&this->sampleRate), size_ui32);
			file.write(reinterpret_cast <
				const char *> (&this->bytesPerSecond), size_ui32);
			file.write(reinterpret_cast <
				const char *> (&this->bytesPerSample), size_ui16);
			file.write(reinterpret_cast <
				const char *> (&this->bitsPerSample), size_ui16);
			file.write(this->data, 4 * sizeof(char));
			file.write(reinterpret_cast <
				const char *> (&this->dataSize), size_ui32);
			for (int i = 0; i < this->sampleCount; i++) {
				file.write(reinterpret_cast <
					const char *> (&this->left[i]), size_ui16);
				if (this->channels == 2) {
					file.write(reinterpret_cast <
						const char *> (&this->right[i]), size_ui16);
				}
			};
			file.close();
		}
	}
short FileWAV::bytesToInteger(byte firstByte, byte secondByte) {
		short s = (short)((secondByte << 8) | firstByte);
		return s;
}
int FileWAV::bytesToInteger4(byte firstByte, byte secondByte, byte thirdByte, byte fourthByte) {
		long result = (((firstByte << 8 & secondByte) << 8 & thirdByte) << 8) & fourthByte;
		result = result & 0xFFFFFFFF;
		return result;
}