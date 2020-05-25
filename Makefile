LINKER	     = g++
CC	     = g++
#LDFLAGS	     = -L/usr/lib/ -lblas -llapack
LDFLAGS	     = -L/home/fafa/lib/opencv-4.3.0/build/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -Wl,-rpath /home/fafa/lib/opencv-4.3.0/build/lib
COPTS	     = -O3 -std=c++17 -Wall --pedantic-errors
INCLUDE		= -I/home/fafa/lib/eigen -I/home/fafa/lib/opencv-4.3.0/build/include/opencv4

OBJS          = main.o\

PROGRAM	      = a.out

all:		$(PROGRAM)

$(PROGRAM): $(OBJS)
		$(LINKER) $(COPTS) $(OBJS) -o $(PROGRAM) $(LDFLAGS)

clean:
		rm -f $(PROGRAM) *.o *~ ;\

.SUFFIXES: .o .cc

.cc.o :
		$(CC) $(COPTS) $(INCLUDE) -c -o $*.o $*.cc

