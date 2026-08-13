deps:
	mkdir -p bin
	gcc -c fileManip.s -o bin/filem.o
	gcc -c exit.s      -o bin/exit.o
	gcc -c mmap.s      -o bin/mmap.o
	gcc -c start.s     -o bin/start.o
	gcc -c lenArgs.s   -o bin/variadic.o
	ld -r bin/filem.o bin/exit.o bin/mmap.o bin/variadic.o -o bin/LSTC_Clibs.o


comp: deps
	gcc -g LSTC_pager.c test.c bin/start.o bin/LSTC_Clibs.o -o bin/test.out --static  -nostdlib -fno-stack-protector
clear:
	rm -r bin
run:
	./bin/test.out
