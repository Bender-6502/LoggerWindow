#pragma warning (disable:4996)
#include "ConsoleWindow.h"
#include <sstream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include <atlstr.h>
#include <filesystem>
#pragma warning (disable:6386)


namespace
{
  std::string timestamp()
  {
    // Get the current time
    time_t rawtime;
    char date[256] = { 0 };
    time(&rawtime);
    auto timeinfo = localtime(&rawtime);
    strftime(date, sizeof(date) - 1, "%x %X", timeinfo);

    // Format the timestamp with the milliseconds
    CStringA stamp;
    stamp.Format("%s.%03d", date, clock() % 1000);

    return stamp.GetString();
  }

  std::wstring wtimestamp()
  {
    // Get the current time
    time_t rawtime;
    wchar_t date[256] = { 0 };
    time(&rawtime);
    auto timeinfo = localtime(&rawtime);
    wcsftime(date, sizeof(date) - 1, L"%x %X", timeinfo);

    // Format the timestamp with the milliseconds
    CStringW stamp;
    stamp.Format(L"%s.%03d", date, clock() % 1000);

    return stamp.GetString();
  }

  std::string filename(const char* pFilename)
  {
    // Get the filename from the path
    std::filesystem::path p(pFilename);
    return p.filename().string();
  }

  std::wstring filename(const wchar_t* pFilename)
  {
    // Get the filename from the path
    std::filesystem::path p(pFilename);
    return p.filename().wstring();
  }

  BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType)
  {
    // Disable ctrl-c and ctrl-break
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
      return TRUE;
    }
    return FALSE;
  }
}

ConsoleWindow::ConsoleWindow()
{}


ConsoleWindow::~ConsoleWindow()
{}


ConsoleWindow& ConsoleWindow::Instance()
{
  static ConsoleWindow consoleWindow;
  return consoleWindow;
}


void ConsoleWindow::Setup(const char* consoleWindowTitle)
{
  // Create a console window
  ::AllocConsole();

  HWND console = ::FindWindowA("ConsoleWindowClass", NULL);
  if (console != NULL)
  {
    // Disable the close button and sys menu
    HMENU menu = ::GetSystemMenu(console, FALSE);
    if (menu != NULL)
      DeleteMenu(menu, SC_CLOSE, MF_BYCOMMAND);

    ::ShowWindow(console, SW_MINIMIZE);
  }

  ::SetConsoleTitleA(consoleWindowTitle);
  ::SetConsoleCtrlHandler(HandlerRoutine, TRUE);
}


void ConsoleWindow::LogEntry(const char* pFilename, const unsigned int nLineNumber, const std::string& message)
{
  // Format the message
  std::ostringstream ss;
  ss
    << filename(pFilename) << "{" << nLineNumber << "}|"
    << timestamp() << "|"
    << message
    << "\n";

  // Write the message to the console
  DWORD written = 0;
  ::WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), ss.str().c_str(), static_cast<DWORD>(ss.str().size()), &written, 0);

  // Write the message to a file
  std::ofstream("LoggerWindow.txt", std::ofstream::out | std::ofstream::app)
    .write(ss.str().data(), ss.str().size())
    .flush();
}


void ConsoleWindow::LogEntry(const wchar_t* pFilename, const unsigned int nLineNumber, const std::wstring& message)
{
  // Format the message
  std::wostringstream ss;
  ss
    << filename(pFilename) << L"{" << nLineNumber << L"}|"
    << wtimestamp() << L"|"
    << message
    << L"\n";

  // Write the message to the console
  DWORD written = 0;
  ::WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), ss.str().c_str(), static_cast<DWORD>(ss.str().size()), &written, 0);

  // Write the message to a file
  std::wofstream(L"LoggerWindow.txt", std::ofstream::out | std::ofstream::app)
    .write(ss.str().data(), ss.str().size())
    .flush();
}


void ConsoleWindow::Teardown()
{
  ::FreeConsole();
}
