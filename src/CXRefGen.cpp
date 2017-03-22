#include <CXRefI.h>

int cxref_indent = 0;

static int cxref_print_newline = true;

void
CXRefPrint(const char *format, ...)
{
  va_list args;

  va_start(args, format);

  std::string str = CStrUtil::vstrprintf(format, &args);

  va_end(args);

  CXRefPrint(str);
}

void
CXRefPrint(const std::string &str)
{
  cxref_control.output_fp->write(str);
}

void
CXRefIPrint(const char *format, ...)
{
  va_list args;

  va_start(args, format);

  std::string str = CStrUtil::vstrprintf(format, &args);

  va_end(args);

  CXRefIPrint(str);
}

void
CXRefIPrint(const std::string &str)
{
  int len1 = str.size();

  for (int i = 0; i < len1; i++) {
    if (cxref_print_newline) {
      for (int j = 0; j < cxref_indent; j++)
        cxref_control.output_fp->write(' ');
    }

    cxref_print_newline = false;

    cxref_control.output_fp->write(str[i]);

    if (str[i] == '\n')
      cxref_print_newline = true;
  }
}
