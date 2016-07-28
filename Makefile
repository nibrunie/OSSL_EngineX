
all: build/engine_ex.so

build/e_ex.o: src/e_ex.c
	$(CC) $(CFLAGS) -fPIC -o $@ -c $^

build/engine_ex.so: build/e_ex.o
	$(CC) $(CFLAGS) -shared -o $@ -lcrypto $^ 

build: build/engine_ex.so

build/basic_digest: tests/basic_digest.c
	$(CC) $(CFLAGS) -o $@ $^ -lcrypto


test: build/basic_digest
	./build/basic_digest

load:
	openssl engine -t -c `pwd`/build/engine_ex.so 

clean: 
	rm build/engine_ex.so build/e_ex.o build/basic_digest

.PHONY: clean load
