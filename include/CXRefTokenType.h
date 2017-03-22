#include <CEnum.h>

CENUM_START(CXRefTokenType)

CENUM_VALUE(NO_TOKEN                  , "No Token"                  , NoToken                 , 0)

CENUM_VALUE(DUMMY_LINE                , "Dummy Line"                , DummyLine               , 1)
CENUM_VALUE(PREPROCESSOR_STATEMENT    , "PreProcessor Statement"    , PreprocessorStatement   , 2)
CENUM_VALUE(PREPROCESSOR_DEFINE       , "PreProcessor Define"       , PreprocessorDefine      , 3)
CENUM_VALUE(COMMENT                   , "Comment"                   , Comment                 , 4)
CENUM_VALUE(IDENTIFIER                , "Identifier"                , Identifier              , 5)
CENUM_VALUE(KEYWORD                   , "Keyword"                   , Keyword                 , 6)
CENUM_VALUE(CONSTANT                  , "Constant"                  , Constant                , 7)
CENUM_VALUE(STRING                    , "String"                    , String                  , 8)
CENUM_VALUE(OPERATOR                  , "Operator"                  , Operator                , 9)
CENUM_VALUE(SEPARATOR                 , "Separator"                 , Separator               , 10)

CENUM_VALUE(TRANSLATION_UNIT          , "Translation Unit"          , TranslationUnit         , 100)
CENUM_VALUE(EXTERNAL_DECLARATION      , "External Declaration"      , ExternalDeclaration     , 101)
CENUM_VALUE(FUNCTION_DEFINITION       , "Function Definition"       , FunctionDefinition      , 102)
CENUM_VALUE(DECLARATION               , "Declaration"               , Declaration             , 103)
CENUM_VALUE(DECLARATION_LIST          , "Declaration List"          , DeclarationList         , 104)
CENUM_VALUE(DECLARATION_SPECIFIERS    , "Declaration Specifiers"    , DeclarationSpecifiers   , 105)
CENUM_VALUE(STORAGE_CLASS_SPECIFIER   , "Storage Class Specifier"   , StorageClassSpecifier   , 106)
CENUM_VALUE(TYPE_SPECIFIER            , "Type Specifier"            , TypeSpecifier           , 107)
CENUM_VALUE(TYPE_QUALIFIER            , "Type Qualifier"            , TypeQualifier           , 108)
CENUM_VALUE(STRUCT_OR_UNION_SPECIFIER , "Struct Or Union Specifier ", StructOrUnionSpecifier  , 109)
CENUM_VALUE(STRUCT_OR_UNION           , "Struct Or Union"           , StructOrUnion           , 110)
CENUM_VALUE(STRUCT_DECLARATION_LIST   , "Struct Declaration List"   , StructDeclarationList   , 111)
CENUM_VALUE(INIT_DECLARATOR_LIST      , "Init Declarator List"      , InitDeclaratorList      , 112)
CENUM_VALUE(INIT_DECLARATOR           , "Init Declarator"           , InitDeclarator          , 113)
CENUM_VALUE(STRUCT_DECLARATION        , "Struct Declaration"        , StructDeclaration       , 114)
CENUM_VALUE(SPECIFIER_QUALIFIER_LIST  , "Specifier Qualifier List"  , SpecifierQualifierList  , 115)
CENUM_VALUE(STRUCT_DECLARATOR_LIST    , "Struct Declarator List"    , StructDeclaratorList    , 116)
CENUM_VALUE(STRUCT_DECLARATOR         , "Struct Declarator"         , StructDeclarator        , 117)
CENUM_VALUE(ENUM_SPECIFIER            , "Enum Specifier"            , EnumSpecifier           , 118)
CENUM_VALUE(ENUMERATOR_LIST           , "Enumerator List"           , EnumeratorList          , 119)
CENUM_VALUE(ENUMERATOR                , "Enumerator"                , Enumerator              , 120)
CENUM_VALUE(DECLARATOR                , "Declarator"                , Declarator              , 121)
CENUM_VALUE(DIRECT_DECLARATOR         , "Direct Declarator"         , DirectDeclarator        , 122)
CENUM_VALUE(POINTER                   , "Pointer"                   , Pointer                 , 123)
CENUM_VALUE(TYPE_QUALIFIER_LIST       , "Type Qualifier List"       , TypeQualifierList       , 124)
CENUM_VALUE(PARAMETER_TYPE_LIST       , "Parameter Type List"       , ParameterTypeList       , 125)
CENUM_VALUE(PARAMETER_LIST            , "Parameter List"            , ParameterList           , 126)
CENUM_VALUE(PARAMETER_DECLARATION     , "Parameter Declaration"     , ParameterDeclaration    , 127)
CENUM_VALUE(IDENTIFIER_LIST           , "Identifier List"           , IdentifierList          , 128)
CENUM_VALUE(INITIALIZER               , "Initializer"               , Initializer             , 129)
CENUM_VALUE(INITIALIZER_LIST          , "Initializer List"          , InitializerList         , 130)
CENUM_VALUE(TYPE_NAME                 , "Type Name"                 , TypeName                , 131)
CENUM_VALUE(ABSTRACT_DECLARATOR       , "Abstract Declarator"       , AbstractDeclarator      , 132)
CENUM_VALUE(DIRECT_ABSTRACT_DECLARATOR, "Direct Abstract Declarator", DirectAbstractDeclarator, 133)
CENUM_VALUE(TYPEDEF_NAME              , "Typedef Name"              , TypedefName             , 134)
CENUM_VALUE(STATEMENT                 , "Statement"                 , Statement               , 135)
CENUM_VALUE(LABELED_STATEMENT         , "Labeled Statement"         , LabeledStatement        , 136)
CENUM_VALUE(EXPRESSION_STATEMENT      , "Expression Statement"      , ExpressionStatement     , 137)
CENUM_VALUE(COMPOUND_STATEMENT        , "Compound Statement"        , CompoundStatement       , 138)
CENUM_VALUE(STATEMENT_LIST            , "Statement List"            , StatementList           , 139)
CENUM_VALUE(SELECTION_STATEMENT       , "Selection Statement"       , SelectionStatement      , 140)
CENUM_VALUE(ITERATION_STATEMENT       , "Iteration Statement"       , IterationStatement      , 141)
CENUM_VALUE(JUMP_STATEMENT            , "Jump Statement"            , JumpStatement           , 142)
CENUM_VALUE(EXPRESSION                , "Expression"                , Expression              , 143)
CENUM_VALUE(ASSIGNMENT_EXPRESSION     , "Assignment Expression"     , AssignmentExpression    , 144)
CENUM_VALUE(ASSIGNMENT_OPERATOR       , "Assignment Operator"       , AssignmentOperator      , 145)
CENUM_VALUE(CONDITIONAL_EXPRESSION    , "Conditional Expression"    , ConditionalExpression   , 146)
CENUM_VALUE(CONSTANT_EXPRESSION       , "Constant Expression"       , ConstantExpression      , 147)
CENUM_VALUE(LOGICAL_OR_EXPRESSION     , "Logical Or Expression"     , LogicalOrExpression     , 148)
CENUM_VALUE(LOGICAL_AND_EXPRESSION    , "Logical And Expression"    , LogicalAndExpression    , 149)
CENUM_VALUE(INCLUSIVE_OR_EXPRESSION   , "Inclusive Or Expression"   , InclusiveOrExpression   , 150)
CENUM_VALUE(EXCLUSIVE_OR_EXPRESSION   , "Exclusive Or Expression"   , ExclusiveOrExpression   , 151)
CENUM_VALUE(AND_EXPRESSION            , "And Expression"            , AndExpression           , 152)
CENUM_VALUE(EQUALITY_EXPRESSION       , "Equality Expression"       , EqualityExpression      , 153)
CENUM_VALUE(RELATIONAL_EXPRESSION     , "Relational Expression"     , RelationalExpression    , 154)
CENUM_VALUE(SHIFT_EXPRESSION          , "Shift Expression"          , ShiftExpression         , 155)
CENUM_VALUE(ADDITIVE_EXPRESSION       , "Additive Expression"       , AdditiveExpression      , 156)
CENUM_VALUE(MULTIPLICATIVE_EXPRESSION , "Multiplicative Expression ", MultiplicativeExpression, 157)
CENUM_VALUE(CAST_EXPRESSION           , "Cast Expression"           , CastExpression          , 158)
CENUM_VALUE(UNARY_EXPRESSION          , "Unary Expression"          , UnaryExpression         , 159)
CENUM_VALUE(UNARY_OPERATOR            , "Unary Operator"            , UnaryOperator           , 160)
CENUM_VALUE(POSTFIX_EXPRESSION        , "Postfix Expression"        , PostfixExpression       , 161)
CENUM_VALUE(PRIMARY_EXPRESSION        , "Primary Expression"        , PrimaryExpression       , 162)
CENUM_VALUE(ARGUMENT_EXPRESSION_LIST  , "Argument Expression List"  , ArgumentExpressionList  , 163)

CENUM_END(CXRefTokenType)
