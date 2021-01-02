CC = gcc
CFLAGS = -fPIC -Wall -Werror
RM = rm -f

LDFLAGS = -shared -lcurl
TARGET_LIB = libopenTIDAL.so

PREFIX ?= /usr/local
INCLUDE_PATH ?= include/openTIDAL
LIBRARY_PATH ?= lib

INSTALL_INCLUDE_PATH = $(DESTDIR)$(PREFIX)/$(INCLUDE_PATH)
INSTALL_LIBRARY_PATH = $(DESTDIR)$(PREFIX)/$(LIBRARY_PATH)
INSTALL ?= cp -a

uname := $(shell sh -c 'uname -s 2>/dev/null || echo false')

ifeq (Darwin, $(uname))
	LDFLAGS = -dynamiclib -lcurl
	TARGET_LIB = libopenTIDAL.dylib
endif

SRCS = $(wildcard src/*.c) $(wildcard src/external/*.c) $(wildcard src/helper/*.c) $(wildcard src/repository/*.c) $(wildcard src/repository/user_service/*.c) $(wildcard src/parse/*.c) $(wildcard src/parse/modules/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all

all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: install
install:
	mkdir -p $(INSTALL_LIBRARY_PATH) $(INSTALL_INCLUDE_PATH)
	$(INSTALL) src/openTIDAL.h $(INSTALL_INCLUDE_PATH)
	$(INSTALL) $(TARGET_LIB) $(INSTALL_LIBRARY_PATH)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
