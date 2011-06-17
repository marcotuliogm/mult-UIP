
PROG = sft


all: CLIENTE
	gcc trata_cmd.o sft_.c -o $(PROG)

CLIENTE:
	gcc -c trata_cmd.c

clean:
	rm -rfv *.o sft $(PROG)
