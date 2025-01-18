CC := gcc
DBG_BIN := lldb
#CFLAGS = -D_GNU_SOURCE
CFLAGS += -std=c11
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
# CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS += -I./libs/
ASANFLAGS=-fsanitize=address -fno-common -fno-omit-frame-pointer
CFLAGS += $(shell pkg-config --cflags sdl2 glfw3 gl glew)
LDFLAGS := $(shell pkg-config --libs sdl2 glfw3 gl glew) -lm
LIBS := -I./lib
SRC_FILES := $(wildcard ./src/*.c)
BIN_DIR := ./bin
BIN := $(BIN_DIR)/opengl
RES_DIR := ./res

build: bin-dir
	@$(CC) $(CFLAGS) $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

bin-dir:
	@mkdir -p $(BIN_DIR)

res-dir:
	@mkdir -p $(RES_DIR)

debug: debug-build
	$(DBG_BIN) $(BIN) $(ARGS)

debug-build: bin-dir
	$(CC) $(CFLAGS) -g $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

run: build
	@$(BIN) $(ARGS)

memcheck:
	@$(CC) -g $(SRC) $(ASANFLAGS) $(CFLAGS) $(INCS) $(LIBS) $(LFLAGS) -o memcheck.out
	@./memcheck.out
	@echo "Memory check passed"

clean:
	rm -rf $(BIN_DIR)/*

gen-compilation-db:
	bear -- make build

gen-compilation-db-make:
	make --always-make --dry-run \
	| grep -wE 'gcc|g\+\+' \
	| grep -w '\-c' \
	| jq -nR '[inputs|{directory:".", command:., file: match(" [^ ]+$").string[1:]}]' \
	> compile_commands.json
