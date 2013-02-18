PROGNAME = m

CC = gcc
CC += -c
CPP = g++
CPP += -c
LD = g++
SRC := src
OBJ := obj
INCLUDE_FLAGS := -I/usr/include/freetype2
OS := $(shell uname)
ifeq ($(OS),Linux)
	SFML_LIBS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
	GL_LIBS := -lGL -lGLU
	LINKER_LIBS := /home/thoff/boost/lib/libboost_system.a /home/thoff/boost/lib/libboost_chrono.a /home/thoff/boost/lib/libboost_thread.a
	LINUX_LIBS := $(SFML_LIBS) $(GL_LIBS) -lfreetype
else ifeq ($(OS),Darwin)
	MAC_LIBS = -framework Carbon -framework OpenGL -framework sfml-system -framework sfml-graphics -framework sfml-window -framework sfml-audio -lX11 -lm  -L/usr/X11R6/lib
else
	@echo "OS not detected"
endif

# find all possible .cpp files
CPPFILES := $(wildcard $(SRC)/*.cpp)

#and substitute their names and paths with object names/paths 
OBJFILES := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(CPPFILES))

# just makes whatever the program is

all: $(PROGNAME)

# Runs through all the object files, and then links them together

$(PROGNAME): $(OBJFILES)
	@echo "source " $(CPPFILES)
	@echo "object files " $(OBJFILES)
	$(LD) -o $(PROGNAME) $(OBJFILES) $(LINUX_LIBS) $(MAC_LIBS) $(LINKER_LIBS)

# This is called for each .o file in the OBJFILES variable
# $< refers to first? arguments, $? to all arguments, $@ to rule name

$(OBJ)/%.o: $(SRC)/%.cpp
	@echo "Trying to compile" $?
	
	$(CPP) $< $(INCLUDE_FLAGS) $(LINUX_LIBS) $(MAC_LIBS) -o $@
	
nothing:
	@echo "'Twas brillig, and the slithy toves"
	@echo "Did gyre and gimble in the wabe;"
	@echo "All mimsy were the borogoves,"
	@echo "And the mome raths outgrabe."

#This ensures that make doesn't actually get any files named clean

.PHONY: clean
	@echo "HEY EVERYONE, THIS GUY'S A PHONY!"

clean:
	rm -f $(OBJ)/*~
	rm -f $(OBJ)/*.o
	rm -f $(SRC)/*.o
	rm -f $(SRC)/*.o
	rm -f $(PROGNAME)
	
	
	

	
