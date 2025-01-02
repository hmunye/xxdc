#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTES_PER_LINE 16

// argc => argument count
// *argv[] => argument vector (array of char * pointers)
int main(int argc, char *argv[]) {
    if (argc != 2) {
        // argv[0] is always the name of the executable
        fprintf(stderr, "USAGE: %s <filename>\n", argv[0]);
        exit(1);
    }

    // `fopen()` opens the file whose name is the string pointed to
    // by path and associates a stream with it. Opening with `r` (read) mode.
    //
    // returns a FILE pointer, otherwise NULL
    FILE *file = fopen(argv[1], "r");

    // `file` will have a memory address of 0x0 if the file could not be found
    if (file == NULL) {
        fprintf(stderr, "ERROR: failed to open file %s: %s\n", argv[1],
                // used to convert an error code stored in the global `errno`
                // variable into a human-readable error message
                strerror(errno));
        exit(1);
    }

    char buffer[BYTES_PER_LINE];
    size_t bytes_read = 0;

    int byte_offset = 0;

    // acquire an exclusive lock on file
    // avoid the overhead of locking the object for each operation
    flockfile(file);

    // using fread() to read BYTES_PER_LINE chunks at a time from file
    while ((bytes_read = fread(buffer, sizeof(char), BYTES_PER_LINE, file)) >
           0) {
        printf("%08x: ", byte_offset);

        for (size_t i = 0; i < bytes_read; i++) {
            if (i % 2 == 0) {
                printf(" ");
            }

            printf("%02x", (unsigned char)buffer[i]);
        }

        printf("\n");

        byte_offset += bytes_read;
    }

    // fread() does not distinguish between end-of-file and error;
    // must use feof() and ferror() to determine which occurred
    if (ferror(file)) {
        fprintf(stderr, "ERROR: failed to read file %s: %s\n", argv[1],
                strerror(errno));
        exit(1);
    }

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
