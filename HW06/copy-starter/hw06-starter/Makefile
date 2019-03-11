
CFLAGS := -g -Wall -Werror
SRCS   := $(wildcard *.c)

all: tools ssort

ssort: $(SRCS)
	gcc $(CFLAGS) -o ssort $(SRCS) -lm -lpthread

tools:
	(cd tools && make)

test:
	@make clean
	@make all
	@tools/gen-input 50000000 data.dat
	/usr/bin/time -p ./ssort 8 data.dat
	@echo
	@(tools/check-sorted data.dat && echo "Data Sorted OK") || echo "Fail"
	@echo
	@rm -f data.dat

clean:
	(cd tools && make clean)
	rm -f ssort data.dat *.plist valgrind.out

.PHONY: clean all tools

