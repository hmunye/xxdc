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
    char ascii_line[BYTES_PER_LINE];

    // need to initialize to 0
    int ascii_line_count = 0;
    int byte_count = 0;

    // acquire an exclusive lock on file
    // avoid the overhead of locking the object for each operation
    flockfile(file);

    while ((ch = getc_unlocked(file)) != EOF) {
        if (byte_count % BYTES_PER_LINE == 0) {
            if (ascii_line_count > 0) {
                // null-terminate array (string) first
                ascii_line[ascii_line_count - 1] = '\0';

                // print current contents of buffer
                printf(" %s\n", ascii_line);

                // reset the buffer and count
                memset(ascii_line, 0, sizeof(ascii_line));
                ascii_line_count = 0;
            }

            // print offset
            printf("%08x: ", byte_count);
        }

        // prints "XXXX XXXX ..." where each group of "XXXX" contains 2 groups
        // of 2 byte hex values
        if (byte_count % 2 == 0) {
            printf("%02x", ch);
        } else {
            printf("%02x ", ch);
        }

        if (ch >= 32 && ch <= 126) {
            // store the byte in the array
            ascii_line[ascii_line_count] = ch;
        } else {
            // non-printable characters replaced by '.'
            ascii_line[ascii_line_count] = '.';
        }

        ascii_line_count++;
        byte_count++;
    }

    if (ascii_line_count > 0) {
        ascii_line[ascii_line_count - 1] = '\0';

        int padding = (BYTES_PER_LINE - ascii_line_count) * 3 - 2;

        // print the remaining part of buffer with padding
        printf("%*s\n", padding, ascii_line);
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
