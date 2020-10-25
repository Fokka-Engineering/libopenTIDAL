CC = gcc
CFLAGS = -fPIC -Wall -Werror
LDFLAGS = -dynamiclib -lcURL
RM = rm -f
TARGET_LIB = libopenTIDAL.dylib

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
