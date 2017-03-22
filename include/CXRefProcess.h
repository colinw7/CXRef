struct CXRefFunc;

void               CXRefGetFunctions
                    (std::vector<CXRefFunc *> &functions);
void               CXRefPrintFunctionPrototypes
                    (std::vector<CXRefFunc *> &functions);
void               CXRefPrintFunctionPrototype
                    (CXRefFunc *func);
void               CXRefPrintFunctionHeaders
                    (std::vector<CXRefFunc *> &functions);
void               CXRefPrintFunctionHeader
                    (CXRefFunc *func);
void               CXRefPrintFunctionVars
                    (CXRefFunc *func);
void               CXRefPrintFunctionCalls
                    (CXRefFunc *func);
void               CXRefPrintFunctionCFlow
                    (CXRefFunc *func);
const std::string &CXRefFuncGetName
                    (CXRefFunc *function);
void               CXRefFuncGetPosition
                    (CXRefFunc *function, int *start_line_num,
                     int *start_char_num, int *end_line_num, int *end_char_num);
