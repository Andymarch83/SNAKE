#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


typedef struct coord { 
    int x;
    int y;
    int value;       
    struct coord *next; /
} object; 

typedef struct head {
    object* obj;
    char* name;
    int tail_x;
    int tail_y; 
    int lastDirection;
    int up;
    int down;
    int left;
    int right;
} snake;


void enqueue(object **pl, char data, int y, int x)
{
    object *ptmp = *pl;
    object *elem;
    elem = malloc(sizeof(object));
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

int right(snake* snakeName, object* screen)
{
    object *pl = snakeName->obj;
    
    if (pl)
    {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->x++;
        if(checkDirection(snakeName, screen)){
            pl->x--;
            return 1;
        }
        
        if (pl->x >= screen->x)
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
    return 0;
}

int left(snake* snakeName, object* screen)
{
    object *pl = snakeName->obj;  
    
    if (pl)
    {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->x--;
        if(checkDirection(snakeName, screen)){
            pl->x++;
            return 1;
        }
        
        if (pl->x <= 0)
            pl->x = screen->x; 
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
    return 0;
}

int up(snake* snakeName, object* screen)
{
    object *pl = snakeName->obj;
    if (pl) {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->y--;
        if(checkDirection(snakeName, screen)){
            pl->y++;
            return 1;
        }
        if (pl->y < 0) 
            pl->y = screen->y; 
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
    return 0;
}

int down(snake* snakeName, object* screen)
{
    object *pl = snakeName->obj;
    
    if (pl)
    {
        int prev_x = pl->x;
        int prev_y = pl->y;
        pl->y++;
        if(checkDirection(snakeName, screen)){
            pl->y--;
            return 1;
        }
        
        if (pl->y > screen->y)
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
    return 0;
}

void setColor(int color) {
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    attroff(COLOR_PAIR(0));
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
    attroff(COLOR_PAIR(4));
    attroff(COLOR_PAIR(5));
    attroff(COLOR_PAIR(6));
    attron(COLOR_PAIR(color));
}

void printScore(snake *snakeName) {
    int score = 0;
    object *current = snakeName->obj;

    
    while (current != NULL && current->next != NULL && current->next->next != NULL && current->next->next->next != NULL) {
        score++;
        current = current->next;
    }
    setColor(0);
    
    mvprintw(LINES / 2 - strlen(snakeName->name), (COLS - 30) / 2, "%s ate %d times", snakeName->name, score);
    refresh();
}

int crawl(snake *snakeName, int ch, object* screen) { // ползать, т.к. move - служебная функция

    if (ch == snakeName->up){
        if(up(snakeName, screen))
            snakeName->lastDirection = snakeName->right;
        else
            snakeName->lastDirection = ch;
    }
    else if (ch == snakeName->down){
        if(down(snakeName, screen))
            snakeName->lastDirection = snakeName->left;
        else
            snakeName->lastDirection = ch;
    }
    else if (ch == snakeName->left){
        if(left(snakeName, screen))
            snakeName->lastDirection = snakeName->up;
        else
            snakeName->lastDirection = ch;
    }
    else if (ch == snakeName->right){
        if(right(snakeName, screen))
            snakeName->lastDirection = snakeName->down;
        else
            snakeName->lastDirection = ch;
    }
    else if (ch == 'P' || ch == 'p' || ch == 'З' || ch == 'з'){
        setColor(0);
        mvprintw(LINES / 2, (COLS - 30) / 2, "Game paused. Press WASD or arrow key to continue...");
        refresh();
        ch = getchar();
    }
    else if (ch == 27){ // выход по Esc
        setColor(0);
        mvprintw(LINES / 2, (COLS - 30) / 2, "You stoped the game!");
        return 1;
    }
    else
        crawl(snakeName, snakeName->lastDirection, screen);
    return 0;
}

void printObject(object *pl, int color)
{
    setColor(color);
    while (pl)
    {
        mvprintw(pl->y, pl->x, "%c", pl->value); 
        pl = pl->next;
    }
}

snake* initSnake(char face, char* name, int y, int x, int up, int down, int left, int right)
{
    object* pq = NULL;
    enqueue(&pq, face, y, x);
    enqueue(&pq, '*', y, x + 1);
    enqueue(&pq, '*', y, x + 2);
    snake *unit = malloc(sizeof(snake));
    unit->obj = pq;
    unit->name = name;
    unit->tail_x = x + 2;
    unit->tail_y = y;
    unit->lastDirection = up;
    unit->up = up;
    unit->down = down;
    unit->left = left;
    unit->right = right;
    return unit;
};

object* initFood(int level, object* screen, int startFood)
{
    object* pq = NULL;
    int counter = startFood-level;
    srand(time(NULL));
    do{
        enqueue(&pq, 'O', (rand() % screen->y-4)+2, (rand() % screen->x-4)+2);
        counter--;
    }
    while((counter) >= 0);
    enqueue(&pq, 'O', (rand() % screen->y-8)+2, (rand() % screen->x-8)+2);
    return pq;
};

int checkDirection(snake *snakeName, object* screen)
{
    object *pl = snakeName->obj; 
    char head_value = pl->value;
    int head_x = pl->x;
    int head_y = pl->y;
    pl = pl->next; 

    
    while (pl)
    {
        
        if (head_x == pl->x && head_y == pl->y)
        {
            return 2175678;
        }
        pl = pl->next;
    }
    return 0;
}

int checkFood(snake* snakeName, object *feeld) {
    int foodEaten = 0; 
    object *head = snakeName->obj;
    if (head != NULL && feeld != NULL) {
        int head_x = head->x;
        int head_y = head->y;
        object *current = feeld;
        object *prev = NULL;
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
                
                enqueue(&head, '*', snakeName->tail_y, snakeName->tail_x);
            } else {
                prev = current; 
                current = current->next; 
            }
        }
    }

    return foodEaten; 
}

object* initScreen(){
    initscr(); 
    noecho(); 
    curs_set(FALSE); 
    nodelay(stdscr, TRUE); 
    keypad(stdscr, TRUE); 
    scrollok(stdscr, TRUE);
    object* screen = NULL; 
    screen = malloc(sizeof(object));
    getmaxyx(stdscr, screen->y, screen->x); 
    clear(); 
    refresh(); 
    return screen;
}

int startHelp(object *screen) {
    clear();
    setColor(0);
    char* menu_items[] = {
        "Краткая справка по игре змейка",
        "",
        "При запуске игры стартует Меню, в котором с помощью нажатия цифр от 0 до 9 можно выбрать нужный пункт:",
        "0. Данная цифра используется для возврата в меню, для последующего выбора нужного пункта", 
        "1. Вывод текущего экрана с краткой справкой. Более детальную информацию можно получить из пункта меню № 8",
        "2. Режим одиночной игры, в данном режиме игрок управляет стрелками клавиатуры одной змейкой", 
        "Задача как можно больше съесть, уменьшающиеся с каждым уровнем, запасы еды - обозначены желтой литерой O", 
        "3. Режим игры человека против человека, с помощью одной клавиатуры, первый игрок управляет стрелками клавиатуры",
        "второй игрок управляет клавишами W - верх, S - низ, A - лево, D - право",
        "4. Режим игры человека против компьютера. Человек управляет стрелками клавиатуры, компьютер с помощью ИИ",
        "5. Режим борьбы клонов: две змейки с ИИ сражаются в нелегкой схватке за ресурсы",
        "6. Задать цвет Анаконды из предложенного списка нужно с помощью цифры выбрать цвет",
        "7. Задать цвет Кобре",
        "8. Открыть файл README.md для чтения, файл должен находиться в одной директории с программой",
        "9. Выход из игры",
        "",
        "ESC - используется для выхода из справки или для выхода из текущего раунда игры",
        "P постановка игры на паузу, для снятия с паузы нужно нажать на управляющие клавиши WASD или стрелки"
    };
    
    int num_items = sizeof(menu_items) / sizeof(menu_items[0]);
    int start_y = 1; 
    int start_x = 1; 
    for (int i = 0; i < num_items; i++) {
        mvprintw(start_y + i, start_x, menu_items[i]);
    }
    refresh();
    int choose = getchar();
    return choose;
}

int startMenu(object *screen) {
    clear();
    setColor(0);
    char* menu_items[] = {
        "Welcome to the snake game!",
        "",
        "Enter digit 0 - 9 to choose menu item:",
        "0. Show or return to this menu", 
        "1. HELP and short description of the game",
        "2. Start single player round", 
        "3. Start round with two players",
        "4. Start round with computer vs player",
        "5. Start round with computer vs computer",
        "6. Set Anaconda color",
        "7. Set Cobra color",
        "8. Open help: README.md",
        "9. Exit the game",
        "",
        "Use ESC to exit help, or stop round",
        "Use P to pause round, then press WASD to continue",
    };
    
    int num_items = sizeof(menu_items) / sizeof(menu_items[0]);
    int start_y = (screen->y - num_items) / 2; 
    int start_x = (screen->x) / 3; 
    for (int i = 0; i < num_items; i++) {
        mvprintw(start_y + i, start_x, menu_items[i]);
    }
    refresh();
    int choose = getchar();
    return choose;
}

int ai(snake* snakeName, object* field, object* screen){
    int last = snakeName->lastDirection;
    int ch = last;
    int right = snakeName->right;
    int left = snakeName->left;
    int up = snakeName->up;
    int down = snakeName->down;
    int food_x = 0;
    int food_y = 0;
    float distance = 1000;
    object *head = snakeName->obj;
    if (head != NULL && field != NULL) {
        int head_x = head->x;
        int head_y = head->y;
        object *current = field;
        object *prev = NULL;
        while (current != NULL) { 
            if(current->value == 'O' && distance > sqrt(pow(head_x - current->x, 2) + pow(head_y - current->y, 2))){
                distance = sqrt(pow(head_x - current->x, 2) + pow(head_y - current->y, 2));
                food_x = current->x;
                food_y = current->y;
            }
            prev = current;
            current = current->next;
        }

        
        float min_distance = 1000;
        int next_move = ch;
        if (head_x > food_x && head_y > food_y) {
            if (up != 0) return up;
            else if (left != 0) return left;
        }
        else if (head_x < food_x && head_y > food_y) {
            if (up != 0) return up;
            else if (right != 0) return right;
        }
        else if (head_x < food_x && head_y < food_y) {
            if (down != 0) return down;
            else if (right != 0) return right;
        }
        else if (head_x > food_x && head_y < food_y) {
            if (down != 0) return down;
            else if (left != 0) return left;
        }
        else if (head_x == food_x && head_y > food_y) {
            if (up != 0) return up;
        }
        else if (head_x == food_x && head_y < food_y) {
            if (down != 0) return down;
        }
        else if (head_x > food_x && head_y == food_y) {
            if (left != 0) return left;
        }
        else if (head_x < food_x && head_y == food_y) {
            if (right != 0) return right;
        }
    }
   return ch; 
}


int main()
{
    setlocale(LC_ALL, "");
    object *screen = initScreen(); 
    int anacondaColor=3;
    int cobraColor=2; 
    int choose = '0';
    startMenu:
    int faults = 0, level = 1, startFood = 10, delay = 200000;
    setColor(0);
    switch (choose) {
        case '0': {
            choose = startMenu(screen);
            break;
        }
        case '1': {
            choose = startHelp(screen);
            break;
        }
        case '2': {
            snake *anaconda = initSnake('@', "Anaconda", 10, 10, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT); 
            object *field = initFood(level, screen, startFood);
            while (faults == 0) {
                printObject(field, 4);
                int ch = getch(); 
                faults += crawl(anaconda, ch, screen); 
                clear(); 
                printObject(anaconda->obj, anacondaColor); 
                    level++;
                    setColor(0);
                    mvprintw(0, 0, "Level: %d", level);
                    refresh();
                    sleep(2);
                    field = initFood(level, screen, startFood);
                }
                usleep(delay/level); 
            }
            printScore(anaconda); 
            mvprintw(screen->y/2, (COLS - 30) / 2, "<Press 0 for Menu or 9 to Exit>");
            refresh();
            choose = getchar();
            free(anaconda);
            free(field);
            break;   
        }
        case '3': {
            snake *anaconda = initSnake('@', "Anaconda", 5, 5, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT); 
            snake *cobra = initSnake('%', "Cobra", 20, 20, 'w', 's', 'a', 'd'); 
            object *field = initFood(level, screen, startFood);
            while (faults == 0) {
                printObject(field, 4);
                int ch = getch(); 
                faults += crawl(anaconda, ch, screen); 
                faults += crawl(cobra, ch, screen); 
                clear(); // Очистка экрана
                printObject(anaconda->obj, anacondaColor); 
                printObject(cobra->obj, cobraColor); 
                if(checkFood(anaconda, field) || checkFood(cobra, field)){ 
                    level++;
                    setColor(0);
                    mvprintw(0, 0, "Level: %d", level);
                    refresh();
                    sleep(2);
                    field = initFood(level, screen, startFood);
                }
                usleep(delay/level); 
            }
            printScore(anaconda);
            printScore(cobra); 
            mvprintw(screen->y/2, (COLS - 30) / 2, "<Press 0 for Menu or 9 to Exit>");
            refresh();
            choose = getchar();
            free(anaconda);
            free(cobra);
            free(field);
            break;
        }
        case '4': {
            snake *anaconda = initSnake('@', "Anaconda", 5, 5, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT); 
            snake *cobra = initSnake('%', "Cobra", 20, 20, 'w', 's', 'a', 'd'); 
            object *field = initFood(level, screen, startFood);
            while (faults == 0) {
                printObject(field, 4);
                int ch = getch(); 
                faults += crawl(anaconda, ch, screen); 
                int ch2 = ai(cobra, field, screen);
                faults += crawl(cobra, ch2, screen); 
                clear(); // Очистка экрана
                printObject(anaconda->obj, anacondaColor); 
                printObject(cobra->obj, cobraColor); 
                if(checkFood(anaconda, field) || checkFood(cobra, field)){ 
                    level++;
                    setColor(0);
                    mvprintw(0, 0, "Level: %d", level);
                    refresh();
                    sleep(2);
                    field = initFood(level, screen, startFood);
                }
                usleep(delay/level); 
            }
            printScore(anaconda);
            printScore(cobra); 
            mvprintw(screen->y/2, (COLS - 30) / 2, "<Press 0 for Menu or 9 to Exit>");
            refresh();
            choose = getchar();
            free(anaconda);
            free(cobra);
            free(field);
            break;
        }
        case '5': {
            snake *anaconda = initSnake('@', "Anaconda", 5, 5, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT); 
            snake *cobra = initSnake('%', "Cobra", 37, 130, 'w', 's', 'a', 'd'); 
            object *field = initFood(level, screen, startFood);
            while (faults == 0) {
                printObject(field, 4);
                int ch = getch(); 
                if(ch == 27) 
                    break;
                int ch1 = ai(anaconda, field, screen);
                faults += crawl(anaconda, ch1, screen); 
                int ch2 = ai(cobra, field, screen);
                faults += crawl(cobra, ch2, screen); 
                clear(); // Очистка экрана
                printObject(anaconda->obj, anacondaColor); 
                printObject(cobra->obj, cobraColor); 
                if(checkFood(anaconda, field) || checkFood(cobra, field)){ 
                    level++;
                    setColor(0);
                    mvprintw(0, 0, "Level: %d", level);
                    refresh();
                    sleep(2);
                    field = initFood(level, screen, startFood);
                }
                usleep(delay/level); 
            }
            printScore(anaconda);
            printScore(cobra); 
            mvprintw(screen->y/2, (COLS - 30) / 2, "<Press 0 for Menu or 9 to Exit>");
            refresh();
            choose = getchar();
            free(anaconda);
            free(cobra);
            free(field);
            break;
        }
        case '6': {
            clear();
            mvprintw(screen->y/2, screen->x/3, "Select digit for Anaconda color:");
            mvprintw(screen->y/2+1, screen->x/3, "0. White");
            mvprintw(screen->y/2+2, screen->x/3, "1. Red");
            mvprintw(screen->y/2+3, screen->x/3, "2. Blue");
            mvprintw(screen->y/2+4, screen->x/3, "3. Green");
            mvprintw(screen->y/2+5, screen->x/3, "4. Yellow");
            mvprintw(screen->y/2+6, screen->x/3, "5. Magenta");
            mvprintw(screen->y/2+7, screen->x/3, "6. CYAN");
            mvprintw(screen->y/2+8, screen->x/3, "Current color: %d", anacondaColor);
            refresh();
            anacondaColor = getchar() - '0';
            clear();
            mvprintw(screen->y/2, screen->x/3, "You selected Anaconda color: %d", anacondaColor);
            mvprintw(screen->y/2+1, screen->x/3, "<Press 0 for Menu or 9 to Exit>");
            refresh();
            choose = getchar();
            break;
        }
        case '7': {
            clear();
            mvprintw(screen->y/2, screen->x/3, "Select digit for Cobra color:");
            mvprintw(screen->y/2+1, screen->x/3, "0. White");
            mvprintw(screen->y/2+2, screen->x/3, "1. Red");
            mvprintw(screen->y/2+3, screen->x/3, "2. Blue");
            mvprintw(screen->y/2+4, screen->x/3, "3. Green");
            mvprintw(screen->y/2+5, screen->x/3, "4. Yellow");
            mvprintw(screen->y/2+6, screen->x/3, "5. Magenta");
            mvprintw(screen->y/2+7, screen->x/3, "6. CYAN");
            mvprintw(screen->y/2+8, screen->x/3, "Current color: %d", cobraColor);
            refresh();
            cobraColor = getchar() - '0';
            clear();
            mvprintw(screen->y/2, screen->x/3, "You selected Cobra color: %d", cobraColor);
            mvprintw(screen->y/2+1, screen->x/3, "<Press 0 for Menu or 9 to Exit>");
            refresh();
            choose = getchar();
            break;
        }

        case '8': {
            FILE *file;
            char *buffer = NULL; 
            size_t bufsize = 0;
            ssize_t bytesRead;
            file = fopen("README.md", "r");
            if (file == NULL) {
                printw("Failed to open the file!\n");
                break;
            }
            clear();
            int row = 0;
            int screenHeight = screen->y;
            int screenWidth = screen->x;
            while ((bytesRead = getline(&buffer, &bufsize, file)) != -1) {
                // Выводим строку на экран
                mvprintw(row, 0, "%s", buffer);
                refresh();
                row++;
                
                if (row >= screenHeight) {
                    printw("\n\nPress any key to continue...");
                    refresh();
                    getchar();
                    clear();
                    row = 0;
                }
            }
            fclose(file);
            
            if (buffer) {
                free(buffer);
            }
            
            printw("\n\n<Press 0 for Menu or 9 to Exit>");
            refresh();
            choose = getchar();
            break;
        }

        case '9': {
            goto end;
            break;
        }
        default:
            choose = '0';
            break;
    }
    goto startMenu;
    end:
    clear();
    setColor(0);
    mvprintw(screen->y/2, (COLS - 30) / 2, "See you next time!");
    refresh();
    sleep(3);
    endwin(); 
    printf("\033[2J\033[H");
    return 0;
}
