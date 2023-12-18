.PHONY: build run clean deps

SRC_DIR = src
BUILD_DIR = .out
OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/internes.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/prompt.o $(BUILD_DIR)/jobs.o $(BUILD_DIR)/shell.o

build: $(OBJ)
	gcc -g -Wall -o jsh $(OBJ) -lreadline -lm

run: build
	./jsh

clean:
	rm -f $(BUILD_DIR)/*.o jsh

deps:
	gcc -MM src/*.c

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	-mkdir -p $(BUILD_DIR)
	gcc -g -Wall -o $@ -c $<

