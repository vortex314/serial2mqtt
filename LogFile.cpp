#include <LogFile.h>

LogFile::LogFile(const char *prefix, uint32_t max_no, uint32_t max_log_size)
    : _prefix(prefix),
      _max_no(max_no),
      _max_log_size(max_log_size){

      };

void LogFile::count(uint32_t cnt) { _max_no = cnt; }
void LogFile::limit(uint32_t size) { _max_log_size = size; }

void LogFile::prefix(const char *prf) { _prefix = prf; }
void LogFile::console(bool onOff) { _console = onOff; }

void LogFile::append(const char *str, uint32_t length)
{
  if (_console)
    fprintf(stdout, "%s\n", str);

  char file1[100], file2[100];

  if (!_fp)
  {
    sprintf(file1, "%s%d%s", _prefix.c_str(), 0, _postfix.c_str());
    _fp = fopen(file1, "a");
    if (_fp == NULL)
    {
      fprintf(stderr, "open logfile %s failed : %d %s", file1, errno,
              strerror(errno));
    }
  }

  if (_fp)
  {
    if (ftell(_fp) > _max_log_size)
    {
      fclose(_fp);
      _fp = 0;

      for (int i = (_max_no - 1); i >= 0; i--)
      {
        sprintf(file1, "%s%d%s", _prefix.c_str(), i, _postfix.c_str());
        sprintf(file2, "%s%d%s", _prefix.c_str(), i + 1, _postfix.c_str());
        rename(file1, file2);
      }

      sprintf(file1, "%s%d%s", _prefix.c_str(), 0, _postfix.c_str());
      _fp = fopen(file1, "a");
      if (_fp == NULL)
      {
        fprintf(stderr, "open logfile %s failed : %d %s", file1, errno,
                strerror(errno));
      }
    }

    fputs(str, _fp);
    fputs("\n", _fp);
    fflush(_fp);
  }
}
