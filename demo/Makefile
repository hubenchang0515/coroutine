target: demo1 demo2 demo3

demo1 : demo1.c ../coroutine.c ../coroutine.h
	gcc -o demo1 demo1.c ../coroutine.c -I.. -W -Wall -O2

demo2 : demo2.c ../coroutine.c ../coroutine.h
	gcc -o demo2 demo2.c ../coroutine.c -I.. -W -Wall -O2

demo3 : demo3.c ../coroutine.c ../coroutine.h
	gcc -o demo3 demo3.c ../coroutine.c -I.. -W -Wall -O2

clean:
	rm demo1 demo2 demo3