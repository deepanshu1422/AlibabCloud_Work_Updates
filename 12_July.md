**How to make a file easiest possible way**

> Make 3 files hellomake.c hellofunc.c hellomake.h

    #include <hellomake.h>
    
    int main() { // call a function in another file myPrintHelloMake();
    
    return(0); }


hellofunc.c

    #include <stdio.h> #include <hellomake.h>
    
    void myPrintHelloMake(void) {
    
    printf("Hello makefiles!\n");
    
    return; }

hellomake.h

    void myPrintHelloMake(void);

**After this make a file 
vi makefile**

    CC=gcc CFLAGS=-I. DEPS = hellomake.h OBJ = hellomake.o hellofunc.o
    
    %.o: %.c $(DEPS) 
    	$(CC) -c -o $@ $< $(CFLAGS)
    
    hellomake: $(OBJ) 
    	$(CC) -o $@ $^ $(CFLAGS)

Or
	makefile as
    
	IDIR =../include
    CC=gcc
    CFLAGS=-I$(IDIR)
    
    ODIR=obj
    LDIR =../lib
    
    LIBS=-lm
    
    _DEPS = hellomake.h
    DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
    
    _OBJ = hellomake.o hellofunc.o 
    OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
    
    
    $(ODIR)/%.o: %.c $(DEPS)
    	$(CC) -c -o $@ $< $(CFLAGS)
    
    hellomake: $(OBJ)
    	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
    
    .PHONY: clean
    
    clean:
    	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
