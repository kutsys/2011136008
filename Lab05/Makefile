# mtest use foo / boo / bar

mtest : mtest.o foo.o boo.o bar.o
	gcc -o mtest mtest.o foo.o boo.o bar.o

mtest.o : mtest.c
	gcc -c -o mtest.o mtest.c

foo.o : foo.c
	gcc -c -o foo.o foo.c

bar.o : bar.c
	gcc -c -o bar.o bar.c

boo.o : boo.c
	gcc -c -o boo.o boo.c

# Delete of Dummy Target(*.o)
clean :
	rm *.o mtest
