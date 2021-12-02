CC=g++
CFLAGS=-I. 
LDFLAGS=-l wiringPi

DEPS = ptwFunctions.h
OBJ = ptwFunctions.o ptwMain.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

ptwMain: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	chmod u+x ptwMain

clean:
	rm -f *.o ptwMain
