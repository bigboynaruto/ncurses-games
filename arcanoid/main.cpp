#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct coord {
	int x, y;

	coord(int _x, int _y): x(_x), y(_y) {}
	bool operator==(coord &c) {
		return (c.x == x && y == c.y);
	}
};
vector<coord> balls;
vector<coord> m;
int num_score = 0, num_balls = 0;

void draw_border(WINDOW*);
void shot(coord);
void refresh_balls(WINDOW*);
void refresh_monsters(WINDOW*);
void add_monster(coord);

int main() {
	srand(time(NULL));
	int d = 20;
	initscr();
	noecho();
	WINDOW* w = newwin(d, 2 * d, 0, 0);
	WINDOW* score = newwin(5, 2 * d, d - 1, 0);
	curs_set(FALSE);
	nodelay(w, TRUE);
    cbreak();
    keypad(w, TRUE);

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	attrset(1);

	draw_border(w);
	wrefresh(w);

	int m_x = 3, m_y = 4;
	for (int i = 0; i < 5; i++) {
		coord c(m_x, m_y);
		m_x += 3;
		m.push_back(c);
	}

	wtimeout(w, 30);
	int d_x = 10;
	while (true) {
		int width, height;
		getmaxyx(w, height, width);

		int ch = wgetch(w);
		if (ch == 'q') break;
		switch (ch) {
			case KEY_LEFT: if (d_x > 2) d_x -= 1; break;
			case KEY_RIGHT: if (d_x < width - 3) d_x += 1; else d_x = width - 3; break;
			case KEY_UP: coord c(d_x, height - 3); shot(c); break;
		}

		wclear(w);
		if ((int)m.size() < 6 * (2 * d - 3)) {
			coord c(rand() % (2 * d - 3) + 2, rand() % 6 + 1);
			add_monster(c);
		}
		refresh_monsters(w);
		refresh_balls(w);


		wmove(w, height - 2, d_x - 1);
		wprintw(w, "I-I");
		draw_border(w);
		draw_border(score);
		wmove(score, 1, 1);
		wprintw(score, "SCORE: %d", num_score);
		wmove(score, 2, 1);
		double div = num_balls == 0 ? 100 : (double)(num_score) / num_balls;
		wprintw(score, "ACCURACY: %.2f%", div);
		wrefresh(w);
	}

	wgetch(w);

	delwin(w);
	endwin();
}

void draw_border(WINDOW* w) {
	int x, y;
	getmaxyx(w, y, x);

	mvwaddstr(w, 0, 0, "+");
	mvwaddstr(w, 0, x - 1, "+");
	mvwaddstr(w, y - 1, 0, "+");
	mvwaddstr(w, y - 1, x - 1, "+");
	for (int i = 1; i < x - 1; i++) {
		mvwaddstr(w, y - 1, i, "-");
		mvwaddstr(w, 0, i, "-");
	}
	for (int i = 1; i < y - 1; i++) {
		mvwaddstr(w, i, x - 1, "|");
		mvwaddstr(w, i, 0, "|");
	}

	wrefresh(w);
}


void shot(coord c) {
	num_balls++;
	balls.push_back(c);
}

void refresh_balls(WINDOW* w) {
	int size = balls.size();
	start_color();
	init_pair(3, COLOR_RED, COLOR_BLACK);

	wattron(w, COLOR_PAIR(3));
	for (int i = 0; i < size; i++) {
		balls[i].y--;
		if (balls[i].y < 1) {
			balls.erase(balls.begin() + i--);
			size--;
		}
		else {
			wmove(w, balls[i].y, balls[i].x);
			wprintw(w, "o");
		}
	}
	wattroff(w, COLOR_PAIR(3));
}

void refresh_monsters(WINDOW* w) {
	int size = m.size();
	int _size = balls.size();
	start_color();
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	wattron(w, COLOR_PAIR(2));
	for (int i = 0; i < size; i++) {
		int j;
		for (j = 0; j < _size; j++) {
			if (balls[j] == m[i]) {
				m.erase(m.begin() + i);
				size--;
				i--;
				balls.erase(balls.begin() + j);
				_size--;
				j--;
				num_score++;
				break;
			}
		}

		if (j >= _size) {
			wmove(w, m[i].y, m[i].x);
			wprintw(w, "#");
		}
	}
	wattroff(w, COLOR_PAIR(2));
}

void add_monster(coord c) {
	int i;
	int size = m.size();
	for (i = 0; i < size; i++) {
		if (c == m[i]) break;
	}
	if (i >= size) m.push_back(c);
}
