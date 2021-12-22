sha256_test: sha256.o sha256_test.c
	gcc -o $@ $^

des_test: des.o des_test.c
	gcc -o $@ $^

aes_test: aes.o aes_test.c
	gcc -o $@ $^

des.o: des.c 
	gcc -c $^

aes.o: aes.c
	gcc -c $^

sha256.o: sha256.c
	gcc -c $^