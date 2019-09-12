all:
	flex fcalc.l
	bison -d fcalc.y
	gcc fcalc.tab.c lex.yy.c -lfl -lm -o fcalc

clean:
	@rm -f fcalc.tab.* lex.yy.c fcalc
