.PHONY: build build-dbg exec clean run

build:
	gcc -Wall -Wextra -o xxdc main.c

build-dbg:
	gcc -g -Wall -Wextra -o xxdc-dbg main.c

exec:
	./xxdc test.c

clean:
	@rm -rf ./xxdc

run: build exec clean
