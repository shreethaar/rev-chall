# Makefile for building chall.dll

CC = i686-w64-mingw32-gcc
WINDRES = i686-w64-mingw32-windres
CFLAGS = -shared -pthread
LDFLAGS = -lwinmm -lntdll -lws2_32 -luser32
SRC = merlin.c
RC = resource.rc
OBJ = resource.o
OUTPUT = chall.dll
STRIP = i686-w64-mingw32-strip  # Strip command for MinGW

# Default target to build the DLL
all: $(OUTPUT)

# Compile the resource file
$(OBJ): $(RC)
	$(WINDRES) $(RC) -o $(OBJ)

# Link the object file and source code to create the DLL
$(OUTPUT): $(SRC) $(OBJ)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC) $(OBJ) $(LDFLAGS)

	# Strip the DLL to remove debug symbols
	$(STRIP) $(OUTPUT)

# Clean up generated files
clean:
	rm -f $(OBJ) $(OUTPUT)

.PHONY: all clean
