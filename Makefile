TARGET = minesweeper

CC = g++
CFLAGS = -c -Wall -Wno-switch
LFLAGS =

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCE_FILES = minesweeper.cpp mineui.cpp source.cpp
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)
SOURCES = $(addprefix $(SRCDIR)/, $(SOURCE_FILES))
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJECT_FILES))

ccend = $(shell echo -e "\033[0m")
ccblue = $(shell echo -e "\033[94m")
ccgreen = $(shell echo -e "\033[32m")

.PHONY: clean

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "$(ccblue)Linking...$(ccend)"
	@$(CC) $(LFLAGS) $(OBJECTS) -o $(BINDIR)/$@
	@echo "$(ccgreen)Done!$(ccend)"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "$(ccblue)Compiling $(ccgreen)$<$(ccend)"
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f $(OBJECTS)
	@rm -f $(BINDIR)/$(TARGET)
	@echo "$(ccgreen)Cleanup complete!$(ccend)"
