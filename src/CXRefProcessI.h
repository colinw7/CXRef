enum class CXRefObjectType {
  CALL  = 0,
  BLOCK = 1
};

enum class CXRefBlockType {
  CODE     = 0,
  FUNCTION = 1,
  DECISION = 2,
  IF       = 3,
  ELIF     = 4,
  ELSE     = 5,
  SWITCH   = 6,
  WHILE    = 7,
  DO       = 8,
  FOR      = 9
};

enum class CXRefStorageClassType {
  AUTOMATIC = 0,
  EXTERNAL  = 1,
  STATIC    = 2
};

enum class CXRefQualifierType {
  NONE     = 0,
  CONSTANT = 1,
  VOLATILE = 2
};

struct CXRefFunc {
  CXRefVar      *returns;
  CXRefStringId  name;
  CXRefStringId  comment;
  CXRefBlock    *block;
  bool           visited;
  int            start_line_num;
  int            start_char_num;
  int            end_line_num;
  int            end_char_num;
};

struct CXRefObject {
  CXRefObjectType  type;
  void            *data;
};

struct CXRefBlock {
  CXRefBlock                 *parent;
  CXRefBlockType              type;
  std::vector<CXRefVar *>     vars;
  std::vector<CXRefVar *>     ext_vars;
  std::vector<CXRefVar *>     glob_vars;
  std::vector<CXRefObject *>  object_list;
  CXRefStringId               comment;
  bool                        gen_comment;
};

struct CXRefCall {
  CXRefStringId               name;
  std::vector<CXRefStringId>  args;
  CXRefStringId               comment;
  CXRefVar                   *returns;
};

struct CXRefVar {
  CXRefStringId  name;
  CXRefType     *type;
  bool           internal;
  bool           used_before_set;
  bool           modified;
  bool           referenced;
};

struct CXRefType {
  CXRefStringId str;
};

void       CXRefProcessGenToken
            (CXRefTokenP token);
void       CXRefProcessTranslationUnit
            (CXRefTokenP);
void       CXRefProcessExternalDeclaration
            (CXRefTokenP);
void       CXRefProcessDeclaration
            (CXRefTokenP);
void       CXRefProcessFunctionDefinition
            (CXRefTokenP);
void       CXRefProcessCompoundStatement
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessStatement
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessElseIfStatement
            (CXRefBlock *, CXRefTokenP, CXRefTokenP);
void       CXRefProcessConstructStatement
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessAssignmentExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessConditionalExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessConstantExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessLogicalOrExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessUnaryExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessPostfixExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessPrimaryExpression
            (CXRefBlock *, CXRefTokenP);
void       CXRefProcessArgumentExpressionList
            (CXRefBlock *, CXRefTokenP);
CXRefFunc *CXRefLookupFunction
            (const std::string &name);
