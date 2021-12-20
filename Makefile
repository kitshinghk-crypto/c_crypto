TARGET = des_test

des_test: des.o des_test.c
	gcc -o $@ $^

aes_test: aes.o aes_test.c
	gcc -o $@ $^

des.o: des.c 
	gcc -c $^

aes.o: aes.c
	gcc -c $^