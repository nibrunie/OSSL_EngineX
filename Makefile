
all: build/engine_ex.so

build/e_ex.o: src/e_ex.c
	$(CC) -fPIC -o $@ -c $^

build/engine_ex.so: build/e_ex.o
	$(CC) -shared -o $@ -lcrypto $^ 

build: build/engine_ex.so
