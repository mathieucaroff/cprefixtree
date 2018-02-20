.PHONY: compile clean test

CC=gcc -Wall -ggdb

compile: tmp bin test

clean:
	rm tmp/*.o
	rmdir tmp

test: bin/test

# See GNU make automatic variables for explainations about $^ and $@
# $^ (names of prerequisite files)
# $@ name of the target file (file to be created)
bin/test: tmp/test.o tmp/lib.prefixtree.o
	$(CC) $^ -o $@

tmp/%.o: source/%.c
	$(CC) $^ -c -o $@

bin:
	mkdir bin

tmp:
	mkdir tmp


