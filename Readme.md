# Speech Recognition C++ with UWP for plain Win32

An easy way to implement Windows SpeechRecognizer UWP in your Win32 Apps

Your function is:

```C++
HRESULT __stdcall  SpeechX1(void* ptr, SpeechX2 x2, const wchar_t* langx = L"en-us", int Mode = 0);
```

Mode = 2, pass a std::vector<std::tuple<std::wstring,std::wstring>>> as a ptr to get all languages supported

```C++
	std::vector<std::tuple<std::wstring, std::wstring>> sx;
	SpeechX1((void*)&sx, 0, 0, 2);
	for (auto& e : sx)
	{
		std::wcout << std::get<0>(e) << L" - " << std::get<1>(e) << std::endl;
	}
```