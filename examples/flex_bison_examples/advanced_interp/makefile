adv: advanc.l advanc.y advanc.h
	bison -d advanc.y
	flex -o advanc.lex.c advanc.l
	gcc -o $@ advanc.tab.c advanc.lex.c advanc.c -lm 

.PHONY: clean

clean:
	@rm -f advanc.tab.h advanc.tab.c advanc.lex.c adv