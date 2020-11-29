CC = gcc
CFLAGS = -fPIC -Wall -Werror
RM = rm -f

LDFLAGS = -shared -lcURL
TARGET_LIB = libopenTIDAL.so

uname := $(shell sh -c 'uname -s 2>/dev/null || echo false')

ifeq (Darwin, $(uname))
	LDFLAGS = -dynamiclib -lcURL
	TARGET_LIB = libopenTIDAL.dylib
endif

SRCS = $(wildcard src/*.c) $(wildcard src/endpoints/*.c) $(wildcard src/parse/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all

all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
