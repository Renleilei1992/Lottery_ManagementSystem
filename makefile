target:bin/test
objs:obj/view.o obj/user.o obj/control.o obj/hashTable.o 

obj/view.o:src/view.c
	gcc -c src/view.c -o obj/view.o -g -Wall -I include

obj/user.o:src/user.c
	gcc -c src/user.c -o obj/user.o -g -Wall -I include

obj/control.o:src/control.c
	gcc -c src/control.c -o obj/control.o -g -Wall -I include

obj/hashTable.o:src/hashTable.c
	gcc -c src/hashTable.c -o obj/hashTable.o -g -Wall -I include

bin/test:src/main.c obj/view.o obj/user.o obj/control.o obj/hashTable.o
	gcc src/main.c obj/view.o obj/user.o obj/control.o obj/hashTable.o -o bin/test -g -Wall -I include

