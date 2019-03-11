
all: ctp atp

ctp: transpose.c vec.c
	gcc -g -o ctp transpose.c vec.c

atp: transpose.S vec.S
	gcc -no-pie -g -o atp transpose.S vec.S

cdemo: ctp
	./tools/gen-matrix.pl -o 8 > /tmp/matrix-8.txt
	cat /tmp/matrix-8.txt
	./ctp < /tmp/matrix-8.txt > /tmp/matrix-8t.txt
	cat /tmp/matrix-8t.txt
	./tools/transpose.pl < /tmp/matrix-8.txt > /tmp/matrix-8r.txt
	./tools/equal.pl /tmp/matrix-8t.txt /tmp/matrix-8r.txt

ademo: atp
	./tools/gen-matrix.pl -o 9 > /tmp/matrix-9.txt
	cat /tmp/matrix-9.txt
	./atp < /tmp/matrix-9.txt > /tmp/matrix-9t.txt
	cat /tmp/matrix-9t.txt
	./tools/transpose.pl < /tmp/matrix-9.txt > /tmp/matrix-9r.txt
	./tools/equal.pl /tmp/matrix-9t.txt /tmp/matrix-9r.txt

clean:
	rm -f atp ctp

.PHONY: all clean cdemo ademo
