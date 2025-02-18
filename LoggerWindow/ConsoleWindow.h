#pragma once
#include <cstdarg>
#include <string>


class ConsoleWindow
{
public:
  ConsoleWindow();
  ~ConsoleWindow();

  static ConsoleWindow& Instance();

  void Setup(const char* consoleWindowTitle);
  void Teardown();

  void LogEntry(const char* pFilename,
    const unsigned int nLineNumber,
    const std::string& message);

  void LogEntry(const wchar_t* pFilename,
    const unsigned int nLineNumber,
    const std::wstring& message);
};
