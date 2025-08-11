NAME=tetris
CC=gcc
SRC=$(shell find . -type f -name '*.c')

LIBS=ncurses
LARGS=-Iutils -Igraphics -Iloops -Icommon -I$(shell pwd)
all:
	make build
build:
	$(CC) -Wall $(LARGS) -Wextra -pedantic -pthread -l$(LIBS) $(SRC) -no-pie -o $(NAME)	
debug:
	$(CC) -Wall $(LARGS) -Wextra -pedantic -pthread -l$(LIBS) $(SRC) -g -no-pie -o $(NAME)	
run:
	make build
	./tetris
bear:
	bear -- $(CC) -Wall $(LARGS) -Wextra -pedantic -pthread -l$(LIBS) $(SRC) -no-pie -o $(NAME)
