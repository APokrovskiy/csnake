CC = gcc

snake: snake.c
	$(CC) -o $@ snake.c