#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
CC=gcc
CFLAGS="-Wall -Wextra -std=c99 -g -O2 -Wno-sign-compare"
LDFLAGS="-lm"
SOURCES="funciones_grupo.c funciones_zapata.c funciones_quiroga.c lib/TDAImagenBMP.c"
EXECUTABLE="bmp_manipuleitor"
BINDIR="bin"

# Create bin directory if it doesn't exist
if [ ! -d "$BINDIR" ]; then
    echo -e "${YELLOW}Creating $BINDIR directory...${NC}"
    mkdir -p "$BINDIR"
fi

# Clean previous builds
if [ -f "$BINDIR/$EXECUTABLE" ]; then
    echo -e "${YELLOW}Cleaning previous build...${NC}"
    rm -f "$BINDIR/$EXECUTABLE"
    rm -f *.o
fi

# Compile
echo -e "${BLUE}Compiling source files...${NC}"
echo "Command: $CC $CFLAGS $SOURCES -o $BINDIR/$EXECUTABLE $LDFLAGS"

if $CC $CFLAGS $SOURCES -o "$BINDIR/$EXECUTABLE" $LDFLAGS; then
    echo -e "${GREEN}✅ Compilation successful!${NC}"
    echo -e "${GREEN}Executable created: $BINDIR/$EXECUTABLE${NC}"
    
    # Check if user wants to run the program
    if [ "$1" = "run" ] || [ "$1" = "-r" ]; then
        echo -e "${BLUE}Running the program...${NC}"
        if [ $# -gt 1 ]; then
            # Pass remaining arguments to the program
            shift
            ./$BINDIR/$EXECUTABLE "$@"
        else
            # Run with help or default
            echo -e "${YELLOW}No arguments provided. Showing usage help:${NC}"
            ./$BINDIR/$EXECUTABLE --help 2>/dev/null || echo "Program compiled successfully but no help available."
        fi
    else
        echo -e "${YELLOW}To run the program, use:${NC}"
        echo "  ./$BINDIR/$EXECUTABLE [options]"
        echo "  Or run: ./build.sh run [options]"
    fi
else
    echo -e "${RED}❌ Compilation failed!${NC}"
    exit 1
fi
