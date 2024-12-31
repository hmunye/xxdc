.PHONY: build exec clean run

build:
	gcc -o xxdc main.c
exec:
	./xxdc
clean:
	@rm -rf ./xxdc

run: build exec clean
