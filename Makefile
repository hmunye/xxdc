.PHONY: build exec clean run

build:
	gcc -o xxdc main.c
exec:
	./xxdc test.txt
clean:
	@rm -rf ./xxdc

run: build exec clean
