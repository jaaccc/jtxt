#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios original;

void disableRaw(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &original); }

void enableRaw(void) {
    tcgetattr(STDIN_FILENO, &original);
    atexit(disableRaw);

    struct termios raw = original;

    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void) {
    enableRaw();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
        ;

    return 0;
}
