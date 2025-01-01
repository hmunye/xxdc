#include <stdio.h>
#include <stdlib.h>

// argc => argument count
// *argv[] => argument vector (array of char * pointers)
int main(int argc, char *argv[]) {
    if (argc != 2) {
        // argv[0] is always the name (or path) of the program being executed
        fprintf(stderr, "USAGE: %s <filename>\n", argv[0]);
        exit(1);
    }

    // `fopen()` opens the file whose name is the string pointed to
    // by path and associates a stream with it. Opening with `r` (read) mode.
    FILE *file = fopen(argv[1], "r");  // Return a FILE pointer, otherwise NULL

    // `file` will have a value of 0x0 if the file could not be found
    if (file == NULL) {
        fprintf(stderr, "ERROR: failed to open file: %s\n", argv[1]);
        exit(1);
    }

    printf("file address: %p\n", file);
}
