#include <ncurses.h>
#include <unistd.h>
#define DELAY 90000

int max_y, max_x; 

struct snake_t{
    int x[100];
    int y[100];
    int length;
} snake;


printSnake() {

    for (int i = snake.length; i > 0; i--) {
        snake.x[i] = snake.x[i-1];
        snake.y[i] = snake.y[i-1];
        mvprintw(snake.y[i], snake.x[i], "*");
    }
    mvprintw(snake.y[0], snake.x[0], "@");
    refresh();
}


void growSnake() {
    if (snake.x[0] < 0 || snake.x[0] >= max_x-1 || snake.y[0] < 0 || snake.y[0] >= max_y-1) 
        snake.length++;
}


void checkDirection() {
    for (int i = 4; i < snake.length; i++) {
        for (int j = 4; j < snake.length; j++)
        {
            if (
                snake.x[0] == snake.x[j] && snake.y[0] == snake.y[i]) {
                mvprintw(snake.y[i], snake.x[j], "X");
                refresh();
                sleep(2);
                mvprintw(max_y/2, max_x/2, "GAME OVER!!!");
                refresh();
                clear();
                sleep(2);
                endwin();
                exit(0); 
            }
        
        }
    }
}

moveSnake(last) {
    switch(last) {
    case 'w':
        snake.y[0] = (snake.y[0] - 1 + max_y) % max_y;
        break;
    case 's':
        snake.y[0] = (snake.y[0] + 1) % max_y;
        break;
    case 'a':
        snake.x[0] = (snake.x[0] - 1 + max_x) % max_x;
        break;
    case 'd':
        snake.x[0] = (snake.x[0] + 1) % max_x;
        break;
    default:
        last = last;
        break;
    }
}

int main() {
    initscr(); 
    noecho(); 
    curs_set(FALSE); 
    nodelay(stdscr, TRUE); 
    keypad(stdscr, TRUE); 
    getmaxyx(stdscr, max_y, max_x);
    snake.x[0] = 30;
    snake.y[0] = 15;
    snake.length = 5;

    int ch = 'a'; 
    int last = ch; 
    while (1) {
        clear(); 
        printSnake();
        ch = getch();
        if (ch == 'w' || ch == 'W' )
            last = 'w';
        else if (ch == 's' || ch == 'S' )
            last = 's';
        else if (ch == 'a' || ch == 'A')
            last = 'a';
        else if (ch == 'd' || ch == 'D')
            last = 'd';
        else if (ch == 27) {
            clear;
            endwin();
            printf("\033[2J\033[1;1H");
            printf("Good buy!");
            return 0; }
        else if(ch == ERR)
            last = last;
        else
            last = last;
        moveSnake(last); 
        growSnake();
        checkDirection();
        usleep(DELAY); 
    }
    endwin(); 
    return 0;
}
