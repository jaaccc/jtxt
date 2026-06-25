#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios original;

void die(const char *s) {
    perror(s);
    exit(1);
}

void disableRaw(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1)
        die("tcsetattr");
}

void enableRaw(void) {
    if (tcgetattr(STDIN_FILENO, &original) == -1)
        die("tcgetattr");
    atexit(disableRaw);

    struct termios raw = original;

    raw.c_cflag |= (CS8);
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_oflag &= ~(OPOST);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int main(void) {
    enableRaw();

    while (1) {
        char c = '\0';

        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
            die("read");
        if (iscntrl(c))
            printf("%d\r\n", c);
        else
            printf("%d ('%c')\r\n", c, c);
        if (c == 'q')
            break;
    }

    return 0;
}
