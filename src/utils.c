#include "utils.h"
#include <GL/glew.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// get_file_contents reads in the contents of a text file unbuffered, allocates space in
// memory where it puts the contents and expects the caller to free said memory.
char *get_file_contents(const char *filename)
{
    FILE *fd = fopen(filename, "r");
    if (!fd) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    // Seek to end of file
    fseek(fd, 0, SEEK_END);
    // Get the position at the end of the file i.e. the size
    uint64_t fsize = ftell(fd);
    rewind(fd);

    // Allocate memory the same size as the size of the file plus a NULL terminator
    char *buf = (char *)malloc(fsize + 1);
    if (!buf) {
        fprintf(stderr, "Error allocating memory for file contents\n");
        fclose(fd);
        return NULL;
    }

    size_t bytes_read = fread(buf, 1, fsize, fd);
    if (bytes_read != fsize) {
        fprintf(stderr, "Failed to read the entire file\n");
        free(buf);
        fclose(fd);
        return NULL;
    }
    buf[fsize] = '\0';

    fclose(fd);

    return buf;
}

void check_gl_error(char *fn_name)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error after %s: %d\n", fn_name, error);
        exit(1);
    }
}
