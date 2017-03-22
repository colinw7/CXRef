enum CXRefObjectType {
  OBJECT_CALL  = 0,
  OBJECT_BLOCK = 1
};

enum CXRefBlockType {
  CODE_BLOCK     = 0,
  FUNCTION_BLOCK = 1,
  DECISION_BLOCK = 2,
  IF_BLOCK       = 3,
  ELIF_BLOCK     = 4,
  ELSE_BLOCK     = 5,
  SWITCH_BLOCK   = 6,
  WHILE_BLOCK    = 7,
  DO_BLOCK       = 8,
  FOR_BLOCK      = 9
};

enum CXRefStorageClassType {
  CLASS_AUTOMATIC = 0,
  CLASS_EXTERNAL  = 1,
  CLASS_STATIC    = 2
};

enum CXRefQualifierType {
  QUALIFIER_NONE     = 0,
  QUALIFIER_CONSTANT = 1,
  QUALIFIER_VOLATILE = 2
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
