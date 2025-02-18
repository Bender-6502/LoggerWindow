#include <Windows.h>
#include <mutex>
#include "..\LoggerWindow\LoggerWindow.h"


class TestHarnessLogger
{
public:
  TestHarnessLogger()
  {
    LoggerWindow::Instance().Setup("TestHarnessLogger Logs");
  }

 ~TestHarnessLogger()
  {
    LoggerWindow::Instance().Teardown();
  }

  void LogEntry(const char* pFilename,
    const unsigned int nLineNumber,
    const char* pFormatString, ...)
  {
    std::lock_guard<std::mutex> lock(m_logLock);
    va_list vaList;
    va_start(vaList, pFormatString);

    LoggerWindow::Instance().LogEntry(pFilename, nLineNumber, pFormatString, vaList);

    va_end(vaList);
  }

  static TestHarnessLogger& Instance()
  {
    if (!m_pInstance)
      m_pInstance = new TestHarnessLogger();
    return *m_pInstance;
  }

  static void KillInstance()
  {
    if (m_pInstance)
    {
      delete m_pInstance;
      m_pInstance = nullptr;
    }
  }

private:

  static TestHarnessLogger* m_pInstance;
  std::mutex m_logLock;
};
TestHarnessLogger* TestHarnessLogger::m_pInstance = nullptr;

#ifndef LOGENTRY
#define LOGENTRY(...) TestHarnessLogger::Instance().LogEntry(__FILE__, __LINE__, __VA_ARGS__)
#endif


int WINAPI WinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPSTR lpCmdLine,
  _In_ int nShowCmd)
{
  LOGENTRY("Log");
  LOGENTRY("Message");
  LOGENTRY("One");
  LOGENTRY("%d", 2);

  MSG msg;
  BOOL bRet = FALSE;
  while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
  {
    if (bRet == -1)
    {
      // handle the error and possibly exit
      break;
    }
    else
    {
      LOGENTRY("Message received: %d", msg.message);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  TestHarnessLogger::Instance().KillInstance();
}
