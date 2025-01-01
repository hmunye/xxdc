#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    FILE *file = fopen(argv[1], "r");  // return a FILE pointer, otherwise NULL

    // `file` will have a memory address of 0x0 if the file could not be found
    if (file == NULL) {
        fprintf(stderr, "ERROR: failed to open file %s: %s\n", argv[1],
                // used to convert an error code stored in the global `errno`
                // variable into a human-readable error message
                strerror(errno));
        exit(1);
    }

    char ch;
    // needs to initialize to 0
    int byte_count = 0;

    // acquire an exclusive lock on file
    // avoid the overhead of locking the object for each operation
    flockfile(file);

    while ((ch = getc_unlocked(file)) != EOF) {
        // how many octets per line (bytes per line)
        if (byte_count % 8 == 0) {
            printf("\n%08x: ", byte_count);
        }

        // prints "XXXX XXXX ..." where each group of "XXXX" contains 2 groups
        // of 2 byte hex values
        if (byte_count % 2 == 0) {
            printf("%02x", ch);
        } else {
            printf("%02x ", ch);
        }

        byte_count++;
    }

    printf("\n");

    // release the lock on file
    funlockfile(file);

    // clean up open file stream
    if (fclose(file) == EOF) {  // return 0, otherwise EOF
        fprintf(stderr, "ERROR: failed to close file %s: %s\n", argv[1],
                strerror(errno));
        exit(1);
    }

    return 0;
}
