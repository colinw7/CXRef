#include <CXRefI.h>

struct CXRefErrorData {
  std::string str;
  CXRefTokenP token;
};

static std::vector<CXRefErrorData *> cxref_error_list;

void
CXRefVError(const char *format, ...)
{
  va_list args;

  va_start(args, format);

  std::string str = CStrUtil::vstrprintf(format, &args);

  va_end(args);

  CXRefError(str);
}

void
CXRefError(const std::string &str)
{
  std::cerr << std::endl;
  std::cerr << "Error - " << str << std::endl;

  CXRefPrintLastToken();

  exit(1);
}

void
CXRefAddVError(const char *format, ...)
{
  va_list args;

  va_start(args, format);

  std::string str = CStrUtil::vstrprintf(format, &args);

  va_end(args);

  CXRefAddError(str);
}

void
CXRefAddError(const std::string &str)
{
  CXRefErrorData *error = new CXRefErrorData;

  error->str   = str;
  error->token = CXRefGetLastToken();

  cxref_error_list.push_back(error);
}

std::string
CXRefGetLastError()
{
  CXRefErrorData *error = cxref_error_list.back();

  cxref_error_list.pop_back();

  return error->str;
}
