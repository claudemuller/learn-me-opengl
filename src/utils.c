#include "utils.h"
#include <GL/glew.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// get_file_contents reads in the contents of a text file, allocates space in
// memory where it puts the contents and expects the caller to free said memory.
char *get_file_contents(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    // Seek to end of file
    fseek(fp, 0, SEEK_END);
    // Get the position at the end of the file i.e. the size
    uint64_t fsize = ftell(fp);
    rewind(fp);

    // Allocate memory the same size as the size of the file plus a NULL terminator
    char *buf = (char *)malloc(fsize + 1);
    if (!buf) {
        fprintf(stderr, "Error allocating memory for file contents\n");
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buf, 1, fsize, fp);
    if (bytes_read != fsize) {
        fprintf(stderr, "Failed to read the entire file\n");
        free(buf);
        fclose(fp);
        return NULL;
    }
    buf[fsize] = '\0';

    fclose(fp);

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
