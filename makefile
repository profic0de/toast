SRC = $(filter-out $(shell find . -wholename './ignore/*.c'),$(shell find . -name '*.c'))
OBJ := $(SRC:%=./build/%.o)
ARGS = -I. -Isrc -Wall -Wno-deprecated-declarations -Wno-nonnull -lcurl -lm
OUT = proxy.o

run: $(OUT)
	@echo --------------------
	@./$(OUT)

$(OUT): $(OBJ)
	gcc -g -fsanitize=address $(ARGS) $(OBJ) -o $(OUT)

./build/%.c.o: %.c
	@mkdir -p $(dir $@)
	gcc -g -fsanitize=address $(ARGS) -c $< -o $@

.PHONY: run, prod, clean

prod:
	gcc $(ARGS) -o $(OUT) $(SRC)
	@echo --------------------
	@./$(OUT)

clean:
	@rm -rf $(OUT)
	@rm -rf ./build/
