LIB_NAME=echeck

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
SHAREDFLAGS = -dynamiclib
SHAREDEXT = dylib
else
SHAREDFLAGS = -shared
SHAREDEXT = so
endif

LIB_SRC=$(LIB_NAME).c
LIB_HDR=$(LIB_NAME).h
LIB_OBJ=$(LIB_SRC:.c=.o)
SO_OBJS=$(LIB_NAME).o
SO_NAME=lib$(LIB_NAME).$(SHAREDEXT)
ifneq ($(UNAME), Darwin)
    SHAREDFLAGS += -Wl,-soname,$(SO_NAME)
endif

A_NAME=lib$(LIB_NAME).a

INCLUDES=-I.
SRC=test-$(LIB_NAME).c
OBJ=test-$(LIB_NAME).o
OUT=test-$(LIB_NAME)

CSTD_CFLAGS=-std=c89
#DEBUG_CFLAGS=-ggdb -O0
DEBUG_CFLAGS=-O3
NOISY_CFLAGS=-Werror -Wall -Wextra -pedantic

CFLAGS += $(CSTD_CFLAGS) $(DEBUG_CFLAGS) $(NOISY_CFLAGS)
LDFLAGS += -L. -l$(LIB_NAME)
CC=gcc

ifeq ("$(LIBDIR)", "")
LIBDIR=/usr/local/lib
endif

ifeq ("$(INCDIR)", "")
INCDIR=/usr/local/include
endif

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

default: $(OUT)

.c.o:
	$(CC) -c -fPIC $(CFLAGS) $< -o $@

$(SO_NAME): $(LIB_OBJ)
	$(CC) $(SHAREDFLAGS) -o $(SO_NAME).1.0 $(SO_OBJS)
	ln -sf ./$(SO_NAME).1.0 ./$(SO_NAME).1
	ln -sf ./$(SO_NAME).1.0 ./$(SO_NAME)

$(A_NAME): $(LIB_OBJ)
	ar -r $(A_NAME) $(SO_OBJS)

$(OUT): $(SO_NAME) $(A_NAME)
	$(CC) -c $(INCLUDES) $(CFLAGS) $(SRC) -o $(OBJ)
	$(CC) $(OBJ) $(A_NAME) -o $(OUT)-static
	$(CC) $(OBJ) $(LDFLAGS) -o $(OUT)-dynamic

check: $(OUT)
	./$(OUT)-static
	LD_LIBRARY_PATH=. ./$(OUT)-dynamic

tidy:
	$(LINDENT) \
                -T FILE \
                -T size_t \
                *.h *.c

clean:
	rm -f *.o *.a *.$(SHAREDEXT)  $(SO_NAME).* $(OUT)-static $(OUT)-dynamic

install:
	 @echo "Installing libraries in $(LIBDIR)"
	 cp -pv $(A_NAME) $(LIBDIR)/
	 cp -Rv $(SO_NAME)* $(LIBDIR)/
	 @echo "Installing headers in $(INCDIR)"
	 cp -pv $(LIB_HDR) $(INCDIR)/
