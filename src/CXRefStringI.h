#define CXRefUndefStrId ((CXRefStringId) 0)
#define CXRefEmptyStrId ((CXRefStringId) 1)

extern CXRefStringId CXRefAutoStrId;
extern CXRefStringId CXRefBreakStrId;
extern CXRefStringId CXRefCaseStrId;
extern CXRefStringId CXRefCharStrId;
extern CXRefStringId CXRefConstStrId;
extern CXRefStringId CXRefContinueStrId;
extern CXRefStringId CXRefDefaultStrId;
extern CXRefStringId CXRefDoStrId;
extern CXRefStringId CXRefDoubleStrId;
extern CXRefStringId CXRefElseStrId;
extern CXRefStringId CXRefEnumStrId;
extern CXRefStringId CXRefExternStrId;
extern CXRefStringId CXRefFloatStrId;
extern CXRefStringId CXRefForStrId;
extern CXRefStringId CXRefGotoStrId;
extern CXRefStringId CXRefIfStrId;
extern CXRefStringId CXRefIntStrId;
extern CXRefStringId CXRefLongStrId;
extern CXRefStringId CXRefRegisterStrId;
extern CXRefStringId CXRefReturnStrId;
extern CXRefStringId CXRefShortStrId;
extern CXRefStringId CXRefSignedStrId;
extern CXRefStringId CXRefSizeofStrId;
extern CXRefStringId CXRefStaticStrId;
extern CXRefStringId CXRefStructStrId;
extern CXRefStringId CXRefSwitchStrId;
extern CXRefStringId CXRefTypedefStrId;
extern CXRefStringId CXRefUnionStrId;
extern CXRefStringId CXRefUnsignedStrId;
extern CXRefStringId CXRefVoidStrId;
extern CXRefStringId CXRefVolatileStrId;
extern CXRefStringId CXRefWhileStrId;

extern CXRefStringId CXRefSemiColonStrId;
extern CXRefStringId CXRefColonStrId;
extern CXRefStringId CXRefCommaStrId;
extern CXRefStringId CXRefEqualStrId;
extern CXRefStringId CXRefLParenStrId;
extern CXRefStringId CXRefRParenStrId;
extern CXRefStringId CXRefLBracketStrId;
extern CXRefStringId CXRefRBracketStrId;
extern CXRefStringId CXRefLBraceStrId;
extern CXRefStringId CXRefRBraceStrId;
extern CXRefStringId CXRefPlusStrId;
extern CXRefStringId CXRefMinusStrId;
extern CXRefStringId CXRefTimesStrId;
extern CXRefStringId CXRefDivideStrId;
extern CXRefStringId CXRefEllipsisStrId;
extern CXRefStringId CXRefTimesEqualStrId;
extern CXRefStringId CXRefDivideEqualStrId;
extern CXRefStringId CXRefModulusEqualStrId;
extern CXRefStringId CXRefPlusEqualStrId;
extern CXRefStringId CXRefMinusEqualStrId;
extern CXRefStringId CXRefLShiftEqualStrId;
extern CXRefStringId CXRefRShiftEqualStrId;
extern CXRefStringId CXRefAndEqualStrId;
extern CXRefStringId CXRefXorEqualStrId;
extern CXRefStringId CXRefOrEqualStrId;
extern CXRefStringId CXRefQuestionStrId;
extern CXRefStringId CXRefLOrStrId;
extern CXRefStringId CXRefLAndStrId;
extern CXRefStringId CXRefOrStrId;
extern CXRefStringId CXRefXorStrId;
extern CXRefStringId CXRefAndStrId;
extern CXRefStringId CXRefEqualsStrId;
extern CXRefStringId CXRefNotEqualsStrId;
extern CXRefStringId CXRefLessStrId;
extern CXRefStringId CXRefGreaterStrId;
extern CXRefStringId CXRefLessEqualStrId;
extern CXRefStringId CXRefGreaterEqualStrId;
extern CXRefStringId CXRefLShiftStrId;
extern CXRefStringId CXRefRShiftStrId;
extern CXRefStringId CXRefModulusStrId;
extern CXRefStringId CXRefIncrementStrId;
extern CXRefStringId CXRefDecrementStrId;
extern CXRefStringId CXRefLXorStrId;
extern CXRefStringId CXRefLNotStrId;
extern CXRefStringId CXRefPeriodStrId;
extern CXRefStringId CXRefReferenceStrId;

extern CXRefStringId CXRefPointerStrId;
extern CXRefStringId CXRefAddressStrId;

extern CXRefStringId CXRefPreProDefinedStrId;
extern CXRefStringId CXRefPreProIfStrId;
extern CXRefStringId CXRefPreProIfdefStrId;
extern CXRefStringId CXRefPreProIfndefStrId;
extern CXRefStringId CXRefPreProErrorStrId;

void                CXRefInitStringIds
                     ();
const std::string  &CXRefIdToString
                     (CXRefStringId id);
const char         *CXRefIdToCStr
                     (CXRefStringId id);
CXRefStringId       CXRefStringToId
                      (const std::string &str);
bool                CXRefIsKeywordString
                     (CXRefStringId str);
