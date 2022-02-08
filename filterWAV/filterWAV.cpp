#include "framework.h"

#include "filterWAV.h"

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

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

FILE * wavFile;
FILE * wavOutputFile;



unsigned long loadWAV(FILE ** wavFile, BYTE ** samples, bool useAsm, int threadCount, LPWSTR filepath) {
	fseek(*wavFile, 0, SEEK_END);
	unsigned long fsize = ftell(*wavFile);
	rewind(*wavFile);
	*samples = (BYTE *)malloc(fsize + 1);

	if (*samples == NULL) {
		fclose(*wavFile);
		return 0;
	}

	fread_s(*samples, fsize + 1, 1, fsize, *wavFile);
	FileWAV file(*samples);
	file.processInThreads(threadCount, useAsm, filepath);
	fclose(*wavFile);

	return file.getDataSize();
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_FILTERWAVCPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILTERWAVCPP));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILTERWAVCPP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(0xf0f0f0);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FILTERWAVCPP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; 

//	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT, 310, 320, nullptr, nullptr, hInstance, nullptr);


	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	switch (message) {
	case WM_CREATE:
		CreateWindowW(L"Button", L"Choose mode",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			10, 10, 115, 210, hWnd, (HMENU)0, hInst, NULL);
		CreateWindowW(L"Button", L"ASM",
			WS_CHILD | WS_GROUP | WS_VISIBLE | BS_AUTORADIOBUTTON,
			20, 30, 100, 30, hWnd, (HMENU)ID_OPTION_ASM, hInst, NULL);
		CreateWindowW(L"Button", L"CPP",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			20, 55, 100, 30, hWnd, (HMENU)ID_OPTION_CPP, hInst, NULL);
		CreateWindowW(L"Button", L"Choose thread count",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			130, 10, 150, 210, hWnd, (HMENU)0, hInst, NULL);
		CreateWindowW(L"Button", L"1",
			WS_CHILD | WS_GROUP | WS_VISIBLE | BS_AUTORADIOBUTTON ,
			140, 30, 130, 30, hWnd, (HMENU)ID_OPTION_1, hInst, NULL);
		CreateWindowW(L"Button", L"2",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			140, 55, 130, 30, hWnd, (HMENU)ID_OPTION_2, hInst, NULL);
		CreateWindowW(L"Button", L"4",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			140, 80, 130, 30, hWnd, (HMENU)ID_OPTION_4, hInst, NULL);
		CreateWindowW(L"Button", L"8",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			140, 105, 130, 30, hWnd, (HMENU)ID_OPTION_8, hInst, NULL);
		CreateWindowW(L"Button", L"16",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			140, 130, 130, 30, hWnd, (HMENU)ID_OPTION_16, hInst, NULL);
		CreateWindowW(L"Button", L"32",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			140, 155, 130, 30, hWnd, (HMENU)ID_OPTION_32, hInst, NULL);
		CreateWindowW(L"Button", L"64",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			140, 180, 130, 30, hWnd, (HMENU)ID_OPTION_64, hInst, NULL);
		CreateWindowW(L"Button", L"Load WAV file", WS_CHILD | WS_VISIBLE,
			10, 230, 270, 25, hWnd, (HMENU)ID_BUTTON, NULL, NULL);

		break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case ID_BUTTON:
			if (SUCCEEDED(hr)) {
				IFileOpenDialog * pFileOpen;

				hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
					IID_IFileOpenDialog, reinterpret_cast <void **> (&pFileOpen));

				if (SUCCEEDED(hr)) {
					hr = pFileOpen->Show(NULL);

					if (SUCCEEDED(hr)) {
						IShellItem * pItem;
						hr = pFileOpen->GetResult(&pItem);
						if (SUCCEEDED(hr)) {
							PWSTR pszFilePath;
							hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

							if (SUCCEEDED(hr)) {
								BYTE * samples;
								_wfopen_s(&wavFile, pszFilePath, L"rb");

								bool useAsm = false;
								int numberOfThreads = 2;
								if (IsDlgButtonChecked(hWnd, ID_OPTION_ASM)) {
									useAsm = true;
								}
								if (IsDlgButtonChecked(hWnd, ID_OPTION_2)) {
									numberOfThreads = 2;
								}
								else if (IsDlgButtonChecked(hWnd, ID_OPTION_4)) {
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
								unsigned int size = loadWAV(&wavFile, &samples, useAsm, numberOfThreads, pszFilePath);
								wchar_t * temp = new wchar_t[128];
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
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
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

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}