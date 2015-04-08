CC=gcc
BINNAME=parse
MAINFILE=parse.c
TIME=`date`
#FLAGS=--static -fdiagnostics-color=always

all:
	@rm -f timestamp.c
	@echo "char DATE[] = \"`date +%Y/%m/%d-%H:%M:%S`\";" > timestamp.c
	$(CC) $(MAINFILE) timestamp.c -o $(BINNAME) ${FLAGS}
	@rm -f timestamp.c

clean:
	@rm ${BINNAME}
