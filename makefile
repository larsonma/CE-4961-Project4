CC=gcc
CFLAGS=-c -Wall
LDFLAGS= -Wall
SOURCES=$(wildcard $(SRCDIR)/*.c)
INCLUDES=$(wildcard $(INCDIR)/*.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXECUTABLE=http_server

SRCDIR = src
INCDIR = inc
OBJDIR = obj
BINDIR = bin

all: $(SOURCES) $(BINDIR)/$(EXECUTABLE) 

$(BINDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCDIR)
	$(CC) $(CFLAGS) $< -I $(word 2,$^) -o $@
	@echo "Compiled "$<" successfully!"

clean:
	rm -rf $(OBJECTS) $(BINDIR)/$(EXECUTABLE)