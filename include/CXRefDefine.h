#include <CGlob.h>

class CPreProDefine {
 public:
  CPreProDefine(const std::string &identifier, const std::string &str);
  CPreProDefine(const std::string &identifier, bool functiom,
                const std::vector<CXRefStringId> &identifiers,
                const std::vector<CXRefCTokenP> &ctokens);
 ~CPreProDefine();

  CXRefStringId getIdentifier() const { return identifier_; }

  CXRefStringId getStr() const { return str_; }

  bool isFunction() const { return function_; }

  uint getNumIdentifiers() const { return uint(identifiers_.size()); }

  const std::vector<CXRefStringId> &getIdentifiers() const { return identifiers_; }

  const std::vector<CXRefCTokenP> &getCTokens() const { return ctokens_; }

  void setTokens(CXRefStringId str);
  void setTokens(const std::vector<CXRefCTokenP> &ctokens);

  void print(std::ostream &os) const;
  void debug_print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CPreProDefine &define) {
    define.print(os);

    return os;
  }

  bool operator==(CXRefStringId identifier) const {
    return (identifier_ == identifier);
  }

  static CPreProDefine *addDefine(const std::string &identifier, const std::string &str) {
    return new CPreProDefine(identifier, str);
  }

  static CPreProDefine *addDefine(const std::string &identifier, bool function,
                                  const std::vector<CXRefStringId> &identifiers,
                                  const std::vector<CXRefCTokenP> &ctokens) {
    return new CPreProDefine(identifier, function, identifiers, ctokens);
  }

  static void addIgnoreDefine(const std::string &pattern);

  static void addIncludeDefine(const std::string &pattern);

  static void removeDefine(CXRefStringId identifier);

  static bool isDefine(const std::string &identifier);

  static bool isDefine(CXRefStringId identifier);

  static CRefPtr<CPreProDefine> getDefine(CXRefStringId identifier);

 private:
  CXRefStringId              identifier_;
  CXRefStringId              str_;
  bool                       function_;
  std::vector<CXRefStringId> identifiers_;
  std::vector<CXRefCTokenP>  ctokens_;
};

typedef CRefPtr<CPreProDefine> CPreProDefineP;

#define CPreProDefineMgrInst CPreProDefineMgr::getInstance()

class CPreProDefineMgr {
 private:
  typedef std::map<CXRefStringId,CPreProDefineP> DefineList;
  typedef std::vector<CGlob>                     GlobList;

  DefineList define_list_;
  GlobList   ignore_define_list_;
  GlobList   include_define_list_;
  bool       debug_defines;

 public:
  static CPreProDefineMgr *getInstance();

  CPreProDefineMgr();
 ~CPreProDefineMgr();

  void addDefine(CPreProDefineP define);
  void addIgnoreDefine(const std::string &pattern);
  void addIncludeDefine(const std::string &pattern);

  bool isDefine(const std::string &identifier);
  bool isDefine(CXRefStringId identifier);

  bool isIgnoreDefine (CXRefStringId pattern);
  bool isIncludeDefine(CXRefStringId pattern);

  CPreProDefineP getDefine(CXRefStringId identifier);

  void removeDefine(CXRefStringId identifier);
};
