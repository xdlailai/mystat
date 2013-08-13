CC = gcc
LDLIBS = -lpcap -lpthread -g
OBJS = mystat.o common.o counter.o dbaccess.o dbshow.o dbxml.o mod_cpu.o mod_mem.o mod_load.o mod_partition.o mod_traffic.o xml_cli.o

all: $(OBJS)
	$(CC) $(LDLIBS) $(OBJS) -o mystat

mystat.o: mystat.c mystat.h common.h counter.h dbaccess.h dbshow.h dbxml.h mod.h xml_cli.h
common.o: common.h common.c
counter.o: common.h counter.c dbaccess.h dbshow.h dbxml.h
dbaccess.o: common.h dbaccess.c
dbshow.o : common.h dbshow.c
dbxml.o :common.h dbxml.c
mod_cpu.o:common.h mod_cpu.c
mod_mem.o:common.h mod_mem.c
mod_load.o:common.h mod_load.c
mod_partition.o:common.h mod_partition.c
mod_traffic.o:common.h mod_traffic.c
xml_cli.o: common.h xml_cli.h xml_cli.c

clean:
	rm -f *.o mystat
