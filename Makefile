all: hello

hello:   dankLex.l dankYacc.y
	rm -f *o shell y.tab.h y.tab.c lex.yy.c
	flex dankLex.l
	bison -dy dankYacc.y
	gcc lex.yy.c y.tab.c main.c -o shell

clean:
	rm -f *o shell y.tab.h y.tab.c lex.yy.c

push:
	rm -f *o shell y.tab.h y.tab.c lex.yy.c
	git add .
	git commit -a -m "Pushed from Make File"
	git pull origin master
	git push origin master
	
cpush:
	rm -f *o shell y.tab.h y.tab.c lex.yy.c
	git add .
	git commit -a -m "$(c)"
	git pull origin master
	git push origin master

start: dankLex.l dankYacc.y
	rm -f *o shell y.tab.h y.tab.c lex.yy.c
	flex dankLex.l
	bison -dy dankYacc.y
	gcc lex.yy.c y.tab.c main.c -o shell
	./shell