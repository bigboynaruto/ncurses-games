#include <ncurses.h>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

struct snake_part {
    int x, y;
};

int main() {
    srand(time(NULL));
    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    clear();
    initscr();

    int vx = 0, vy = -1;

    int r = 20;

    int map[r][r];
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < r; ++j) {
            map[i][j] = 0;
        }
    }

    vector<snake_part> snake;
    int _y = 10;
    for (int i = 0; i < 5; i++) {
        snake_part s = {10, _y};
        map[_y][10] = 1;
        snake.push_back(s);
        _y--;
    }
    int num = snake.size();
    map[snake[num - 1].y][snake[num - 1].x] = 2;

    int a_x = 15, a_y = 15;
    map[a_y][a_x] = 3;
    timeout(300);
    while (true) {
        clear();
        for (int i = 0; i < r; ++i) {
            printw(" ");
            for (int j = 0; j < r; ++j) {
                if (map[i][j] == 0) printw(". ");
                else if (map[i][j] == 1) printw("* ");
                else if (map[i][j] == 2) printw("@ ");
                else printw("# ");
            }
            printw("\n");
        }

        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        }
        else {
            if (ch == KEY_LEFT) {
                vx = -1;
                vy = 0;
            }
            else if (ch == KEY_RIGHT) {
                vx = 1;
                vy = 0;
            }
            else if (ch == KEY_UP) {
                vx = 0;
                vy = -1;
            }
            else if (ch == KEY_DOWN) {
                vx = 0;
                vy = 1;
            }
        }

        if (snake[num - 1].x == a_x && snake[num - 1].y == a_y) {
            num++;
            snake_part s = {(a_x + vx + r) % r, (a_y + vy + r) % r};
            map[a_y][a_x] = 2;
            snake.push_back(s);
            a_x = rand() % r;
            a_y = rand() % r;
            while (map[a_y][a_x] != 0) {
                a_x = rand() % r;
                a_y = rand() % r;
            }
        }

        for (int i = 0; i < r; i++) {
            for (int j = 0; j < r; j++) map[i][j] = 0;
        }

        map[a_y][a_x] = 3;
        map[snake[0].y][snake[0].x] = 0;
        for (int i = 0; i < num - 1; i++) {
            snake[i].x = snake[i + 1].x;
            snake[i].y = snake[i + 1].y;
            map[snake[i].y][snake[i].x] = 1;
        }
        snake[num - 1].x += vx + r;
        snake[num - 1].y += vy + r;
        snake[num - 1].x %= r;
        snake[num - 1].y %= r;
        map[snake[num - 1].y][snake[num - 1].x] = 2;
    }

    getch();

    endwin();
}
