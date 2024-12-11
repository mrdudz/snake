
all: snake.prg snake80.prg snake128.prg

snake.prg: main.c
	cl65 -t c64 -o snake.prg main.c
snake80.prg: main.c
	cl65 -DCOL80 -t c64 -o snake80.prg main.c c64-soft80.o
snake128.prg: main.c
	cl65 -DCOL80 -t c128 -o snake128.prg main.c

test: snake.prg
	x64sc -default snake.prg
test80: snake80.prg
	x64sc -default snake80.prg
test128: snake128.prg
	x128 -default -80 snake128.prg

clean:
	$(RM)  snake.prg snake80.prg snake128.prg
