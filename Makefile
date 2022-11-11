CC = g++
CFLAGS = -g --std=c++14 -Wall -Werror -pedantic
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

all: sokoban lint

sokoban: sokoban.o
	$(CC) $(CFLAGS) sokoban.o -o sokoban $(LIB)

sokoban.o: sokoban.cpp
	$(CC) $(CFLAGS) -c sokoban.cpp

lint:
	cpplint --filter=-whitespace/line_length,-runtime/references,-build/c++11 --root=. *

clean:
	rm *.o sokoban