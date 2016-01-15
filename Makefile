#Target name
TARGET = experimentUI

#directory where to place output files
OBJDIR = $(CURDIR)/build

#directory where source files are
SRCDIR = $(CURDIR)/src

#directory where header files are
INCLUDES = $(CURDIR)/include

# CPPFLAGS = compiler options for C and C++
CFLAGS = -Wall -g -O -ffunction-sections -fdata-sections

#compiler tool
CC = /usr/bin/gcc

#all source files
SOURCES := $(wildcard $(SRCDIR)/*.c)

#If testing comment this out
FILTERED := $(filter-out %/testUI.c, $(SOURCES))

#Place all object files in build dir
OBJ := $(addprefix $(OBJDIR)/, $(notdir $(FILTERED:.c=.o)))

.PHONY: clean

all: $(TARGET)
	@echo "Compiling $(TARGET)"

# Dependencies for compiling the output file
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDES) -o $(TARGET) $(OBJ) 

# Dependencies for compiling .o files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo "[CC]\t$<"
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I$(INCLUDES) -o "$@" -c "$<"

clean:
	@echo Cleaning...
	@rm -rf "$(OBJDIR)"
	@rm $(TARGET)
