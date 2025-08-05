valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./tetris 2>&1 | tee valgrind-out.txt
