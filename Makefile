ecurve_test: arithmetic.o ecurve.o ecurve_test.c
	gcc -g -o $@ $^

arith_test: arithmetic.o arith_test.c
	gcc -o $@ $^

ccmaes_test: ccmaes.o aes.o ccmaes_test.c
	gcc -o $@ $^

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

ccmaes.o: ccmaes.c
	gcc -c $^

arithmetic.o: arithmetic.c
	gcc -c $^

ecurve.o: ecurve.c
	gcc -c -g $^