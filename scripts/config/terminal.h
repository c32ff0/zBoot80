#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#define setup_term() \
    int c;   \
    static struct termios oldt, newt; \
\
    /*tcgetattr gets the parameters of the current terminal\
    STDIN_FILENO will tell tcgetattr that it should write the settings\
    of stdin to oldt*/\
    tcgetattr( STDIN_FILENO, &oldt);\
    /*now the settings will be copied*/\
    newt = oldt;\
\
    /*ICANON normally takes care that one line at a time will be processed\
    that means it will return if it sees a \
 or an EOF or an EOL*/\
    newt.c_lflag &= ~(ICANON);          \
\
    /*Those new settings will be set to STDIN\
    TCSANOW tells tcsetattr to change attributes immediately. */\
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);\

#define end_term()\
    /*restore the old settings*/\
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
