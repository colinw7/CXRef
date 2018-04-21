CC = g++
AR = ar
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib

all: $(LIB_DIR)/libCXRef.a

SRC = \
CXRefControl.cpp \
CXRefCToken.cpp \
CXRefDebug.cpp \
CXRefDefine.cpp \
CXRefError.cpp \
CXRefGen.cpp \
CXRefInterp.cpp \
CXRefParse.cpp \
CXRefPrePro.cpp \
CXRefProcess.cpp \
CXRefPro.cpp \
CXRefProto.cpp \
CXRefString.cpp \
CXRefToken.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++14 \
-I. -I$(INC_DIR) \
-I../../CCeil/include \
-I../../CUtil/include \
-I../../CFile/include \
-I../../CStrUtil/include \
-I../../CGlob/include \

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCXRef.a

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

.SUFFIXES: .cpp

$(LIB_DIR)/libCXRef.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCXRef.a $(OBJS)
