CC = gcc
LDLIBS = -lpcap
OBJS = mystat.o common.o counter.o dbaccess.o dbshow.o dbxml.o

all: $(OBJS)
	$(CC) $(LDLIBS) $(OBJS) -o mystat

mystat.o: mystat.c mystat.h common.h counter.h dbaccess.h dbshow.h dbxml.h
common.o: common.h common.c
counter.o: common.h counter.c dbaccess.h dbshow.h dbxml.h
dbaccess.o: common.h dbaccess.c
dbshow.o : common.h dbshow.c
dbxml.o :common.h dbxml.c

clean:
	rm -f *.o mystat
