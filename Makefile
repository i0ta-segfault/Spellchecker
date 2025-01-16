all : main

main: main.c
	gcc main.c wagner_fischer.c trie.c -o main -lncurses

clean:
	rm -rf main
