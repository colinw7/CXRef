CXRefTokenP   CXRefReadGenToken
               (CXRefTokenType type, bool required);
CXRefTokenP   CXRefReadTranslationUnit
               (bool required);
CXRefTypedef *CXRefAddTypedef
               (CXRefTokenP token, CXRefStringId name, CXRefStringId type);
void          CXRefRemoveTypedef
               (const std::string &name);
bool          CXRefIsTypedef
               (const std::string &name);
std::string   CXRefGetTypedefType
               (const std::string &name);
