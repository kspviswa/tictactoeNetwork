# 
# MAKE FILE - To Compile the TicTacToe Network
#
#

GNU_COMPILER = g++

SRC = client.cpp \
      processMsg.cpp \
      registerMsg.cpp 

INC = client.h common.h processMsg.h

COMPLIER_FLAGS = -Wall -g 

OUT_EXE = game

build: $(SRC)
	$(GNU_COMPILER) $(COMPLIER_FLAGS) -o $(OUT_EXE) $(SRC)

clean:
	rm -rf game game *.o
