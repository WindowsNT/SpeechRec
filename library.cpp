#include "stdafx.h"

typedef HRESULT(__stdcall* SpeechX2)(void* ptr, const wchar_t* reco, int Confi);

#ifndef STATICLIB
extern "C" 
#endif
HRESULT __stdcall  SpeechX1(void* ptr, SpeechX2 x2, const wchar_t* langx = L"en-us", int Mode = 0)
{
	try
	{
		HANDLE hEv1 = CreateEvent(0, 0, 0, 0);
		HANDLE hEv2 = CreateEvent(0, 0, 0, 0);

		Language lang(langx);
		SpeechRecognizer sr(lang);

		if (Mode == 2)
		{
			std::vector<std::tuple<std::wstring, std::wstring>>* sx = (std::vector<std::tuple<std::wstring, std::wstring>>*)ptr;
			auto langs = sr.SupportedGrammarLanguages();
			auto sz = langs.Size();
			for (uint32_t i = 0; i < sz; i++)
			{
				auto ll = langs.GetAt(i).as<Language>();
				auto str = ll.DisplayName();
				auto str2 = str.c_str();
				auto str3 = ll.LanguageTag();
				auto str4 = str3.c_str();

				auto tu = std::make_tuple<std::wstring, std::wstring>(str2, str4);
				sx->push_back(tu);
			}
			return S_OK;
		}

		auto cg = sr.ContinuousRecognitionSession();
		auto e1 = sr.CompileConstraintsAsync();
		IAsyncOperation<SpeechRecognitionResult> e3;
		IAsyncAction e2;


		sr.HypothesisGenerated([&](const IInspectable& sender, SpeechRecognitionHypothesisGeneratedEventArgs  const& args)
			{
				auto te = args.Hypothesis().Text();
				x2(ptr, te.c_str(), (int)-1);
			});

		cg.ResultGenerated([&](const IInspectable& sender, SpeechContinuousRecognitionResultGeneratedEventArgs const& args)
			{
				auto conf = args.Result().Confidence();
				auto hs = args.Result().Text();
				x2(ptr, hs.c_str(), (int)conf);
			});

		ResetEvent(hEv1);
		ResetEvent(hEv2);
		e1.Completed([&](const IInspectable& sender, AsyncStatus h)
			{
				if (h != AsyncStatus::Completed)
				{
					SetEvent(hEv1);
					return;
				}
				if (Mode == 1)
				{
					e3 = sr.RecognizeWithUIAsync();
					e3.Completed([](const IInspectable& sender, AsyncStatus cx)
						{
						});
				}
				else
				{
					e2 = cg.StartAsync();
					e2.Completed([&](const IInspectable& sender, AsyncStatus cx)
						{

						});
				}
				SetEvent(hEv1);
			});
		if (WaitForSingleObject(hEv1, 1000) != WAIT_OBJECT_0)
		{
			CloseHandle(hEv1);
			CloseHandle(hEv2);
			return E_FAIL;
		}

		for (;;)
		{
			if (WaitForSingleObject(hEv2, 0) == WAIT_OBJECT_0)
				break;
			if (FAILED(x2(ptr, 0, 0)))
				break;
		}
		if (e2)
			e2.Cancel();
		if (e3)
			e3.Cancel();
		auto stopped = cg.StopAsync();
		ResetEvent(hEv1);
		stopped.Completed([&](const IInspectable& sender, AsyncStatus h)
			{
				SetEvent(hEv1);
			});
		WaitForSingleObject(hEv1, 1000);
		CloseHandle(hEv1);
		CloseHandle(hEv2);
		return S_OK;
	}
	catch (winrt::hresult_error const& ex)
	{
		winrt::hresult hr = ex.to_abi(); // HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND).
		winrt::hstring message = ex.message(); // The system cannot find the file specified.
		std::wstring str = message.c_str();
		MessageBox(0, str.c_str(), 0, 0);
	}
	return E_FAIL;

//	cg.ResultGenerated();
//	event_token ResultGenerated(TypedEventHandler<SpeechContinuousRecognitionSession, SpeechContinuousRecognitionResultGeneratedEventArgs const&> const& handler) const;

//	sr.CurrentLanguage = L"";
}




#ifndef STATICLIB
extern "C"
#endif
HRESULT __stdcall  SpeechX3(const wchar_t* t, std::vector<uint8_t> * tx, bool XML)
{
	if (!tx)
		return E_POINTER;
	tx->clear();
	HANDLE hEv1 = CreateEvent(0, 0, 0, 0);
	try
	{
		SpeechSynthesizer sy;
		IAsyncOperation<SpeechSynthesisStream> as = XML ? sy.SynthesizeSsmlToStreamAsync(t) : sy.SynthesizeTextToStreamAsync(t);
		as.Completed([&](const IInspectable& sender, AsyncStatus h)
			{
				if (h != AsyncStatus::Completed)
				{
					SetEvent(hEv1);
					return;
				}
				SpeechSynthesisStream sse = as.GetResults();
//				auto is = sse.GetInputStreamAt(0);
				auto sz = sse.Size();
				Buffer bu((uint32_t)sz);
				InputStreamOptions opt = InputStreamOptions::None;
				sse.ReadAsync(bu, (uint32_t)sz, opt);
				const uint8_t* d = bu.data();
				tx->resize(bu.Length());
				memcpy(tx->data(), d, tx->size());
				SetEvent(hEv1);
			});
		for (int i = 0; i < 100; i++)
		{
			if (WaitForSingleObject(hEv1, 0) == WAIT_OBJECT_0)
				break;
			Sleep(100);
		}
		CloseHandle(hEv1);
		return S_OK;
	}
	catch (winrt::hresult_error const& ex)
	{
		winrt::hresult hr = ex.to_abi(); // HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND).
		winrt::hstring message = ex.message(); // The system cannot find the file specified.
		std::wstring str = message.c_str();
		MessageBox(0, str.c_str(), 0, 0);
	}

	CloseHandle(hEv1);
	return E_FAIL;
}
