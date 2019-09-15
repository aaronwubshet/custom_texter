/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/***** data ***/

struct termios org_termios;

/*** terminal ***/

void die(const char *s){
    perror(s);
    exit(1);
}
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios) == -1){
        die("tscsetter");
    };
}
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &org_termios) == -1) die("tcgettr");
    tcgetattr(STDIN_FILENO, &org_termios);
    atexit(disableRawMode);

    struct termios raw = org_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

}

/*** init ***/
int main() {
    enableRawMode();
    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) ==-1 && errno!= EAGAIN) die("read");
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else{
            printf("%d : %c \r\n", c, c);
        }
        if (c=='q') break;
    }
    return 0;
    
}