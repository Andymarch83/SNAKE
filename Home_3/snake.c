#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

int max_y, max_x, tail_x, tail_y, lastDirection = 'w';
typedef struct tail { 
    int x;
    int y;
    char value;       
    struct tail *next; 
} snake; 
typedef struct meal{
    int x;
    int y;
    char value;
    struct meal *next;
} food;
void enqueue(struct tail **pl, char data, int y, int x)
{
    struct tail *ptmp = *pl;
    struct tail *elem;
    elem = malloc(sizeof(struct tail));
    elem->x = x;
    elem->y = y;
    elem->value = data;
    elem->next = NULL;
    if (*pl == NULL)
    {
        *pl = elem;
        return;
    }
    while (ptmp->next)
        ptmp = ptmp->next;
    ptmp->next = elem;
}

void right(struct tail *pl)
{
    if (pl)
    {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->x++;
        if (pl->x >= max_x-1)
            pl->x = 0; 
        pl = pl->next;
        while (pl)
        {
            int temp_x = pl->x;
            int temp_y = pl->y;
            pl->x = prev_x;
            pl->y = prev_y;
            prev_x = temp_x;
            prev_y = temp_y;
            pl = pl->next;
        }
    }
}

void left(struct tail *pl)
{
    if (pl)
    {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->x--;
        if (pl->x <= 0)
            pl->x = max_x - 1; 
        pl = pl->next;
        while (pl)
        {
            int temp_x = pl->x;
            int temp_y = pl->y;
            pl->x = prev_x;
            pl->y = prev_y;
            prev_x = temp_x;
            prev_y = temp_y;
            pl = pl->next;
        }
    }
}

void up(struct tail *pl)
{
    if (pl) {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->y--;
        if (pl->y < 1) 
            pl->y = max_y-1;
        pl = pl->next;
        while (pl){
            int temp_x = pl->x;
            int temp_y = pl->y;
            pl->x = prev_x;
            pl->y = prev_y;
            prev_x = temp_x;
            prev_y = temp_y;
            pl = pl->next;
        }
    }
}
void down(struct tail *pl)
{
    if (pl)
    {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->y++;
        if (pl->y > max_y-1)
            pl->y = 1;
        pl = pl->next;
        while (pl)
        {
            int temp_x = pl->x;
            int temp_y = pl->y;
            pl->x = prev_x;
            pl->y = prev_y;
            prev_x = temp_x;
            prev_y = temp_y;
            pl = pl->next;
        }
    }
}

void printExit(struct tail *snake) {
    int score = 0;
    struct tail *current = snake;
    while (current != NULL && current->next != NULL && current->next->next != NULL && current->next->next->next != NULL) {
        score++;
        current = current->next;
    }
    mvprintw(LINES / 2, (COLS - 30) / 2, "Your score is %d, Goodbye!", score);
    refresh();
}

void crawl(struct tail *pl, int ch) { 
    if (ch == 'w' || ch == 'W'){
        up(pl);
        lastDirection = ch;
    }
    else if (ch == 's' || ch == 'S'){
        down(pl); 
        lastDirection = ch;
    }
    else if (ch == 'a' || ch == 'A'){
        left(pl);
        lastDirection = ch;
    }
    else if (ch == 'd' || ch == 'D'){
        right(pl);
        lastDirection = ch;
    }
    else if (ch == 'P' || ch == 'p'){
        lastDirection = ch;
        mvprintw(LINES / 2, (COLS - 30) / 2, "PAUSE");
        refresh();
    }
    else if (ch == 27) {
        refresh();
        printExit(pl);
        sleep(2);
        clear;
        endwin();
        exit(0);
    }
    else
        ch = lastDirection;
}
void printStruct(struct tail *pl)
{
    while (pl)
    {
        mvprintw(pl->y, pl->x, "%c", pl->value); 
        tail_x = pl->x;
        tail_y = pl->y;
        pl = pl->next;
    }
}
struct tail *initSnake()
{
    struct tail *pq = NULL;
    enqueue(&pq, '@', max_y/2, max_x/2);
    enqueue(&pq, '*', max_y/2, max_x/2 + 1);
    enqueue(&pq, '*', max_y/2, max_x/2 + 2);
    return pq;
};
struct meal *initFood(int level)
{
    struct meal *pq = NULL;
    enqueue(&pq, 'O', max_y-level*3, max_x-level*3);
    return pq;
};
void checkDirection(struct tail *head)
{
    struct tail *pl = head;
    char head_value = pl->value;
    int head_x = pl->x;
    int head_y = pl->y;
    pl = pl->next;
    while (pl)
    {
        if (head_x == pl->x && head_y == pl->y)
        {
            pl->value = 'X';
            clear();
            printStruct(head);
            refresh();
            sleep(2);
            mvprintw(max_y/2, max_x/2, "Game over!");
            refresh();
            clear();
            sleep(2);
            endwin();
            exit(0);
        }
        pl = pl->next;
    }
}
int checkFood(struct tail *snake, struct meal *feeld) {
    int foodEaten = 0;
    if (anaconda != NULL && feeld != NULL) {
        struct tail *head = snake;
        int head_x = head->x;
        int head_y = head->y;
        struct meal *current = feeld;
        struct meal *prev = NULL;
        while (current != NULL) {
            if (head_x == current->x && head_y == current->y) {
                foodEaten = 1;
                if (prev != NULL) {
                    prev->next = current->next;
                    free(current); 
                    current = prev->next; 
                } else {
                    feeld = current->next;
                    free(current);
                    current = feeld;
                }
                enqueue(&snake, '*', tail_y, tail_x);
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
    return foodEaten; 
}
void initScreen(){
    initscr();
    noecho();
    curs_set(FALSE); 
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, max_y, max_x);
    clear(); 
    refresh();
}
void printLevel(struct tail *snake) {
    int level = 0;
    struct tail *current = snake;
    while (current != NULL && current->next != NULL && current->next->next != NULL) {
        level++;
        current = current->next;
    }
    mvprintw(0, 0, "Level: %d", level);
    refresh();
}
int main()
{
    int delay = 200000;
    int level = 1;
    initScreen(); 
    struct tail *snake = initSnake(); 
    struct meal *field = initFood(level);
    while (1) {
        printStruct(field);
        int ch = getch(); 
        if(ch == ERR)
            ch = lastDirection;
        crawl(snake, ch); 
        clear(); 
        checkDirection(snake);
        printStruct(snake); 
        if(checkFood(snake, field)){ 
            level++;
            printLevel(snake);
            sleep(2);
            field = initFood(level);
        }
        usleep(delay/level); 
    }
    endwin();
    return 0;
}
