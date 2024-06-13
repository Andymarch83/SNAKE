# Домашнее задание №5


## Задание 1. Цвет змеек


Добавить цвет для двух змеек и еды.

Понадобится функция:

void setColor(int objectType){

attroff(COLOR_PAIR(1));

attroff(COLOR_PAIR(2));

attroff(COLOR_PAIR(3));

switch (objectType){

case 1:{ // SNAKE1

attron(COLOR_PAIR(1));

break;

}

case 2:{ // SNAKE2

attron(COLOR_PAIR(2));

break;

}

case 3:{ // FOOD

attron(COLOR_PAIR(3));

break;

}

}

}

В теле main() цвета инициализируем:

start_color();

init_pair(1, COLOR_RED,

COLOR_BLACK);

init_pair(2, COLOR_BLUE,

COLOR_BLACK);

init_pair(3, COLOR_GREEN,

COLOR_BLACK);


## Задание 2. Стартовое меню

Добавить стартовое меню (приветствие, выбор режима, выбор цвета змейки и
т.д.).

Написать функцию void startMenu().


# *Задание 3. Исполняемый файл

Попробовать внести изменения в исполняемый файл, чтобы в стандартном

режиме змейка не погибала при самопересечении.

Для этого необходимо найти вызов функции isCrush() и поменять вызов на
нужное нам возвращаемое значение 0.
