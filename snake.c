
#include "snake.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    bool is_game_over = false;
    int fruit_counter = 0;

    // Для создания новых звеньев змейки
    struct snake_node *new_node = NULL;

    setup();
    while (!is_game_over)
    {
        if (is_ate)
        {
            generate_new_fruit();
            new_node = create_snake_node();
        }
        input();
        move();
        is_game_over = check_is_collision();
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
    if (snake.next)
        free_snake_node(snake.next);
    getc(stdin);
    return 0;
}

void outbuf_cls(void)
{
    for (int i = 0; i < MAP_H; i++)
        for (int j = 0; j < MAP_W; j++)
            outbuf[i][j] = ' ';
}

void outbuf_draw(void)
{
    for (int i = 0; i < MAP_H; i++)
    {
        for (int j = 0; j < MAP_W; j++)
            putchar(outbuf[i][j]);
        putchar('\n');
    }
}

void is_map_valid(void)
{
    assert((sizeof(map) / sizeof(*map)) == MAP_H && "map height must be equal MAP_H");

    for (int i = 0; i < MAP_H; i++)
        assert(strlen(map[i]) == MAP_W && "map width must be equal MAP_W");
}

void map_draw(void)
{
    for (int i = 0; i < MAP_H; i++)
        for (int j = 0; j < MAP_W; j++)
            outbuf[i][j] = map[i][j];
}

void generate_new_fruit(void)
{
    fruit.x = 1 + rand() % (MAP_W - 2);
    fruit.y = 1 + rand() % (MAP_H - 2);
    is_ate = false;
}

void fruit_draw(void)
{
    outbuf[fruit.y][fruit.x] = FRUIT_CHAR;
}

void snake_setup(void)
{
    snake.val.x = 1;
    snake.val.y = MAP_H / 2;
    snake.next = NULL;
    snake_dir = RIGHT;
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
    else
        fprintf(stderr, "send_crd error: recipient ptr has NULL");
}

void move(void)
{
    if (snake.next)
        send_crd(snake.next, snake.val);

    switch (snake_dir)
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

    if (snake.val.x == fruit.x && snake.val.y == fruit.y)
        is_ate = true;
}

bool check_is_collision(void)
{
    // Проверка на столкновение со стенками
    if (map[snake.val.y][snake.val.x] == WALL_CHAR)
        return true;

    // Проверка на столкновение с самим собой
    if (snake.next)
        for (struct snake_node *i = snake.next; i->next; i = i->next)
            if (i->val.x == snake.val.x && i->val.y == snake.val.y)
                return true;

    return false;
}

struct snake_node *create_snake_node(void)
{
    struct snake_node *new_node = (struct snake_node *)malloc(sizeof(struct snake_node));
    new_node->next = NULL;
    new_node->val = last_node->val;
    return new_node;
}

void link_snake_node(struct snake_node *node)
{
    last_node = last_node->next = node;
}

void free_snake_node(struct snake_node *node)
{
    if (node->next)
        free_snake_node(node->next);
    free(node);
}

void draw_snake(void)
{
    struct snake_node *i = &snake;

    do
    {
        outbuf[i->val.y][i->val.x] = SNAKE_CHAR;
        if (i->next)
            i = i->next;
    } while (i->next);
    outbuf[i->val.y][i->val.x] = SNAKE_CHAR;
}

void input(void)
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

void setup(void)
{
    is_map_valid();
    snake_setup();
    generate_new_fruit();
}

void draw(void)
{
    system("cls");
    outbuf_cls();
    map_draw();
    if (!is_ate)
        fruit_draw();
    draw_snake();
    outbuf_draw();
}
