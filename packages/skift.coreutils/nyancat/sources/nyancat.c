#include <skift/cstring.h>
#include <skift/iostream.h>
#include <skift/process.h>

#include "nyancat-frames.h"

char * colors[256] = { NULL };

#define MIN_ROW 20
#define MAX_ROW 45

#define MIN_COL 10
#define MAX_COL 50

int main() {
	printf("\033[H\033[2J");

    colors[',']  = "\033[0;34;44m";  /* Blue background */
    colors['.']  = "\033[1;37;47m";  /* White stars */
    colors['\''] = "\033[0;30;40m";  /* Black border */
    colors['@']  = "\033[1;37;47m";  /* Tan poptart */
    colors['$']  = "\033[1;35;45m";  /* Pink poptart */
    colors['-']  = "\033[1;31;41m";  /* Red poptart */
    colors['>']  = "\033[1;31;41m";  /* Red rainbow */
    colors['&']  = "\033[0;33;43m";  /* Orange rainbow */
    colors['+']  = "\033[1;33;43m";  /* Yellow Rainbow */
    colors['#']  = "\033[1;32;42m";  /* Green rainbow */
    colors['=']  = "\033[1;34;44m";  /* Light blue rainbow */
    colors[';']  = "\033[0;34;44m";  /* Dark blue rainbow */
    colors['*']  = "\033[1;30;40m";  /* Gray cat face */
    colors['%']  = "\033[1;35;45m";  /* Pink cheeks */

	int playing = 1;
	size_t i = 0;
	char last = 0;
	size_t y, x;
	while (playing) {
		for (y = MIN_ROW; y < MAX_ROW; ++y) {
			for (x = MIN_COL; x < MAX_COL; ++x) {
				if (frames[i][y][x] != last && colors[(int)frames[i][y][x]]) {
					last = frames[i][y][x];
					printf("%s##", colors[(int)frames[i][y][x]]);
				} else {
					printf("##");
				}
			}
			if (y != MAX_ROW - 1)
				printf("\n");
		}
		++i;
		if (!frames[i]) {
			i = 0;
		}
		printf("\033[H");
		
		iostream_flush(out_stream);

		process_sleep(9);
		// usleep(90000);
	}

	return 0;
}