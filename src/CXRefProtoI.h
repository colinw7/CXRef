enum CXRefProtoArgType {
  ARG_UNKNOWN = -1,
  ARG_INPUT   = 0,
  ARG_OUTPUT  = 1,
  ARG_BOTH    = 2
};

struct CXRefProto {
  CXRefStringId                  name;
  std::vector<CXRefProtoArgType> types;
  bool                           varargs;
  bool                           dummy;
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
