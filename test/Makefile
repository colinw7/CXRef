CC = g++
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = .
LIB_DIR = ../lib
BIN_DIR = ../bin

all: $(BIN_DIR)/CXRefPro

SRC = \
CXRefPro.cpp

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++11 \
-I$(INC_DIR) \
-I../../CFile/include \
-I../../CStrUtil/include \
-I../../CGlob/include \
-I../../CUtil/include \
-I.

LFLAGS = \
-L$(LIB_DIR) \
-L../../CXRef/lib \
-L../../CCeil/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CRegExp/lib \
-L../../CStrUtil/lib \
-L../../CGlob/lib \
-L../../CUtil/lib \

LIBS = \
-lCXRef -lCCeil -lCFile -lCOS -lCUtil -lCRegExp -lCStrUtil -lCGlob -ltre

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CXRefPro

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CXRefPro: $(OBJS) $(LIB_DIR)/libCXRef.a
	$(CC) $(LDEBUG) -o $(BIN_DIR)/CXRefPro $(OBJS) $(LFLAGS) $(LIBS)
