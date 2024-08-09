#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

bool is_game_over;

// OUTPUT
//////////////////////////////////////////////////////////////
#define OUTPUT_BUFFER_H 9
#define OUTPUT_BUFFER_W 20
char output_buffer[OUTPUT_BUFFER_H][OUTPUT_BUFFER_W];

// obufcls == output buffer clear
void obufcls( void )
{
    for (int i = 0; i < OUTPUT_BUFFER_H; i++)
        for (int j = 0; j < OUTPUT_BUFFER_W; j++)
            output_buffer[i][j] = ' ';
}

// obufdisplay == output buffer display
void obufdisplay( void )
{
    for (int i = 0; i < OUTPUT_BUFFER_H; i++)
    {
        for (int j = 0; j < OUTPUT_BUFFER_W; j++)
            putchar(output_buffer[i][j]);
        putchar('\n');
    }
}
////////////////////////////////////////////////////////////

// Map
//////////////////////////////////////////////////////////////////////////
// CONDITION: MAP_H && MAP_W MUST BE LESS THAN OUTPUT_BUFFER_SIZE
// H == HEIGHT; W == WIDTH
#define MAP_H 9
#define MAP_W 20
#define WALL '#'
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
    "####################",
};

void map_setup( void )
{
    assert(strlen(map[0]) == MAP_W && "map width must be equal MAP_W");
    assert(( sizeof(map) / sizeof(*map)) == MAP_H && "map height must be equal MAP_H");
}

void draw_map( void )
{
    for (int i = 0; i < MAP_H; i++)
        for (int j = 0; j < MAP_W; j++)
            output_buffer[i][j] = map[i][j];
}

//////////////////////////////////////////////////////////////////////

typedef struct {int x, y;} coords_t;
// Fruit
//////////////////////////////////////////////////////////////////
#define FRUIT 'f'
coords_t fruit;
bool is_ate = true;

void generate_new_fruit( void )
{
    fruit.x = 1 + rand() % (MAP_W - 2);
    fruit.y = 1 + rand() % (MAP_H - 2);
    is_ate = false;
}

void draw_fruit( void )
{
    output_buffer[fruit.y][fruit.x] = FRUIT;
}
////////////////////////////////////////////////////////////////

// snake
//////////////////////////////////////////////////////////////////////

#define SNAKE 'O'

enum DIRECTION {UP, DOWN, LEFT, RIGHT};

struct snake_node
{
    coords_t val;
    struct snake_node *next;
};

struct snake_node snake;
struct snake_node *last_node = &snake;
enum DIRECTION snake_dir;

void snake_setup( void )
{
    snake.val.x = 5;
    snake.val.y = 5;
    snake.next = NULL;
    snake_dir = UP;
}

// Посылает цепочку сообщений о новой координате каждого узла
void send_crd(struct snake_node *recipient, coords_t new_crd)
{
    if (recipient)
    {
        if (recipient->next)
            send_crd(recipient->next, recipient->val);
        recipient->val = new_crd;
    }
    else fprintf(stderr, "send_crd error: recipient ptr has NULL");
}

// Смена координат змейки (Движение)
void move ( void )
{
    if (snake.next)
        send_crd(snake.next, snake.val);

    switch(snake_dir)
    {
        case UP: 
            snake.val.y--;
            break;
        case DOWN: 
            snake.val.y++;
            break;
        case LEFT: 
            snake.val.x--;
            break;
        case RIGHT: 
            snake.val.x++;
            break;
    }

    if (map[snake.val.y][snake.val.x] == WALL)
        is_game_over = true;
    
    if (snake.val.x == fruit.x && snake.val.y == fruit.y)
        is_ate = true;

    // Проверка на столкновение с самим собой
    if (snake.next)
        for(struct snake_node *i = snake.next ; !is_game_over && i->next; i = i->next)
            if (i->val.x == snake.val.x && i->val.y == snake.val.y)
                is_game_over = true;
}

struct snake_node * create_snake_node( void )
{
    struct snake_node *new_node = (struct snake_node*)malloc( sizeof(struct snake_node) );
    new_node->next = NULL;
    new_node->val = last_node->val;
    return new_node;
}

// Приципляет один узел змейки.
void link_snake_node( struct snake_node *node )
{
    last_node = last_node->next = node;
}

void free_snake_node( struct snake_node *node )
{
    if (node->next) free_snake_node(node->next);
    free(node);
}

void draw_snake(void)
{
    struct snake_node *i = &snake;

    do
    {
        output_buffer[i->val.y][i->val.x] = SNAKE;
        if (i->next) i = i->next;
    } while (i->next);
    output_buffer[i->val.y][i->val.x] = SNAKE;
}
//////////////////////////////////////////////////////////////////


// INPUT (KEYBOARD)
////////////////////////////////////////////////////////////////////

void input( void )
{
    if (_kbhit())
        switch (_getch())
        {
            case 'w':
                if (snake_dir != DOWN)
                    snake_dir = UP;
                break;
            case 'a':
                if (snake_dir != RIGHT)
                    snake_dir = LEFT;
                break;
            case 's':
                if (snake_dir != UP)
                    snake_dir = DOWN;
                break;
            case 'd':
                if (snake_dir != LEFT)
                    snake_dir = RIGHT;
                break;
        }
}
////////////////////////////////////////////////////////////////

void setup( void )
{
    map_setup();
    snake_setup();
    generate_new_fruit();
    is_game_over = false;
}

void draw(void)
{
    system("cls");
    obufcls();
    draw_map();
    if (!is_ate)
        draw_fruit();
    draw_snake();
    obufdisplay();
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    struct snake_node * new_node = NULL;
    int fruit_counter = 0;

    setup();
    while(!is_game_over)
    {
        if (is_ate)
        {
            generate_new_fruit();
            new_node = create_snake_node();
        }
        input();
        move();
        if (new_node)
        {
            fruit_counter++;
            link_snake_node(new_node);
            new_node = NULL;
        }
        draw();
        printf("fruits counter = %d\n", fruit_counter);
        Sleep(250);
    }
    getc(stdin);
    return 0;
}
