SRC_DIR=src
TST_DIR=tst

ASSO_DIR=$(SRC_DIR)/association
SET_DIR=$(SRC_DIR)/set

CC=gcc
CPPFLAGS= -I$(SRC_DIR) -I$(ASSO_DIR) -I$(SET_DIR)
CFLAGS=-Wall -g -O0

all = build

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build : $(ASSO_DIR)/association.o $(SET_DIR)/set.o $(SRC_DIR)/main.o
	$(CC) $^ -o main

test : test_set_func test_set_struc
	valgrind ./test_set_struc
	valgrind ./test_set_func

test_set_struc : $(SET_DIR)/set.o $(TST_DIR)/test_set_func.c
	$(CC) $^ -o test_set_struc

test_set_func : $(SET_DIR)/set.o $(TST_DIR)/test_set_func.c
	$(CC) $^ -o test_set_func

clean :
	rm -rf *.o $(SRC_DIR)/*.o $(SRC_DIR)/*/*.o main test* vgcore*