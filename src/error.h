#ifndef ERROR_H_
#define ERROR_H_

enum {
    SUCCESS,
    ERR_ALLOC,
    ERR_OPENING_FILE,
    ERR_SDL_INIT,
    ERR_SDL_CREATE_WIN_RENDER,
    ERR_SDL_LOADING_BMP,
};

extern char err_additional[256];
extern const char *err_messages[];

void err_handle(const int);
int err_fatal(const int, const char *);

#endif // !ERROR_H_
