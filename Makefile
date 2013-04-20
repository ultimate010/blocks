all:
	cc -g stocks.c -o  stocks
run:all
	./stocks
clean:
	rm stocks tags
new:
	ctags -R *
