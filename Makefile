gameoflife: gameoflife.c
	$(CC) -Ofast -o gameoflife gameoflife.c -lncurses -ltinfo

run: gameoflife
	./gameoflife

clean:
	rm -f gameoflife
