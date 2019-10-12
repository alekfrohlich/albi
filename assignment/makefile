all: albi

albi: albi.l albi.y
	flex -o albi.yy.c albi.l
	bison -d albi.y
	gcc albi.tab.c albi.yy.c -lm -o albi

.PHONY: clean

clean:
	@rm -f albi.tab.* albi.yy.c albi