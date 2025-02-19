#include "sys_log.h"

WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_IETF);

std::deque<log_msg> syslog_queue;

void printTimestamp(Print *_logOutput, int logLevel)
{
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 0))
  {
    char timeStringBuff[50]{}; // 50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%F %T ", &timeinfo);
    _logOutput->print(timeStringBuff);
  }
  else
  {
    char c[32]{};
    sprintf(c, "%10lu ", millis());
    _logOutput->print(c);
  }
}

void printNewline(Print *_logOutput, int logLevel)
{
  _logOutput->print("");
}

void sys_log(log_prio_level_t log_prio, const char *inputString, ...)
{

  log_msg input;
  va_list args;

  va_start(args, inputString);

  vsnprintf(input.msg, sizeof(input.msg), inputString, args);

  va_end(args);

#if defined(ENABLE_SERIAL)
  Serial.println(input.msg);
#endif

  input.code = log_prio;

  syslog_queue.push_back(input);
}
