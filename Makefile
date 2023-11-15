.PHONY: build run clean deps

SRC_DIR = src
BUILD_DIR = .out
OBJ = main.o parser.o

build: $(OBJ)
	gcc -g -Wall -o jsh $(OBJ:%=$(BUILD_DIR)/%) -lreadline

run: build
	./jsh

clean:
	rm -f $(BUILD_DIR)/*.o jsh

deps:
	gcc -MM src/*.c

%.o: $(SRC_DIR)/%.c
	-mkdir -p $(BUILD_DIR)
	gcc -g -Wall -o $(BUILD_DIR)/$@ -c $<

