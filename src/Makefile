PROG = main

# Variables pour la compilation des fichiers
CC        =  g++
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG -I$(INCDIR) -std=c++11
LDFLAGS   =  -g


all: $(PROG)

#%.o: %.cpp %.hpp
#	$(CC) -o $@ -c $< $(CPPFLAGS) $(CFLAGS)

%.o: %.cpp %.hpp
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

$(PROG): main.o Image.o Pixel.o Histogram.o
	$(CC) -o $@ $^ $(LDFLAGS)

cleanall : clean
	rm  $(PROG)

clean :
	rm *.o

