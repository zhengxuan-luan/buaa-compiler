out: scanner

scanner: lex.yy.c
	gcc -o $@ $<

lex.yy.c: scaner.l
	flex $<