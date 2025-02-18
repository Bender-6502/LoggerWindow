# LoggerWindow
This c++ library is designed to open a console window for your Windows application and list any log entries sent to it. The setup process is handled with a single include file which dynamically loads the library, and instantiates a console window ready for use. Assuming your Windows application project already has logging and real-time logging would be a benefit, then its a simple process to setup.

## How to use
First add the include file to your code:
```
#include "..\LoggerWindow\LoggerWindow.h"
```
To setup the LoggerWindow from your code, call the following:
```
LoggerWindow::Instance().Setup("TestHarnessLogger Logs");
```
To add a log entry:
```
  void LogEntry(const char* pFilename,
    const unsigned int nLineNumber,
    const char* pFormatString, ...)
  {
    va_list vaList;
    va_start(vaList, pFormatString);

    LoggerWindow::Instance().LogEntry(pFilename, nLineNumber, pFormatString, vaList);

    va_end(vaList);
  }
```
And finally, tear down when logging has finished:
```
LoggerWindow::Instance().Teardown();
```
## More help
For more help see the TestHarness included with the library
