enum class CXRefProtoArgType {
  UNKNOWN = -1,
  INPUT   = 0,
  OUTPUT  = 1,
  BOTH    = 2
};

struct CXRefProto {
  CXRefStringId                  name;
  std::vector<CXRefProtoArgType> types;
  bool                           varargs { false };
  bool                           dummy { false };
  CXRefStringId                  include;
};

typedef CRefPtr<CXRefProto> CXRefProtoP;

void              CXRefReadPrototypes
                   (const std::string &filename);
void              CXRefFilePrototypes
                   (const std::string &filename);
void              CXRefAddPrototype
                   (CXRefStringId name, std::vector<CXRefProtoArgType> &types, bool varargs);
void              CXRefAddDummyPrototype
                   (CXRefStringId name);
CXRefProtoP       CXRefGetPrototype
                   (CXRefStringId name);
CXRefProtoArgType CXRefGetPrototypeArgType
                   (CXRefProtoP proto, uint arg_no);
