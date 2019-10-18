simp: simp.l simp.y ast.h
	bison -d simp.y
	flex -o simp.lex.c simp.l
	cc -o $@ simp.tab.c simp.lex.c ast.c

.PHONY: clean

clean:
	@rm -f simp.tab.h simp.tab.c simp.lex.c simp