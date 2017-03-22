#include <CXRefI.h>

CPreProDefineMgr *
CPreProDefineMgr::
getInstance()
{
  static CPreProDefineMgr *instance;

  if (! instance)
    instance = new CPreProDefineMgr;

  return instance;
}

CPreProDefineMgr::
CPreProDefineMgr()
{
}

CPreProDefineMgr::
~CPreProDefineMgr()
{
}

void
CPreProDefineMgr::
addDefine(CPreProDefineP define)
{
  CXRefStringId identifier = define->getIdentifier();

  if (isIgnoreDefine(identifier) && ! isIncludeDefine(identifier))
    define->setTokens(identifier);

  removeDefine(identifier);

  define_list_[identifier] = define;

  if (debug_defines) {
    define->debug_print(std::cerr);

    std::cerr << std::endl;
  }
}

void
CPreProDefineMgr::
addIgnoreDefine(const std::string &pattern)
{
  ignore_define_list_.push_back(CGlob(pattern));
}

void
CPreProDefineMgr::
addIncludeDefine(const std::string &pattern)
{
  include_define_list_.push_back(CGlob(pattern));
}

bool
CPreProDefineMgr::
isDefine(const std::string &identifier)
{
  CXRefStringId id = CXRefStringToId(identifier);

  return isDefine(id);
}

bool
CPreProDefineMgr::
isDefine(CXRefStringId identifier)
{
  DefineList::const_iterator p = define_list_.find(identifier);

  return (p != define_list_.end());
}

bool
CPreProDefineMgr::
isIgnoreDefine(CXRefStringId id)
{
  uint num = ignore_define_list_.size();

  if (num > 0) {
    const std::string &name = CXRefIdToString(id);

    for (uint i = 0; i < num; ++i)
      if (ignore_define_list_[i].compare(name))
        return true;
  }

  return false;
}

bool
CPreProDefineMgr::
isIncludeDefine(CXRefStringId id)
{
  uint num = include_define_list_.size();

  if (num > 0) {
    const std::string &name = CXRefIdToString(id);

    for (uint i = 0; i < num; i++)
      if (include_define_list_[i].compare(name))
        return true;
  }

  return false;
}

CPreProDefineP
CPreProDefineMgr::
getDefine(CXRefStringId identifier)
{
  DefineList::const_iterator p = define_list_.find(identifier);

  if (p == define_list_.end())
    return CPreProDefineP();

  /*-------*/

  if (isIgnoreDefine(identifier) && ! isIncludeDefine(identifier))
    return CPreProDefineP();

  /*-------*/

  return (*p).second;
}

void
CPreProDefineMgr::
removeDefine(CXRefStringId identifier)
{
  DefineList::const_iterator p = define_list_.find(identifier);

  if (p != define_list_.end())
    define_list_.erase(identifier);
}

//-----------

CPreProDefine::
CPreProDefine(const std::string &identifier, const std::string &str) :
 identifier_(CXRefStringToId(identifier)), function_(false)
{
  CXRefStringToCTokens(str, ctokens_);

  CPreProDefineMgrInst->addDefine(CPreProDefineP(this));
}

CPreProDefine::
CPreProDefine(const std::string &identifier, bool function,
              const std::vector<CXRefStringId> &identifiers,
              const std::vector<CXRefCTokenP> &ctokens) :
 identifier_(CXRefStringToId(identifier)), function_(function),
 identifiers_(identifiers), ctokens_(ctokens)
{
  CPreProDefineMgrInst->addDefine(CPreProDefineP(this));
}

CPreProDefine::
~CPreProDefine()
{
}

void
CPreProDefine::
setTokens(CXRefStringId str)
{
  ctokens_.clear();

  std::string str1 = CXRefIdToString(str);

  CXRefStringToCTokens(str1, ctokens_);
}

void
CPreProDefine::
setTokens(const std::vector<CXRefCTokenP> &ctokens)
{
  ctokens_ = ctokens;
}

void
CPreProDefine::
debug_print(std::ostream &os) const
{
  os << identifier_;

  if (function_) {
    os << '(';

    uint num_identifiers = identifiers_.size();

    for (uint i = 0; i < num_identifiers; ++i) {
      if (i > 0) os << ',';

      os << identifiers_[i];
    }

    os << ')';
  }

  std::string str = CXRefCTokenListToString(ctokens_, " ");

  os << ' ' << str;
}

void
CPreProDefine::
print(std::ostream &os) const
{
  os << "#define " << CStrUtil::single_quote(CXRefIdToString(identifier_));

  if (function_) {
    os << "(";

    uint num_identifiers = identifiers_.size();

    for (uint i = 0; i < num_identifiers; ++i) {
      if (i > 0) os << ", ";

      os << CStrUtil::single_quote(CXRefIdToString(identifiers_[i]));
    }

    os << ")";
  }

  uint num_ctokens = ctokens_.size();

  for (uint i = 0; i < num_ctokens; ++i)
    os << " " << CStrUtil::single_quote(CXRefIdToString(ctokens_[i]->str));
}

void
CPreProDefine::
addIgnoreDefine(const std::string &pattern)
{
  return CPreProDefineMgrInst->addIgnoreDefine(pattern);
}

void
CPreProDefine::
addIncludeDefine(const std::string &pattern)
{
  return CPreProDefineMgrInst->addIncludeDefine(pattern);
}

void
CPreProDefine::
removeDefine(CXRefStringId identifier)
{
  return CPreProDefineMgrInst->removeDefine(identifier);
}

bool
CPreProDefine::
isDefine(const std::string &identifier)
{
  return CPreProDefineMgrInst->isDefine(identifier);
}

bool
CPreProDefine::
isDefine(CXRefStringId identifier)
{
  return CPreProDefineMgrInst->isDefine(identifier);
}

CRefPtr<CPreProDefine>
CPreProDefine::
getDefine(CXRefStringId identifier)
{
  return CPreProDefineMgrInst->getDefine(identifier);
}
