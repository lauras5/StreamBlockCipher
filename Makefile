CC=gcc
FILES=cipher.c
OUT_EXE = cipher.out
all: build
build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)
clean:
	rm -f *.o $(OUT_EXE)


