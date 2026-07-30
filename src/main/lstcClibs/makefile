deps:
	mkdir -p bin
	gcc -c fileManip.s -o bin/filem.o
	gcc -c exit.s -o bin/exit.o
	gcc -c mmap.s -o bin/mmap.o
	gcc -c start.s -o bin/start.o

make: deps
	gcc bin/filem.o bin/exit.o bin/mmap.o bin/start.o LSTC_pager.c test.c -o bin/test.out -nostdlib -fno-stack-protector

clear:
	rm -r bin
