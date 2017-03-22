#include <CXRefI.h>

static int debug_level = 0;
static int debug_depth = 0;

static void CXRefDebugPrintToken1(CXRefTokenP);

void
CXRefSetDebug(int level)
{
  debug_level = level;
}

bool
CXRefIsDebugOn()
{
  return (debug_level > 0);
}

void
CXRefDebugEnter(const std::string &str)
{
  if (debug_level > 1) {
    for (int i = 0; i < debug_depth; i++)
      std::cerr << " ";

    std::cerr << "> " << str << std::endl;

    ++debug_depth;
  }
}

void
CXRefDebugLeave(const std::string &str)
{
  if (debug_level > 1) {
    --debug_depth;

    for (int i = 0; i < debug_depth; i++)
      std::cerr << " ";

    std::cerr << "< " << str << std::endl;
  }
}

void
CXRefDebugPrintToken(CXRefTokenP token)
{
  if (debug_level < 1 || ! token.isValid())
    return;

  std::cerr << "<" << CXRefGetTypeName(token->type) << "> " <<
               CXRefIdToString(token->file) << "@" <<
               token->line_no << "," << token->char_no << ": ";

  CXRefDebugPrintToken1(token);

  std::cerr << std::endl;
}

void
CXRefDebugPrintToken1(CXRefTokenP token)
{
  if (token->str != CXRefEmptyStrId)
    std::cerr << CXRefIdToString(token->str) << " ";

  std::list<CXRefTokenP>::iterator ptoken1 = token->child_list.begin();
  std::list<CXRefTokenP>::iterator ptoken2 = token->child_list.end  ();

  for ( ; ptoken1 != ptoken2; ++ptoken1)
    CXRefDebugPrintToken1(*ptoken1);
}
