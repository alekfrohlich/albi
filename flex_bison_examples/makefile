all:
	flex calc.l
	bison -d calc.y
	gcc calc.tab.c lex.yy.c -lfl -lm -o calc

clean:
	@rm -f calc.tab.* lex.yy.c calc
