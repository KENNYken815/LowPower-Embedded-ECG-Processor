CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude

BUILD := build
LIB := $(BUILD)/libecg.a
DEMO := $(BUILD)/ecg_demo
TEST := $(BUILD)/ecg_tests

CORE_SRC := $(wildcard src/*.c)
CORE_OBJ := $(patsubst src/%.c,$(BUILD)/%.o,$(CORE_SRC))

.PHONY: all demo test clean

all: demo test

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB): $(CORE_OBJ)
	ar rcs $@ $^

$(DEMO): examples/ecg_demo.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -lm -o $@

$(TEST): tests/test_ecg.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -lm -o $@

demo: $(DEMO)
	./$(DEMO)

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD)
