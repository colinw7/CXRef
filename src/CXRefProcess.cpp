#include <CXRefI.h>

struct CXRefIfElseData {
  std::vector<CXRefBlock *> blocks;
  std::vector<CXRefTokenP> expressions;
};

static std::vector<CXRefVar *>         cxref_vars;
static CXRefFunc                      *cxref_function        = nullptr;
static std::vector<CXRefFunc *>        cxref_function_list;
static CXRefVar                       *last_var_modified     = nullptr;
static CXRefIfElseData                *cxref_if_else_data    = nullptr;
static std::vector<CXRefIfElseData *>  cxref_if_else_stack;
static bool                            processing_call_args  = false;
static CXRefStringId                   current_block_comment = CXRefEmptyStrId;
static CXRefStringId                   cxref_label_comment   = CXRefEmptyStrId;
static CXRefStorageClassType           default_storage_class = CXRefStorageClassType::EXTERNAL;
static int                             call_depth            = 0;

static void           CXRefCompressBlocks
                       (CXRefFunc *);
static void           CXRefCompressBlock
                       (CXRefBlock *);
static void           CXRefSetFunctionReturns
                       (CXRefFunc *, CXRefTokenP, CXRefTokenP, CXRefTokenP);
static void           CXRefSetFunctionName
                       (CXRefFunc *, CXRefTokenP);
static void           CXRefSetFunctionNewParameterList
                       (CXRefBlock *, CXRefTokenP);
static void           CXRefSetFunctionOldParameterList
                       (CXRefBlock *, CXRefTokenP, CXRefTokenP);
static void           CXRefProcessAddFunctionProto
                       (CXRefFunc *);
static void           CXRefAddFunctionCall
                       (CXRefBlock *, CXRefTokenP, CXRefTokenP);
static CXRefVar      *CXRefAddIdentifierModified
                       (CXRefBlock *, CXRefTokenP);
static CXRefVar      *CXRefAddIdentifierReferenced
                       (CXRefBlock *, CXRefTokenP);
static CXRefVar      *CXRefGetBlockVariable
                       (CXRefBlock *, CXRefStringId name, int *level);
static CXRefVar      *CXRefGetBlockExtVariable
                       (CXRefBlock *, CXRefVar *);
static CXRefVar      *CXRefGetBlockGlobVariable
                       (CXRefBlock *, CXRefVar *);
static void           CXRefDeclarationListToVars
                       (CXRefTokenP declaration_list, std::vector<CXRefVar *> &vars);
static void           CXRefDeclarationToVars
                       (CXRefTokenP declaration, std::vector<CXRefVar *> &vars);
static CXRefTokenP    CXRefGetVariableName
                       (CXRefTokenP parent);
static CXRefTokenP    CXRefGetPostfixExpressionIdentifier
                       (CXRefTokenP postfix_expression);
static CXRefTokenP    CXRefGetPrimaryExpressionIdentifier
                       (CXRefTokenP primary_expression);
static CXRefStringId  CXRefGetTypeString
                       (CXRefTokenP declaration_specifiers, CXRefTokenP pointer,
                        CXRefTokenP direct_declarator, CXRefStorageClassType *storage_class,
                        CXRefQualifierType *type_qualifier);
static void           CXRefPrintObjectVars
                       (CXRefObject *);
static void           CXRefPrintObjectCalls
                       (CXRefObject *object, CXRefFunc *function);
static void           CXRefPrintObjectCFlow
                       (CXRefObject *);
static void           CXRefPrintBlockCFlow
                       (CXRefBlock *);
static CXRefStringId  CXRefGetBlockType
                       (CXRefBlock *);
static CXRefObject   *CXRefCreateObject
                       (CXRefObjectType type, void *data);
static CXRefBlock    *CXRefStartNewSubBlock
                       (CXRefBlock *block, CXRefBlockType type, CXRefTokenP);
static CXRefBlock    *CXRefStartNewBlock
                       (CXRefBlock *block, CXRefBlockType type, CXRefTokenP);
static CXRefBlock    *CXRefCreateBlock
                       (CXRefBlock *block, CXRefBlockType type);
static CXRefCall     *CXRefCreateCall
                       (CXRefTokenP, std::vector<CXRefTokenP> &tokens);
static bool           CXRefRemoveCasts
                       (CXRefTokenP token);
static CXRefVar      *CXRefCreateVar
                       (CXRefTokenP, CXRefStringId type);
static CXRefVar      *CXRefDupVar
                       (CXRefVar *);
static CXRefType     *CXRefCreateType
                       (CXRefStringId name);

namespace CXRefProcessTokenUtil {
  bool NoToken  (CXRefTokenP) { return false; }
  bool DummyLine(CXRefTokenP) { return false; }

  bool PreprocessorStatement(CXRefTokenP) { return false; }
  bool PreprocessorDefine   (CXRefTokenP) { return false; }

  bool Comment(CXRefTokenP) { return false; }

  bool Identifier(CXRefTokenP) { return false; }
  bool Keyword   (CXRefTokenP) { return false; }
  bool Constant  (CXRefTokenP) { return false; }
  bool String    (CXRefTokenP) { return false; }
  bool Operator  (CXRefTokenP) { return false; }
  bool Separator (CXRefTokenP) { return false; }

  bool TranslationUnit(CXRefTokenP token) {
    CXRefProcessTranslationUnit(token);
    return true;
  }
  bool ExternalDeclaration(CXRefTokenP token) {
    CXRefProcessExternalDeclaration(token);
    return true;
  }
  bool FunctionDefinition(CXRefTokenP token) {
    CXRefProcessFunctionDefinition(token);
    return true;
  }
  bool Declaration(CXRefTokenP token) {
    CXRefProcessDeclaration(token);
    return true;
  }
  bool DeclarationList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessDeclarationList(token);
    return true;
  }
  bool DeclarationSpecifiers(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessDeclarationSpecifiers(token);
    return true;
  }
  bool StorageClassSpecifier(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStorageClassSpecifier(token);
    return true;
  }
  bool TypeSpecifier(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessTypeSpecifier(token);
    return true;
  }
  bool TypeQualifier(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessTypeQualifier(token);
    return true;
  }
  bool StructOrUnionSpecifier(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStructOrUnionSpecifier(token);
    return true;
  }
  bool StructOrUnion(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStructOrUnion(token);
    return true;
  }
  bool StructDeclarationList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStructDeclarationList(token);
    return true;
  }
  bool InitDeclaratorList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessInitDeclaratorList(token);
    return true;
  }
  bool InitDeclarator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessInitDeclarator(token);
    return true;
  }
  bool StructDeclaration(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStructDeclaration(token);
    return true;
  }
  bool SpecifierQualifierList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessSpecifierQualifierList(token);
    return true;
  }
  bool StructDeclaratorList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStructDeclaratorList(token);
    return true;
  }
  bool StructDeclarator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStructDeclarator(token);
    return true;
  }
  bool EnumSpecifier(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessEnumSpecifier(token);
    return true;
  }
  bool EnumeratorList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessEnumeratorList(token);
    return true;
  }
  bool Enumerator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessEnumerator(token);
    return true;
  }
  bool Declarator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessDeclarator(token);
    return true;
  }
  bool DirectDeclarator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessDirectDeclarator(token);
    return true;
  }
  bool Pointer(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessPointer(token);
    return true;
  }
  bool TypeQualifierList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessTypeQualifierList(token);
    return true;
  }
  bool ParameterTypeList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessParameterTypeList(token);
    return true;
  }
  bool ParameterList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessParameterList(token);
    return true;
  }
  bool ParameterDeclaration(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessParameterDeclaration(token);
    return true;
  }
  bool IdentifierList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessIdentifierList(token);
    return true;
  }
  bool Initializer(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessInitializer(token);
    return true;
  }
  bool InitializerList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessInitializerList(token);
    return true;
  }
  bool TypeName(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessTypeName(token);
    return true;
  }
  bool AbstractDeclarator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessAbstractDeclarator(token);
    return true;
  }
  bool DirectAbstractDeclarator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessDirectAbstractDeclarator(token);
    return true;
  }
  bool TypedefName(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessTypedefName(token);
    return true;
  }
  bool Statement(CXRefTokenP token) {
    CXRefProcessStatement(nullptr, token);
    return true;
  }
  bool LabeledStatement(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessLabeledStatement(token);
    return true;
  }
  bool ExpressionStatement(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessExpressionStatement(token);
    return true;
  }
  bool CompoundStatement(CXRefTokenP token) {
    CXRefProcessCompoundStatement(nullptr, token);
    return true;
  }
  bool StatementList(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessStatementList(token);
    return true;
  }
  bool SelectionStatement(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessSelectionStatement(token);
    return true;
  }
  bool IterationStatement(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessIterationStatement(token);
    return true;
  }
  bool JumpStatement(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessJumpStatement(token);
    return true;
  }
  bool Expression(CXRefTokenP token) {
    CXRefProcessExpression(nullptr, token);
    return true;
  }
  bool AssignmentExpression(CXRefTokenP token) {
    CXRefProcessAssignmentExpression(nullptr, token);
    return true;
  }
  bool AssignmentOperator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessAssignmentOperator(nullptr, token);
    return true;
  }
  bool ConditionalExpression(CXRefTokenP token) {
    CXRefProcessConditionalExpression(nullptr, token);
    return true;
  }
  bool ConstantExpression(CXRefTokenP token) {
    CXRefProcessConstantExpression(nullptr, token);
    return true;
  }
  bool LogicalOrExpression(CXRefTokenP token) {
    CXRefProcessLogicalOrExpression(nullptr, token);
    return true;
  }
  bool LogicalAndExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessLogicalAndExpression(nullptr, token);
    return true;
  }
  bool InclusiveOrExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessInclusiveOrExpression(nullptr, token);
    return true;
  }
  bool ExclusiveOrExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessExclusiveOrExpression(nullptr, token);
    return true;
  }
  bool AndExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessAndExpression(nullptr, token);
    return true;
  }
  bool EqualityExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessEqualityExpression(nullptr, token);
    return true;
  }
  bool RelationalExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessRelationalExpression(nullptr, token);
    return true;
  }
  bool ShiftExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessShiftExpression(nullptr, token);
    return true;
  }
  bool AdditiveExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessAdditiveExpression(nullptr, token);
    return true;
  }
  bool MultiplicativeExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessMultiplicativeExpression(nullptr, token);
    return true;
  }
  bool CastExpression(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessCastExpression(nullptr, token);
    return true;
  }
  bool UnaryExpression(CXRefTokenP token) {
    CXRefProcessUnaryExpression(nullptr, token);
    return true;
  }
  bool UnaryOperator(CXRefTokenP token) {
    assert(token.isValid());
    //CXRefProcessUnaryOperator(token);
    return true;
  }
  bool PostfixExpression(CXRefTokenP token) {
    CXRefProcessPostfixExpression(nullptr, token);
    return true;
  }
  bool PrimaryExpression(CXRefTokenP token) {
    CXRefProcessPrimaryExpression(nullptr, token);
    return true;
  }
  bool ArgumentExpressionList(CXRefTokenP token) {
    CXRefProcessArgumentExpressionList(nullptr, token);
    return true;
  }
}

void
CXRefProcessGenToken(CXRefTokenP token)
{
  bool rc = false;

#define CENUM_PROCESS
#define CENUM_PROCESS_VAR    token->type
#define CENUM_PROCESS_PREFIX rc = CXRefProcessTokenUtil
#define CENUM_PROCESS_ARG    token
#include <CXRefTokenType.h>
#undef CENUM_PROCESS
#undef CENUM_PROCESS_VAR
#undef CENUM_PROCESS_PREFIX
#undef CENUM_PROCESS_ARG

  if (! rc)
    CXRefPrintToken(token);
}

/*
 * translation-unit:
 *  external-declaration
 *  translation-unit external-declaration
 */

void
CXRefProcessTranslationUnit(CXRefTokenP translation_unit)
{
  std::vector<CXRefTokenP> external_declarations;

  CXRefGetAllTokenChildrenOfType(translation_unit, EXTERNAL_DECLARATION, external_declarations);

  for (uint i = 0; i < external_declarations.size(); i++)
    CXRefProcessExternalDeclaration(external_declarations[i]);
}

/*
 * external-declaration:
 *  function-declaration
 *  declaration
 */

void
CXRefProcessExternalDeclaration(CXRefTokenP external_declaration)
{
  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(external_declaration, children);

  if (CXRefIsTokenType(children[0], DECLARATION)) {
    default_storage_class = CXRefStorageClassType::EXTERNAL;

    CXRefProcessDeclaration(children[0]);
  }
  else {
    default_storage_class = CXRefStorageClassType::AUTOMATIC;

    CXRefProcessFunctionDefinition(children[0]);
  }
}

/*
 * function-definition:
 *  declaration-specifiers(opt) declarator
 *    init-declaration-list(opt) compound-statement
 */

void
CXRefProcessFunctionDefinition(CXRefTokenP function_definition)
{
  assert(CXRefIsTokenType(function_definition, FUNCTION_DEFINITION));

  //------

  CXRefTokenP declaration_specifiers =
    CXRefGetTokenChildOfType(function_definition, DECLARATION_SPECIFIERS);
  CXRefTokenP declarator =
    CXRefGetTokenChildOfType(function_definition, DECLARATOR);
  CXRefTokenP declaration_list =
    CXRefGetTokenChildOfType(function_definition, DECLARATION_LIST);
  CXRefTokenP compound_statement =
    CXRefGetTokenChildOfType(function_definition, COMPOUND_STATEMENT);

  //------

  /*
   * declarator:
   *  pointer(opt) direct_declarator
   */

  CXRefTokenP pointer = CXRefGetTokenChildOfType(declarator, POINTER);

  CXRefTokenP direct_declarator = CXRefGetTokenChildOfType(declarator, DIRECT_DECLARATOR);

  //------

  /*
   * direct_declarator:
   *  direct_declarator '(' parameter_type_list ')'
   *  direct_declarator '(' identifier_list(opt) ')'
   */

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(direct_declarator, children);

  if ((children.size() != 3 && children.size() != 4) ||
      CXRefIsToken(children[1], OPERATOR, CXRefLBracketStrId))
    return;

  CXRefTokenP direct_declarator1 = children[0];

  CXRefTokenP function_parameters;

  if (children.size() == 3)
    function_parameters = CXRefTokenP();
  else
    function_parameters = children[2];

  //------

  CXRefStringId comment = CXRefEmptyStrId;

  if (declaration_specifiers.isValid())
    comment = CXRefGetCurrentComment(declaration_specifiers);

  CXRefBlock *block = CXRefCreateBlock(nullptr, CXRefBlockType::FUNCTION);

  CXRefFunc *function = new CXRefFunc;

  function->returns = nullptr;
  function->comment = comment;
  function->block   = block;
  function->visited = 0;

  std::string file;

  CXRefGetTokenStart(function_definition, file, &function->start_line_num,
                     &function->start_char_num);
  CXRefGetTokenEnd  (function_definition, file, &function->end_line_num  ,
                     &function->end_char_num);

  //------

  block = CXRefStartNewBlock(function->block, CXRefBlockType::CODE, CXRefTokenP());

  //------

  /* Set Return Type String */

  CXRefSetFunctionReturns(function, declaration_specifiers, pointer, direct_declarator1);

  //------

  /* Set Function Name */

  CXRefSetFunctionName(function, direct_declarator1);

  //------

  /* Process New or Old Style Parameter List */

  if (CXRefIsTokenType(function_parameters, PARAMETER_TYPE_LIST)) {
    if (declaration_list.isValid())
      return;

    CXRefSetFunctionNewParameterList(function->block, function_parameters);
  }
  else
    CXRefSetFunctionOldParameterList(function->block, function_parameters, declaration_list);

  //------

  cxref_function = function;

  CXRefProcessCompoundStatement(block, compound_statement);

  cxref_function = nullptr;

  /*-------------*/

  if (cxref_control.compress)
    CXRefCompressBlocks(function);

  /*-------------*/

  cxref_function_list.push_back(function);

  /*-------------*/

  CXRefProcessAddFunctionProto(function);
}

void
CXRefProcessDeclaration(CXRefTokenP declaration)
{
  std::vector<CXRefVar *> vars;

  CXRefDeclarationToVars(declaration, vars);

  cxref_vars = vars;
}

void
CXRefCompressBlocks(CXRefFunc *function)
{
  CXRefObject *object = function->block->object_list.front();

  CXRefBlock *block = reinterpret_cast<CXRefBlock *>(object->data);

  uint num_objects = uint(block->object_list.size());

  for (uint i = 0; i < num_objects; i++) {
    object = block->object_list[i];

    if (object->type == CXRefObjectType::BLOCK)
      CXRefCompressBlock(reinterpret_cast<CXRefBlock *>(object->data));
  }
}

void
CXRefCompressBlock(CXRefBlock *block)
{
  uint num_objects = uint(block->object_list.size());

  for (uint i = 0; i < num_objects; i++) {
    CXRefObject *object = block->object_list[i];

    if (object->type == CXRefObjectType::BLOCK)
      CXRefCompressBlock(reinterpret_cast<CXRefBlock *>(object->data));
  }

  /*------------*/

  num_objects = uint(block->object_list.size());

  if (num_objects != 1)
    return;

  CXRefObject *object = block->object_list.front();

  if (object->type != CXRefObjectType::BLOCK)
    return;

  CXRefBlock *block1 = reinterpret_cast<CXRefBlock *>(object->data);

  if (block1->comment != CXRefEmptyStrId && ! block1->gen_comment &&
      block ->comment != CXRefEmptyStrId && ! block ->gen_comment)
    return;

  if ((block1->vars.size() != 0 || block1->ext_vars.size() != 0 || block1->glob_vars.size() != 0) &&
      (block ->vars.size() != 0 || block ->ext_vars.size() != 0 || block ->glob_vars.size() != 0)) {
    if (block1->vars.size()      > block->vars.size()     ||
        block1->ext_vars.size()  > block->ext_vars.size() ||
        block1->glob_vars.size() > block->glob_vars.size())
      return;

    uint j;

    for (uint i = 0; i < block1->vars.size(); i++) {
      for (j = 0; j < block1->vars.size(); j++)
        if (block1->vars[i]->name == block ->vars[j]->name)
          break;

      if (j >= block1->vars.size())
        return;
    }

    for (uint i = 0; i < block1->ext_vars.size(); i++) {
      for (j = 0; j < block1->ext_vars.size(); j++)
        if (block1->ext_vars[i]->name == block ->ext_vars[j]->name)
          break;

      if (j >= block1->ext_vars.size())
        return;
    }

    for (uint i = 0; i < block1->glob_vars.size(); i++) {
      for (j = 0; j < block1->glob_vars.size(); j++)
        if (block1->glob_vars[i]->name == block ->glob_vars[j]->name)
          break;

      if (j >= block1->glob_vars.size())
        return;
    }
  }
  else {
    if (block->vars.size() == 0) {
      block->vars      = block1->vars;
      block->ext_vars  = block1->ext_vars;
      block->glob_vars = block1->glob_vars;
    }
  }

  if (block->comment == CXRefEmptyStrId || (block->gen_comment && ! block1->gen_comment))
    block->comment = block1->comment;

  if (block->type < block1->type)
    block->type = block1->type;

  block->object_list = block1->object_list;

  delete block1;
  delete object;
}

void
CXRefSetFunctionReturns(CXRefFunc *function, CXRefTokenP declaration_specifiers,
                        CXRefTokenP pointer, CXRefTokenP direct_declarator)
{
  if ((declaration_specifiers.isValid() &&
       ! CXRefIsTokenType(declaration_specifiers, DECLARATION_SPECIFIERS)) ||
      (pointer.isValid() && ! CXRefIsTokenType(pointer, POINTER))) {
    std::cerr << "Invalid token for CXRefSetFunctionReturns" << std::endl;
    assert(false);
  }

  /*-----------------*/

  CXRefStorageClassType storage_class;
  CXRefQualifierType    type_qualifier;

  CXRefStringId str = CXRefGetTypeString(declaration_specifiers, pointer, direct_declarator,
                                         &storage_class, &type_qualifier);

  function->returns = CXRefCreateVar(CXRefTokenP(), str);

  if (storage_class == CXRefStorageClassType::STATIC)
    function->returns->internal = true;

  function->returns->modified   = 1;
  function->returns->referenced = 1;
}

void
CXRefSetFunctionName(CXRefFunc *function, CXRefTokenP direct_declarator)
{
  assert(CXRefIsTokenType(direct_declarator, DIRECT_DECLARATOR));

  /*-----------------*/

  std::string name = CXRefTokenToString(direct_declarator);

  function->name = CXRefStringToId(name);
}

void
CXRefSetFunctionNewParameterList(CXRefBlock *block, CXRefTokenP parameter_type_list)
{
  assert(CXRefIsTokenType(parameter_type_list, PARAMETER_TYPE_LIST));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(parameter_type_list, children);

  bool variable_args = false;

  if (children.size() == 3)
    variable_args = true;

  CXRefTokenP parameter_list = children[0];

  /*-----------*/

  std::vector<CXRefTokenP> parameter_declarations;

  CXRefGetAllTokenChildrenOfType(parameter_list, PARAMETER_DECLARATION, parameter_declarations);

  for (uint i = 0; i < parameter_declarations.size(); i++) {
    std::vector<CXRefTokenP> children1;

    CXRefGetBaseTokenChildren(parameter_declarations[i], children1);

    std::string type_str = " ";

    CXRefTokenP identifier;

    for (uint j = 0; j < children1.size(); ++j) {
      if (! identifier.isValid() && CXRefIsTokenType(children1[j], IDENTIFIER) &&
          CXRefIsTokenParentOfType(children1[j], TYPEDEF_NAME))
        identifier = children1[j];
      else {
        if (CXRefSpaceBeforeToken(children1[j]) && type_str[type_str.size() - 1] != ' ')
          type_str += " ";

        type_str += CXRefTokenToString(children1[j]);

        if (CXRefSpaceAfterToken(children1[j]) && type_str[type_str.size() - 1] != ' ')
          type_str += " ";
      }
    }

    if (type_str[0] == ' ')
      type_str = type_str.substr(1);

    if (type_str[type_str.size() - 1] == ' ')
      type_str = type_str.substr(0, type_str.size() - 1);

    CXRefVar *var = CXRefCreateVar(identifier, CXRefStringToId(type_str));

    block->vars.push_back(var);
  }

  if (variable_args) {
    CXRefVar *var = CXRefCreateVar(CXRefTokenP(), CXRefEllipsisStrId);

    block->vars.push_back(var);
  }
}

void
CXRefSetFunctionOldParameterList(CXRefBlock *block, CXRefTokenP identifier_list,
                                 CXRefTokenP declaration_list)
{
  if ((identifier_list.isValid() && ! CXRefIsTokenType(identifier_list, IDENTIFIER_LIST)) ||
      (declaration_list.isValid() && ! CXRefIsTokenType(declaration_list, DECLARATION_LIST))) {
    std::cerr << "Invalid token for CXRefSetFunctionOldParameterList" << std::endl;
    assert(false);
  }

  /*-----------------*/

  std::vector<CXRefTokenP> identifiers;

  if (identifier_list.isValid())
    CXRefGetAllTokenChildrenOfType(identifier_list, IDENTIFIER, identifiers);

  for (uint i = 0; i < identifiers.size(); i++) {
    CXRefVar *var = CXRefCreateVar(identifiers[i], CXRefEmptyStrId);

    block->vars.push_back(var);
  }

  /*--------------*/

  std::vector<CXRefTokenP> declarations;

  if (declaration_list.isValid())
    CXRefGetAllTokenChildrenOfType(declaration_list, DECLARATION, declarations);

  for (uint i = 0; i < declarations.size(); i++) {
    CXRefTokenP declaration_specifiers =
      CXRefGetTokenChildOfType(declarations[i], DECLARATION_SPECIFIERS);
    CXRefTokenP init_declarator_list =
      CXRefGetTokenChildOfType(declarations[i], INIT_DECLARATOR_LIST);

    if (! init_declarator_list.isValid())
      continue;

    std::vector<CXRefTokenP> init_declarators;

    CXRefGetAllTokenChildrenOfType(init_declarator_list, INIT_DECLARATOR, init_declarators);

    for (uint j = 0; j < init_declarators.size(); j++) {
      CXRefTokenP declarator = CXRefGetTokenChildOfType(init_declarators[j], DECLARATOR);

      CXRefTokenP pointer = CXRefGetTokenChildOfType(declarator, POINTER);

      CXRefTokenP direct_declarator = CXRefGetTokenChildOfType(declarator, DIRECT_DECLARATOR);

      CXRefTokenP identifier = CXRefGetTokenChildOfType(direct_declarator, IDENTIFIER);

      if (! identifier.isValid())
        continue;

      std::string str = CXRefTokenToString(identifier);

      CXRefStringId id = CXRefStringToId(str);

      for (uint k = 0; k < block->vars.size(); k++) {
        if (block->vars[k]->name == id) {
          CXRefStorageClassType storage_class;
          CXRefQualifierType    type_qualifier;

          CXRefStringId str1 =
            CXRefGetTypeString(declaration_specifiers, pointer, direct_declarator,
                               &storage_class, &type_qualifier);

          block->vars[k]->type = CXRefCreateType(str1);
        }
      }
    }
  }
}

void
CXRefProcessAddFunctionProto(CXRefFunc *function)
{
  bool varargs = false;

  std::vector<CXRefProtoArgType> types;

  for (uint i = 0; i < function->block->vars.size(); i++) {
    if (function->block->vars[i]->type->str == CXRefEllipsisStrId) {
      varargs = true;

      continue;
    }

    CXRefProtoArgType type = CXRefProtoArgType::INPUT;

    if (function->block->vars[i]->used_before_set) {
      if (function->block->vars[i]->modified)
        type = CXRefProtoArgType::BOTH;
      else
        type = CXRefProtoArgType::INPUT;
    }
    else if (function->block->vars[i]->modified)
      type = CXRefProtoArgType::OUTPUT;
    else
      type = CXRefProtoArgType::INPUT;

    types.push_back(type);
  }

  CXRefAddPrototype(function->name, types, varargs);
}

void
CXRefProcessCompoundStatement(CXRefBlock *block, CXRefTokenP compound_statement)
{
  assert(CXRefIsTokenType(compound_statement, COMPOUND_STATEMENT));

  /*-----------------*/

  CXRefStringId comment = CXRefGetCurrentComment(compound_statement);

  CXRefBlock *block1 = block;

  if (comment != CXRefEmptyStrId) {
    block1 = CXRefStartNewBlock(block, CXRefBlockType::CODE, CXRefTokenP());

    block1->comment = comment;
  }

  /*-----------------*/

  CXRefTokenP declaration_list = CXRefGetTokenChildOfType(compound_statement, DECLARATION_LIST);

  if (declaration_list.isValid())
    CXRefDeclarationListToVars(declaration_list, block->vars);

  /*-----------------*/

  CXRefTokenP statement_list = CXRefGetTokenChildOfType(compound_statement, STATEMENT_LIST);

  std::vector<CXRefTokenP> statements;

  if (statement_list.isValid())
    CXRefGetAllTokenChildrenOfType(statement_list, STATEMENT, statements);

  /*-----------------*/

  if (cxref_control.split_char != '\0') {
    std::vector<CXRefStringId> comments;

    std::vector<char> splitters;

    splitters.resize(statements.size() + 1);

    for (uint i = 0; i < statements.size(); i++) {
      std::string start_file;
      int         start_line_no;
      int         start_char_no;

      if (i == 0)
        CXRefGetTokenStart(compound_statement, start_file, &start_line_no, &start_char_no);
      else
        CXRefGetTokenEnd(statements[i - 1], start_file, &start_line_no, &start_char_no);

      std::string end_file;
      int         end_line_no;
      int         end_char_no;

      CXRefGetTokenEnd(compound_statement, end_file, &end_line_no, &end_char_no);

      CXRefStringId comment1 =
        CXRefGetCurrentSplitter(statements[i], start_line_no, end_line_no, &splitters[i]);

      comments.push_back(comment1);
    }

    uint i = 0;

    while (i < statements.size()) {
      if (comments[i] != CXRefEmptyStrId && i < statements.size() - 1 && ! splitters[i + 1]) {
        CXRefBlock *block2 = CXRefStartNewBlock(block1, CXRefBlockType::CODE, CXRefTokenP());

        block2->comment = comments[i];

        CXRefProcessStatement(block2, statements[i]);

        ++i;

        while (i < statements.size() && ! splitters[i]) {
          CXRefProcessStatement(block2, statements[i]);

          ++i;
        }
      }
      else {
        current_block_comment = comments[i];

        CXRefProcessStatement(block1, statements[i]);

        current_block_comment = CXRefEmptyStrId;

        ++i;
      }
    }
  }
  else {
    for (uint i = 0; i < statements.size(); i++)
      CXRefProcessStatement(block1, statements[i]);
  }
}

void
CXRefProcessStatement(CXRefBlock *block, CXRefTokenP statement)
{
  assert(CXRefIsTokenType(statement, STATEMENT));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(statement, children);

  CXRefTokenP child = children[0];

  if      (CXRefIsTokenType(child, LABELED_STATEMENT)) {
    CXRefBlock *sub_block = CXRefStartNewSubBlock(block, CXRefBlockType::CODE, child);

    if (! sub_block)
      sub_block = block;

    CXRefTokenP constant_expression = CXRefGetTokenChildOfType(child, CONSTANT_EXPRESSION);

    if (constant_expression.isValid())
      CXRefProcessConstantExpression(sub_block, constant_expression);

    CXRefTokenP statement1 = CXRefGetTokenChildOfType(child, STATEMENT);

    if (constant_expression.isValid()) {
      std::string comment = "case " + CXRefTokenToString(constant_expression);

      cxref_label_comment = CXRefStringToId(comment);
    }
    else {
      CXRefTokenP identifier = CXRefGetTokenChildOfType(child, IDENTIFIER);

      if (! identifier.isValid())
        cxref_label_comment = CXRefStringToId("default");
    }

    CXRefProcessStatement(sub_block, statement1);

    if (cxref_label_comment != CXRefEmptyStrId) {
      sub_block->gen_comment = true;

      cxref_label_comment = CXRefEmptyStrId;
    }
  }
  else if (CXRefIsTokenType(child, EXPRESSION_STATEMENT)) {
    CXRefBlock *sub_block = CXRefStartNewSubBlock(block, CXRefBlockType::CODE, child);

    if (! sub_block)
      sub_block = block;

    CXRefTokenP expression = CXRefGetTokenChildOfType(child, EXPRESSION);

    if (expression.isValid())
      CXRefProcessExpression(sub_block, expression);

    if (cxref_label_comment != CXRefEmptyStrId && sub_block->comment  == CXRefEmptyStrId) {
      sub_block->comment = cxref_label_comment;

      cxref_label_comment = CXRefEmptyStrId;
    }
  }
  else if (CXRefIsTokenType(child, COMPOUND_STATEMENT)) {
    CXRefBlock *sub_block = CXRefStartNewBlock(block, CXRefBlockType::CODE, child);

    CXRefProcessCompoundStatement(sub_block, child);

    if (cxref_label_comment != CXRefEmptyStrId && sub_block->comment  == CXRefEmptyStrId) {
      sub_block->comment = cxref_label_comment;

      cxref_label_comment = CXRefEmptyStrId;
    }
  }
  else if (CXRefIsTokenType(child, SELECTION_STATEMENT)) {
    std::vector<CXRefTokenP> children1;

    CXRefGetTokenChildren(child, children1);

    CXRefTokenP sstatement;

    if      (children1.size() == 5 && CXRefIsToken(children1[0], KEYWORD, CXRefIfStrId)) {
      sstatement = children1[4];

      CXRefBlock *sub_block = CXRefStartNewBlock(block, CXRefBlockType::IF, children1[3]);

      if (sub_block->comment == CXRefEmptyStrId) {
        std::string comment = "if (" + CXRefTokenToString(children1[2]) + ")";

        sub_block->comment = CXRefStringToId(comment);

        sub_block->gen_comment = true;
      }

      CXRefProcessExpression(sub_block, children1[2]);

      CXRefProcessConstructStatement(sub_block, sstatement);
    }
    else if (children1.size() == 7 && CXRefIsToken(children1[0], KEYWORD, CXRefIfStrId)) {
      if (cxref_if_else_data)
        cxref_if_else_stack.push_back(cxref_if_else_data);

      cxref_if_else_data = new CXRefIfElseData;

      CXRefTokenP expression = children1[2];

      sstatement = children1[4];

      CXRefBlock *sub_block  = CXRefStartNewBlock(block, CXRefBlockType::DECISION, CXRefTokenP());
      CXRefBlock *sub_block1 = CXRefStartNewBlock(sub_block, CXRefBlockType::IF, children1[3]);

      cxref_if_else_data->blocks     .push_back(sub_block1);
      cxref_if_else_data->expressions.push_back(expression);

      CXRefProcessExpression(sub_block1, expression);

      CXRefProcessConstructStatement(sub_block1, sstatement);

      sstatement = children1[6];

      CXRefProcessElseIfStatement(sub_block, sstatement, children1[5]);

      uint num = uint(cxref_if_else_data->blocks.size());

      bool single_comment = false;

      if (cxref_if_else_data->blocks[0]->comment != CXRefEmptyStrId) {
        uint i;

        for (i = 1; i < num; i++)
          if (cxref_if_else_data->blocks[i]->comment != CXRefEmptyStrId)
            break;

        if (i >= num)
          single_comment = true;
      }

      if (single_comment) {
        sub_block->comment = cxref_if_else_data->blocks[0]->comment;

        cxref_if_else_data->blocks[0]->comment = CXRefEmptyStrId;
      }

      for (uint i = 0; i < num; i++) {
        sub_block1 = cxref_if_else_data->blocks[i];
        expression = cxref_if_else_data->expressions[i];

        if (sub_block1->comment != CXRefEmptyStrId || ! expression.isValid())
          continue;

        if      (sub_block1->type == CXRefBlockType::IF) {
          std::string comment =  "if (" + CXRefTokenToString(expression) + ")";

          sub_block1->comment = CXRefStringToId(comment);

          sub_block1->gen_comment = true;
        }
        else if (sub_block1->type == CXRefBlockType::ELIF) {
          std::string comment = "else if (" + CXRefTokenToString(expression) + ")";

          sub_block1->comment = CXRefStringToId(comment);

          sub_block1->gen_comment = true;
        }
      }

      delete cxref_if_else_data;

      if (! cxref_if_else_stack.empty()) {
        cxref_if_else_data = cxref_if_else_stack.back();

        cxref_if_else_stack.pop_back();
      }
      else
        cxref_if_else_data = nullptr;
    }
    else if (children1.size() == 5 && CXRefIsToken(children1[0], KEYWORD, CXRefSwitchStrId)) {
      sstatement = children1[4];

      CXRefBlock *sub_block = CXRefStartNewBlock(block, CXRefBlockType::SWITCH, children1[3]);

      if (sub_block->comment == CXRefEmptyStrId) {
        std::string comment = "switch (" + CXRefTokenToString(children1[2]) + ")";

        sub_block->comment = CXRefStringToId(comment);

        sub_block->gen_comment = true;
      }

      CXRefProcessExpression(sub_block, children1[2]);

      CXRefProcessConstructStatement(sub_block, sstatement);
    }
  }
  else if (CXRefIsTokenType(child, ITERATION_STATEMENT)) {
    std::vector<CXRefTokenP> children1;

    CXRefGetTokenChildren(child, children1);

    CXRefTokenP istatement;

    if      (children1.size() == 5 && CXRefIsToken(children1[0], KEYWORD, CXRefWhileStrId)) {
      istatement = children1[4];

      CXRefBlock *sub_block = CXRefStartNewBlock(block, CXRefBlockType::WHILE, children1[3]);

      if (sub_block->comment == CXRefEmptyStrId) {
        std::string comment = "while (" + CXRefTokenToString(children1[2]) + ")";

        sub_block->comment = CXRefStringToId(comment);

        sub_block->gen_comment = true;
      }

      CXRefProcessExpression(sub_block, children1[2]);

      CXRefProcessConstructStatement(sub_block, istatement);
    }
    else if (children1.size() == 7 && CXRefIsToken(children1[0], KEYWORD, CXRefDoStrId)) {
      istatement = children1[1];

      CXRefBlock *sub_block = CXRefStartNewBlock(block, CXRefBlockType::DO, children1[0]);

      if (sub_block->comment == CXRefEmptyStrId) {
        std::string comment = "until (" + CXRefTokenToString(children1[4]) + ")";

        sub_block->comment = CXRefStringToId(comment);

        sub_block->gen_comment = true;
      }

      CXRefProcessConstructStatement(sub_block, istatement);

      CXRefProcessExpression(sub_block, children1[4]);
    }
    else if ((children1.size() >= 6 && children1.size() <= 9) &&
             CXRefIsToken(children1[0], KEYWORD, CXRefForStrId)) {
      istatement = children1[children1.size() - 1];

      int i = 2;

      CXRefTokenP expr1;

      if (CXRefIsTokenType(children1[i], EXPRESSION))
        expr1 = children1[i++];

      i++;

      CXRefTokenP expr2;

      if (CXRefIsTokenType(children1[i], EXPRESSION))
        expr2 = children1[i++];

      i++;

      CXRefTokenP expr3;

      if (CXRefIsTokenType(children1[i], EXPRESSION))
        expr3 = children1[i++];

      i++;

      if (expr1.isValid())
        CXRefProcessExpression(block, expr1);

      CXRefBlock *sub_block =
        CXRefStartNewBlock(block, CXRefBlockType::FOR, children1[children1.size() - 2]);

      if (sub_block->comment == CXRefEmptyStrId) {
        std::string comment = "for (" + CXRefTokenToString(expr1) + ";" +
                                        CXRefTokenToString(expr2) + ";" +
                                        CXRefTokenToString(expr3) + ")";

        sub_block->comment = CXRefStringToId(comment);

        sub_block->gen_comment = true;
      }

      if (expr2.isValid())
        CXRefProcessExpression(sub_block, expr2);

      CXRefProcessConstructStatement(sub_block, istatement);

      if (expr3.isValid())
        CXRefProcessExpression(sub_block, expr3);
    }
  }
  else if (CXRefIsTokenType(child, JUMP_STATEMENT)) {
    std::vector<CXRefTokenP> children1;

    /* Add Module if jump statement has comment */

    CXRefBlock *sub_block = CXRefStartNewSubBlock(block, CXRefBlockType::CODE, child);

    if (! sub_block)
      sub_block = block;

    CXRefGetTokenChildren(child, children1);

    if (CXRefIsToken(children1[0], KEYWORD, CXRefReturnStrId) && children1.size() == 3) {
      if (cxref_function && cxref_function->returns->name == CXRefEmptyStrId) {
        CXRefTokenP identifier = CXRefGetVariableName(children1[1]);

        if (identifier.isValid()) {
          std::string name = CXRefTokenToString(identifier);

          CXRefStringId id = CXRefStringToId(name);

          if (! CPreProDefine::isDefine(id))
            cxref_function->returns->name = id;
        }
      }

      CXRefProcessExpression(sub_block, children1[1]);
    }
  }
}

void
CXRefProcessElseIfStatement(CXRefBlock *block, CXRefTokenP statement, CXRefTokenP /*comment_token*/)
{
  assert(CXRefIsTokenType(statement, STATEMENT));

  /*-----------------*/

  CXRefTokenP selection_statement = CXRefGetTokenChildOfType(statement, SELECTION_STATEMENT);

  CXRefTokenP keyword = (selection_statement.isValid() ?
    CXRefGetTokenChildOfType(selection_statement, KEYWORD) : CXRefTokenP());

  if (keyword.isValid() && CXRefIsToken(keyword, KEYWORD, CXRefIfStrId)) {
    std::vector<CXRefTokenP> children;

    CXRefGetTokenChildren(selection_statement, children);

    CXRefTokenP expression = children[2];
    CXRefTokenP statement1 = children[4];

    CXRefBlock *sub_block = CXRefStartNewBlock(block, CXRefBlockType::ELIF, children[3]);

    cxref_if_else_data->blocks     .push_back(sub_block);
    cxref_if_else_data->expressions.push_back(expression);

    CXRefProcessExpression(sub_block, expression);

    CXRefProcessConstructStatement(sub_block, statement1);

    if (children.size() == 7) {
      statement1 = children[6];

      CXRefProcessElseIfStatement(block, statement1, children[5]);
    }
  }
  else {
    CXRefBlock *sub_block = CXRefStartNewBlock(block, CXRefBlockType::ELSE, statement);

    cxref_if_else_data->blocks     .push_back(sub_block);
    cxref_if_else_data->expressions.push_back(CXRefTokenP());

    CXRefProcessConstructStatement(sub_block, statement);
  }
}

void
CXRefProcessConstructStatement(CXRefBlock *block, CXRefTokenP statement)
{
  assert(CXRefIsTokenType(statement, STATEMENT));

  /*-----------------*/

  CXRefTokenP compound_statement = CXRefGetTokenChildOfType(statement, COMPOUND_STATEMENT);

  if (compound_statement.isValid())
    CXRefProcessCompoundStatement(block, compound_statement);
  else
    CXRefProcessStatement(block, statement);
}

void
CXRefProcessExpression(CXRefBlock *block, CXRefTokenP expression)
{
  assert(CXRefIsTokenType(expression, EXPRESSION));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(expression, children);

  if (children.size() == 1)
    CXRefProcessAssignmentExpression(block, children[0]);
  else {
    CXRefProcessExpression(block, children[0]);

    CXRefProcessAssignmentExpression(block, children[2]);
  }
}

void
CXRefProcessAssignmentExpression(CXRefBlock *block, CXRefTokenP assignment_expression)
{
  assert(CXRefIsTokenType(assignment_expression, ASSIGNMENT_EXPRESSION));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(assignment_expression, children);

  if (children.size() == 3) {
    CXRefTokenP identifier = CXRefGetVariableName(children[0]);

    if (identifier.isValid())
      last_var_modified = CXRefAddIdentifierModified(block, identifier);

    CXRefProcessUnaryExpression(block, children[0]);

    CXRefProcessAssignmentExpression(block, children[2]);

    last_var_modified = nullptr;
  }
  else
    CXRefProcessConditionalExpression(block, children[0]);
}

void
CXRefProcessConditionalExpression(CXRefBlock *block, CXRefTokenP conditional_expression)
{
  assert(CXRefIsTokenType(conditional_expression, CONDITIONAL_EXPRESSION));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(conditional_expression, children);

  if (children.size() == 5) {
    CXRefProcessLogicalOrExpression  (block, children[0]);
    CXRefProcessExpression           (block, children[2]);
    CXRefProcessConditionalExpression(block, children[4]);
  }
  else
    CXRefProcessLogicalOrExpression(block, children[0]);
}

void
CXRefProcessConstantExpression(CXRefBlock *block, CXRefTokenP constant_expression)
{
  assert(CXRefIsTokenType(constant_expression, CONSTANT_EXPRESSION));

  /*-----------------*/

  CXRefTokenP conditional_expression =
    CXRefGetTokenChildOfType(constant_expression, CONDITIONAL_EXPRESSION);

  CXRefProcessConditionalExpression(block, conditional_expression);
}

void
CXRefProcessLogicalOrExpression(CXRefBlock *block, CXRefTokenP logical_or_expression)
{
  assert(CXRefIsTokenType(logical_or_expression, LOGICAL_OR_EXPRESSION));

  /*-----------------*/

  std::vector<CXRefTokenP> unary_expressions;

  CXRefGetAllTokenChildrenOfType(logical_or_expression, UNARY_EXPRESSION, unary_expressions);

  for (uint i = 0; i < unary_expressions.size(); i++)
    CXRefProcessUnaryExpression(block, unary_expressions[i]);
}

void
CXRefProcessUnaryExpression(CXRefBlock *block, CXRefTokenP unary_expression)
{
  assert(CXRefIsTokenType(unary_expression, UNARY_EXPRESSION));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(unary_expression, children);

  if      (children.size() == 1)
    CXRefProcessPostfixExpression(block, children[0]);
  else if (children.size() == 2 &&
           (CXRefIsToken(children[0], OPERATOR, CXRefIncrementStrId) ||
            CXRefIsToken(children[0], OPERATOR, CXRefDecrementStrId))) {
    CXRefTokenP identifier = CXRefGetVariableName(children[1]);

    if (identifier.isValid())
      CXRefAddIdentifierModified(block, identifier);

    CXRefProcessUnaryExpression(block, children[1]);
  }
  else if (children.size() == 2 && CXRefIsTokenType(children[1], CAST_EXPRESSION)) {
    std::vector<CXRefTokenP> unary_expressions;

    CXRefGetAllTokenChildrenOfType(children[1], UNARY_EXPRESSION, unary_expressions);

    for (uint i = 0; i < unary_expressions.size(); i++)
      CXRefProcessUnaryExpression(block, unary_expressions[i]);
  }
  else if (children.size() == 2 && CXRefIsTokenType(children[1], UNARY_EXPRESSION))
    CXRefProcessUnaryExpression(block, children[1]);
}

void
CXRefProcessPostfixExpression(CXRefBlock *block, CXRefTokenP postfix_expression)
{
  assert(CXRefIsTokenType(postfix_expression, POSTFIX_EXPRESSION));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(postfix_expression, children);

  if      (children.size() == 1)
    CXRefProcessPrimaryExpression(block, children[0]);
  else if (children.size() == 4 && CXRefIsToken(children[1], OPERATOR, CXRefLBracketStrId)) {
    CXRefProcessPostfixExpression(block, children[0]);
    CXRefProcessExpression       (block, children[2]);
  }
  else if ((children.size() == 4 || children.size() == 3) &&
           CXRefIsToken(children[1], OPERATOR, CXRefLParenStrId)) {
    CXRefTokenP identifier;
    CXRefTokenP argument_expression_list;

    if (children.size() == 4)
      argument_expression_list = children[2];
    else
      argument_expression_list = CXRefTokenP();

    if (argument_expression_list.isValid())
      CXRefProcessArgumentExpressionList
       (block, argument_expression_list);

    identifier = CXRefGetVariableName(children[0]);

    if (identifier.isValid())
      CXRefAddFunctionCall(block, identifier, argument_expression_list);
  }
  else if (children.size() == 2 &&
           (CXRefIsToken(children[1], OPERATOR, CXRefIncrementStrId) ||
            CXRefIsToken(children[1], OPERATOR, CXRefDecrementStrId))) {
    CXRefTokenP identifier = CXRefGetVariableName(children[0]);

    if (identifier.isValid())
      CXRefAddIdentifierModified(block, identifier);

    CXRefProcessPostfixExpression(block, children[0]);
  }
  else
    CXRefProcessPostfixExpression(block, children[0]);
}

void
CXRefProcessPrimaryExpression(CXRefBlock *block, CXRefTokenP primary_expression)
{
  assert(CXRefIsTokenType(primary_expression, PRIMARY_EXPRESSION));

  /*-----------------*/

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(primary_expression, children);

  if      (children.size() == 1 && CXRefIsTokenType(children[0], IDENTIFIER)) {
    if (! processing_call_args)
      CXRefAddIdentifierReferenced(block, children[0]);
  }
  else if (children.size() == 3)
    CXRefProcessExpression(block, children[1]);
}

void
CXRefProcessArgumentExpressionList(CXRefBlock *block, CXRefTokenP argument_expression_list)
{
  assert(CXRefIsTokenType(argument_expression_list, ARGUMENT_EXPRESSION_LIST));

  /*-----------------*/

  std::vector<CXRefTokenP> assignment_expressions;

  CXRefGetAllTokenChildrenOfType(argument_expression_list, ASSIGNMENT_EXPRESSION,
                                 assignment_expressions);

  processing_call_args = true;

  for (uint i = 0; i < assignment_expressions.size(); i++)
    CXRefProcessAssignmentExpression(block, assignment_expressions[i]);

  processing_call_args = false;
}

void
CXRefAddFunctionCall(CXRefBlock *block, CXRefTokenP identifier,
                     CXRefTokenP argument_expression_list)
{
  if (! CXRefIsTokenType(identifier, IDENTIFIER) ||
      (argument_expression_list.isValid() &&
       ! CXRefIsTokenType(argument_expression_list, ARGUMENT_EXPRESSION_LIST))) {
    std::cerr << "Invalid token for CXRefAddFunctionCall" << std::endl;
    assert(false);
  }

  /*-----------------*/

  std::vector<CXRefTokenP> assignment_expressions;

  CXRefGetAllTokenChildrenOfType(argument_expression_list, ASSIGNMENT_EXPRESSION,
                                 assignment_expressions);

  /*-----------------*/

  CXRefCall *call = CXRefCreateCall(identifier, assignment_expressions);

  CXRefObject *object = CXRefCreateObject(CXRefObjectType::CALL, reinterpret_cast<char *>(call));

  if (block)
    block->object_list.push_back(object);

  /*-----------------*/

  if (cxref_control.prototypes) {
    CXRefProtoP prototype = CXRefGetPrototype(call->name);

    if (prototype.isValid()) {
      for (uint i = 0; i < assignment_expressions.size(); i++) {
        CXRefTokenP identifier1 = CXRefGetVariableName(assignment_expressions[i]);

        if (! identifier1.isValid())
          continue;

        CXRefProtoArgType type = CXRefGetPrototypeArgType(prototype, i + 1);

        if      (type == CXRefProtoArgType::INPUT)
          CXRefAddIdentifierReferenced(block, identifier1);
        else if (type == CXRefProtoArgType::OUTPUT)
          CXRefAddIdentifierModified  (block, identifier1);
        else if (type == CXRefProtoArgType::BOTH) {
          CXRefAddIdentifierReferenced(block, identifier1);
          CXRefAddIdentifierModified  (block, identifier1);
        }
        else
          CXRefAddIdentifierReferenced(block, identifier1);
      }
    }
    else {
      for (uint i = 0; i < assignment_expressions.size(); i++) {
        CXRefTokenP identifier1 = CXRefGetVariableName(assignment_expressions[i]);

        if (! identifier1.isValid())
          continue;

        CXRefAddIdentifierReferenced(block, identifier1);
      }

      std::cerr << "No prototype found for " << call->name << std::endl;

      CXRefAddDummyPrototype(call->name);
    }
  }
}

CXRefVar *
CXRefAddIdentifierModified(CXRefBlock *block, CXRefTokenP identifier)
{
  assert(CXRefIsTokenType(identifier, IDENTIFIER));

  /*-----------------*/

  int level;

  CXRefVar *var = CXRefGetBlockVariable(block, identifier->str, &level);

  if (! var) {
//  std::cerr << "Identifier " << identifier->str << " not declared - " <<
//               identifier->file << "(#" << identifier->line << ")" << std::endl;
    return nullptr;
  }

  /*-----------------*/

  var->modified++;

  /*-----------------*/

  if      (level > 0) {
    while (block && level > 0) {
      CXRefVar *ext_var = CXRefGetBlockExtVariable(block, var);

      if (ext_var)
        ext_var->modified++;

      block = block->parent;

      level--;
    }
  }
  else if (level == -1) {
    CXRefVar *glob_var = CXRefGetBlockGlobVariable(block, var);

    if (glob_var)
      glob_var->modified++;
  }

  return var;
}

CXRefVar *
CXRefAddIdentifierReferenced(CXRefBlock *block, CXRefTokenP identifier)
{
  assert(CXRefIsTokenType(identifier, IDENTIFIER));

  /*-----------------*/

  int level;

  CXRefVar *var = CXRefGetBlockVariable(block, identifier->str, &level);

  if (! var) {
/*
    std::cerr << "Identifier " << identifier->str << " not declared - " <<
                 identifier->file << "(#" << identifier->line << ")" << std::endl;
*/
    return nullptr;
  }

  /*-----------------*/

  var->referenced++;

  if (var->modified <= 0)
    var->used_before_set = true;

  /*-----------------*/

  if      (level > 0) {
    while (block && level > 0) {
      CXRefVar *ext_var = CXRefGetBlockExtVariable(block, var);

      if (ext_var) {
        ext_var->referenced++;

        if (ext_var->modified <= 0)
          ext_var->used_before_set = true;
      }

      block = block->parent;

      level--;
    }
  }
  else if (level == -1) {
    CXRefVar *glob_var = CXRefGetBlockGlobVariable(block, var);

    if (glob_var) {
      glob_var->referenced++;

      if (glob_var->modified <= 0)
        glob_var->used_before_set = true;
    }
  }

  return var;
}

CXRefVar *
CXRefGetBlockVariable(CXRefBlock *block, CXRefStringId name, int *level)
{
  if (name == CXRefEmptyStrId)
    return nullptr;

  *level = 0;

  while (block) {
    for (uint i = 0; i < block->vars.size(); i++)
      if (block->vars[i]->name == name)
        return block->vars[i];

    (*level)++;

    block = block->parent;
  }

  *level = -1;

  for (uint i = 0; i < cxref_vars.size(); i++)
    if (cxref_vars[i]->name == name)
      return cxref_vars[i];

  return nullptr;
}

CXRefVar *
CXRefGetBlockExtVariable(CXRefBlock *block, CXRefVar *var)
{
  for (uint i = 0; i < block->ext_vars.size(); i++)
    if (block->ext_vars[i]->name == var->name)
      return block->ext_vars[i];

  CXRefVar *ext_var = CXRefDupVar(var);

  ext_var->used_before_set = false;
  ext_var->modified        = 0;
  ext_var->referenced      = 0;

  block->ext_vars.push_back(ext_var);

  return ext_var;
}

CXRefVar *
CXRefGetBlockGlobVariable(CXRefBlock *block, CXRefVar *var)
{
  if (var->name == CXRefEmptyStrId)
    return nullptr;

  CXRefVar *glob_var = nullptr;

  uint i;

  for (i = 0; i < block->glob_vars.size(); i++)
    if (block->glob_vars[i]->name == var->name)
      break;

  if (i >= block->glob_vars.size()) {
    glob_var = CXRefDupVar(var);

    glob_var->used_before_set = false;
    glob_var->modified        = 0;
    glob_var->referenced      = 0;

    block->glob_vars.push_back(glob_var);
  }
  else
    glob_var = block->glob_vars[i];

  return glob_var;
}

void
CXRefDeclarationListToVars(CXRefTokenP declaration_list, std::vector<CXRefVar *> &vars)
{
  assert(CXRefIsTokenType(declaration_list, DECLARATION_LIST));

  /*-----------------*/

  std::vector<CXRefTokenP> declarations;

  CXRefGetAllTokenChildrenOfType(declaration_list, DECLARATION, declarations);

  for (uint i = 0; i < declarations.size(); i++) {
    std::vector<CXRefVar *> vars1;

    CXRefDeclarationToVars(declarations[i], vars1);

    copy(vars1.begin(), vars1.end(), back_inserter(vars));
  }
}

void
CXRefDeclarationToVars(CXRefTokenP declaration, std::vector<CXRefVar *> &vars)
{
  assert(CXRefIsTokenType(declaration, DECLARATION));

  /*-----------------*/

  CXRefTokenP declaration_specifiers =
    CXRefGetTokenChildOfType(declaration, DECLARATION_SPECIFIERS);
  CXRefTokenP init_declarator_list =
    CXRefGetTokenChildOfType(declaration, INIT_DECLARATOR_LIST);

  if (! init_declarator_list.isValid())
    return;

  std::vector<CXRefTokenP> init_declarators;

  CXRefGetAllTokenChildrenOfType(init_declarator_list, INIT_DECLARATOR, init_declarators);

  /*-----------------*/

  std::vector<CXRefVar *> var_list;

  for (uint i = 0; i < init_declarators.size(); i++) {
    CXRefTokenP declarator  = CXRefGetTokenChildOfType(init_declarators[i], DECLARATOR);
    CXRefTokenP initializer = CXRefGetTokenChildOfType(init_declarators[i], INITIALIZER);

    CXRefTokenP pointer           = CXRefGetTokenChildOfType(declarator, POINTER);
    CXRefTokenP direct_declarator = CXRefGetTokenChildOfType(declarator, DIRECT_DECLARATOR);

    std::vector<CXRefTokenP> identifiers;

    CXRefGetAllTokenChildrenOfType(direct_declarator, IDENTIFIER, identifiers);

    if (identifiers.size() > 0) {
      CXRefStorageClassType storage_class;
      CXRefQualifierType    type_qualifier;

      CXRefStringId str =
        CXRefGetTypeString(declaration_specifiers, pointer, direct_declarator,
                           &storage_class, &type_qualifier);

/*
      if (storage_class == CXRefStorageClassType::EXTERNAL)
        continue;
*/

      CXRefVar *var = CXRefCreateVar(identifiers[0], str);

      if (storage_class == CXRefStorageClassType::STATIC)
        var->internal = true;

      var_list.push_back(var);

      if (initializer.isValid())
        var->modified++;
    }
  }

  /*-----------------*/

  vars = var_list;
}

CXRefTokenP
CXRefGetVariableName(CXRefTokenP parent)
{
  std::vector<CXRefTokenP> postfix_expressions;

  CXRefGetAllTokenChildrenOfType(parent, POSTFIX_EXPRESSION, postfix_expressions);

  CXRefTokenP identifier;

  if (postfix_expressions.size() > 0)
    identifier = CXRefGetPostfixExpressionIdentifier(postfix_expressions[0]);
  else {
    std::vector<CXRefTokenP> primary_expressions;

    CXRefGetAllTokenChildrenOfType(parent, PRIMARY_EXPRESSION, primary_expressions);

    if (primary_expressions.size() > 0)
      identifier = CXRefGetPrimaryExpressionIdentifier(primary_expressions[0]);
  }

  return identifier;
}

CXRefTokenP
CXRefGetPostfixExpressionIdentifier(CXRefTokenP postfix_expression)
{
  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(postfix_expression, children);

  CXRefTokenP identifier;

  if      (children.size() == 1)
    identifier = CXRefGetPrimaryExpressionIdentifier(children[0]);
  else if (! CXRefIsToken(children[1], OPERATOR, CXRefLParenStrId))
    identifier = CXRefGetPostfixExpressionIdentifier(children[0]);

  return identifier;
}

CXRefTokenP
CXRefGetPrimaryExpressionIdentifier(CXRefTokenP primary_expression)
{
  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(primary_expression, children);

  CXRefTokenP identifier;

  if      (children.size() == 1 && CXRefIsTokenType(children[0], IDENTIFIER))
    identifier = children[0];
  else if (children.size() == 3)
    identifier = CXRefGetVariableName(children[1]);

  return identifier;
}

CXRefStringId
CXRefGetTypeString(CXRefTokenP declaration_specifiers, CXRefTokenP pointer,
                   CXRefTokenP direct_declarator, CXRefStorageClassType *storage_class,
                   CXRefQualifierType *type_qualifier)
{
  if ((declaration_specifiers.isValid() &&
       ! CXRefIsTokenType(declaration_specifiers, DECLARATION_SPECIFIERS)) ||
      (pointer.isValid() && ! CXRefIsTokenType(pointer, POINTER)) ||
      (direct_declarator.isValid() && ! CXRefIsTokenType(direct_declarator, DIRECT_DECLARATOR))) {
    std::cerr << "Invalid token for CXRefGetTypeString" << std::endl;
    assert(false);
  }

  /*-----------------*/

  std::vector<CXRefTokenP> tokens;

  if (declaration_specifiers.isValid()) {
    std::vector<CXRefTokenP> storage_class_specifiers;

    CXRefGetAllTokenChildrenOfType(declaration_specifiers, STORAGE_CLASS_SPECIFIER,
                                   storage_class_specifiers);

    *storage_class = default_storage_class;

    for (uint i = 0; i < storage_class_specifiers.size(); i++) {
      std::string str1 = CXRefTokenToString(storage_class_specifiers[i]);

      if      (str1 == "static")
        *storage_class = CXRefStorageClassType::STATIC;
      else if (str1 == "extern")
        *storage_class = CXRefStorageClassType::EXTERNAL;
    }

    /*-----------------*/

    std::vector<CXRefTokenP> type_qualifiers;

    CXRefGetAllTokenChildrenOfType(declaration_specifiers, TYPE_QUALIFIER, type_qualifiers);

    *type_qualifier = CXRefQualifierType::NONE;

    for (uint i = 0; i < type_qualifiers.size(); i++) {
      std::string str1 = CXRefTokenToString(type_qualifiers[i]);

      if      (str1 == "const")
        *type_qualifier = CXRefQualifierType::CONSTANT;
      else if (str1 == "volatile")
        *type_qualifier = CXRefQualifierType::VOLATILE;
    }

    /*-----------------*/

    std::vector<CXRefTokenP> type_specifiers;

    CXRefGetAllTokenChildrenOfType(declaration_specifiers, TYPE_SPECIFIER, type_specifiers);

    for (uint i = 0; i < type_specifiers.size(); i++)
      tokens.push_back(type_specifiers[i]);
  }
  else {
    *storage_class  = default_storage_class;
    *type_qualifier = CXRefQualifierType::NONE;

    CXRefTokenP token = CXRefCreateToken(KEYWORD, CXRefIntStrId);

    tokens.push_back(token);
  }

  if (pointer.isValid())
    tokens.push_back(pointer);

  if (direct_declarator.isValid()) {
    std::vector<CXRefTokenP> children;

    CXRefGetBaseTokenChildren(direct_declarator, children);

    for (uint i = 0; i < children.size(); i++)
      if (! CXRefIsTokenType(children[i], IDENTIFIER))
        tokens.push_back(children[i]);
  }

  std::string str = CXRefTokensToString(tokens);

  return CXRefStringToId(str);
}

void
CXRefGetFunctions(std::vector<CXRefFunc *> &functions)
{
  functions = cxref_function_list;
}

CXRefFunc *
CXRefLookupFunction(CXRefStringId name)
{
  int num_functions = int(cxref_function_list.size());

  for (int i = 0; i < num_functions; i++) {
    CXRefFunc *function = cxref_function_list[i];

    if (function->name == name)
      return function;
  }

  return nullptr;
}

void
CXRefPrintFunctionPrototypes(std::vector<CXRefFunc *> &functions)
{
  int etype_len    = 0;
  int ename_len    = 0;
  int epointer_len = 0;

  int itype_len    = 0;
  int iname_len    = 0;
  int ipointer_len = 0;

  for (uint i = 0; i < functions.size(); i++) {
    std::string str = CXRefIdToString(functions[i]->returns->type->str);

    int type_len = int(str.size());

    int len1 = type_len - 1;

    int pointer_len = 0;

    while (len1 >= 0 && str[len1] == '*') {
      len1--;

      type_len--;

      pointer_len++;
    }

    if (pointer_len > 0) {
      while (len1 >= 0 && str[len1] == ' ') {
        len1--;

        type_len--;
      }
    }

    std::string name = CXRefIdToString(functions[i]->name);

    int name_len = int(name.size());

    if (functions[i]->returns && functions[i]->returns->internal) {
      if (type_len    > itype_len   ) itype_len    = type_len;
      if (pointer_len > ipointer_len) ipointer_len = pointer_len;
      if (name_len    > iname_len   ) iname_len    = name_len;
    }
    else {
      if (type_len    > etype_len   ) etype_len    = type_len;
      if (pointer_len > epointer_len) epointer_len = pointer_len;
      if (name_len    > ename_len   ) ename_len    = name_len;
    }
  }

  int num_internal = 0;
  int num_external = 0;

  for (uint i = 0; i < functions.size(); i++) {
    if (functions[i]->returns && functions[i]->returns->internal)
      num_internal++;
    else
      num_external++;
  }

  int pos = 0;

  for (uint i = 0; i < functions.size(); i++) {
    if (functions[i]->returns && functions[i]->returns->internal)
      continue;

    if (! cxref_control.show_external)
      continue;

    std::string return_type = CXRefIdToString(functions[i]->returns->type->str);

    uint len = uint(return_type.size());

    int len1 = len - 1;

    int pointer_len = 0;

    while (len1 >= 0 && return_type[len1] == '*') {
      len1--;

      pointer_len++;
    }

    if (pointer_len > 0) {
      while (len1 >= 0 && return_type[len1] == ' ')
        len1--;

      return_type = return_type.substr(0, len1 + 1);
    }

    cxref_control.output_fp->printf("extern %-*s ", etype_len, return_type.c_str());

    pos += etype_len + 9;

    for (int j = 0; j < epointer_len - pointer_len; j++) {
      cxref_control.output_fp->printf(" ");

      ++pos;
    }

    for (int j = 0; j < pointer_len; j++) {
      cxref_control.output_fp->printf("*");

      ++pos;
    }

    std::string name = CXRefIdToString(functions[i]->name);

    cxref_control.output_fp->printf("%-*s\n", ename_len, name.c_str());

    pos = 0;

    cxref_control.output_fp->printf("       %-*s  ", etype_len + epointer_len, "");

    pos = etype_len + epointer_len + 9;

    if      (cxref_control.ansi_proto) {
      cxref_control.output_fp->printf("(");

      pos++;
    }
    else {
      cxref_control.output_fp->printf("ARGS((");

      pos += 6;
    }

    int start_pos = pos;

    bool text_output = false;

    if (functions[i]->block->vars.size() > 0) {
      for (uint j = 0; j < functions[i]->block->vars.size(); j++) {
        std::string var_type = CXRefIdToString(functions[i]->block->vars[j]->type->str);

        len = uint(var_type.size());

        bool pointer = false;

        if (len > 0 && var_type[len - 1] == '*')
          pointer = true;

        if (cxref_control.show_proto_names &&
            functions[i]->block->vars[j]->name != CXRefEmptyStrId) {
          std::string var_name = CXRefIdToString(functions[i]->block->vars[j]->name);

          len += uint(var_name.size());

          if (! pointer)
            len++;
        }

        if (text_output)
          len++;

        if (j < functions[i]->block->vars.size() - 1)
          len++;
        else
          len += 3;

        if (len + pos > 79 && len + start_pos <= 79) {
          cxref_control.output_fp->printf("\n");

          pos = 0;

          cxref_control.output_fp->printf("%-*s", start_pos, "");

          pos += start_pos;

          text_output = false;
        }

        if (text_output) {
          cxref_control.output_fp->printf(" ");

          ++pos;
        }

        cxref_control.output_fp->printf("%s", var_type.c_str());

        pos += int(var_type.size());

        if (cxref_control.show_proto_names &&
            functions[i]->block->vars[j]->name != CXRefEmptyStrId) {
          std::string var_name = CXRefIdToString(functions[i]->block->vars[j]->name);

          if (! pointer) {
            cxref_control.output_fp->printf(" ");

            ++pos;
          }

          cxref_control.output_fp->printf("%s", var_name.c_str());

          pos += int(var_name.size());
        }

        if (j < functions[i]->block->vars.size() - 1) {
          cxref_control.output_fp->printf(",");

          ++pos;
        }

        text_output = true;
      }
    }
    else {
      cxref_control.output_fp->printf("void");

      pos += 4;
    }

    if (cxref_control.ansi_proto)
      cxref_control.output_fp->printf(");\n");
    else
      cxref_control.output_fp->printf("));\n");

    pos = 0;
  }

  if (num_external > 0 && num_internal > 0)
    cxref_control.output_fp->printf("\n");

  for (uint i = 0; i < functions.size(); i++) {
    if (! functions[i]->returns || ! functions[i]->returns->internal)
      continue;

    if (! cxref_control.show_internal)
      continue;

    std::string return_type = CXRefIdToString(functions[i]->returns->type->str);

    uint len = uint(return_type.size());

    int len1 = len - 1;

    uint pointer_len = 0;

    while (len1 >= 0 && return_type[len1] == '*') {
      len1--;

      pointer_len++;
    }

    if (pointer_len > 0) {
      while (len1 >= 0 && return_type[len1] == ' ')
        len1--;

      return_type = return_type.substr(0, len1 + 1);
    }

    pos = 0;

    cxref_control.output_fp->printf("static %-*s ", itype_len, return_type.c_str());

    pos += itype_len + 9;

    for (uint j = 0; j < ipointer_len - pointer_len; j++) {
      cxref_control.output_fp->printf(" ");

      ++pos;
    }

    for (uint j = 0; j < pointer_len; j++) {
      cxref_control.output_fp->printf("*");

      ++pos;
    }

    if (pointer_len > 0)
      return_type[len1 + 1] = ' ';

    std::string name = CXRefIdToString(functions[i]->name);

    cxref_control.output_fp->printf("%-*s\n", iname_len, name.c_str());

    pos = 0;

    cxref_control.output_fp->printf("       %-*s  ", itype_len + ipointer_len, "");

    pos += itype_len + ipointer_len + 9;

    if      (cxref_control.ansi_proto) {
      cxref_control.output_fp->printf("(");

      pos++;
    }
    else {
      cxref_control.output_fp->printf("ARGS((");

      pos += 6;
    }

    uint start_pos = pos;

    bool text_output = false;

    if (functions[i]->block->vars.size() > 0) {
      for (uint j = 0; j < functions[i]->block->vars.size(); j++) {
        std::string var_type = CXRefIdToString(functions[i]->block->vars[j]->type->str);

        len = uint(var_type.size());

        bool pointer = false;

        if (len > 0 && var_type[len - 1] == '*')
          pointer = true;

        if (cxref_control.show_proto_names &&
            functions[i]->block->vars[j]->name != CXRefEmptyStrId) {
          std::string var_name = CXRefIdToString(functions[i]->block->vars[j]->name);

          len += uint(var_name.size());

          if (! pointer)
            len++;
        }

        if (text_output)
          len++;

        if (j < functions[i]->block->vars.size() - 1)
          len++;
        else
          len += 3;

        if (len + pos > 79 && len + start_pos <= 79) {
          cxref_control.output_fp->printf("\n");

          pos = 0;

          cxref_control.output_fp->printf("%-*s", start_pos, "");

          pos += start_pos;

          text_output = false;
        }

        if (text_output) {
          cxref_control.output_fp->printf(" ");

          ++pos;
        }

        cxref_control.output_fp->printf("%s", var_type.c_str());

        pos += int(var_type.size());

        if (cxref_control.show_proto_names &&
            functions[i]->block->vars[j]->name != CXRefEmptyStrId) {
          std::string var_name = CXRefIdToString(functions[i]->block->vars[j]->name);

          if (! pointer) {
            cxref_control.output_fp->printf(" ");

            ++pos;
          }

          cxref_control.output_fp->printf("%s", var_name.c_str());

          pos += int(var_name.size());
        }

        if (j < functions[i]->block->vars.size() - 1) {
          cxref_control.output_fp->printf(",");

          ++pos;
        }

        text_output = true;
      }
    }
    else {
      cxref_control.output_fp->printf("void");

      pos += 4;
    }

    if (cxref_control.ansi_proto)
      cxref_control.output_fp->printf(");\n");
    else
      cxref_control.output_fp->printf("));\n");

    pos = 0;
  }
}

void
CXRefPrintFunctionHeaders(std::vector<CXRefFunc *> &functions)
{
  for (uint i = 0; i < functions.size(); i++)
    CXRefPrintFunctionHeader(functions[i]);
}

void
CXRefPrintFunctionHeader(CXRefFunc *function)
{
  bool is_void_return = (function->returns->type->str == CXRefVoidStrId);

  if (function->returns->name == CXRefEmptyStrId)
    function->returns->name = CXRefStringToId("retval");

  std::string return_name = CXRefIdToString(function->returns->name);

  cxref_control.output_fp->printf("/*--------------------------------------"
                                  "-------------------------------------*\n");
  cxref_control.output_fp->printf(" *\n");

  std::string name = CXRefIdToString(function->name);

  cxref_control.output_fp->printf(" * ##NAME\n");
  cxref_control.output_fp->printf(" *    %s\n", name.c_str());
  cxref_control.output_fp->printf(" *\n");

  cxref_control.output_fp->printf(" * ##FUNCTION\n");
  cxref_control.output_fp->printf(" *    Description of Function\n");
  cxref_control.output_fp->printf(" *\n");

  cxref_control.output_fp->printf(" * ##CALL_DETAILS\n");

  if (! is_void_return) {
    std::string return_type = CXRefIdToString(function->returns->type->str);

    cxref_control.output_fp->printf(" *    %s", return_type.c_str());

    int len1 = int(return_type.size());

    if (return_type[len1 - 1] != '*')
      cxref_control.output_fp->printf(" ");

    std::string return_var = CXRefIdToString(function->returns->name);

    cxref_control.output_fp->printf("%s =\n", return_var.c_str());
  }
  else
    cxref_control.output_fp->printf(" *    (void)\n");

  cxref_control.output_fp->printf(" *      %s(", name.c_str());

  int start_pos = int(name.size() + 9);

  int pos = start_pos;

  bool is_void_args = (function->block->vars.size() == 1 &&
                       function->block->vars[0]->type->str == CXRefVoidStrId);

  if (! is_void_args) {
    bool first_arg = true;

    for (uint i = 0; i < function->block->vars.size(); i++) {
      std::string var_type = CXRefIdToString(function->block->vars[i]->type->str);
      std::string var_name = CXRefIdToString(function->block->vars[i]->name);

      int len1 = int(var_type.size());

      bool is_vararg = (function->block->vars[i]->type->str == CXRefEllipsisStrId);

      int len;

      if (! is_vararg) {
        len = int(var_type.size() + var_name.size());

        if (var_type[len1 - 1] != '*')
          len++;
      }
      else
        len = 3;

      if (! first_arg)
        len++;

      if (i == function->block->vars.size() - 1)
        len++;

      if (pos > start_pos && len + pos > 72) {
        pos = start_pos;

        cxref_control.output_fp->printf("\n * %*.*s", start_pos - 3, start_pos - 3, " ");

        first_arg = true;
      }

      if (! first_arg)
        cxref_control.output_fp->printf(" ");

      if (! is_vararg) {
        cxref_control.output_fp->printf("%s", var_type.c_str());

        if (var_type[len1 - 1] != '*')
          cxref_control.output_fp->printf(" ");

        cxref_control.output_fp->printf("%s", var_name.c_str());
      }
      else
        cxref_control.output_fp->printf("...");

      if (i < function->block->vars.size() - 1)
        cxref_control.output_fp->printf(",");

      first_arg = false;

      pos += len;
    }

    cxref_control.output_fp->printf(");\n");
  }
  else
    cxref_control.output_fp->printf("void);\n");

  cxref_control.output_fp->printf(" *\n");

  int max_len = 0;

  if (! is_void_return)
    max_len = int(name.size());

  for (uint i = 0; i < function->block->vars.size(); i++) {
    if (is_void_args)
      continue;

    int len;

    if (function->block->vars[i]->type->str != CXRefEllipsisStrId) {
      std::string var_name = CXRefIdToString(function->block->vars[i]->name);

      len = int(var_name.size());
    }
    else
      len = 3;

    if (len > max_len)
      max_len = len;
  }

  cxref_control.output_fp->printf(" * ##INPUT_ARGS\n");

  bool output = false;

  for (uint i = 0; i < function->block->vars.size(); i++) {
    if (is_void_args)
      continue;

    if (function->block->vars[i]->type->str != CXRefEllipsisStrId) {
      if (function->block->vars[i]->modified && ! function->block->vars[i]->used_before_set)
        continue;

      if (output)
        cxref_control.output_fp->printf(" *\n");

      std::string var_name = CXRefIdToString(function->block->vars[i]->name);

      cxref_control.output_fp->printf(" *    %-*.*s : Description of Arg\n",
                                      max_len, max_len, var_name.c_str());
    }
    else {
      if (output)
        cxref_control.output_fp->printf(" *\n");

      cxref_control.output_fp->printf(" *    %-*.*s : Variable arguments\n",
                                      max_len, max_len, "...");
    }

    output = true;
  }

  if (! output)
    cxref_control.output_fp->printf(" *    None\n");

  cxref_control.output_fp->printf(" *\n");

  cxref_control.output_fp->printf(" * ##OUTPUT_ARGS\n");

  output = false;

  for (uint i = 0; i < function->block->vars.size(); i++) {
    if (is_void_args)
      continue;

    if (function->block->vars[i]->type->str == CXRefEllipsisStrId)
      continue;

    if (! function->block->vars[i]->modified)
      continue;

    if (output)
      cxref_control.output_fp->printf(" *\n");

    std::string var_name = CXRefIdToString(function->block->vars[i]->name);

    cxref_control.output_fp->printf(" *    %-*.*s : Description of Arg\n",
                                    max_len, max_len, var_name.c_str());

    output = true;
  }

  if (! output)
    cxref_control.output_fp->printf(" *    None\n");

  cxref_control.output_fp->printf(" *\n");

  cxref_control.output_fp->printf(" * ##RETURN_ARG\n");

  if (! is_void_return)
    cxref_control.output_fp->printf(" *    %-*.*s : Description of Arg\n",
                                    max_len, max_len, return_name.c_str());
  else
    cxref_control.output_fp->printf(" *    None\n");

  cxref_control.output_fp->printf(" * \n");

  cxref_control.output_fp->printf(" * ##NOTES\n");
  cxref_control.output_fp->printf(" *    None\n");
  cxref_control.output_fp->printf(" * \n");

  cxref_control.output_fp->printf(" *-------------------------------------"
                                    "-------------------------------------*\n");

  cxref_control.output_fp->printf("\n");
}

void
CXRefPrintFunctionPrototype(CXRefFunc *function)
{
  std::string name = CXRefIdToString(function->name);

  std::string return_type = CXRefIdToString(function->returns->type->str);

  cxref_control.output_fp->printf("%s %s ", return_type.c_str(), name.c_str());

  if      (cxref_control.ansi_proto)
    cxref_control.output_fp->printf("(");
  else
    cxref_control.output_fp->printf("ARGS((");

  for (uint i = 0; i < function->block->vars.size(); i++) {
    std::string var_type = CXRefIdToString(function->block->vars[i]->type->str);

    if (i > 0) cxref_control.output_fp->printf(", ");

    cxref_control.output_fp->printf("%s", var_type.c_str());
  }

  if (cxref_control.ansi_proto)
    cxref_control.output_fp->printf(")\n");
  else
    cxref_control.output_fp->printf("))\n");
}

void
CXRefPrintFunctionVars(CXRefFunc *function)
{
  if (function->comment != CXRefEmptyStrId)
    cxref_control.output_fp->printf("%s\n", CXRefIdToCStr(function->comment));

  cxref_control.output_fp->printf("%s\n", CXRefIdToCStr(function->name));

  process_vector(function->block->object_list, CXRefPrintObjectVars);
}

void
CXRefPrintObjectVars(CXRefObject *object)
{
  if (object->type != CXRefObjectType::BLOCK)
    return;

  CXRefBlock *block = reinterpret_cast<CXRefBlock *>(object->data);

  if (block->comment != CXRefEmptyStrId)
    cxref_control.output_fp->printf("%s\n", CXRefIdToCStr(block->comment));

  CXRefStringId type = CXRefGetBlockType(block);

  if (type != CXRefEmptyStrId)
    cxref_control.output_fp->printf(" %s { ", CXRefIdToCStr(type));
  else
    cxref_control.output_fp->printf(" { ");

  for (uint i = 0; i < block->vars.size(); i++) {
    std::string var_type = CXRefIdToString(block->vars[i]->type->str);
    std::string var_name = CXRefIdToString(block->vars[i]->name);

    cxref_control.output_fp->printf("%s %s [%sMod %d, Ref %d] ; ",
      var_type.c_str(), var_name.c_str(),
      (block->vars[i]->used_before_set ?  "Used before set " : ""),
      block->vars[i]->modified, block->vars[i]->referenced);
  }

  if (block->ext_vars.size() > 0) {
    cxref_control.output_fp->printf("\nextern ");

    for (uint i = 0; i < block->ext_vars.size(); i++) {
      std::string var_type = CXRefIdToString(block->ext_vars[i]->type->str);
      std::string var_name = CXRefIdToString(block->ext_vars[i]->name);

      cxref_control.output_fp->printf("%s %s [%sMod %d, Ref %d] ; ",
        var_type.c_str(), var_name.c_str(),
        (block->ext_vars[i]->used_before_set ?  "Used before set " : ""),
        block->ext_vars[i]->modified, block->ext_vars[i]->referenced);
    }
  }

  if (! block->object_list.empty()) {
    cxref_control.output_fp->printf("\n");

    process_vector(block->object_list, CXRefPrintObjectVars);
  }

  cxref_control.output_fp->printf("}\n");
}

void
CXRefPrintFunctionCalls(CXRefFunc *function)
{
  call_depth = 1;

/*
  if (function->comment != CXRefEmptyStrId)
    cxref_control.output_fp->printf("%s\n", function->comment.c_str());
*/

  if (cxref_control.output_html) {
    cxref_control.output_fp->printf("<h2>%s</h2>\n", CXRefIdToCStr(function->name));

    cxref_control.output_fp->printf("<ul>\n");
  }
  else
    cxref_control.output_fp->printf("%s\n", CXRefIdToCStr(function->name));

  function->visited = 1;

  process_vector(function->block->object_list, CXRefPrintObjectCalls, function);

  function->visited = 0;

  if (cxref_control.output_html)
    cxref_control.output_fp->printf("</ul>");
}

void
CXRefPrintObjectCalls(CXRefObject *object, CXRefFunc *function)
{
  if (object->type != CXRefObjectType::BLOCK)
    return;

  CXRefBlock *block = reinterpret_cast<CXRefBlock *>(object->data);

/*
  if (block->comment != CXRefEmptyStrId)
    cxref_control.output_fp->printf("%s\n", block->comment.c_str());

  CXRefStringId type = CXRefGetBlockType(block);

  if (type != CXRefEmptyStrId)
    cxref_control.output_fp->printf(" %s { ", type.c_str());
  else
    cxref_control.output_fp->printf(" { ");
*/

  int num = int(block->object_list.size());

  for (int i = 0; i < num; i++) {
    CXRefObject *object1 = block->object_list[i];

    if (object1->type == CXRefObjectType::CALL) {
      CXRefCall *call = reinterpret_cast<CXRefCall *>(object1->data);

/*
      cxref_control.output_fp->printf("%s(%d)\n", call->name.c_str(), call->args.size());
*/

      if (cxref_control.output_html)
        cxref_control.output_fp->printf("<li>%s</li>\n", CXRefIdToCStr(call->name));
      else {
        for (int j = 0; j < call_depth; j++)
          cxref_control.output_fp->printf("  ");

        cxref_control.output_fp->printf("%s\n", CXRefIdToCStr(call->name));
      }

      CXRefFunc *function1 = nullptr;

      if (call->name != function->name && call_depth < 8)
        function1 = CXRefLookupFunction(call->name);

      if (function1 && function1->visited)
        function1 = nullptr;

      if (function1) {
        call_depth++;

        if (cxref_control.output_html)
          cxref_control.output_fp->printf("<ul>\n");

        function1->visited = 1;

        process_vector(function1->block->object_list, CXRefPrintObjectCalls, function);

        function1->visited = 0;

        if (cxref_control.output_html)
          cxref_control.output_fp->printf("</ul>\n");

        call_depth--;
      }
    }
    else
      CXRefPrintObjectCalls(object1, function);
  }

/*
  cxref_control.output_fp->printf("\n");
  cxref_control.output_fp->printf("}\n");
*/
}

void
CXRefPrintFunctionCFlow(CXRefFunc *function)
{
  cxref_indent = 0;

  CXRefIPrint("Function: %s\n", CXRefIdToCStr(function->name));

  if (function->comment != CXRefEmptyStrId)
    CXRefIPrint("Comment: %s\n", CXRefIdToCStr(function->comment));

  if (function->returns && function->returns->name != CXRefEmptyStrId)
    CXRefIPrint("Returns: %s\n", CXRefIdToCStr(function->returns->name));

  cxref_indent++;

  for (uint i = 0; i < function->block->vars.size(); i++) {
    if (function->block->vars[i]->name == CXRefEmptyStrId)
      continue;

    if      (function->block->vars[i]->used_before_set && function->block->vars[i]->modified)
      CXRefIPrint("Input/Output");
    else if (function->block->vars[i]->modified)
      CXRefIPrint("Output");
    else
      CXRefIPrint("Input");

    CXRefIPrint(" Var: %s\n", CXRefIdToCStr(function->block->vars[i]->name));
  }

  process_vector(function->block->object_list, CXRefPrintObjectCFlow);
}

void
CXRefPrintObjectCFlow(CXRefObject *object)
{
  if (object->type == CXRefObjectType::BLOCK)
    CXRefPrintBlockCFlow(reinterpret_cast<CXRefBlock *>(object->data));
  else {
    CXRefCall *call = reinterpret_cast<CXRefCall *>(object->data);

    CXRefIPrint("Call: ");

    if (call->returns)
      CXRefIPrint("%s = ", CXRefIdToCStr(call->returns->name));

    CXRefIPrint("%s(", CXRefIdToCStr(call->name));

    for (uint i = 0; i <  call->args.size(); i++) {
      if (i > 0) CXRefIPrint(", ");

      CXRefIPrint("%s", CXRefIdToCStr(call->args[i]));
    }

    CXRefIPrint(")\n");

    if (call->comment != CXRefEmptyStrId)
      CXRefIPrint("Comment: %s\n", CXRefIdToCStr(call->comment));
  }
}

void
CXRefPrintBlockCFlow(CXRefBlock *block)
{
  CXRefStringId type = CXRefGetBlockType(block);

  if (type != CXRefEmptyStrId) {
    std::string type_str =  CStrUtil::toUpper(CXRefIdToString(type));

    CXRefIPrint("%s Block:\n", type_str.c_str());
  }
  else
    CXRefIPrint("Block:\n");

  if (block->comment != CXRefEmptyStrId)
    CXRefIPrint("Comment: %s\n", CXRefIdToCStr(block->comment));

  cxref_indent++;

  for (uint i = 0; i < block->ext_vars.size(); i++) {
    if (block->ext_vars[i]->name == CXRefEmptyStrId)
      continue;

    if (block->ext_vars[i]->modified)
      CXRefIPrint("Output");
    else
      CXRefIPrint("Input");

    CXRefIPrint(" Var: %s\n", CXRefIdToCStr(block->ext_vars[i]->name));
  }

  process_vector(block->object_list, CXRefPrintObjectCFlow);

  cxref_indent--;
}

CXRefStringId
CXRefGetBlockType(CXRefBlock *block)
{
  if      (block->type == CXRefBlockType::CODE    ) return CXRefEmptyStrId;
  else if (block->type == CXRefBlockType::FUNCTION) return CXRefEmptyStrId;
  else if (block->type == CXRefBlockType::DECISION) return CXRefEmptyStrId;
  else if (block->type == CXRefBlockType::IF      ) return CXRefIfStrId;
  else if (block->type == CXRefBlockType::ELIF    ) return CXRefStringToId("elif");
  else if (block->type == CXRefBlockType::ELSE    ) return CXRefElseStrId;
  else if (block->type == CXRefBlockType::SWITCH  ) return CXRefSwitchStrId;
  else if (block->type == CXRefBlockType::WHILE   ) return CXRefWhileStrId;
  else if (block->type == CXRefBlockType::DO      ) return CXRefDoStrId;
  else if (block->type == CXRefBlockType::FOR     ) return CXRefForStrId;
  else                                              return CXRefUndefStrId;
}

CXRefObject *
CXRefCreateObject(CXRefObjectType type, void *data)
{
  CXRefObject *object = new CXRefObject;

  object->type = type;
  object->data = data;

  return object;
}

CXRefBlock *
CXRefStartNewSubBlock(CXRefBlock *parent, CXRefBlockType type, CXRefTokenP token)
{
  CXRefStringId comment;

  if (current_block_comment == CXRefEmptyStrId)
    comment = CXRefGetCurrentComment(token);
  else {
    comment = current_block_comment;

    current_block_comment = CXRefEmptyStrId;
  }

  if (comment == CXRefEmptyStrId)
    return nullptr;

  std::string comment_str = CXRefIdToString(comment);

  uint i;

  for (i = 0; i < comment_str.size(); i++)
    if (isalpha(comment_str[i]))
      break;

  if (i >= comment_str.size())
    return nullptr;

  CXRefBlock *sub_block = CXRefStartNewBlock(parent, type, CXRefTokenP());

  sub_block->comment = comment;

  return sub_block;
}

CXRefBlock *
CXRefStartNewBlock(CXRefBlock *parent, CXRefBlockType type, CXRefTokenP token)
{
  CXRefStringId comment = CXRefEmptyStrId;

  if (token.isValid()) {
    if (current_block_comment == CXRefEmptyStrId)
      comment = CXRefGetCurrentComment(token);
    else {
      comment = current_block_comment;

      current_block_comment = CXRefEmptyStrId;
    }

    if (comment != CXRefEmptyStrId) {
      uint i;

      std::string comment_str = CXRefIdToString(comment);

      for (i = 0; i < comment_str.size(); i++)
        if (isalpha(comment_str[i]))
          break;

      if (i >= comment_str.size())
        comment = CXRefEmptyStrId;
    }
  }

  CXRefBlock *block = CXRefCreateBlock(parent, type);

  block->comment = comment;

  CXRefObject *object = CXRefCreateObject(CXRefObjectType::BLOCK, reinterpret_cast<void *>(block));

  if (parent)
    parent->object_list.push_back(object);

  return block;
}

CXRefBlock *
CXRefCreateBlock(CXRefBlock *parent, CXRefBlockType type)
{
  CXRefBlock *block = new CXRefBlock;

  block->parent      = parent;
  block->type        = type;
  block->comment     = CXRefEmptyStrId;
  block->gen_comment = false;

  return block;
}

CXRefCall *
CXRefCreateCall(CXRefTokenP identifier, std::vector<CXRefTokenP> &assignment_expressions)
{
  CXRefStringId comment = CXRefGetCurrentComment(identifier);

  CXRefCall *call = new CXRefCall;

  std::string name = CXRefTokenToString(identifier);

  call->name    = CXRefStringToId(name);
  call->comment = comment;
  call->returns = last_var_modified;

  for (uint i = 0; i < call->args.size(); i++) {
    std::vector<CXRefTokenP> children;

    CXRefGetAllTokenChildrenForFunction(assignment_expressions[i], CXRefRemoveCasts, children);

    std::string name1 = CXRefTokensToString(children);

    call->args.push_back(CXRefStringToId(name1));
  }

  return call;
}

bool
CXRefRemoveCasts(CXRefTokenP token)
{
  static int num_remove_tokens = 0;

  if (num_remove_tokens > 0) {
    num_remove_tokens--;

    return false;
  }

  if (! CXRefIsTokenType(token, CAST_EXPRESSION))
    return true;

  int num_children = CXRefGetTokenNumChildren(token);

  if (num_children == 4)
    num_remove_tokens = 3;

  return true;
}

CXRefVar *
CXRefCreateVar(CXRefTokenP identifier, CXRefStringId type)
{
  CXRefVar *var = new CXRefVar;

  if (identifier.isValid()) {
    std::string name = CXRefTokenToString(identifier);

    var->name = CXRefStringToId(name);
  }

  if (type != CXRefEmptyStrId)
    var->type = CXRefCreateType(type);
  else
    var->type = CXRefCreateType(CXRefIntStrId);

  var->internal = false;

  var->used_before_set = false;
  var->modified        = 0;
  var->referenced      = 0;

  return var;
}

CXRefVar *
CXRefDupVar(CXRefVar *var)
{
  CXRefVar *var1 = new CXRefVar;

  var1->name            = var->name;
  var1->type            = var->type;
  var1->used_before_set = var->used_before_set;
  var1->modified        = var->modified;
  var1->referenced      = var->referenced;

  return var1;
}

CXRefType *
CXRefCreateType(CXRefStringId str)
{
  CXRefType *type = new CXRefType;

  type->str = str;

  return type;
}

const std::string &
CXRefFuncGetName(CXRefFunc *function)
{
  return CXRefIdToString(function->name);
}

void
CXRefFuncGetPosition(CXRefFunc *function, int *start_line_num, int *start_char_num,
                     int *end_line_num, int *end_char_num)
{
  *start_line_num = function->start_line_num;
  *start_char_num = function->start_char_num;
  *end_line_num   = function->end_line_num;
  *end_char_num   = function->end_char_num;
}
