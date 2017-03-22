#ifndef CXRefTrace_H
#define CXRefTrace_H

#include <CXRefTokenI.h>

#define CXREF_TRACE(m) \
  CXRefTrace t##__LINE__ = CXRefTrace(m)

#define CXREF_TRACE_TOKEN(m,t) \
  CXRefTrace t##__LINE__ = CXRefTrace(m,t)

class CXRefTrace {
 private:
  const char  *msg_;
  CXRefTokenP  token_;

 public:
  CXRefTrace(const char *msg) :
   msg_(msg), token_() {
    enter();
  }

  CXRefTrace(const char *msg, CXRefTokenP token) :
   msg_(msg), token_(token) {
    enter();
  }

 ~CXRefTrace() {
    leave();
  }

  static uint setLevel(uint level) {
    uint *plevel = getLevelP();

    std::swap(*plevel, level);

    return level;
  }

  static bool isEnabled() {
    return (getLevel() > 0);
  }

  static void printToken(CXRefTokenP token) {
    if (isEnabled()) {
      std::cerr << "<" << CXRefGetTypeName(token->type) << "> " <<
                   CXRefIdToString(token->file) << "@" <<
                   token->line_no << "," << token->char_no << ": ";

      printToken1(token);

      std::cerr << std::endl;
    }
  }

 private:
  void enter() const {
    if (isEnabled()) {
      for (uint i = 0; i < getDepth(); ++i)
        std::cerr << " ";

      std::cerr << "> " << msg_ << std::endl;

      setDepth(getDepth() + 1);
    }
  }

  void leave() const {
    if (isEnabled()) {
      if (token_.isValid())
        printToken(token_);

      setDepth(getDepth() - 1);

      for (uint i = 0; i < getDepth(); i++)
        std::cerr << " ";

      std::cerr << "< " << msg_ << std::endl;
    }
  }

  static void printToken1(CXRefTokenP token) {
    if (token->str != CXRefEmptyStrId)
      std::cerr << CXRefIdToString(token->str) << " ";

    std::list<CXRefTokenP>::iterator ptoken1 = token->child_list.begin();
    std::list<CXRefTokenP>::iterator ptoken2 = token->child_list.end  ();

    for ( ; ptoken1 != ptoken2; ++ptoken1) {
      CXRefTokenP token1 = *ptoken1;

      printToken1(token1);
    }
  }

  static uint getLevel() {
    return *getLevelP();
  }

  static uint *getLevelP() {
    static uint level;

    return &level;
  }

  static void setDepth(uint depth) {
    uint *pdepth = getDepthP();

    *pdepth = depth;
  }

  static uint getDepth() {
    return *getDepthP();
  }

  static uint *getDepthP() {
    static uint depth;

    return &depth;
  }
};

void CXRefDebugEnter(const std::string &str);
void CXRefDebugLeave(const std::string &str);
void CXRefDebugPrintToken(CXRefTokenP);

#endif
