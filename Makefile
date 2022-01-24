hash_drbg_test: hash_drbg.o sha256_8u.o hash_drbg_test.c
	gcc -g -o $@ $^

rsa_test: arithmetic.o rsa.o sha256_8u.o rsa_test.c
	gcc -g -o $@ $^

ecdh_test: arithmetic.o ecurve.o ecdh.o ecdh_test.c
	gcc -g -o $@ $^

ecdsa_test: sha256_8u.o arithmetic.o ecurve.o ecdsa.o ecdsa_test.c
	gcc -g -o $@ $^

ecurve_test: arithmetic.o ecurve.o ecurve_test.c
	gcc -g -o $@ $^

sha256_8u_test: sha256_8u.o sha256_8u_test.c
	gcc -o $@ $^

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

sha256_8u.o: sha256_8u.c
	gcc -c $^

ccmaes.o: ccmaes.c
	gcc -c $^

arithmetic.o: arithmetic.c
	gcc -c $^

ecurve.o: ecurve.c
	gcc -c -g $^

ecdsa.o: ecdsa.c
	gcc -c -g $^

ecdh.o: ecdh.c
	gcc -c -g $^

rsa.o: rsa.c
	gcc -c -g $^

hash_drbg.o: hash_drbg.c
	gcc -c -g $^