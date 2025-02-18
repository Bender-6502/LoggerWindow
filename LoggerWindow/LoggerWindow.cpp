#pragma warning (disable:4996)
#include "ConsoleWindow.h"
#include <cstdarg>
#include <memory>


extern "C"
{
  __declspec(dllexport) void Setup(const char* pCaption)
  {
    // Open new window
    ConsoleWindow::Instance().Setup(pCaption);
  }

  __declspec(dllexport) void LogEntry(const char* pFilename,
    const unsigned int nLineNumber,
    const char* pFormatString,
    va_list args)
  {
    auto buffer = std::make_unique<char[]>(5000);
    vsnprintf(&buffer[0], 5000, pFormatString, args);
    ConsoleWindow::Instance().LogEntry(pFilename, nLineNumber, &buffer[0]);
  }

  __declspec(dllexport) void WLogEntry(const wchar_t* pFilename,
    const unsigned int nLineNumber,
    const wchar_t* pFormatString,
    va_list args)
  {
    auto buffer = std::make_unique<wchar_t[]>(5000);
    _vsnwprintf(&buffer[0], 5000, pFormatString, args);
    ConsoleWindow::Instance().LogEntry(pFilename, nLineNumber, &buffer[0]);
  }

  __declspec(dllexport) void Teardown()
  {
    ConsoleWindow::Instance().Teardown();
  }
}
