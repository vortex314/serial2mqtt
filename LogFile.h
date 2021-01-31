#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>

class LogFile
{
  std::string _fileName;
  FILE *_fp = 0;

  std::string _prefix = "LogFile";
  const int _max_no = 5;
  const int _max_log_size = 4 * 1024 * 1024;
  std::string _postfix = ".log";
  bool _console = false;

public:
  LogFile(const char *prefix, uint32_t max_no = 5,
          uint32_t max_log_size = 4 * 1024 * 1024);
  void prefix(const char *prf);
  void console(bool);
  void append(const char *line, uint32_t length);
};