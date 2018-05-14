PREFIX ?= ..
LIBDIR=$(PREFIX)/lib
SYSTEM_LIBS=libcrypto
objectFiles:=$(wildcard $(LIBDIR)/*.o)

EXECUTABLE=../svcs

$(EXECUTABLE): $(objectFiles)
	g++ $(objectFiles) `pkg-config --cflags --libs ${SYSTEM_LIBS}` -o $(EXECUTABLE)
