CC=g++
CFLAGS= -g -c -Wall
EXECUTABLE = AnalogClock
OUTDIR = Debug
LDFLAGS = -lglut -lGLU -lGL

SOURCES = main.cpp \
		  Sprite.cpp ImageLoader.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(OUTDIR)/$(EXECUTABLE)
	@echo built $(EXECUTABLE) successfully!
	
$(OUTDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(addprefix $(OUTDIR)/, $(OBJECTS)) -o $@

%.o:
	$(CC) $(CFLAGS) src/$*.cpp -o $(OUTDIR)/$@
	
clean:
	rm -rf $(OUTDIR)/*o $(OUTDIR)/$(EXECUTABLE)
