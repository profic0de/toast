SRC = $(filter-out $(shell find . -wholename './ignore/*.c'),$(shell find . -name '*.c'))
OBJ := $(SRC:%=./build/%.o)
ARGs = -std=c23 -I. -Isrc -Wall -Wno-misleading-indentation -Wno-deprecated-declarations -Wno-nonnull -Wno-sequence-point -Wno-multichar -Wno-restrict -Wno-unused-value -Wno-switch -Wno-bool-operation
ARGS = -O0 $(ARGs)
# ARGS = -O3 $(ARGs)
OUT = out

PROGRAM_ARGS = ""

run: $(OUT)
	@echo --------------------
	@./$(OUT) $(PROGRAM_ARGS)

$(OUT): $(OBJ)
	gcc -g -fsanitize=address $(ARGS) $(OBJ) -o $(OUT)

./build/%.c.o: %.c
	@mkdir -p $(dir $@)
	gcc -g -fsanitize=address $(ARGS) -c $< -o $@

.PHONY: run, prod, clean, git, make, ai

git:
	@git add .
	@git commit -m "$$(date +%d/%-m/%y)"
	@git push

make: # Used to be called 'test' but I renamed it to 'make' because of muscle memory
	gcc $(ARGS) -o ./ignore/test ./ignore/test.c
	@echo --------------------
	@cd ./ignore && ./test $(PROGRAM_ARGS)

prod:
	gcc $(ARGS) -o $(OUT) $(SRC)
	@echo --------------------
	@./$(OUT) $(PROGRAM_ARGS)

debug:
	gcc $(ARGS) -o $(OUT) -g $(SRC)
	@echo --------------------
	@./$(OUT) $(PROGRAM_ARGS)

clean:
	@rm -rf $(OUT)
	@rm -rf ./build/
