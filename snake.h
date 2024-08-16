#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// Размеры карты
#define MAP_H 9
#define MAP_W 20
#define WALL_CHAR '#'
#define SNAKE_CHAR 'O'
#define FRUIT_CHAR 'f'

// Типы
//---------------------------------------
typedef struct
{
    int x, y;
} coords_t;

struct snake_node
{
    coords_t val;
    struct snake_node *next;
};

enum DIRECTION
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
//---------------------------------------

// Глобальные переменные
//-----------------------------------------

char outbuf[MAP_H][MAP_W]; // Буфер вывода

const char *map[MAP_H] =
    {
        "####################",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "####################"};

coords_t fruit;     // Фрукт
bool is_ate = true; // Съеден ли

// Змейка
struct snake_node snake;
struct snake_node *last_node = &snake;
enum DIRECTION snake_dir;

//-----------------------------------------

// Функции
//-----------------------------------------

// outbuf
void outbuf_cls(void);
void outbuf_draw(void);

// map
void is_map_valid(void); // assert-функция
void map_draw(void);     // Отрисовка на outbuf

// fruit
void generate_new_fruit(void);
void fruit_draw(void); // Отрисовка на outbuf

// snake
void snake_setup(void);
void move(void);
bool check_is_collision(void);
struct snake_node *create_snake_node(void);
void free_snake_node(struct snake_node *node);
void link_snake_node(struct snake_node *node);
void draw_snake(void);

// Ввод с клавиатуры
void input(void);

// main
void setup(void);
void draw(void);
//-----------------------------------------