
all: build/engine_ex.so

build/e_ex.o: src/e_ex.c
	$(CC) $(CFLAGS) -fPIC -o $@ -c $^

build/engine_ex.so: build/e_ex.o
	$(CC) $(CFLAGS) -shared -o $@ -lcrypto $^ 

build: build/engine_ex.so

load:
	openssl engine -t -c `pwd`/build/engine_ex.so 

clean: 
	rm build/engine_ex.so build/e_ex.o

.PHONY: clean load
