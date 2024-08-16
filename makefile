CC = gcc

snake: snake.c snake.h
	$(CC) -o $@ snake.c