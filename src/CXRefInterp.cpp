#include <CXRefI.h>

struct CXRefTypedef {
  CXRefStringId              name;
  CXRefStringId              type;
  bool                       referenced;
  std::vector<CXRefStringId> file_list;
};

typedef CRefPtr<CXRefTypedef> CXRefTypedefP;

/*--------------------------------------------------*/

typedef std::map<CXRefStringId,CXRefTypedefP> StringTypedefMap;

static StringTypedefMap         cxref_string_typedef_map;
static CXRefTokenP              cxref_typedef_token;
static CXRefStringId            cxref_typedef_name    = CXRefEmptyStrId;
static CXRefStringId            cxref_typedef_file    = CXRefEmptyStrId;
static uint                     cxref_typedef_line_no = 0;
static uint                     cxref_typedef_char_no = 0;
static std::vector<CXRefTokenP> cxref_type_tokens;

/*--------------------------------------------------*/

static CXRefTokenP    CXRefReadExternalDeclaration
                       (bool required);
static CXRefTokenP    CXRefReadFunctionDefinition
                       (bool required);
static bool           CXRefCheckFunctionDefinition
                       (CXRefTokenP token);
static CXRefTokenP    CXRefReadDeclaration
                       (bool required);
static CXRefTokenP    CXRefReadDeclarationList
                       (bool required);
static CXRefTokenP    CXRefReadDeclarationSpecifiers
                       (bool required);
static CXRefTokenP    CXRefReadDeclarationSpecifiers1
                       (bool required);
static CXRefTokenP    CXRefReadStorageClassSpecifier
                       (bool required);
static CXRefTokenP    CXRefReadTypeSpecifier
                       (bool required);
static CXRefTokenP    CXRefReadTypeQualifier
                       (bool required);
static CXRefTokenP    CXRefReadStructOrUnionSpecifier
                       (bool required);
static CXRefTokenP    CXRefReadStructOrUnion
                       (bool required);
static CXRefTokenP    CXRefReadStructDeclarationList
                       (bool required);
static CXRefTokenP    CXRefReadInitDeclaratorList
                       (bool required);
static CXRefTokenP    CXRefReadInitDeclarator
                       (bool required);
static CXRefTokenP    CXRefReadStructDeclaration
                       (bool required);
static CXRefTokenP    CXRefReadSpecifierQualifierList
                       (bool required);
static CXRefTokenP    CXRefReadSpecifierQualifierList1
                       (bool required);
static CXRefTokenP    CXRefReadStructDeclaratorList
                       (bool required);
static CXRefTokenP    CXRefReadStructDeclarator
                       (bool required);
static CXRefTokenP    CXRefReadEnumSpecifier
                       (bool required);
static CXRefTokenP    CXRefReadEnumeratorList
                       (bool required);
static CXRefTokenP    CXRefReadEnumerator
                       (bool required);
static CXRefTokenP    CXRefReadDeclarator
                       (bool required);
static CXRefTokenP    CXRefReadDirectDeclarator
                       (bool required);
static CXRefTokenP    CXRefReadPointer
                       (bool required);
static CXRefTokenP    CXRefReadTypeQualifierList
                       (bool required);
static CXRefTokenP    CXRefReadParameterTypeList
                       (bool required);
static CXRefTokenP    CXRefReadParameterList
                       (bool required);
static CXRefTokenP    CXRefReadParameterDeclaration
                       (bool required);
static CXRefTokenP    CXRefReadIdentifierList
                       (bool required);
static CXRefTokenP    CXRefReadInitializer
                       (bool required);
static CXRefTokenP    CXRefReadInitializerList
                       (bool required);
static CXRefTokenP    CXRefReadTypeName
                       (bool required);
static CXRefTokenP    CXRefReadAbstractDeclarator
                       (bool required);
static CXRefTokenP    CXRefReadDirectAbstractDeclarator
                       (bool required);
static CXRefTokenP    CXRefReadTypedefName
                       (bool required);
static CXRefTokenP    CXRefReadStatement
                       (bool required);
static CXRefTokenP    CXRefReadLabeledStatement
                       (bool required);
static CXRefTokenP    CXRefReadExpressionStatement
                       (bool required);
static CXRefTokenP    CXRefReadCompoundStatement
                       (bool required);
static CXRefTokenP    CXRefReadStatementList
                       (bool required);
static CXRefTokenP    CXRefReadSelectionStatement
                       (bool required);
static CXRefTokenP    CXRefReadIterationStatement
                       (bool required);
static CXRefTokenP    CXRefReadJumpStatement
                       (bool required);
static CXRefTokenP    CXRefReadExpression
                       (bool required);
static CXRefTokenP    CXRefReadAssignmentExpression
                       (bool required);
static CXRefTokenP    CXRefReadAssignmentOperator
                       (bool required);
static CXRefTokenP    CXRefReadConditionalExpression
                       (bool required);
static CXRefTokenP    CXRefReadConstantExpression
                       (bool required);
static CXRefTokenP    CXRefReadLogicalOrExpression
                       (bool required);
static CXRefTokenP    CXRefReadLogicalAndExpression
                       (bool required);
static CXRefTokenP    CXRefReadInclusiveOrExpression
                       (bool required);
static CXRefTokenP    CXRefReadExclusiveOrExpression
                       (bool required);
static CXRefTokenP    CXRefReadAndExpression
                       (bool required);
static CXRefTokenP    CXRefReadEqualityExpression
                       (bool required);
static CXRefTokenP    CXRefReadRelationalExpression
                       (bool required);
static CXRefTokenP    CXRefReadShiftExpression
                       (bool required);
static CXRefTokenP    CXRefReadAdditiveExpression
                       (bool required);
static CXRefTokenP    CXRefReadMultiplicativeExpression
                       (bool required);
static CXRefTokenP    CXRefReadCastExpression
                       (bool required);
static CXRefTokenP    CXRefReadUnaryExpression
                       (bool required);
static CXRefTokenP    CXRefReadUnaryOperator
                       (bool required);
static CXRefTokenP    CXRefReadPostfixExpression
                       (bool required);
static CXRefTokenP    CXRefReadPrimaryExpression
                       (bool required);
static CXRefTokenP    CXRefReadArgumentExpressionList
                       (bool required);
static CXRefTokenP    CXRefReadComment
                       (bool required);
static CXRefTokenP    CXRefReadIdentifier
                       (bool required);
static CXRefTokenP    CXRefReadKeyword
                       (bool required);
static CXRefTokenP    CXRefReadConstant
                       (bool required);
static CXRefTokenP    CXRefReadString
                       (bool required);
static CXRefTokenP    CXRefReadOperator
                       (bool required);
static CXRefTokenP    CXRefReadSeparator
                       (bool required);
static CXRefTokenP    CXRefReadTokenOfType
                       (CXRefTokenType type);
static bool           CXRefIsLastToken
                       ();
static bool           CXRefIsStorageClassSpecifier
                       (CXRefTokenP token);
static bool           CXRefIsTypeSpecifier
                       (CXRefTokenP token);
static bool           CXRefIsTypeQualifier
                       (CXRefTokenP token);
static bool           CXRefIsTypedefToken
                       (CXRefTokenP token);
static void           CXRefUpdateTypedefList
                       (CXRefTokenP token);
static bool           CXRefCheckTypedefDeclarationSpecifiers
                       (CXRefTokenP token);
static void           CXRefAddTypedefInitDeclarator
                       (CXRefTokenP declaration, CXRefTokenP init_declarator_list);
static void           CXRefAddDeclarationTypedef
                       (CXRefTokenP declaration, CXRefStringId name);
static CXRefTypedefP  CXRefGetTypedefForString
                       (CXRefStringId name);
static CXRefTokenP    CXRefRemoveTypedefFromDeclarationSpecifiers
                       (CXRefTokenP declaration_specifiers);
static void           CXRefRestackToken
                       (CXRefTokenP token);
static bool           CXRefIsValidTypeTokens
                       (CXRefTokenP token);
static void           CXRefSkipBracedTokens
                       (CXRefTokenP token);

namespace CXRefReadTokenUtil {
  CXRefTokenP NoToken  (bool) { return CXRefTokenP(); }
  CXRefTokenP DummyLine(bool) { return CXRefTokenP(); }

  CXRefTokenP PreprocessorStatement(bool) { return CXRefTokenP(); }
  CXRefTokenP PreprocessorDefine   (bool) { return CXRefTokenP(); }

  CXRefTokenP Comment(bool req) { return CXRefReadComment(req); }

  CXRefTokenP Identifier(bool req) { return CXRefReadIdentifier(req); }
  CXRefTokenP Keyword   (bool req) { return CXRefReadKeyword   (req); }
  CXRefTokenP Constant  (bool req) { return CXRefReadConstant  (req); }
  CXRefTokenP String    (bool req) { return CXRefReadString    (req); }
  CXRefTokenP Operator  (bool req) { return CXRefReadOperator  (req); }
  CXRefTokenP Separator (bool req) { return CXRefReadSeparator (req); }

  CXRefTokenP TranslationUnit(bool req) { return CXRefReadTranslationUnit(req); }
  CXRefTokenP ExternalDeclaration(bool req) { return CXRefReadExternalDeclaration(req); }
  CXRefTokenP FunctionDefinition(bool req) { return CXRefReadFunctionDefinition(req); }
  CXRefTokenP Declaration(bool req) { return CXRefReadDeclaration(req); }
  CXRefTokenP DeclarationList(bool req) { return CXRefReadDeclarationList(req); }
  CXRefTokenP DeclarationSpecifiers(bool req) { return CXRefReadDeclarationSpecifiers(req); }
  CXRefTokenP StorageClassSpecifier(bool req) { return CXRefReadStorageClassSpecifier(req); }
  CXRefTokenP TypeSpecifier(bool req) { return CXRefReadTypeSpecifier(req); }
  CXRefTokenP TypeQualifier(bool req) { return CXRefReadTypeQualifier(req); }
  CXRefTokenP StructOrUnionSpecifier(bool req) { return CXRefReadStructOrUnionSpecifier(req); }
  CXRefTokenP StructOrUnion(bool req) { return CXRefReadStructOrUnion(req); }
  CXRefTokenP StructDeclarationList(bool req) { return CXRefReadStructDeclarationList(req); }
  CXRefTokenP InitDeclaratorList(bool req) { return CXRefReadInitDeclaratorList(req); }
  CXRefTokenP InitDeclarator(bool req) { return CXRefReadInitDeclarator(req); }
  CXRefTokenP StructDeclaration(bool req) { return CXRefReadStructDeclaration(req); }
  CXRefTokenP SpecifierQualifierList(bool req) { return CXRefReadSpecifierQualifierList(req); }
  CXRefTokenP StructDeclaratorList(bool req) { return CXRefReadStructDeclaratorList(req); }
  CXRefTokenP StructDeclarator(bool req) { return CXRefReadStructDeclarator(req); }
  CXRefTokenP EnumSpecifier(bool req) { return CXRefReadEnumSpecifier(req); }
  CXRefTokenP EnumeratorList(bool req) { return CXRefReadEnumeratorList(req); }
  CXRefTokenP Enumerator(bool req) { return CXRefReadEnumerator(req); }
  CXRefTokenP Declarator(bool req) { return CXRefReadDeclarator(req); }
  CXRefTokenP DirectDeclarator(bool req) { return CXRefReadDirectDeclarator(req); }
  CXRefTokenP Pointer(bool req) { return CXRefReadPointer(req); }
  CXRefTokenP TypeQualifierList(bool req) { return CXRefReadTypeQualifierList(req); }
  CXRefTokenP ParameterTypeList(bool req) { return CXRefReadParameterTypeList(req); }
  CXRefTokenP ParameterList(bool req) { return CXRefReadParameterList(req); }
  CXRefTokenP ParameterDeclaration(bool req) { return CXRefReadParameterDeclaration(req); }
  CXRefTokenP IdentifierList(bool req) { return CXRefReadIdentifierList(req); }
  CXRefTokenP Initializer(bool req) { return CXRefReadInitializer(req); }
  CXRefTokenP InitializerList(bool req) { return CXRefReadInitializerList(req); }
  CXRefTokenP TypeName(bool req) { return CXRefReadTypeName(req); }
  CXRefTokenP AbstractDeclarator(bool req) { return CXRefReadAbstractDeclarator(req); }
  CXRefTokenP DirectAbstractDeclarator(bool req) { return CXRefReadDirectAbstractDeclarator(req); }
  CXRefTokenP TypedefName(bool req) { return CXRefReadTypedefName(req); }
  CXRefTokenP Statement(bool req) { return CXRefReadStatement(req); }
  CXRefTokenP LabeledStatement(bool req) { return CXRefReadLabeledStatement(req); }
  CXRefTokenP ExpressionStatement(bool req) { return CXRefReadExpressionStatement(req); }
  CXRefTokenP CompoundStatement(bool req) { return CXRefReadCompoundStatement(req); }
  CXRefTokenP StatementList(bool req) { return CXRefReadStatementList(req); }
  CXRefTokenP SelectionStatement(bool req) { return CXRefReadSelectionStatement(req); }
  CXRefTokenP IterationStatement(bool req) { return CXRefReadIterationStatement(req); }
  CXRefTokenP JumpStatement(bool req) { return CXRefReadJumpStatement(req); }
  CXRefTokenP Expression(bool req) { return CXRefReadExpression(req); }
  CXRefTokenP AssignmentExpression(bool req) { return CXRefReadAssignmentExpression(req); }
  CXRefTokenP AssignmentOperator(bool req) { return CXRefReadAssignmentOperator(req); }
  CXRefTokenP ConditionalExpression(bool req) { return CXRefReadConditionalExpression(req); }
  CXRefTokenP ConstantExpression(bool req) { return CXRefReadConstantExpression(req); }
  CXRefTokenP LogicalOrExpression(bool req) { return CXRefReadLogicalOrExpression(req); }
  CXRefTokenP LogicalAndExpression(bool req) { return CXRefReadLogicalAndExpression(req); }
  CXRefTokenP InclusiveOrExpression(bool req) { return CXRefReadInclusiveOrExpression(req); }
  CXRefTokenP ExclusiveOrExpression(bool req) { return CXRefReadExclusiveOrExpression(req); }
  CXRefTokenP AndExpression(bool req) { return CXRefReadAndExpression(req); }
  CXRefTokenP EqualityExpression(bool req) { return CXRefReadEqualityExpression(req); }
  CXRefTokenP RelationalExpression(bool req) { return CXRefReadRelationalExpression(req); }
  CXRefTokenP ShiftExpression(bool req) { return CXRefReadShiftExpression(req); }
  CXRefTokenP AdditiveExpression(bool req) { return CXRefReadAdditiveExpression(req); }
  CXRefTokenP MultiplicativeExpression(bool req) { return CXRefReadMultiplicativeExpression(req); }
  CXRefTokenP CastExpression(bool req) { return CXRefReadCastExpression(req); }
  CXRefTokenP UnaryExpression(bool req) { return CXRefReadUnaryExpression(req); }
  CXRefTokenP UnaryOperator(bool req) { return CXRefReadUnaryOperator(req); }
  CXRefTokenP PostfixExpression(bool req) { return CXRefReadPostfixExpression(req); }
  CXRefTokenP PrimaryExpression(bool req) { return CXRefReadPrimaryExpression(req); }
  CXRefTokenP ArgumentExpressionList(bool req) { return CXRefReadArgumentExpressionList(req); }
}

CXRefTokenP
CXRefReadGenToken(CXRefTokenType type, bool required)
{
  CXRefTokenP token;

#define CENUM_PROCESS
#define CENUM_PROCESS_VAR    type
#define CENUM_PROCESS_PREFIX token = CXRefReadTokenUtil
#define CENUM_PROCESS_ARG    required
#include <CXRefTokenType.h>
#undef CENUM_PROCESS
#undef CENUM_PROCESS_VAR
#undef CENUM_PROCESS_PREFIX
#undef CENUM_PROCESS_ARG

  return token;
}

/*
 * translation-unit:
 *  external-declaration
 *  translation-unit external-declaration
 */

CXRefTokenP
CXRefReadTranslationUnit(bool /*required*/)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadTranslationUnit", token);

  if (cxref_control.trace)
    std::cerr << "Tokenising     ..." << std::endl;

  CXRefTokenP token1 = CXRefReadExternalDeclaration(false);

  while (token1.isValid()) {
    CXRefTokenP token2 = CXRefCreateToken(TRANSLATION_UNIT);

    if (token.isValid())
      CXRefAddTokenChild(token, token2);

    CXRefAddTokenChild(token1, token2);

    token = token2;

    if (CXRefIsLastToken())
      break;

    token1 = CXRefReadExternalDeclaration(false);
  }

  if (! CXRefIsLastToken())
    CXRefError("Invalid Translation Unit\n");

  return token;
}

/*
 * external-declaration:
 *  function-declaration
 *  declaration
 */

CXRefTokenP
CXRefReadExternalDeclaration(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadExternalDeclaration", token);

#ifdef GUESS_TYPEDEFS
  bool typedef_tried = false;
#endif

#ifdef GUESS_TYPEDEFS
 again:
#endif
  token = CXRefReadTokenOfType(EXTERNAL_DECLARATION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclarationSpecifiers(false);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadDeclarator(false);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, SEPARATOR, CXRefSemiColonStrId) ||
            CXRefIsToken(token3, SEPARATOR, CXRefCommaStrId) ||
            CXRefIsToken(token3, OPERATOR , CXRefEqualStrId)) {
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);

          token1 = CXRefReadDeclaration(required);

          if (token1.isValid()) {
            token = CXRefCreateToken(EXTERNAL_DECLARATION);

            CXRefAddTokenChild(token1, token);
          }
        }
        else {
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);

          token1 = CXRefReadFunctionDefinition(required);

          if (token1.isValid()) {
            token = CXRefCreateToken(EXTERNAL_DECLARATION);

            CXRefAddTokenChild(token1, token);
          }
        }
      }
      else {
        CXRefRestackToken(token1);

        token1 = CXRefReadDeclaration(required);

        if (token1.isValid()) {
          token = CXRefCreateToken(EXTERNAL_DECLARATION);

          CXRefAddTokenChild(token1, token);
        }
      }
    }
    else {
      token1 = CXRefReadFunctionDefinition(required);

      if (token1.isValid()) {
        token = CXRefCreateToken(EXTERNAL_DECLARATION);

        CXRefAddTokenChild(token1, token);
      }
    }

#ifdef GUESS_TYPEDEFS
    if (! token.isValid() && ! typedef_tried) {
      std::vector<CXRefTokenP> children;

      CXRefTokenP token1 = CXRefReadDeclarationSpecifiers(false);

      CXRefTokenP token2 = CXRefReadToken();

      if (! CXRefIsTokenType(token2, DECLARATOR))
        goto fail;

      CXRefGetTokenChildren(token2, &children);

      if (children.size() != 1)
        goto fail;

      token2 = children[0];

      CXRefGetTokenChildren(token2, &children);

      if (children.size() != 1)
        goto fail;

      token2 = children[0];

      if (! CXRefIsTokenType(token2, IDENTIFIER))
        goto fail;

      std::cerr << "Assuming " << token2->str << " is a typedef\n";

      CXRefAddTypedef(token2, token2->str, CXRefIntStrId);

      CXRefRestackToken(token2);

      if (token1.isValid()) {
        std::vector<CXRefTokenP> token_list1;

        CXRefGetTokenChildren(token1, &children);

        token_list1.push_back(children[0]);

        while (children.size() == 2) {
          CXRefGetTokenChildren(children[0], &children);

          token_list1.push_back(children[0]);
        }

        while (! token_list1.empty()) {
          token1 = token_list1.back();

          token_list1.pop_back();

          CXRefRestackToken(token1);
        }
      }

      typedef_tried = true;

      goto again;

 fail:
      typedef_tried = true;

      CXRefRestackToken(token2);
      CXRefRestackToken(token1);
    }
#endif
  }

  if (! token.isValid() && cxref_typedef_token.isValid()) {
    CXRefTokenP last_token = CXRefGetLastToken();

    std::cerr << "Last Token - " << CXRefIdToString(last_token->file) << "(" <<
                 last_token->line_no << "," << last_token->char_no << ")" << std::endl;

/*
    if (cxref_typedef_name != CXRefUndefStrId && last_token->file == cxref_typedef_file &&
        (last_token->line_no < cxref_typedef_line_no ||
         (last_token->line_no == cxref_typedef_line_no &&
          last_token->char_no <= cxref_typedef_char_no))) {
      std::cerr << "Failed" << std::endl;

      CXRefRemoveTypedef(cxref_typedef_name);
    }
*/

    cxref_typedef_name    = cxref_typedef_token->str;
    cxref_typedef_file    = last_token->file;
    cxref_typedef_line_no = last_token->line_no;
    cxref_typedef_char_no = last_token->char_no;

    std::cerr << "Trying typedef " << CXRefIdToString(cxref_typedef_name) << " - " <<
                 CXRefIdToString(cxref_typedef_token->file) << "(" <<
                 cxref_typedef_token->line_no << "," <<
                 cxref_typedef_token->char_no << ")" << std::endl;

    CXRefAddTypedef(cxref_typedef_token, cxref_typedef_name, CXRefIntStrId);

    cxref_typedef_token = CXRefTokenP();

    token = CXRefReadExternalDeclaration(required);

    if (token.isValid())
      std::cerr << "Success" << std::endl;
    else
      std::cerr << "Failed" << std::endl;
  }

  return token;
}

/*
 * function-definition:
 *  declaration-specifiers(opt) declarator
 *    init-declaration-list(opt) compound-statement
 */

CXRefTokenP
CXRefReadFunctionDefinition(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadFunctionDefinition", token);

  token = CXRefReadTokenOfType(FUNCTION_DEFINITION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclarationSpecifiers(false);

    CXRefTokenP token2 = CXRefReadDeclarator(required);

    if (token2.isValid()) {
      CXRefTokenP token3 = CXRefReadDeclarationList(false);

      CXRefTokenP token4 = CXRefReadCompoundStatement(required);

      if (token4.isValid()) {
        token = CXRefCreateToken(FUNCTION_DEFINITION);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
        CXRefAddTokenChild(token3, token);
        CXRefAddTokenChild(token4, token);
      }
      else {
        CXRefRestackToken(token3);
        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else
      CXRefRestackToken(token1);
  }

  if (token.isValid()) {
    if (! CXRefCheckFunctionDefinition(token)) {
      CXRefError("Invalid Function Definition\n");
      CXRefPrintToken(token);

      token = CXRefTokenP();
    }
  }

  return token;
}

/*
 * 1) storage class specifiers are only 'extern' or 'static'
 * 2) can't return function or array (???)
 * 3) direct-declarator is one of :-
 *     direct-declarator '(' parameter-type-list ')'
 *     direct-declarator '(' identifier-list(opt) ')'
 *    where direct-declarator is an optionally parenthesized
 *    identifier.
 * 4) no declaration-list if direct-declarator is of the first form
 * 5) each declarator in the parameter-type-list must have an
 *    identifier unless it contains just void
 * 6) the declaration-list for the second form may contain only
 *    declarations for the parameter names, no initialisation and
 *    only register storage class.
 */

bool
CXRefCheckFunctionDefinition(CXRefTokenP token)
{
  CXRefGetTokenChildOfType(token, DECLARATION_SPECIFIERS);

  CXRefTokenP token2 = CXRefGetTokenChildOfType(token, DECLARATOR);
  CXRefTokenP token3 = CXRefGetTokenChildOfType(token, DECLARATION_LIST);

  CXRefGetTokenChildOfType(token, COMPOUND_STATEMENT);

  CXRefGetTokenChildOfType(token2, POINTER);

  CXRefTokenP token22 = CXRefGetTokenChildOfType(token2, DIRECT_DECLARATOR);

  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(token22, children);

  /* Test 3) */

  if ((children.size() != 3 && children.size() != 4) ||
      CXRefIsToken(children[1], OPERATOR, CXRefLBracketStrId))
    return false;

  /* Test 4) */

  if (CXRefIsTokenType(children[2], PARAMETER_TYPE_LIST) && token3.isValid())
    return false;

  return true;
}

/*
 * declaration:
 *  declaration-specifiers init-declarator-list(opt)
 */

CXRefTokenP
CXRefReadDeclaration(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadDeclaration", token);

  token = CXRefReadTokenOfType(DECLARATION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclarationSpecifiers(required);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadInitDeclaratorList(false);

      CXRefTokenP token3 = CXRefReadToken();

      if (! token2.isValid() && ! CXRefIsToken(token3, SEPARATOR, CXRefSemiColonStrId)) {
        token2 = CXRefRemoveTypedefFromDeclarationSpecifiers(token1);

        if (token2.isValid()) {
          std::cerr << "Typedef " << CXRefIdToString(token2->str) << " redeclared" << std::endl;

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);

          token2 = CXRefReadInitDeclaratorList(false);

          token3 = CXRefReadToken();
        }
      }

      if (CXRefIsToken(token3, SEPARATOR, CXRefSemiColonStrId)) {
        token = CXRefCreateToken(DECLARATION);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
        CXRefAddTokenChild(token3, token);
      }
      else {
        CXRefAddError("Missing Semi-Colon in Declaration");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token3);
        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
  }

  if (token.isValid())
    CXRefUpdateTypedefList(token);

  return token;
}

/*
 * declaration-specifiers:
 *  storage-class-specifier declaration-specifiers(opt)
 *  type-specifier declaration-specifiers(opt)
 *  type-qualifier declaration-specifiers(opt)
 */

CXRefTokenP
CXRefReadDeclarationSpecifiers(bool required)
{
  std::vector<CXRefTokenP> type_tokens = cxref_type_tokens;

  cxref_type_tokens.clear();

  CXRefTokenP token = CXRefReadDeclarationSpecifiers1(required);

  cxref_type_tokens = type_tokens;

  return token;
}

CXRefTokenP
CXRefReadDeclarationSpecifiers1(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadDeclarationSpecifiers", token);

  token = CXRefReadTokenOfType(DECLARATION_SPECIFIERS);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if      (CXRefIsStorageClassSpecifier(token1)) {
      CXRefStackToken(token1);

      token1 = CXRefReadStorageClassSpecifier(required);

      if (token1.isValid()) {
        token = CXRefCreateToken(DECLARATION_SPECIFIERS);

        CXRefAddTokenChild(token1, token);
      }
    }
    else if (CXRefIsTypeSpecifier(token1)) {
      CXRefStackToken(token1);

      token1 = CXRefReadTypeSpecifier(required);

      if (token1.isValid()) {
        if (! CXRefIsValidTypeTokens(token1)) {
          CXRefAddError("Invalid Token in Declaration Specifiers");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token1);

          return token;
        }

        cxref_type_tokens.push_back(token1);

        token = CXRefCreateToken(DECLARATION_SPECIFIERS);

        CXRefAddTokenChild(token1, token);
      }
    }
    else if (CXRefIsTypeQualifier(token1)) {
      CXRefStackToken(token1);

      token1 = CXRefReadTypeQualifier(required);

      if (token1.isValid()) {
        token = CXRefCreateToken(DECLARATION_SPECIFIERS);

        CXRefAddTokenChild(token1, token);
      }
    }
    else {
      CXRefAddError("Invalid Token in Declaration Specifiers");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);

      return token;
    }

    if (! token.isValid()) {
      CXRefAddError("Invalid Token in Declaration Specifiers");

      if (required)
        CXRefError(CXRefGetLastError());

      return token;
    }

    token1 = CXRefReadDeclarationSpecifiers1(false);

    if (token1.isValid())
      CXRefAddTokenChild(token1, token);
  }

  return token;
}

/*
 * declaration-list:
 *  declaration
 *  declaration-list declaration
 */

CXRefTokenP
CXRefReadDeclarationList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadDeclarationList", token);

  token = CXRefReadTokenOfType(DECLARATION_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclaration(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(DECLARATION_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclaration(false);

    while (token1.isValid()) {
      CXRefTokenP token2 = CXRefCreateToken(DECLARATION_LIST);

      CXRefAddTokenChild(token , token2);
      CXRefAddTokenChild(token1, token2);

      token = token2;

      token1 = CXRefReadDeclaration(false);
    }
  }

  return token;
}

/*
 * storage-class-specifier:
 *  'auto'
 *  'register'
 *  'static'
 *  'extern'
 *  'typedef'
 */

CXRefTokenP
CXRefReadStorageClassSpecifier(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStorageClassSpecifier", token);

  token = CXRefReadTokenOfType(STORAGE_CLASS_SPECIFIER);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsStorageClassSpecifier(token1)) {
      token = CXRefCreateToken(STORAGE_CLASS_SPECIFIER);

      CXRefAddTokenChild(token1, token);
    }
    else {
      CXRefAddError("Storage Class Specifier not Found");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * typedef-name:
 *  identifier
 */

CXRefTokenP
CXRefReadTypedefName(bool /*required*/)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadTypedefName", token);

  token = CXRefReadTokenOfType(TYPEDEF_NAME);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (token1.isValid() && token1->type == IDENTIFIER) {
      if (CXRefIsTypedefToken(token1)) {
        token = CXRefCreateToken(TYPEDEF_NAME);

        CXRefAddTokenChild(token1, token);
      }
      else
        CXRefStackToken(token1);
    }
    else
      CXRefStackToken(token1);
  }

  return token;
}

/*
 * type-specifier:
 *  'void'
 *  'char'
 *  'short'
 *  'int'
 *  'long'
 *  'float'
 *  'double'
 *  'signed'
 *  'unsigned'
 *  struct-or-union-specifier
 *  enum-specifier
 *  typedef-name
 */

CXRefTokenP
CXRefReadTypeSpecifier(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadTypeSpecifier", token);

  token = CXRefReadTokenOfType(TYPE_SPECIFIER);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsTypeSpecifier(token1)) {
      if (CXRefIsToken(token1, KEYWORD, CXRefStructStrId) ||
          CXRefIsToken(token1, KEYWORD, CXRefUnionStrId )) {
        CXRefRestackToken(token1);

        token1 = CXRefReadStructOrUnionSpecifier(required);

        if (token1.isValid()) {
          token = CXRefCreateToken(TYPE_SPECIFIER);

          CXRefAddTokenChild(token1, token);
        }
      }
      else if (CXRefIsToken(token1, KEYWORD, CXRefEnumStrId)) {
        CXRefRestackToken(token1);

        token1 = CXRefReadEnumSpecifier(required);

        if (token1.isValid()) {
          token = CXRefCreateToken(TYPE_SPECIFIER);

          CXRefAddTokenChild(token1, token);
        }
      }
      else if (CXRefIsTypedefToken(token1)) {
        CXRefRestackToken(token1);

        token1 = CXRefReadTypedefName(required);

        if (token1.isValid()) {
          token = CXRefCreateToken(TYPE_SPECIFIER);

          CXRefAddTokenChild(token1, token);
        }
      }
      else {
        token = CXRefCreateToken(TYPE_SPECIFIER);

        CXRefAddTokenChild(token1, token);
      }
    }
    else {
      CXRefAddError("Type Specifier not Found");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * enum-specifier:
 *  'enum' identifier(opt) '{' enumerator-list '}'
 *  'enum' identifier
 */

CXRefTokenP
CXRefReadEnumSpecifier(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadEnumSpecifier", token);

  token = CXRefReadTokenOfType(ENUM_SPECIFIER);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, KEYWORD, CXRefEnumStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (! CXRefIsTokenType(token2, IDENTIFIER)) {
        CXRefRestackToken(token2);

        token2 = CXRefTokenP();
      }

      CXRefTokenP token3 = CXRefReadToken();

      if (CXRefIsToken(token3, SEPARATOR, CXRefLBraceStrId)) {
        if (CXRefGetSkipCode()) {
          token = CXRefCreateToken(ENUM_SPECIFIER);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);

          CXRefSkipBracedTokens(token);
        }
        else {
          CXRefTokenP token4 = CXRefReadEnumeratorList(required);

          if (token4.isValid()) {
            CXRefTokenP token5 = CXRefReadToken();

            if (CXRefIsToken(token5, SEPARATOR, CXRefRBraceStrId)) {
              token = CXRefCreateToken(ENUM_SPECIFIER);

              CXRefAddTokenChild(token1, token);
              CXRefAddTokenChild(token2, token);
              CXRefAddTokenChild(token3, token);
              CXRefAddTokenChild(token4, token);
              CXRefAddTokenChild(token5, token);
            }
            else {
              CXRefAddError("Missing Close Brace for Enum Specifier");

              if (required)
                CXRefError(CXRefGetLastError());

              CXRefRestackToken(token5);
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
      }
      else {
        CXRefRestackToken(token3);

        if (token2.isValid()) {
          token = CXRefCreateToken(ENUM_SPECIFIER);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
        }
        else {
          CXRefAddError("Missing Open Brace for Enum Specifier");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token1);
        }
      }
    }
    else {
      CXRefAddError("Missing Enum Keyword for Enum Specifier");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * enumerator-list:
 *  enumerator
 *  enumerator-list ',' enumerator
 */

CXRefTokenP
CXRefReadEnumeratorList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadEnumeratorList", token);

  token = CXRefReadTokenOfType(ENUMERATOR_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadEnumerator(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(ENUMERATOR_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadEnumerator(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(ENUMERATOR_LIST);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * enumerator:
 *  identifier
 *  identifier '=' constant-expression
 */

CXRefTokenP
CXRefReadEnumerator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadEnumerator", token);

  token = CXRefReadTokenOfType(ENUMERATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsTokenType(token1, IDENTIFIER)) {
      token = CXRefCreateToken(ENUMERATOR);

      CXRefAddTokenChild(token1, token);

      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefEqualStrId)) {
        CXRefAddTokenChild(token2, token);

        CXRefTokenP token3 = CXRefReadConstantExpression(required);

        if (token3.isValid())
          CXRefAddTokenChild(token3, token);
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token2);
    }
  }

  return token;
}

/*
 * init-declarator-list:
 *  init-declarator
 *  init-declarator-list ',' init-declarator
 */

CXRefTokenP
CXRefReadInitDeclaratorList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadInitDeclaratorList", token);

  token = CXRefReadTokenOfType(INIT_DECLARATOR_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadInitDeclarator(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(INIT_DECLARATOR_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadInitDeclarator(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(INIT_DECLARATOR_LIST);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * init-declarator:
 *  declarator
 *  declarator '=' initializer
 */

CXRefTokenP
CXRefReadInitDeclarator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadInitDeclarator", token);

  token = CXRefReadTokenOfType(INIT_DECLARATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclarator(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(INIT_DECLARATOR);

      CXRefAddTokenChild(token1, token);

      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefEqualStrId)) {
        CXRefAddTokenChild(token2, token);

        CXRefTokenP token3 = CXRefReadInitializer(required);

        if (token3.isValid())
          CXRefAddTokenChild(token3, token);
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token2);
    }
  }

  return token;
}

/*
 * direct-declarator:
 *  identifier
 *  '(' declarator ')'
 *  direct-declarator '[' constant-expression(opt) ']'
 *  direct-declarator '(' parameter-type-list ')'
 *  direct-declarator '(' identifier-list(opt) ')'
 */

CXRefTokenP
CXRefReadDirectDeclarator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadDirectDeclarator", token);

  token = CXRefReadTokenOfType(DIRECT_DECLARATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsTokenType(token1, IDENTIFIER)) {
      token = CXRefCreateToken(DIRECT_DECLARATOR);

      CXRefAddTokenChild(token1, token);
    }
    else if (CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
      CXRefTokenP token2 = CXRefReadDeclarator(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
          token = CXRefCreateToken(DIRECT_DECLARATOR);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token1);
    }
    else {
      CXRefAddError("Invalid Token for Direct Declarator");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (token1.isValid()) {
      if (CXRefIsToken(token1, OPERATOR, CXRefLBracketStrId)) {
        CXRefTokenP token2 = CXRefReadConstantExpression(false);

        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRBracketStrId)) {
          CXRefTokenP token4 = CXRefCreateToken(DIRECT_DECLARATOR);

          CXRefAddTokenChild(token , token4);
          CXRefAddTokenChild(token1, token4);
          CXRefAddTokenChild(token2, token4);
          CXRefAddTokenChild(token3, token4);

          token = token4;
        }
        else {
          CXRefAddError("Missing Close Square Bracket in Direct Declarator");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);

          break;
        }
      }
      else if (CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
        CXRefTokenP token2 = CXRefReadParameterTypeList(false);

        if (token2.isValid()) {
          CXRefTokenP token3 = CXRefReadToken();

          if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
            CXRefTokenP token4 = CXRefCreateToken(DIRECT_DECLARATOR);

            CXRefAddTokenChild(token , token4);
            CXRefAddTokenChild(token1, token4);
            CXRefAddTokenChild(token2, token4);
            CXRefAddTokenChild(token3, token4);

            token = token4;
          }
          else {
            CXRefAddError("Missing Close Round Bracket in Direct Declarator");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);

            break;
          }
        }
        else {
          CXRefTokenP token2 = CXRefReadIdentifierList(false);

          CXRefTokenP token3 = CXRefReadToken();

          if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
            CXRefTokenP token4 = CXRefCreateToken(DIRECT_DECLARATOR);

            CXRefAddTokenChild(token , token4);
            CXRefAddTokenChild(token1, token4);
            CXRefAddTokenChild(token2, token4);
            CXRefAddTokenChild(token3, token4);

            token = token4;
          }
          else {
            CXRefAddError("Missing Close Round Bracket in Direct Declarator");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);

            break;
          }
        }
      }
      else {
        CXRefRestackToken(token1);

        break;
      }

      token1 = CXRefReadToken();
    }
  }

  return token;
}

/*
 * type-qualifier:
 *  'const'
 *  'volatile'
 */

CXRefTokenP
CXRefReadTypeQualifier(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadTypeQualifier", token);

  token = CXRefReadTokenOfType(TYPE_QUALIFIER);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsTypeQualifier(token1)) {
      token = CXRefCreateToken(TYPE_QUALIFIER);

      CXRefAddTokenChild(token1, token);
    }
    else {
      CXRefAddError("Type Qualifier not Found");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * struct-or-union:
 *  'struct'
 *  'union'
 */

CXRefTokenP
CXRefReadStructOrUnion(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStructOrUnion", token);

  token = CXRefReadTokenOfType(STRUCT_OR_UNION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, KEYWORD, CXRefStructStrId) ||
        CXRefIsToken(token1, KEYWORD, CXRefUnionStrId )) {
      token = CXRefCreateToken(STRUCT_OR_UNION);

      CXRefAddTokenChild(token1, token);
    }
    else {
      CXRefAddError("Struct or Union not Found");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * struct-declaration-list:
 *  struct-declaration
 *  struct-declaration-list struct-declaration
 */

CXRefTokenP
CXRefReadStructDeclarationList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStructDeclarationList", token);

  token = CXRefReadTokenOfType(STRUCT_DECLARATION_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadStructDeclaration(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(STRUCT_DECLARATION_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadStructDeclaration(false);

    while (token1.isValid()) {
      CXRefTokenP token2 = CXRefCreateToken(STRUCT_DECLARATION_LIST);

      CXRefAddTokenChild(token , token2);
      CXRefAddTokenChild(token1, token2);

      token = token2;

      token1 = CXRefReadStructDeclaration(false);
    }
  }

  return token;
}

/*
 * struct-declarator-list:
 *  struct-declarator
 *  struct-declarator-list ',' struct-declarator
 */

CXRefTokenP
CXRefReadStructDeclaratorList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStructDeclaratorList", token);

  token = CXRefReadTokenOfType(STRUCT_DECLARATOR_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadStructDeclarator(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(STRUCT_DECLARATOR_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadStructDeclarator(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(STRUCT_DECLARATOR_LIST);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * struct-declarator:
 *  declarator
 *  declarator(opt) ':' constant-expression
 */

CXRefTokenP
CXRefReadStructDeclarator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStructDeclarator", token);

  token = CXRefReadTokenOfType(STRUCT_DECLARATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclarator(false);

    CXRefTokenP token2 = CXRefReadToken();

    if (CXRefIsToken(token2, OPERATOR, CXRefColonStrId)) {
      CXRefTokenP token3 = CXRefReadConstantExpression(required);

      if (token3.isValid()) {
        token = CXRefCreateToken(STRUCT_DECLARATOR);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
        CXRefAddTokenChild(token3, token);
      }
      else {
        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else {
      CXRefRestackToken(token2);

      if (token1.isValid()) {
        token = CXRefCreateToken(STRUCT_DECLARATOR);

        CXRefAddTokenChild(token1, token);
      }
      else {
        CXRefAddError("Missing Colon for Struct Declarator");

        if (required)
          CXRefError(CXRefGetLastError());
      }
    }
  }

  return token;
}

/*
 * struct-or-union-specifier:
 *  struct-or-union identifier(opt) '{' struct-declaration-list '}'
 *  struct-or-union identifier
 */

CXRefTokenP
CXRefReadStructOrUnionSpecifier(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStructOrUnionSpecifier", token);

  token = CXRefReadTokenOfType(STRUCT_OR_UNION_SPECIFIER);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadStructOrUnion(required);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadToken();

      if (! CXRefIsTokenType(token2, IDENTIFIER)) {
        CXRefRestackToken(token2);

        token2 = CXRefTokenP();
      }

      CXRefTokenP token3 = CXRefReadToken();

      if (CXRefIsToken(token3, SEPARATOR, CXRefLBraceStrId)) {
        if (CXRefGetSkipCode()) {
          token = CXRefCreateToken(STRUCT_OR_UNION_SPECIFIER);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);

          CXRefSkipBracedTokens(token);
        }
        else {
          CXRefTokenP token4 = CXRefReadStructDeclarationList(required);

          if (token4.isValid()) {
            CXRefTokenP token5 = CXRefReadToken();

            if (CXRefIsToken(token5, SEPARATOR, CXRefRBraceStrId)) {
              token = CXRefCreateToken(STRUCT_OR_UNION_SPECIFIER);

              CXRefAddTokenChild(token1, token);
              CXRefAddTokenChild(token2, token);
              CXRefAddTokenChild(token3, token);
              CXRefAddTokenChild(token4, token);
              CXRefAddTokenChild(token5, token);
            }
            else {
              CXRefAddError("Missing Close Brace for Struct/Union Specifier");

              if (required)
                CXRefError(CXRefGetLastError());

              CXRefRestackToken(token5);
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
      }
      else {
        CXRefRestackToken(token3);

        if (token2.isValid()) {
          token = CXRefCreateToken(STRUCT_OR_UNION_SPECIFIER);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
        }
        else {
          CXRefAddError("Missing Open Brace for Struct/Union Specifier");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token1);
        }
      }
    }
  }

  return token;
}

/*
 * struct-declaration:
 *  specifier-qualifier-list struct-declarator-list
 */

CXRefTokenP
CXRefReadStructDeclaration(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStructDeclaration", token);

  token = CXRefReadTokenOfType(STRUCT_DECLARATION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadSpecifierQualifierList(required);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadStructDeclaratorList(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, SEPARATOR, CXRefSemiColonStrId)) {
          token = CXRefCreateToken(STRUCT_DECLARATION);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefAddError("Missing Semi-Colon in Struct Declaration");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * specifier-qualifier-list:
 *  type-specifier specifier-qualifier-list(opt)
 *  type-qualifier specifier-qualifier-list(opt)
 */

CXRefTokenP
CXRefReadSpecifierQualifierList(bool required)
{
  std::vector<CXRefTokenP> type_tokens = cxref_type_tokens;

  cxref_type_tokens.clear();

  CXRefTokenP token = CXRefReadSpecifierQualifierList1(required);

  cxref_type_tokens = type_tokens;

  return token;
}

CXRefTokenP
CXRefReadSpecifierQualifierList1(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadSpecifierQualifierList", token);

  token = CXRefReadTokenOfType(SPECIFIER_QUALIFIER_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if      (CXRefIsTypeSpecifier(token1)) {
      CXRefRestackToken(token1);

      token1 = CXRefReadTypeSpecifier(required);

      if (! CXRefIsValidTypeTokens(token1)) {
        CXRefAddError("Invalid Token in Specifier Qualifier List");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token1);

        return token;
      }

      cxref_type_tokens.push_back(token1);

      token = CXRefCreateToken(SPECIFIER_QUALIFIER_LIST);

      CXRefAddTokenChild(token1, token);
    }
    else if (CXRefIsTypeQualifier(token1)) {
      CXRefRestackToken(token1);

      token1 = CXRefReadTypeQualifier(required);

      token = CXRefCreateToken(SPECIFIER_QUALIFIER_LIST);

      CXRefAddTokenChild(token1, token);
    }
    else {
      CXRefAddError("Invalid Token in Specifier Qualifier List");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);

      return token;
    }

    token1 = CXRefReadSpecifierQualifierList1(false);

    if (token1.isValid())
      CXRefAddTokenChild(token1, token);
  }

  return token;
}

/*
 * declarator:
 *  pointer(opt) direct-declarator
 */

CXRefTokenP
CXRefReadDeclarator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadDeclarator", token);

  token = CXRefReadTokenOfType(DECLARATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadPointer(false);

    CXRefTokenP token2 = CXRefReadDirectDeclarator(required);

    if (token2.isValid()) {
      token = CXRefCreateToken(DECLARATOR);

      CXRefAddTokenChild(token1, token);
      CXRefAddTokenChild(token2, token);
    }
    else
      CXRefRestackToken(token1);
  }

  return token;
}

/*
 * pointer:
 *  '*' type-qualifier-list(opt)
 *  '*' type-qualifier-list(opt) pointer
 */

CXRefTokenP
CXRefReadPointer(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadPointer", token);

  token = CXRefReadTokenOfType(POINTER);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, OPERATOR, CXRefPointerStrId)) {
      CXRefTokenP token2 = CXRefReadTypeQualifierList(false);
      CXRefTokenP token3 = CXRefReadPointer(false);

      token = CXRefCreateToken(POINTER);

      CXRefAddTokenChild(token1, token);
      CXRefAddTokenChild(token2, token);
      CXRefAddTokenChild(token3, token);
    }
    else {
      CXRefAddError("Pointer Character not Found");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * type-qualifier-list:
 *  type-qualifier
 *  type-qualifier-list type-qualifier
 */

CXRefTokenP
CXRefReadTypeQualifierList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadTypeQualifierList", token);

  token = CXRefReadTokenOfType(TYPE_QUALIFIER_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadTypeQualifier(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(TYPE_QUALIFIER_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadTypeQualifier(false);

    while (token1.isValid()) {
      CXRefTokenP token2 = CXRefCreateToken(TYPE_QUALIFIER_LIST);

      CXRefAddTokenChild(token , token2);
      CXRefAddTokenChild(token1, token2);

      token = token2;

      token1 = CXRefReadTypeQualifier(false);
    }
  }

  return token;
}

/*
 * parameter-type-list:
 *  parameter-list
 *  parameter-list ',' '...'
 */

CXRefTokenP
CXRefReadParameterTypeList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadParameterTypeList", token);

  token = CXRefReadTokenOfType(PARAMETER_TYPE_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadParameterList(required);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, SEPARATOR, CXRefCommaStrId)) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefEllipsisStrId)) {
          token = CXRefCreateToken(PARAMETER_TYPE_LIST);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefAddError("Missing Ellipsis for Parameter Type List");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        token = CXRefCreateToken(PARAMETER_TYPE_LIST);

        CXRefAddTokenChild(token1, token);

        CXRefRestackToken(token2);
      }
    }
  }

  return token;
}

/*
 * parameter-list:
 *  parameter-declaration
 *  parameter-list ',' parameter-declaration
 */

CXRefTokenP
CXRefReadParameterList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadParameterList", token);

  token = CXRefReadTokenOfType(PARAMETER_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadParameterDeclaration(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(PARAMETER_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadParameterDeclaration(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(PARAMETER_LIST);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * parameter-declaration:
 *  declaration-specifiers declarator
 *  declaration-specifiers abstract-declarator(opt)
 */

CXRefTokenP
CXRefReadParameterDeclaration(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadParameterDeclaration", token);

#ifdef GUESS_TYPEDEFS
  bool typedef_tried = false;

 again:
#endif
  token = CXRefReadTokenOfType(PARAMETER_DECLARATION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadDeclarationSpecifiers(required);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadDeclarator(false);

      if (token2.isValid()) {
        token = CXRefCreateToken(PARAMETER_DECLARATION);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
      }
      else {
        token2 = CXRefReadAbstractDeclarator(false);

        token = CXRefCreateToken(PARAMETER_DECLARATION);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
      }
    }

#ifdef GUESS_TYPEDEFS
    if (! token.isValid() && ! typedef_tried) {
      std::vector<CXRefTokenP> children;

      if (! token1.isValid()) {
        CXRefTokenP token2 = CXRefReadToken();

        if (! CXRefIsTokenType(token2, IDENTIFIER))
          goto fail;

        std::cerr << "Assuming " << token2->str << " is a typedef" << std::endl;

        CXRefAddTypedef(token2, token2->str, CXRefIntStrId);

        CXRefRestackToken(token2);
      }
      else {
        CXRefTokenP token1 = CXRefReadDeclarationSpecifiers(false);

        CXRefTokenP token2 = CXRefReadToken();

        if (! CXRefIsTokenType(token2, DECLARATOR))
          goto fail;

        CXRefGetTokenChildren(token2, &children);

        if (children.size() != 1)
          goto fail;

        token2 = children[0];

        CXRefGetTokenChildren(token2, &children);

        if (children.size() != 1)
          goto fail;

        token2 = children[0];

        if (! CXRefIsTokenType(token2, IDENTIFIER))
          goto fail;

        std::cerr << "Assuming " << token2->str << " is a typedef" << std::endl;

        CXRefAddTypedef(token2, token2->str, CXRefIntStrId);

        CXRefRestackToken(token2);
      }

      if (token1.isValid()) {
        std::vector<CXRefTokenP> token_list1;

        CXRefGetTokenChildren(token1, &children);

        token_list1.push_back(children[0]);

        while (children.size() == 2) {
          CXRefGetTokenChildren(children[0], &children);

          token_list1.push_back(children[0]);
        }

        while (! token_list1.empty()) {
          token1 = token_list1.back();

          token_list1.pop_back();

          CXRefRestackToken(token1);
        }
      }

      typedef_tried = true;

      goto again;

 fail:
      typedef_tried = true;

      CXRefRestackToken(token2);
      CXRefRestackToken(token1);
    }
#endif
  }

  return token;
}

/*
 * identifier-list:
 *  identifier
 *  identifier-list ',' identifier
 */

CXRefTokenP
CXRefReadIdentifierList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadIdentifierList", token);

  token = CXRefReadTokenOfType(IDENTIFIER_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsTokenType(token1, IDENTIFIER)) {
      token = CXRefCreateToken(IDENTIFIER_LIST);

      CXRefAddTokenChild(token1, token);
    }
    else {
      CXRefAddError("Missing Identifier for Identifier List");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsTokenType(token2, IDENTIFIER)) {
        CXRefTokenP token3 = CXRefCreateToken(IDENTIFIER_LIST);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else {
        CXRefRestackToken(token2);

        break;
      }

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * initializer:
 *  assignment-expression
 *  '{' initializer-list '}'
 *  '{' initializer-list ',' '}'
 */

CXRefTokenP
CXRefReadInitializer(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadInitializer", token);

  token = CXRefReadTokenOfType(INITIALIZER);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, SEPARATOR, CXRefLBraceStrId)) {
      if (CXRefGetSkipCode()) {
        token = CXRefCreateToken(INITIALIZER);

        CXRefAddTokenChild(token1, token);

        CXRefSkipBracedTokens(token);
      }
      else {
        CXRefTokenP token2 = CXRefReadInitializerList(required);

        if (token2.isValid()) {
          CXRefTokenP token3 = CXRefReadToken();

          if (CXRefIsToken(token3, SEPARATOR, CXRefRBraceStrId)) {
            token = CXRefCreateToken(INITIALIZER);

            CXRefAddTokenChild(token1, token);
            CXRefAddTokenChild(token2, token);
            CXRefAddTokenChild(token3, token);
          }
          else if (CXRefIsToken(token3, SEPARATOR, CXRefCommaStrId)) {
            CXRefTokenP token4 = CXRefReadToken();

            if (CXRefIsToken(token4, SEPARATOR, CXRefRBraceStrId)) {
              token = CXRefCreateToken(INITIALIZER);

              CXRefAddTokenChild(token1, token);
              CXRefAddTokenChild(token2, token);
              CXRefAddTokenChild(token3, token);
              CXRefAddTokenChild(token4, token);
            }
            else {
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
    }
    else {
      CXRefRestackToken(token1);

      CXRefTokenP token1 = CXRefReadAssignmentExpression(required);

      if (token1.isValid()) {
        token = CXRefCreateToken(INITIALIZER);

        CXRefAddTokenChild(token1, token);
      }
    }
  }

  return token;
}

/*
 * initializer-list:
 *  initializer
 *  initializer-list ',' initializer
 */

CXRefTokenP
CXRefReadInitializerList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadInitializerList", token);

  token = CXRefReadTokenOfType(INITIALIZER_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadInitializer(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(INITIALIZER_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadInitializer(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(INITIALIZER_LIST);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * type-name:
 *  specifier-qualifier-list abstract-declarator
 */

CXRefTokenP
CXRefReadTypeName(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadTypeName", token);

  token = CXRefReadTokenOfType(TYPE_NAME);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadSpecifierQualifierList(required);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadAbstractDeclarator(false);

      token = CXRefCreateToken(TYPE_NAME);

      CXRefAddTokenChild(token1, token);
      CXRefAddTokenChild(token2, token);
    }
  }

  return token;
}

/*
 * abstract-declarator:
 *  pointer
 *  pointer(opt) direct-abstract-declarator
 */

CXRefTokenP
CXRefReadAbstractDeclarator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadAbstractDeclarator", token);

  token = CXRefReadTokenOfType(ABSTRACT_DECLARATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadPointer(false);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadDirectAbstractDeclarator(false);

      token = CXRefCreateToken(ABSTRACT_DECLARATOR);

      CXRefAddTokenChild(token1, token);
      CXRefAddTokenChild(token2, token);
    }
    else {
      token1 = CXRefReadDirectAbstractDeclarator(required);

      if (token1.isValid()) {
        token = CXRefCreateToken(ABSTRACT_DECLARATOR);

        CXRefAddTokenChild(token1, token);
      }
    }
  }

  return token;
}

/*
 * direct-abstract-declarator:
 *  '(' abstract-declarator ')'
 *  direct-abstract-declarator '[' constant-expression(opt) ']'
 *  direct-abstract-declarator '(' parameter-type-list(opt) ')'
 */

CXRefTokenP
CXRefReadDirectAbstractDeclarator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadDirectAbstractDeclarator", token);

  token = CXRefReadTokenOfType(DIRECT_ABSTRACT_DECLARATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
      CXRefTokenP token2 = CXRefReadAbstractDeclarator(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
          token = CXRefCreateToken(DIRECT_ABSTRACT_DECLARATOR);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefTokenP token2 = CXRefReadParameterTypeList(false);

        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
          token = CXRefCreateToken(DIRECT_ABSTRACT_DECLARATOR);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
    }
    else if (CXRefIsToken(token1, OPERATOR, CXRefLBracketStrId)) {
      CXRefTokenP token2 = CXRefReadConstantExpression(false);

      CXRefTokenP token3 = CXRefReadToken();

      if (CXRefIsToken(token3, OPERATOR, CXRefRBracketStrId)) {
        token = CXRefCreateToken(DIRECT_ABSTRACT_DECLARATOR);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
        CXRefAddTokenChild(token3, token);
      }
      else {
        CXRefRestackToken(token3);
        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else {
      CXRefAddError("Invalid Token for Direct Abstract Declarator");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefLBracketStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
      if (CXRefIsToken(token1, OPERATOR, CXRefLBracketStrId)) {
        CXRefTokenP token2 = CXRefReadConstantExpression(false);

        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRBracketStrId)) {
          CXRefTokenP token4 = CXRefCreateToken(DIRECT_ABSTRACT_DECLARATOR);

          CXRefAddTokenChild(token , token4);
          CXRefAddTokenChild(token1, token4);
          CXRefAddTokenChild(token2, token4);
          CXRefAddTokenChild(token3, token4);

          token = token4;
        }
        else {
          CXRefAddError("Missing Close Square Bracket in Direct Abstract Declarator");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);

          break;
        }
      }
      else if (CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
        CXRefTokenP token2 = CXRefReadParameterTypeList(false);

        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
          CXRefTokenP token4 = CXRefCreateToken(DIRECT_ABSTRACT_DECLARATOR);

          CXRefAddTokenChild(token , token4);
          CXRefAddTokenChild(token1, token4);
          CXRefAddTokenChild(token2, token4);
          CXRefAddTokenChild(token3, token4);

          token = token4;
        }
        else {
          CXRefAddError("Missing Close Round Bracket in Direct Abstract Declarator");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);

          break;
        }
      }

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * statement:
 *  labeled-statement
 *  expression-statement
 *  compound-statement
 *  selection-statement
 *  iteration-statement
 *  jump-statement
 */

CXRefTokenP
CXRefReadStatement(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStatement", token);

  token = CXRefReadTokenOfType(STATEMENT);

  if (! token.isValid()) {
    CXRefTokenP             token1 = CXRefReadLabeledStatement(false);
    if (! token1.isValid()) token1 = CXRefReadExpressionStatement(false);
    if (! token1.isValid()) token1 = CXRefReadCompoundStatement(false);
    if (! token1.isValid()) token1 = CXRefReadSelectionStatement(false);
    if (! token1.isValid()) token1 = CXRefReadIterationStatement(false);
    if (! token1.isValid()) token1 = CXRefReadJumpStatement(required);

    if (! token1.isValid())
      return token;

    token = CXRefCreateToken(STATEMENT);

    CXRefAddTokenChild(token1, token);
  }

  return token;
}

/*
 * labeled-statement:
 *  identifier ':' statement
 *  'case' constant-expression ':' statement
 *  'default' ':' statement
 */

CXRefTokenP
CXRefReadLabeledStatement(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadLabeledStatement", token);

  token = CXRefReadTokenOfType(LABELED_STATEMENT);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsTokenType(token1, IDENTIFIER)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefColonStrId)) {
        CXRefTokenP token3 = CXRefReadStatement(required);

        if (token3.isValid()) {
          token = CXRefCreateToken(LABELED_STATEMENT);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefAddError("Missing Colon in Labeled Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefCaseStrId)) {
      CXRefTokenP token2 = CXRefReadConstantExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefColonStrId)) {
          CXRefTokenP token4 = CXRefReadStatement(required);

          if (token4.isValid()) {
            token = CXRefCreateToken(LABELED_STATEMENT);

            CXRefAddTokenChild(token1, token);
            CXRefAddTokenChild(token2, token);
            CXRefAddTokenChild(token3, token);
            CXRefAddTokenChild(token4, token);
          }
          else {
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefAddError("Missing Colon in Case Statement");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token1);
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefDefaultStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefColonStrId)) {
        CXRefTokenP token3 = CXRefReadStatement(required);

        if (token3.isValid()) {
          token = CXRefCreateToken(LABELED_STATEMENT);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefAddError("Missing Colon in Default Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else {
      CXRefAddError("Invalid Labeled Statement");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * expression-statement:
 *  expression(opt) ';'
 */

CXRefTokenP
CXRefReadExpressionStatement(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadExpressionStatement", token);

  token = CXRefReadTokenOfType(EXPRESSION_STATEMENT);

  if (! token.isValid()) {
    cxref_typedef_token = CXRefTokenP();

    CXRefTokenP token1 = CXRefReadExpression(false);

    CXRefTokenP token2 = CXRefReadToken();

#ifdef GUESS_TYPEDEFS
    if (token1.isValid() && ! CXRefIsToken(token2, SEPARATOR, CXRefSemiColonStrId) &&
        cxref_typedef_token.isValid()) {
      std::vector<CXRefTokenP> children;

      std::cout << "Assuming " << cxref_typedef_token->str << " is a typedef" << std::endl;

      CXRefAddTypedef(cxref_typedef_token, cxref_typedef_token->str, CXRefIntStrId);

      CXRefGetBaseTokenChildren(token1, &children);

      CXRefRestackToken(token2);

      for (int i = children.size() - 1; i >= 0; i--)
        CXRefRestackToken(children[i]);

      token1 = CXRefReadExpression(false);

      token2 = CXRefReadToken();
    }
#endif

    if (CXRefIsToken(token2, SEPARATOR, CXRefSemiColonStrId)) {
      token = CXRefCreateToken(EXPRESSION_STATEMENT);

      CXRefAddTokenChild(token1, token);
      CXRefAddTokenChild(token2, token);
    }
    else {
      CXRefAddError("Invalid Expression Statement");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token2);
      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * compound-statement:
 *  '{' declaration-list(opt) statement-list(opt) '}'
 */

CXRefTokenP
CXRefReadCompoundStatement(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadCompoundStatement", token);

  token = CXRefReadTokenOfType(COMPOUND_STATEMENT);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, SEPARATOR, CXRefLBraceStrId)) {
      if (CXRefGetSkipCode()) {
        token = CXRefCreateToken(COMPOUND_STATEMENT);

        CXRefAddTokenChild(token1, token);

        CXRefSkipBracedTokens(token);
      }
      else {
        CXRefTokenP token2 = CXRefReadDeclarationList(false);

        CXRefTokenP token3 = CXRefReadStatementList(false);

        CXRefTokenP token4 = CXRefReadToken();

        if (CXRefIsToken(token4, SEPARATOR, CXRefRBraceStrId)) {
          token = CXRefCreateToken(COMPOUND_STATEMENT);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
          CXRefAddTokenChild(token4, token);
        }
        else {
          CXRefAddError("Missing Closing Brace for Compound Statement");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token4);
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
    }
    else {
      CXRefAddError("Invalid Compound Statement");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * statement-list:
 *  statement
 *  statement-list statement
 */

CXRefTokenP
CXRefReadStatementList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadStatementList", token);

  token = CXRefReadTokenOfType(STATEMENT_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadStatement(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(STATEMENT_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadStatement(false);

    while (token1.isValid()) {
      CXRefTokenP token2 = CXRefCreateToken(STATEMENT_LIST);

      CXRefAddTokenChild(token , token2);
      CXRefAddTokenChild(token1, token2);

      token = token2;

      token1 = CXRefReadStatement(false);
    }
  }

  return token;
}

/*
 * selection-statement:
 *  'if' '(' expression ')' statement
 *  'if' '(' expression ')' statement 'else' statement
 *  'switch' '(' expression ')' statement
 */

CXRefTokenP
CXRefReadSelectionStatement(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadSelectionStatement", token);

  token = CXRefReadTokenOfType(SELECTION_STATEMENT);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, KEYWORD, CXRefIfStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefLParenStrId)) {
        CXRefTokenP token3 = CXRefReadExpression(required);

        if (token3.isValid()) {
          CXRefTokenP token4 = CXRefReadToken();

          if (CXRefIsToken(token4, OPERATOR, CXRefRParenStrId)) {
            CXRefTokenP token5 = CXRefReadStatement(required);

            if (token5.isValid()) {
              CXRefTokenP token6 = CXRefReadToken();

              if (CXRefIsToken(token6, KEYWORD, CXRefElseStrId)) {
                CXRefTokenP token7 = CXRefReadStatement(required);

                if (token7.isValid()) {
                  token = CXRefCreateToken(SELECTION_STATEMENT);

                  CXRefAddTokenChild(token1, token);
                  CXRefAddTokenChild(token2, token);
                  CXRefAddTokenChild(token3, token);
                  CXRefAddTokenChild(token4, token);
                  CXRefAddTokenChild(token5, token);
                  CXRefAddTokenChild(token6, token);
                  CXRefAddTokenChild(token7, token);
                }
                else {
                  CXRefRestackToken(token6);
                  CXRefRestackToken(token5);
                  CXRefRestackToken(token4);
                  CXRefRestackToken(token3);
                  CXRefRestackToken(token2);
                  CXRefRestackToken(token1);
                }
              }
              else {
                CXRefRestackToken(token6);

                token = CXRefCreateToken(SELECTION_STATEMENT);

                CXRefAddTokenChild(token1, token);
                CXRefAddTokenChild(token2, token);
                CXRefAddTokenChild(token3, token);
                CXRefAddTokenChild(token4, token);
                CXRefAddTokenChild(token5, token);
              }
            }
            else {
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefAddError("Missing Close Round Bracket for If Statement");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token4);
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefAddError("Missing Open Round Bracket for If Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefSwitchStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefLParenStrId)) {
        CXRefTokenP token3 = CXRefReadExpression(required);

        if (token3.isValid()) {
          CXRefTokenP token4 = CXRefReadToken();

          if (CXRefIsToken(token4, OPERATOR, CXRefRParenStrId)) {
            CXRefTokenP token5 = CXRefReadStatement(required);

            if (token5.isValid()) {
              token = CXRefCreateToken(SELECTION_STATEMENT);

              CXRefAddTokenChild(token1, token);
              CXRefAddTokenChild(token2, token);
              CXRefAddTokenChild(token3, token);
              CXRefAddTokenChild(token4, token);
              CXRefAddTokenChild(token5, token);
            }
            else {
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefAddError("Missing Close Round Bracket for Switch Statement");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token4);
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefAddError("Missing Open Round Bracket for Switch Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else {
      CXRefAddError("Invalid Selection Statement");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * iteration-statement:
 *  'while' '(' expression ')' statement
 *  'do' statement 'while' '(' expression ')' ';'
 *  'for' '(' expression(opt) ';' expression(opt) ';' expression(opt) ')' statement
 */

CXRefTokenP
CXRefReadIterationStatement(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadIterationStatement", token);

  token = CXRefReadTokenOfType(ITERATION_STATEMENT);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, KEYWORD, CXRefWhileStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefLParenStrId)) {
        CXRefTokenP token3 = CXRefReadExpression(required);

        if (token3.isValid()) {
          CXRefTokenP token4 = CXRefReadToken();

          if (CXRefIsToken(token4, OPERATOR, CXRefRParenStrId)) {
            CXRefTokenP token5 = CXRefReadStatement(required);

            if (token5.isValid()) {
              token = CXRefCreateToken(ITERATION_STATEMENT);

              CXRefAddTokenChild(token1, token);
              CXRefAddTokenChild(token2, token);
              CXRefAddTokenChild(token3, token);
              CXRefAddTokenChild(token4, token);
              CXRefAddTokenChild(token5, token);
            }
            else {
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefAddError("Missing Close Round Bracket for While Statement");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token4);
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefAddError("Missing Open Round Bracket for While Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefDoStrId)) {
      CXRefTokenP token2 = CXRefReadStatement(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, KEYWORD, CXRefWhileStrId)) {
          CXRefTokenP token4 = CXRefReadToken();

          if (CXRefIsToken(token4, OPERATOR, CXRefLParenStrId)) {
            CXRefTokenP token5 = CXRefReadExpression(required);

            if (token5.isValid()) {
              CXRefTokenP token6 = CXRefReadToken();

              if (CXRefIsToken(token6, OPERATOR, CXRefRParenStrId)) {
                CXRefTokenP token7 = CXRefReadToken();

                if (CXRefIsToken(token7, SEPARATOR, CXRefSemiColonStrId)) {
                  token = CXRefCreateToken(ITERATION_STATEMENT);

                  CXRefAddTokenChild(token1, token);
                  CXRefAddTokenChild(token2, token);
                  CXRefAddTokenChild(token3, token);
                  CXRefAddTokenChild(token4, token);
                  CXRefAddTokenChild(token5, token);
                  CXRefAddTokenChild(token6, token);
                  CXRefAddTokenChild(token7, token);
                }
                else {
                  CXRefAddError("Missing Semi-Colon for Do While Statement");

                  if (required)
                    CXRefError(CXRefGetLastError());

                  CXRefRestackToken(token7);
                  CXRefRestackToken(token6);
                  CXRefRestackToken(token5);
                  CXRefRestackToken(token4);
                  CXRefRestackToken(token3);
                  CXRefRestackToken(token2);
                  CXRefRestackToken(token1);
                }
              }
              else {
                CXRefAddError("Missing Close Round Bracket for Do While Statement");

                if (required)
                  CXRefError(CXRefGetLastError());

                CXRefRestackToken(token6);
                CXRefRestackToken(token5);
                CXRefRestackToken(token4);
                CXRefRestackToken(token3);
                CXRefRestackToken(token2);
                CXRefRestackToken(token1);
              }
            }
            else {
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefAddError("Missing Open Round Bracket for Do While Statement");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token4);
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefAddError("Missing While for Do While Statement");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token1);
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefForStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefLParenStrId)) {
        CXRefTokenP token3 = CXRefReadExpression(false);

        CXRefTokenP token4 = CXRefReadToken();

        if (CXRefIsToken(token4, SEPARATOR, CXRefSemiColonStrId)) {
          CXRefTokenP token5 = CXRefReadExpression(false);

          CXRefTokenP token6 = CXRefReadToken();

          if (CXRefIsToken(token6, SEPARATOR, CXRefSemiColonStrId)) {
            CXRefTokenP token7 = CXRefReadExpression(false);

            CXRefTokenP token8 = CXRefReadToken();

            if (CXRefIsToken(token8, OPERATOR, CXRefRParenStrId)) {
              CXRefTokenP token9 = CXRefReadStatement(required);

              if (token9.isValid()) {
                token = CXRefCreateToken(ITERATION_STATEMENT);

                CXRefAddTokenChild(token1, token);
                CXRefAddTokenChild(token2, token);
                CXRefAddTokenChild(token3, token);
                CXRefAddTokenChild(token4, token);
                CXRefAddTokenChild(token5, token);
                CXRefAddTokenChild(token6, token);
                CXRefAddTokenChild(token7, token);
                CXRefAddTokenChild(token8, token);
                CXRefAddTokenChild(token9, token);
              }
              else {
                CXRefRestackToken(token8);
                CXRefRestackToken(token7);
                CXRefRestackToken(token6);
                CXRefRestackToken(token5);
                CXRefRestackToken(token4);
                CXRefRestackToken(token3);
                CXRefRestackToken(token2);
                CXRefRestackToken(token1);
              }
            }
            else {
              CXRefAddError("Missing Close Round Bracket for For Statement");

              if (required)
                CXRefError(CXRefGetLastError());

              CXRefRestackToken(token8);
              CXRefRestackToken(token7);
              CXRefRestackToken(token6);
              CXRefRestackToken(token5);
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefAddError("Missing Second Semi-Colon for For Statement");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token6);
            CXRefRestackToken(token5);
            CXRefRestackToken(token4);
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefAddError("Missing First Semi-Colon for For Statement");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token4);
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefAddError("Missing Open Round Bracket for For Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else {
      CXRefAddError("Invalid Iteration Statement");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * jump-statement:
 *  'goto' identifier ';'
 *  'continue' ';'
 *  'break' ';'
 *  'return' expression(opt) ';'
 */

CXRefTokenP
CXRefReadJumpStatement(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadJumpStatement", token);

  token = CXRefReadTokenOfType(JUMP_STATEMENT);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, KEYWORD, CXRefGotoStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsTokenType(token2, IDENTIFIER)) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, SEPARATOR, CXRefSemiColonStrId)) {
          token = CXRefCreateToken(JUMP_STATEMENT);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefAddError("Missing Semi-Colon for Goto Statement");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefAddError("Missing Identifier for Goto Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefContinueStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, SEPARATOR, CXRefSemiColonStrId)) {
        token = CXRefCreateToken(JUMP_STATEMENT);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
      }
      else {
        CXRefAddError("Missing Semi-Colon for Continue Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefBreakStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, SEPARATOR, CXRefSemiColonStrId)) {
        token = CXRefCreateToken(JUMP_STATEMENT);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
      }
      else {
        CXRefAddError("Missing Semi-Colon for Break Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else if (CXRefIsToken(token1, KEYWORD, CXRefReturnStrId)) {
      CXRefTokenP token2 = CXRefReadExpression(false);

      CXRefTokenP token3 = CXRefReadToken();

      if (CXRefIsToken(token3, SEPARATOR, CXRefSemiColonStrId)) {
        token = CXRefCreateToken(JUMP_STATEMENT);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);
        CXRefAddTokenChild(token3, token);
      }
      else {
        CXRefAddError("Missing Semi-Colon for Return Statement");

        if (required)
          CXRefError(CXRefGetLastError());

        CXRefRestackToken(token3);
        CXRefRestackToken(token2);
        CXRefRestackToken(token1);
      }
    }
    else {
      CXRefAddError("Invalid Jump Statement");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * constant-expression:
 *  conditional-expression
 */

CXRefTokenP
CXRefReadConstantExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadConstantExpression", token);

  token = CXRefReadTokenOfType(CONSTANT_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadConditionalExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(CONSTANT_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  return token;
}

/*
 * expression:
 *  assignment-expression
 *  expression ',' assignment-expression
 */

CXRefTokenP
CXRefReadExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadExpression", token);

  token = CXRefReadTokenOfType(EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadAssignmentExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadAssignmentExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * assignment-operator:
 *  '=' '*=' '/=' '%=' '+=' '-=' '<<=' '>>=' '&=' '^=' '|='
 */

CXRefTokenP
CXRefReadAssignmentOperator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadAssignmentOperator", token);

  token = CXRefReadTokenOfType(ASSIGNMENT_OPERATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, OPERATOR, CXRefEqualStrId  ) ||
        CXRefIsToken(token1, OPERATOR, CXRefTimesEqualStrId ) ||
        CXRefIsToken(token1, OPERATOR, CXRefDivideEqualStrId ) ||
        CXRefIsToken(token1, OPERATOR, CXRefModulusEqualStrId ) ||
        CXRefIsToken(token1, OPERATOR, CXRefPlusEqualStrId ) ||
        CXRefIsToken(token1, OPERATOR, CXRefMinusEqualStrId ) ||
        CXRefIsToken(token1, OPERATOR, CXRefLShiftEqualStrId) ||
        CXRefIsToken(token1, OPERATOR, CXRefRShiftEqualStrId) ||
        CXRefIsToken(token1, OPERATOR, CXRefAndEqualStrId ) ||
        CXRefIsToken(token1, OPERATOR, CXRefXorEqualStrId ) ||
        CXRefIsToken(token1, OPERATOR, CXRefOrEqualStrId )) {
      token = CXRefCreateToken(ASSIGNMENT_OPERATOR);

      CXRefAddTokenChild(token1, token);
    }
    else {
      CXRefAddError("Invalid Assignment Operator");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * assignment-expression:
 *  conditional-expression
 *  unary-expression assignment-operator assignment-expression
 */

CXRefTokenP
CXRefReadAssignmentExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadAssignmentExpression", token);

  token = CXRefReadTokenOfType(ASSIGNMENT_EXPRESSION);

  if (token.isValid())
    return token;

  CXRefTokenP token1 = CXRefReadConditionalExpression(false);

  if (token1.isValid()) {
    CXRefTokenP token2 = CXRefCollapseToType(token1, UNARY_EXPRESSION);

    if (token2.isValid()) {
      CXRefTokenP token3 = CXRefReadAssignmentOperator(false);

      if (token3.isValid()) {
        CXRefTokenP token4 = CXRefReadAssignmentExpression(false);

        if (token4.isValid()) {
          token = CXRefCreateToken(ASSIGNMENT_EXPRESSION);

          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
          CXRefAddTokenChild(token4, token);
        }
        else
          CXRefRestackToken(token3);
      }
    }

    if (! token.isValid()) {
      token = CXRefCreateToken(ASSIGNMENT_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }

    return token;
  }

  CXRefTokenP token2 = CXRefReadUnaryExpression(required);

  if (! token2.isValid())
    return token;

  CXRefTokenP token3 = CXRefReadAssignmentOperator(required);

  if (! token3.isValid()) {
    CXRefRestackToken(token2);

    return token;
  }

  CXRefTokenP token4 = CXRefReadAssignmentExpression(required);

  if (! token4.isValid()) {
    CXRefRestackToken(token3);
    CXRefRestackToken(token2);

    return token;
  }

  token = CXRefCreateToken(ASSIGNMENT_EXPRESSION);

  CXRefAddTokenChild(token2, token);
  CXRefAddTokenChild(token3, token);
  CXRefAddTokenChild(token4, token);

  return token;
}

/*
 * conditional-expression:
 *  logical-or-expression
 *  logical-or-expression '?' expression ':' conditional-expression
 */

CXRefTokenP
CXRefReadConditionalExpression(bool required)
{
  CXRefTokenP token = CXRefReadTokenOfType(CONDITIONAL_EXPRESSION);

  CXREF_TRACE_TOKEN("CXRefReadConditionalExpression", token);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadLogicalOrExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(CONDITIONAL_EXPRESSION);

      CXRefAddTokenChild(token1, token);

      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefQuestionStrId)) {
        CXRefTokenP token3 = CXRefReadExpression(required);

        if (token3.isValid()) {
          CXRefTokenP token4 = CXRefReadToken();

          if (CXRefIsToken(token4, OPERATOR, CXRefColonStrId)) {
            CXRefTokenP token5 = CXRefReadConditionalExpression(required);

            if (token5.isValid()) {
              CXRefAddTokenChild(token2, token);
              CXRefAddTokenChild(token3, token);
              CXRefAddTokenChild(token4, token);
              CXRefAddTokenChild(token5, token);
            }
            else {
              CXRefRestackToken(token4);
              CXRefRestackToken(token3);
              CXRefRestackToken(token2);
              CXRefRestackToken(token1);
            }
          }
          else {
            CXRefAddError("Missing Colon for ?: Expression");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token4);
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token2);
    }
  }

  return token;
}

/*
 * logical-or-expression:
 *  logical-and-expression
 *  logical-or-expression '||' logical-and-expression
 */

CXRefTokenP
CXRefReadLogicalOrExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadLogicalOrExpression", token);

  token = CXRefReadTokenOfType(LOGICAL_OR_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadLogicalAndExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(LOGICAL_OR_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefLOrStrId)) {
      CXRefTokenP token2 = CXRefReadLogicalAndExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(LOGICAL_OR_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * logical-and-expression:
 *  inclusive-or-expression
 *  logical-and-expression '&&' inclusive-or-expression
 */

CXRefTokenP
CXRefReadLogicalAndExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadLogicalAndExpression", token);

  token = CXRefReadTokenOfType(LOGICAL_AND_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadInclusiveOrExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(LOGICAL_AND_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefLAndStrId)) {
      CXRefTokenP token2 = CXRefReadInclusiveOrExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(LOGICAL_AND_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * inclusive-or-expression:
 *  exclusive-or-expression
 *  inclusive-or-expression '|' exclusive-or-expression
 */

CXRefTokenP
CXRefReadInclusiveOrExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadInclusiveOrExpression", token);

  token = CXRefReadTokenOfType(INCLUSIVE_OR_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadExclusiveOrExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(INCLUSIVE_OR_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefOrStrId)) {
      CXRefTokenP token2 = CXRefReadExclusiveOrExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(INCLUSIVE_OR_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * exclusive-or-expression:
 *  and-expression
 *  exclusive-or-expression '^' and-expression
 */

CXRefTokenP
CXRefReadExclusiveOrExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadExclusiveOrExpression", token);

  token = CXRefReadTokenOfType(EXCLUSIVE_OR_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadAndExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(EXCLUSIVE_OR_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefXorStrId)) {
      CXRefTokenP token2 = CXRefReadAndExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(EXCLUSIVE_OR_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * and-expression:
 *  equality-expression
 *  and-expression '&' equality-expression
 */

CXRefTokenP
CXRefReadAndExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadAndExpression", token);

  token = CXRefReadTokenOfType(AND_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadEqualityExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(AND_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefAndStrId)) {
      CXRefTokenP token2 = CXRefReadEqualityExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(AND_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * equality-expression:
 *  relational-expression
 *  equality-expression '==' relational-expression
 *  equality-expression '!=' relational-expression
 */

CXRefTokenP
CXRefReadEqualityExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadEqualityExpression", token);

  token = CXRefReadTokenOfType(EQUALITY_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadRelationalExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(EQUALITY_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefEqualsStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefNotEqualsStrId)) {
      CXRefTokenP token2 = CXRefReadRelationalExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(EQUALITY_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * relational-expression:
 *  shift-expression
 *  relational-expression '<' shift-expression
 *  relational-expression '>' shift-expression
 *  relational-expression '<=' shift-expression
 *  relational-expression '>=' shift-expression
 */

CXRefTokenP
CXRefReadRelationalExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadRelationalExpression", token);

  token = CXRefReadTokenOfType(RELATIONAL_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadShiftExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(RELATIONAL_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefLessStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefGreaterStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefLessEqualStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefGreaterEqualStrId)) {
      CXRefTokenP token2 = CXRefReadShiftExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(RELATIONAL_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * shift-expression:
 *  additive-expression
 *  shift-expression '<<' additive-expression
 *  shift-expression '>>' additive-expression
 */

CXRefTokenP
CXRefReadShiftExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadShiftExpression", token);

  token = CXRefReadTokenOfType(SHIFT_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadAdditiveExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(SHIFT_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefLShiftStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefRShiftStrId)) {
      CXRefTokenP token2 = CXRefReadAdditiveExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(SHIFT_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * additive-expression:
 *  multiplicative-expression
 *  additive-expression '+' multiplicative-expression
 *  additive-expression '-' multiplicative-expression
 */

CXRefTokenP
CXRefReadAdditiveExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadAdditiveExpression", token);

  token = CXRefReadTokenOfType(ADDITIVE_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadMultiplicativeExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(ADDITIVE_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefPlusStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefMinusStrId)) {
      CXRefTokenP token2 = CXRefReadMultiplicativeExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(ADDITIVE_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * multiplicative-expression:
 *  cast-expression
 *  multiplicative-expression '*' cast-expression
 *  multiplicative-expression '/' cast-expression
 *  multiplicative-expression '%' cast-expression
 */

CXRefTokenP
CXRefReadMultiplicativeExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadMultiplicativeExpression", token);

  token = CXRefReadTokenOfType(MULTIPLICATIVE_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadCastExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(MULTIPLICATIVE_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefTimesStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefDivideStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefModulusStrId)) {
      CXRefTokenP token2 = CXRefReadCastExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(MULTIPLICATIVE_EXPRESSION);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * cast-expression:
 *  unary-expression
 *  '(' type-name ')' cast-expression
 */

CXRefTokenP
CXRefReadCastExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadCastExpression", token);

  token = CXRefReadTokenOfType(CAST_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
      CXRefTokenP token2 = CXRefReadTypeName(false);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
          CXRefTokenP token4 = CXRefReadCastExpression(required);

          if (token4.isValid()) {
            token = CXRefCreateToken(CAST_EXPRESSION);

            CXRefAddTokenChild(token1, token);
            CXRefAddTokenChild(token2, token);
            CXRefAddTokenChild(token3, token);
            CXRefAddTokenChild(token4, token);
          }
          else {
            CXRefRestackToken(token3);
            CXRefRestackToken(token2);
            CXRefRestackToken(token1);
          }
        }
        else {
          CXRefAddError("Missing Close Round Bracket for Cast");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else {
        CXRefRestackToken(token1);

        CXRefTokenP token1 = CXRefReadUnaryExpression(false);

        if (token1.isValid()) {
          token = CXRefCreateToken(CAST_EXPRESSION);

          CXRefAddTokenChild(token1, token);
        }
      }
    }
    else {
      CXRefRestackToken(token1);

      CXRefTokenP token1 = CXRefReadUnaryExpression(false);

      if (token1.isValid()) {
        token = CXRefCreateToken(CAST_EXPRESSION);

        CXRefAddTokenChild(token1, token);
      }
    }
  }

  return token;
}

/*
 * unary-operator:
 *  '&' '*' '+' '-' '~' '!'
 */

CXRefTokenP
CXRefReadUnaryOperator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadUnaryOperator", token);

  token = CXRefReadTokenOfType(UNARY_OPERATOR);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if (CXRefIsToken(token1, OPERATOR, CXRefAddressStrId) ||
        CXRefIsToken(token1, OPERATOR, CXRefPointerStrId) ||
        CXRefIsToken(token1, OPERATOR, CXRefPlusStrId   ) ||
        CXRefIsToken(token1, OPERATOR, CXRefMinusStrId  ) ||
        CXRefIsToken(token1, OPERATOR, CXRefLXorStrId   ) ||
        CXRefIsToken(token1, OPERATOR, CXRefLNotStrId   )) {
      token = CXRefCreateToken(UNARY_OPERATOR);

      CXRefAddTokenChild(token1, token);

      return token;
    }

    CXRefRestackToken(token1);

    CXRefAddError("Invalid Unary Operator");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

/*
 * unary-expression:
 *  postfix-expression
 *  '++' unary-expression
 *  '--' unary-expression
 *  unary-operator cast-expression
 *  'sizeof' unary-expression
 *  'sizeof' '(' type-name ')'
 */

CXRefTokenP
CXRefReadUnaryExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadUnaryExpression", token);

  token = CXRefReadTokenOfType(UNARY_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadPostfixExpression(false);

    if (token1.isValid()) {
      token = CXRefCreateToken(UNARY_EXPRESSION);

      CXRefAddTokenChild(token1, token);

      return token;
    }

    token1 = CXRefReadToken();

    if (CXRefIsToken(token1, OPERATOR, CXRefIncrementStrId) ||
        CXRefIsToken(token1, OPERATOR, CXRefDecrementStrId)) {
      CXRefTokenP token2 = CXRefReadUnaryExpression(required);

      if (token2.isValid()) {
        token = CXRefCreateToken(UNARY_EXPRESSION);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);

        return token;
      }
    }

    CXRefRestackToken(token1);

    token1 = CXRefReadUnaryOperator(false);

    if (token1.isValid()) {
      CXRefTokenP token2 = CXRefReadCastExpression(required);

      if (token2.isValid()) {
        token = CXRefCreateToken(UNARY_EXPRESSION);

        CXRefAddTokenChild(token1, token);
        CXRefAddTokenChild(token2, token);

        return token;
      }
    }

    token1 = CXRefReadToken();

    if (CXRefIsToken(token1, KEYWORD, CXRefSizeofStrId)) {
      CXRefTokenP token2 = CXRefReadToken();

      if (CXRefIsToken(token2, OPERATOR, CXRefLParenStrId)) {
        CXRefTokenP token3 = CXRefReadTypeName(false);

        if (token3.isValid()) {
          CXRefTokenP token4 = CXRefReadToken();

          if (CXRefIsToken(token4, OPERATOR, CXRefRParenStrId)) {
            token = CXRefCreateToken(UNARY_EXPRESSION);

            CXRefAddTokenChild(token1, token);
            CXRefAddTokenChild(token2, token);
            CXRefAddTokenChild(token3, token);
            CXRefAddTokenChild(token4, token);

            return token;
          }

          CXRefAddError("Missing Close Round Bracket for Sizeof Type");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token4);
          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);

          return token;
        }

        CXRefRestackToken(token2);

        token2 = CXRefReadUnaryExpression(required);

        if (token2.isValid()) {
          token = CXRefCreateToken(UNARY_EXPRESSION);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);

          return token;
        }

        CXRefRestackToken(token1);

        return token;
      }
      else {
        CXRefRestackToken(token2);

        token2 = CXRefReadUnaryExpression(required);

        if (token2.isValid()) {
          token = CXRefCreateToken(UNARY_EXPRESSION);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);

          return token;
        }

        CXRefRestackToken(token1);

        return token;
      }
    }

    CXRefRestackToken(token1);

    CXRefAddError("Invalid Unary Expression");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

/*
 * postfix-expression:
 *  primary-expression
 *  postfix-expression '[' expression ']'
 *  postfix-expression '(' ')'
 *  postfix-expression '(' argument-expression-list ')'
 *  postfix-expression '.' identifier
 *  postfix-expression '->' identifier
 *  postfix-expression '++'
 *  postfix-expression '--'
 */

CXRefTokenP
CXRefReadPostfixExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadPostfixExpression", token);

  token = CXRefReadTokenOfType(POSTFIX_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadPrimaryExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(POSTFIX_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, OPERATOR, CXRefLBracketStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefLParenStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefPeriodStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefReferenceStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefIncrementStrId) ||
           CXRefIsToken(token1, OPERATOR, CXRefDecrementStrId)) {
      if (CXRefIsToken(token1, OPERATOR, CXRefLBracketStrId)) {
        CXRefTokenP token2 = CXRefReadExpression(required);

        if (token2.isValid()) {
          CXRefTokenP token3 = CXRefReadToken();

          if (CXRefIsToken(token3, OPERATOR, CXRefRBracketStrId)) {
            CXRefTokenP token4 = CXRefCreateToken(POSTFIX_EXPRESSION);

            CXRefAddTokenChild(token , token4);
            CXRefAddTokenChild(token1, token4);
            CXRefAddTokenChild(token2, token4);
            CXRefAddTokenChild(token3, token4);

            token = token4;
          }
          else {
            CXRefAddError("Missing Close Square Bracket for Array Index");

            if (required)
              CXRefError(CXRefGetLastError());

            CXRefRestackToken(token3);
            CXRefRestackToken(token2);

            break;
          }
        }
        else
          break;
      }
      else if (CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
        cxref_typedef_token = CXRefTokenP();

        CXRefTokenP token2 = CXRefReadArgumentExpressionList(false);

        CXRefTokenP token3 = CXRefReadToken();

#ifdef GUESS_TYPEDEFS
        if (token2.isValid() && ! CXRefIsToken(token3, OPERATOR, CXRefRParenStrId) &&
            cxref_typedef_token.isValid()) {
          std::vector<CXRefTokenP> children;

          std::cerr << "Assuming " << cxref_typedef_token->str << " is a typedef" << std::endl;

          CXRefAddTypedef(cxref_typedef_token, cxref_typedef_token->str, CXRefIntStrId);

          CXRefGetBaseTokenChildren(token2, &children);

          CXRefRestackToken(token3);

          for (int i = children.size() - 1; i >= 0; i--)
            CXRefRestackToken(children[i]);

          token2 = CXRefReadArgumentExpressionList(false);

          token3 = CXRefReadToken();
        }
#endif

        if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
          CXRefTokenP token4 = CXRefCreateToken(POSTFIX_EXPRESSION);

          CXRefAddTokenChild(token , token4);
          CXRefAddTokenChild(token1, token4);
          CXRefAddTokenChild(token2, token4);
          CXRefAddTokenChild(token3, token4);

          token = token4;
        }
        else {
          CXRefAddError("Missing Close Round Bracket for Function Argument List");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);

          break;
        }
      }
      else if (CXRefIsToken(token1, OPERATOR, CXRefPeriodStrId) ||
               CXRefIsToken(token1, OPERATOR, CXRefReferenceStrId)) {
        CXRefTokenP token2 = CXRefReadToken();

        if (CXRefIsTokenType(token2, IDENTIFIER)) {
          CXRefTokenP token3 = CXRefCreateToken(POSTFIX_EXPRESSION);

          CXRefAddTokenChild(token , token3);
          CXRefAddTokenChild(token1, token3);
          CXRefAddTokenChild(token2, token3);

          token = token3;
        }
        else {
          CXRefRestackToken(token2);

          break;
        }
      }
      else if (CXRefIsToken(token1, OPERATOR, CXRefIncrementStrId) ||
               CXRefIsToken(token1, OPERATOR, CXRefDecrementStrId)) {
        CXRefTokenP token2 = CXRefCreateToken(POSTFIX_EXPRESSION);

        CXRefAddTokenChild(token , token2);
        CXRefAddTokenChild(token1, token2);

        token = token2;
      }

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

/*
 * primary-expression:
 *  identifier
 *  constant
 *  string
 *  '(' expression ')'
 */

CXRefTokenP
CXRefReadPrimaryExpression(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadPrimaryExpression", token);

  token = CXRefReadTokenOfType(PRIMARY_EXPRESSION);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    if      (CXRefIsTokenType(token1, IDENTIFIER) ||
             CXRefIsTokenType(token1, CONSTANT)) {
      token = CXRefCreateToken(PRIMARY_EXPRESSION);

      CXRefAddTokenChild(token1, token);
    }
    else if (CXRefIsTokenType(token1, STRING)) {
      token = CXRefCreateToken(PRIMARY_EXPRESSION);

      CXRefAddTokenChild(token1, token);

      token1 = CXRefReadToken();

      while (token1.isValid() && CXRefIsTokenType(token1, STRING)) {
        CXRefAddTokenChild(token1, token);

        token1 = CXRefReadToken();
      }

      if (token1.isValid())
        CXRefRestackToken(token1);
    }
    else if (CXRefIsToken(token1, OPERATOR, CXRefLParenStrId)) {
      CXRefTokenP token2 = CXRefReadExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefReadToken();

        if (CXRefIsToken(token3, OPERATOR, CXRefRParenStrId)) {
          token = CXRefCreateToken(PRIMARY_EXPRESSION);

          CXRefAddTokenChild(token1, token);
          CXRefAddTokenChild(token2, token);
          CXRefAddTokenChild(token3, token);
        }
        else {
          CXRefAddError("Missing Close Round Bracket for Primary Expression");

          if (required)
            CXRefError(CXRefGetLastError());

          CXRefRestackToken(token3);
          CXRefRestackToken(token2);
          CXRefRestackToken(token1);
        }
      }
      else
        CXRefRestackToken(token1);
    }
    else {
      CXRefAddError("Invalid Primary Expression");

      if (required)
        CXRefError(CXRefGetLastError());

      CXRefRestackToken(token1);
    }
  }

  return token;
}

/*
 * argument-expression-list:
 *  assignment-expression
 *  argument-expression-list ',' assignment-expression
 */

CXRefTokenP
CXRefReadArgumentExpressionList(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadArgumentExpressionList", token);

  token = CXRefReadTokenOfType(ARGUMENT_EXPRESSION_LIST);

  if (! token.isValid()) {
    CXRefTokenP token1 = CXRefReadAssignmentExpression(required);

    if (token1.isValid()) {
      token = CXRefCreateToken(ARGUMENT_EXPRESSION_LIST);

      CXRefAddTokenChild(token1, token);
    }
  }

  if (token.isValid()) {
    CXRefTokenP token1 = CXRefReadToken();

    while (CXRefIsToken(token1, SEPARATOR, CXRefCommaStrId)) {
      CXRefTokenP token2 = CXRefReadAssignmentExpression(required);

      if (token2.isValid()) {
        CXRefTokenP token3 = CXRefCreateToken(ARGUMENT_EXPRESSION_LIST);

        CXRefAddTokenChild(token , token3);
        CXRefAddTokenChild(token1, token3);
        CXRefAddTokenChild(token2, token3);

        token = token3;
      }
      else
        break;

      token1 = CXRefReadToken();
    }

    CXRefRestackToken(token1);
  }

  return token;
}

CXRefTokenP
CXRefReadComment(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadComment", token);

  token = CXRefReadTokenOfType(COMMENT);

  if (! token.isValid()) {
    CXRefAddError("Invalid Comment");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

CXRefTokenP
CXRefReadIdentifier(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadIdentifier", token);

  token = CXRefReadTokenOfType(IDENTIFIER);

  if (! token.isValid()) {
    CXRefAddError("Invalid Identifier");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

CXRefTokenP
CXRefReadKeyword(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadKeyword", token);

  token = CXRefReadTokenOfType(KEYWORD);

  if (! token.isValid()) {
    CXRefAddError("Invalid Keyword");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

CXRefTokenP
CXRefReadConstant(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadConstant", token);

  token = CXRefReadTokenOfType(CONSTANT);

  if (! token.isValid()) {
    CXRefAddError("Invalid Constant");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

CXRefTokenP
CXRefReadString(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadString", token);

  token = CXRefReadTokenOfType(STRING);

  if (! token.isValid()) {
    CXRefAddError("Invalid String");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

CXRefTokenP
CXRefReadOperator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadOperator", token);

  token = CXRefReadTokenOfType(OPERATOR);

  if (! token.isValid()) {
    CXRefAddError("Invalid Operator");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

CXRefTokenP
CXRefReadSeparator(bool required)
{
  CXRefTokenP token;

  CXREF_TRACE_TOKEN("CXRefReadSeparator", token);

  token = CXRefReadTokenOfType(SEPARATOR);

  if (! token.isValid()) {
    CXRefAddError("Invalid Separator");

    if (required)
      CXRefError(CXRefGetLastError());
  }

  return token;
}

CXRefTokenP
CXRefReadTokenOfType(CXRefTokenType type)
{
  CXRefTokenP token = CXRefReadToken();

  if (token.isValid()) {
    if (token->type == type)
      return token;

    CXRefStackToken(token);
  }

  return CXRefTokenP();
}

bool
CXRefIsLastToken()
{
  CXRefTokenP token = CXRefReadToken();

  if (! token.isValid())
    return true;

  CXRefStackToken(token);

  return false;
}

// storage-class-specifier:
//  'auto', 'extern', 'register', 'static', 'typedef'
bool
CXRefIsStorageClassSpecifier(CXRefTokenP token)
{
  if (token.isValid() && token->type == STORAGE_CLASS_SPECIFIER)
    return true;

  if (token.isValid() && token->type == KEYWORD) {
    if (token->str == CXRefAutoStrId     || token->str == CXRefExternStrId ||
        token->str == CXRefRegisterStrId || token->str == CXRefStaticStrId ||
        token->str == CXRefTypedefStrId)
        return true;
  }

  return false;
}

bool
CXRefIsTypeSpecifier(CXRefTokenP token)
{
  if (token.isValid() && token->type == TYPE_SPECIFIER)
    return true;

  if      (token.isValid() && token->type == KEYWORD) {
    // 'char', 'double', 'enum', 'float', 'int', 'long',
    // 'short', 'signed', 'struct', 'union', 'unsigned', 'void'

    return (token->str == CXRefCharStrId     || token->str == CXRefDoubleStrId ||
            token->str == CXRefEnumStrId     || token->str == CXRefFloatStrId ||
            token->str == CXRefIntStrId      || token->str == CXRefLongStrId ||
            token->str == CXRefShortStrId    || token->str == CXRefSignedStrId ||
            token->str == CXRefStructStrId   || token->str == CXRefUnionStrId ||
            token->str == CXRefUnsignedStrId || token->str == CXRefVoidStrId);
  }
  else if (token.isValid() && token->type == IDENTIFIER) {
    if (CXRefIsTypedefToken(token))
      return true;

/*
    if (cxref_typedef_! token.isValid())
*/
    cxref_typedef_token = token;

/*
    std::cout << cxref_typedef_token->str << " could be a typedef" << std::endl;
*/

    return false;
  }
  else
    return false;
}

// 'const', 'volatile'
bool
CXRefIsTypeQualifier(CXRefTokenP token)
{
  if (token.isValid() && token->type == TYPE_QUALIFIER)
    return true;

  if (token.isValid() && token->type == KEYWORD) {
    return (token->str == CXRefConstStrId || token->str == CXRefVolatileStrId);
  }

  return false;
}

bool
CXRefIsTypedefToken(CXRefTokenP token)
{
  if (! token.isValid() || token->type != IDENTIFIER)
    return false;

  CXRefTypedefP def = CXRefGetTypedefForString(token->str);

  if (! def.isValid())
    return false;

  def->referenced = true;

  def->file_list.push_back(token->file);

  return true;
}

void
CXRefUpdateTypedefList(CXRefTokenP declaration)
{
  bool is_typedef = false;

  CXRefTokenP declaration_specifiers =
    CXRefGetTokenChildOfType(declaration, DECLARATION_SPECIFIERS);

  if (declaration_specifiers.isValid())
    is_typedef = CXRefCheckTypedefDeclarationSpecifiers(declaration_specifiers);

  if (! is_typedef)
    return;

  CXRefTokenP init_declarator_list =
    CXRefGetTokenChildOfType(declaration, INIT_DECLARATOR_LIST);

  if (init_declarator_list.isValid())
    CXRefAddTypedefInitDeclarator(declaration, init_declarator_list);
}

bool
CXRefCheckTypedefDeclarationSpecifiers(CXRefTokenP declaration_specifiers)
{
  std::vector<CXRefTokenP> children;

  bool is_typedef = false;

  CXRefGetBaseTokenChildren(declaration_specifiers, children);

  if (CXRefIsToken(children[0], KEYWORD, CXRefTypedefStrId))
    is_typedef = true;

  return is_typedef;
}

void
CXRefAddTypedefInitDeclarator(CXRefTokenP declaration, CXRefTokenP init_declarator_list)
{
  std::vector<CXRefTokenP> init_declarators;

  CXRefGetAllTokenChildrenOfType(init_declarator_list, INIT_DECLARATOR, init_declarators);

  for (uint i = 0; i < init_declarators.size(); i++) {
    CXRefTokenP declarator = CXRefGetTokenChildOfType(init_declarators[i], DECLARATOR);

    if (! declarator.isValid())
      continue;

    CXRefTokenP direct_declarator = CXRefGetTokenChildOfType(declarator, DIRECT_DECLARATOR);

    if (! direct_declarator.isValid())
      continue;

    std::vector<CXRefTokenP> identifiers;

    CXRefGetAllTokenChildrenOfType(direct_declarator, IDENTIFIER, identifiers);

    if (identifiers.size() > 0)
      CXRefAddDeclarationTypedef(declaration, identifiers[0]->str);
  }
}

void
CXRefAddDeclarationTypedef(CXRefTokenP declaration, CXRefStringId name)
{
  std::vector<CXRefTokenP> children;

  CXRefGetBaseTokenChildren(declaration, children);

  std::vector<CXRefTokenP> children1;

  for (uint i = 0; i < children.size(); i++) {
    if (children[i]->str == CXRefTypedefStrId ||
        children[i]->str == name ||
        children[i]->str == CXRefSemiColonStrId)
      continue;

    children1.push_back(children[i]);
  }

  std::string type = CXRefTokensToString(children1);

  CXRefStringId type_id = CXRefStringToId(type);

  CXRefAddTypedef(declaration, name, type_id);
}

CXRefTypedef *
CXRefAddTypedef(CXRefTokenP token, CXRefStringId name, CXRefStringId type)
{
  if (CXRefTrace::isEnabled())
    std::cout << "Typedef : " << CXRefIdToString(name) << std::endl;

  CXRefTypedef *def = new CXRefTypedef;

  def->name       = name;
  def->type       = type;
  def->referenced = false;

  cxref_string_typedef_map[name] = def;

  if (token.isValid())
    def->file_list.push_back(token->file);

  return def;
}

void
CXRefRemoveTypedef(const std::string &name)
{
  CXRefStringId id = CXRefStringToId(name);

  CXRefTypedefP def = CXRefGetTypedefForString(id);

  if (! def.isValid())
    return;

  cxref_string_typedef_map.erase(id);
}

bool
CXRefIsTypedef(const std::string &name)
{
  CXRefStringId id = CXRefStringToId(name);

  CXRefTypedefP def = CXRefGetTypedefForString(id);

  return def.isValid();
}

std::string
CXRefGetTypedefType(const std::string &name)
{
  CXRefStringId id = CXRefStringToId(name);

  CXRefTypedefP def = CXRefGetTypedefForString(id);

  if (def.isValid())
    return CXRefIdToString(def->type);
  else
    return "";
}

CXRefTypedefP
CXRefGetTypedefForString(CXRefStringId name)
{
  StringTypedefMap::iterator p = cxref_string_typedef_map.find(name);

  if (p == cxref_string_typedef_map.end())
    return CXRefTypedefP();

  return (*p).second;
}

void
CXRefPrintTypedefs(const std::string &filename)
{
  CXRefStringId filename_id = CXRefStringToId(filename);

  StringTypedefMap::iterator p1 = cxref_string_typedef_map.begin();
  StringTypedefMap::iterator p2 = cxref_string_typedef_map.end  ();

  for ( ; p1 != p2; ++p1) {
    CXRefTypedefP def = (*p1).second;

    if (! def.isValid())
      continue;

/*
    if (! def->referenced)
      continue;
*/

    if (filename_id != CXRefEmptyStrId) {
      int num_files = def->file_list.size();

      int j = 0;

      for ( ; j < num_files; ++j) {
        CXRefStringId typedef_file = def->file_list[j];

        if (filename_id != typedef_file)
          break;
      }

      if (j <= num_files)
        continue;
    }

    std::cout << CXRefIdToString(def->name) << std::endl;
  }
}

CXRefTokenP
CXRefRemoveTypedefFromDeclarationSpecifiers(CXRefTokenP declaration_specifiers)
{
  std::vector<CXRefTokenP> children;

  CXRefGetTokenChildren(declaration_specifiers, children);

  CXRefTokenP parent;

  while (children.size() == 2) {
    parent = children[1];

    CXRefGetTokenChildren(children[1], children);
  }

  if (children[0]->type != TYPE_SPECIFIER)
    return CXRefTokenP();

  CXRefGetTokenChildren(children[0], children);

  if (children[0]->type != TYPEDEF_NAME)
    return CXRefTokenP();

  CXRefGetTokenChildren(children[0], children);

  if (parent.isValid())
    parent->parent->child_list.remove(parent);

  return children[0];
}

void
CXRefRestackToken(CXRefTokenP token)
{
  if (! token.isValid())
    return;

  if (token->str != CXRefEmptyStrId) {
    CXRefStackToken(token);
    return;
  }

  std::vector<CXRefTokenP> children;

  CXRefGetBaseTokenChildren(token, children);

  for (int i = children.size() - 1; i >= 0; i--)
    CXRefStackToken(children[i]);
}

bool
CXRefIsValidTypeTokens(CXRefTokenP token)
{
  int num_tokens = cxref_type_tokens.size();

  if (num_tokens == 0)
    return true;

  uint num_signs      = 0;
  uint num_std_types  = 0;
  uint num_user_types = 0;

  for (int i = 0; i < num_tokens; ++i) {
    CXRefTokenP token1 = cxref_type_tokens[i];

    if (token1->type == TYPE_SPECIFIER) {
      CXRefTokenP token2 = CXRefGetTokenChildOfType(token1, KEYWORD);

      if (token2.isValid())
        token1 = token2;
    }

    if (token1->type == KEYWORD) {
      if (token1->str == CXRefSignedStrId ||
          token1->str == CXRefUnsignedStrId)
        ++num_signs;
      else
        ++num_std_types;
    }
    else {
      ++num_user_types;
    }
  }

  if (token->type == TYPE_SPECIFIER) {
    CXRefTokenP token1 = CXRefGetTokenChildOfType(token, KEYWORD);

    if (token1.isValid())
      token = token1;
  }

  if (token->type == KEYWORD) {
    if (token->str == CXRefSignedStrId ||
        token->str == CXRefUnsignedStrId)
      ++num_signs;
    else
      ++num_std_types;
  }
  else {
    ++num_user_types;
  }

  if (num_user_types > 0) {
    if (num_std_types > 0)
      goto fail;

    if (num_signs > 0)
      goto fail;
  }

  return true;

 fail:
  return false;
}

void
CXRefSkipBracedTokens(CXRefTokenP token)
{
  uint depth = 1;

  CXRefTokenP token1 = CXRefReadToken();

  while (token1.isValid() && depth > 0) {
    if      (CXRefIsToken(token1, SEPARATOR, CXRefLBraceStrId))
      ++depth;
    else if (CXRefIsToken(token1, SEPARATOR, CXRefRBraceStrId))
      --depth;

    CXRefAddTokenChild(token1, token);

    token1 = CXRefReadToken();
  }

  if (token1.isValid())
    CXRefRestackToken(token1);

  if (depth != 0)
    CXRefAddError("Missing Closing Brace for Skip Braced Tokens");
}
