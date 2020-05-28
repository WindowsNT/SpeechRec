#pragma warning(disable:4100)
#pragma warning(disable:5205)

#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <winrt/base.h>
#include <atlbase.h>
#include <winrt/Windows.system.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.SpeechRecognition.h>
#include <winrt/Windows.Media.SpeechSynthesis.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Storage.Streams.h>


#pragma comment(lib, "windowsapp")

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Media::SpeechRecognition;
using namespace Windows::Media::SpeechSynthesis;
using namespace Windows::Globalization;
using namespace winrt::Windows::Storage::Streams;

