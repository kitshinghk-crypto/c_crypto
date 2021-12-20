TARGET = des_test

des_test: des.o des_test.c
	gcc -o $@ $^

des.o: des.c 
	gcc -c $^