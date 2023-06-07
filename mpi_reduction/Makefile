LINK_TARGET = reduction
CC = mpicc
OBJS = \
	reduction.o
REBUILDABLES = $(OBJS) $(LINK_TARGET) 

all : $(LINK_TARGET)

clean: 
	rm -f $(REBUILDABLES)

% : %.o
	$(CC) -o  $@ $^ -lm

%.o : %.c
	$(CC) -c $@ -c $< -lm


 