#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#pragma comment(lib,"winmm")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



typedef HRESULT(__stdcall* SpeechX2)(void* ptr, const wchar_t* reco, int conf);
typedef HRESULT(__stdcall* p)(void* ptr, SpeechX2, const wchar_t*, int);
typedef HRESULT(__stdcall* p2)(const wchar_t* t, std::vector<uint8_t>* tx, bool XML);

#ifdef STATICLIB
HRESULT __stdcall  SpeechX1(void* ptr, SpeechX2 x2, const wchar_t* langx = L"en-us", int Mode = 0);
HRESULT __stdcall  SpeechX3(const wchar_t* t, std::vector<uint8_t>* tx, bool XML);
#pragma comment(lib,"speechrecognition.lib")
#endif

HRESULT __stdcall  x2(void* ptr, const wchar_t* reco, int conf)
{
	p2 P2 = (p2)ptr;
	if (!reco)
		return S_OK;
	if (conf == -1)
	{
		// Typing...
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
			| FOREGROUND_GREEN);
		std::wcout << L"\r" << (reco);
		return S_OK;
	}

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
		| FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::wcout << std::endl << conf << L" " << (reco) << std::endl;
	if (P2)
	{
		std::vector<uint8_t> tt;
		P2(reco, &tt, 0);

		FILE* fw = 0;
		DeleteFile(L"1.wav");
		_wfopen_s(&fw, L"1.wav", L"wb");
		fwrite(tt.data(), 1, tt.size(), fw);
		fclose(fw);
		PlaySound(L"1.wav", 0,SND_SYNC | SND_FILENAME);

	}


	return S_OK;
}


int main()
{
#ifdef STATICLIB
	p P = SpeechX1;
	p2 P2 = SpeechX3;
#else
	auto hL = LoadLibrary(L"speechrecognition.dll");
	if (!hL)
		return 0;
	p P = (p)GetProcAddress(hL, "SpeechX1");
	if (!P)
		return 0;
	p2 P2 = (p)GetProcAddress(hL, "SpeechX3");
	if (!P2)
		return 0;
#endif

	// Detect languages
	std::vector<std::tuple<std::wstring, std::wstring>> sx;
	P((void*)&sx, x2, L"en-us", 2);
	if (sx.empty())
	{
		std::wcout << L"No languages found.";
		return 0;
	}

	std::wcout << L"Languages found" << std::endl;
	for (auto& e : sx)
	{
		std::wcout << std::get<0>(e) << L" - " << std::get<1>(e) << std::endl;
	}
	std::wcout << std::endl << L"Picking up first language. Please speak." << std::endl;


	P(P2, x2, std::get<1>(sx[0]).c_str(), 0);

}