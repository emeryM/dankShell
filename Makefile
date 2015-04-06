all: hello

hello:   dankLex.l dankYacc.y
	rm -f *o dankShell y.tab.h y.tab.c lex.yy.c
	flex dankLex.l
	bison -d dankYacc.y
	gcc lex.yy.c y.tab.c main.c -o dankShell

clean:
	rm -f *o dankShell y.tab.h y.tab.c lex.yy.c

push:
	rm -f *o dankShell y.tab.h y.tab.c lex.yy.c
	git add .
	git commit -a -m "Pushed from Make File"
	git pull origin master
	git push origin master
	
cpush:
	rm -f *o dankShell y.tab.h y.tab.c lex.yy.c
	git add .
	git commit -a -m "$(c)"
	git pull origin master
	git push origin master

start: dankLex.l dankYacc.y
	rm -f *o dankShell y.tab.h y.tab.c lex.yy.c
	flex dankLex.l
	bison -d dankYacc.y
	gcc lex.yy.c y.tab.c main.c -o dankShell
	./dankShell