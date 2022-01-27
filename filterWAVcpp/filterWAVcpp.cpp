// filterWAVcpp.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "filterWAVcpp.h"


#define MAX_LOADSTRING 100
#define ID_OPTION_ASM 1
#define ID_OPTION_CPP 2

#define ID_OPTION_1 3
#define ID_OPTION_2 4
#define ID_OPTION_4 5
#define ID_OPTION_8 6
#define ID_OPTION_16 7
#define ID_OPTION_32 8
#define ID_OPTION_64 9

#define ID_BUTTON 10
#define ID_CONFIRM 11



HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

FILE* wavFile;
FILE* wavOutputFile;
std::mutex m;
typedef void (__stdcall *FILTER)(int* tab, int* copy, unsigned int min, unsigned int max);
typedef void   (*FILTERC)(int* tab, int* copy, unsigned int min, unsigned int max);




class FileWAV {
protected:
	char riff[4];
	int fileSize;
	char wave[4];
	byte fmt[4];
	byte formatByteSize[4];
	int channels;
	int sampleRate;
	int bytesPerSecond;
	int bytesPerSample;
	int bitsPerSample;
	char data[4];
	int dataSize;
	int* left;
	int* right;
	int* leftOriginal;
	int* rightOriginal;
	int sampleCount;
public: FileWAV(byte* wav) {
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

	byte channelsArray[2] = { wav[22], wav[23] };

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


	byte bytesPerSampleArray[2] = { wav[32], wav[33] };

	memcpy(&bytesPerSample, bytesPerSampleArray, sizeof(short));



	byte bitsPerSampleArray[2] = { wav[34], wav[35] };


	memcpy(&bitsPerSample, bitsPerSampleArray, sizeof(short));


	for (int i = 0; i < 4; i++) {
		data[i] = (char)wav[36 + i];
	};



	byte dataSizeArray[4];

	for (int i = 0; i < 4; i++) {
		dataSizeArray[i] = wav[40 + i];
	};

	memcpy(&dataSize, dataSizeArray, sizeof(int));

	int pos = 44;

	unsigned int samples = (unsigned int)dataSize / ((unsigned int)channels * ((unsigned int)bitsPerSample / 8));

	sampleCount = samples;




	left = new int[ceil(samples / 2)];
	leftOriginal = new int[ceil(samples / 2)];
	if (channels == 2) { // should also check if more than 2 channels
		right = new int[ceil(samples / 2)];
		rightOriginal = new int[ceil(samples / 2)];
	}
	else {
		right = NULL;
		rightOriginal = NULL;
	}
	int i = 0;
	while (pos < samples * 2)
	{


		left[i] = bytesToInteger(wav[pos], wav[pos + 1]);
		leftOriginal[i] = bytesToInteger(wav[pos], wav[pos + 1]);

		pos += 2;
		if (channels == 2)
		{


			right[i] = bytesToInteger(wav[pos], wav[pos + 1]);
			rightOriginal[i] = bytesToInteger(wav[pos], wav[pos + 1]);

			pos += 2;
		}
		i++;
	}


}
		int getFileSize() {
			return this->fileSize;

		}
		int getDataSize() {
			return this->dataSize;

		}
		int* getLeft() {
			return this->left;
		}
		int getSampleCount() {
			return this->sampleCount;
		}
		int getBytesPerSample() {
			return this->bytesPerSample;
		}
		int getBitsPerSample() {
			return this->bitsPerSample;
		}
		int getChannels() {
			return this->channels;
		}
		void writeTestResultsToTxt(int* tab, int* copy, int size, int testTime) {
			HANDLE originalFile = CreateFile(
				L"C:\\Users\\Sum\\Desktop\\Desktop\\original.txt",
				GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,
				CREATE_NEW,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (originalFile == INVALID_HANDLE_VALUE)
			{
				// TODO: Invalid filepath warning
			}
			unsigned int offset = 0;

			
			std::string strTextOriginal = "Elapsed time: " + std::to_string(testTime);
			strTextOriginal += "\n";
			for (int i = 0; i < size; i++) {
				strTextOriginal += std::to_string(copy[offset + i]) + "\n";
			}
			DWORD bytesWrittenOriginal;
			WriteFile(
				originalFile,
				strTextOriginal.c_str(),
				strTextOriginal.size(),
				&bytesWrittenOriginal,
				nullptr);


			CloseHandle(originalFile);

			HANDLE modifiedFile = CreateFile(
				L"C:\\Users\\Sum\\Desktop\\Desktop\\modified.txt",
				GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,
				CREATE_NEW,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (originalFile == INVALID_HANDLE_VALUE)
			{
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
				strTextModified.size(),
				&bytesWrittenModified,
				nullptr);


			CloseHandle(modifiedFile);
		}

		void divideIntoThreads(unsigned int numberOfThreads, bool useAsm) {

			
			HINSTANCE dllHandleC = NULL;
			HINSTANCE dllHandleAsm = NULL;
			dllHandleC = LoadLibrary(L"libraryC.dll");
			dllHandleAsm = LoadLibrary(L"libraryASM.dll");
			unsigned int size = ceil(sampleCount / 2);
			
			FILTER filter;
			FILTERC filterC;

			if (useAsm) {
				
				
				filter = (FILTER)GetProcAddress(dllHandleAsm, "filterASM");
			}
			else {
				
				filterC = (FILTERC)GetProcAddress(dllHandleC, "filterC");
			}




			std::vector < std::thread > threads;


			float sectionSizeFloat = size / numberOfThreads;
			unsigned int sectionSize = ceil(sectionSizeFloat);
			unsigned int lowerBoundry;
			unsigned int upperBoundry;

			auto time_begin = std::chrono::high_resolution_clock::now();
			if (useAsm) {
				threads.push_back(std::thread(filter, rightOriginal, right, 0, sectionSize));
				for (int i = 1; i < numberOfThreads; i++) {
					lowerBoundry = i * sectionSize;
					upperBoundry = min((i + 1) * sectionSize - 1, size);

					threads.push_back(std::thread(filter, rightOriginal, right, lowerBoundry, upperBoundry));

				}
			}
			else {
				threads.push_back(std::thread(filterC, rightOriginal, right, 0, sectionSize));
				for (int i = 1; i < numberOfThreads; i++) {
					lowerBoundry = i * sectionSize;
					upperBoundry = min((i + 1) * sectionSize - 1, size);

					threads.push_back(std::thread(filterC, rightOriginal, right, lowerBoundry, upperBoundry));

				}
			}
			





			for (int i = 0; i < numberOfThreads; i++) {
				threads[i].join();
			}

			auto time_end = std::chrono::high_resolution_clock::now();

			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin);

			

			writeTestResultsToTxt(right, rightOriginal, size, elapsedTime.count());
		}
private:
	static double bytesToInteger(byte firstByte, byte secondByte)
	{

		short s = (short)((secondByte << 8) | firstByte);
		return (int)s;
	}
};





unsigned long loadWAV(FILE** wavFile, BYTE** samples, bool useAsm, int threadCount) {
    

    fseek(*wavFile, 0, SEEK_END);
    unsigned long fsize = ftell(*wavFile);
    rewind(*wavFile);

    *samples = (BYTE*)malloc(fsize + 1);

    if (*samples == NULL) {
        fclose(*wavFile);
        return 0;
    }


    fread_s(*samples, fsize + 1, 1, fsize, *wavFile);

    FileWAV file(*samples);
    
    file.divideIntoThreads(threadCount, useAsm);
    

    fclose(*wavFile);

    //return fsize;
    return file.getDataSize();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
   
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.

   


    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FILTERWAVCPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    FILTER filterC;
    FILTER filterAsm;
    HINSTANCE dllHandleC = NULL;
    HINSTANCE dllHandleAsm = NULL;
    dllHandleC = LoadLibrary(L"libraryC.dll");
    dllHandleAsm = LoadLibrary(L"libraryAsm.lib");
 


   // filterC = (FILTER)GetProcAddress(dllHandleC, "FilterC");
    filterC = (FILTER)GetProcAddress(dllHandleAsm, "FilterC");



    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
       
       
    

        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILTERWAVCPP));

    MSG msg;

    // Główna pętla komunikatów:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILTERWAVCPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FILTERWAVCPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    switch (message)
    {
    case WM_CREATE:
        CreateWindowW(L"Button", L"Choose mode",
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            10, 10, 120, 80, hWnd, (HMENU)0, hInst, NULL);
        CreateWindowW(L"Button", L"ASM",
            WS_CHILD | WS_GROUP | WS_VISIBLE | BS_AUTORADIOBUTTON ,
            20, 30, 100, 30, hWnd, (HMENU)ID_OPTION_ASM, hInst, NULL);
        CreateWindowW(L"Button", L"CPP",
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            20, 55, 100, 30, hWnd, (HMENU)ID_OPTION_CPP, hInst, NULL);
        CreateWindowW(L"Button", L"Choose thread count",
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            150, 10, 120, 310, hWnd, (HMENU)0, hInst, NULL);
    //    CreateWindowW(L"Button", L"1",
    //        WS_CHILD | WS_GROUP | WS_VISIBLE | BS_AUTORADIOBUTTON ,
    //        160, 30, 100, 30, hWnd, (HMENU)ID_OPTION_1, hInst, NULL);
        CreateWindowW(L"Button", L"2",
            WS_CHILD | WS_GROUP | WS_VISIBLE | BS_AUTORADIOBUTTON,
            160, 55, 100, 30, hWnd, (HMENU)ID_OPTION_2, hInst, NULL);
        CreateWindowW(L"Button", L"4",
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            160, 80, 100, 30, hWnd, (HMENU)ID_OPTION_4, hInst, NULL);
        CreateWindowW(L"Button", L"8",
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            160, 105, 100, 30, hWnd, (HMENU)ID_OPTION_8, hInst, NULL);
        CreateWindowW(L"Button", L"16",
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            160, 130, 100, 30, hWnd, (HMENU)ID_OPTION_16, hInst, NULL);
        CreateWindowW(L"Button", L"32",
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            160, 155, 100, 30, hWnd, (HMENU)ID_OPTION_32, hInst, NULL);
        CreateWindowW(L"Button", L"64",
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            160, 180, 100, 30, hWnd, (HMENU)ID_OPTION_64, hInst, NULL);
        CreateWindowW(L"Button", L"Load WAV file", WS_CHILD | WS_VISIBLE,
            300, 10, 100, 25, hWnd, (HMENU)ID_BUTTON, NULL, NULL);
	
     
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case ID_BUTTON:
                if (SUCCEEDED(hr))
                {
                    IFileOpenDialog* pFileOpen;

                    // Create the FileOpenDialog object.
                    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

                    if (SUCCEEDED(hr))
                    {
                        // Show the Open dialog box.
                        hr = pFileOpen->Show(NULL);

                        // Get the file name from the dialog box.
                        if (SUCCEEDED(hr))
                        {
                            IShellItem* pItem;
                            hr = pFileOpen->GetResult(&pItem);
                            if (SUCCEEDED(hr))
                            {
                                PWSTR pszFilePath;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                // Display the file name to the user.
                                if (SUCCEEDED(hr))
                                {
                                    BYTE* samples;
                                    _wfopen_s(&wavFile, pszFilePath, L"rb");
			
									bool useAsm = false;
									int numberOfThreads = 2;
									if (IsDlgButtonChecked(hWnd, ID_OPTION_ASM)) {
										useAsm = true;
									}
									if (IsDlgButtonChecked(hWnd, ID_OPTION_2)) {
										numberOfThreads = 2;
									} else if (IsDlgButtonChecked(hWnd, ID_OPTION_4)) {
										numberOfThreads = 4;
									}
									else if (IsDlgButtonChecked(hWnd, ID_OPTION_8)) {
										numberOfThreads = 8;
									}
									else if (IsDlgButtonChecked(hWnd, ID_OPTION_16)) {
										numberOfThreads = 16;
									}
									else if (IsDlgButtonChecked(hWnd, ID_OPTION_32)) {
										numberOfThreads = 32;
									}
									else if (IsDlgButtonChecked(hWnd, ID_OPTION_64)) {
										numberOfThreads = 64;
									}
                                    unsigned int size = loadWAV(&wavFile, &samples, useAsm, numberOfThreads);
                                    wchar_t* temp = new wchar_t[128];
                                    wsprintfW(temp, L"File path \n Size: %d", size);
                                    MessageBoxW(NULL, pszFilePath, temp, MB_OK);
                                    CoTaskMemFree(pszFilePath);
                                }
                                pItem->Release();
                            }
                        }
                        pFileOpen->Release();
                    }
                    CoUninitialize();
                }
                break;
			
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Procedura obsługi komunikatów dla okna informacji o programie.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
