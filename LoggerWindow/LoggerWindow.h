#pragma once
#include <cstdarg>
#include <Windows.h>


class LoggerWindow
{
public:

  inline LoggerWindow()
    : m_module()
    , m_fnSetup()
    , m_fnLogEntry()
    , m_fnWLogEntry()
    , m_fnTeardown()
  {}

  inline ~LoggerWindow()
  {}

  inline static LoggerWindow& Instance()
  {
    static LoggerWindow loggerWindow;
    return loggerWindow;
  }

  inline void Setup(const char* caption)
  {
    m_module = LoadLibraryA("LoggerWindow.dll");
    if (m_module)
    {
      m_fnSetup = reinterpret_cast<SetupFn>(GetProcAddress(m_module, "Setup"));
      m_fnLogEntry = reinterpret_cast<LogEntryFn>(GetProcAddress(m_module, "LogEntry"));
      m_fnWLogEntry = reinterpret_cast<WLogEntryFn>(GetProcAddress(m_module, "WLogEntry"));
      m_fnTeardown = reinterpret_cast<TeardownFn>(GetProcAddress(m_module, "Teardown"));

      if (m_fnSetup)
        m_fnSetup(caption);
    }
  }

  inline void LogEntry(const char* pFilename,
    const unsigned int nLineNumber,
    const char* pFormatString, ...)
  {
    va_list pVAList;
    va_start(pVAList, pFormatString);
    LogEntry(pFilename, nLineNumber, pFormatString, pVAList);
    va_end(pVAList);
  }

  inline void LogEntry(const wchar_t* pFilename,
    const unsigned int nLineNumber,
    const wchar_t* pFormatString, ...)
  {
    va_list pVAList;
    va_start(pVAList, pFormatString);
    LogEntry(pFilename, nLineNumber, pFormatString, pVAList);
    va_end(pVAList);
  }

  inline void LogEntry(const char* pFilename,
    const unsigned int nLineNumber,
    const char* pFormatString,
    va_list args)
  {
    if (m_fnLogEntry)
      m_fnLogEntry(pFilename, nLineNumber, pFormatString, args);
  }

  inline void LogEntry(const wchar_t* pFilename,
    const unsigned int nLineNumber,
    const wchar_t* pFormatString,
    va_list args)
  {
    if (m_fnWLogEntry)
      m_fnWLogEntry(pFilename, nLineNumber, pFormatString, args);
  }

  inline void Teardown()
  {
    if (m_fnTeardown)
      m_fnTeardown();

    if (m_module)
    {
      FreeLibrary(m_module);
      m_module = nullptr;
      m_fnSetup = nullptr;
      m_fnLogEntry = nullptr;
      m_fnWLogEntry = nullptr;
      m_fnTeardown = nullptr;
    }
  }

private:

  HMODULE m_module;

  using SetupFn = bool(*)(const char*);
  SetupFn m_fnSetup;

  using LogEntryFn = bool(*)(const char*, const unsigned int, const char*, va_list);
  LogEntryFn m_fnLogEntry;

  using WLogEntryFn = bool(*)(const wchar_t*, const unsigned int, const wchar_t*, va_list);
  WLogEntryFn m_fnWLogEntry;

  using TeardownFn = bool(*)();
  TeardownFn m_fnTeardown;
};
