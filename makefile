#Automated compiler for ADWS - Arduino Development Web Server

#############################
#  COMPILER CONFIG SECTION  #
#############################

TARGET = local-test
CXX = clang++
CXXFLAGS = -Wall

AR = ar
ARFLAGS = rcs

###########################
#  LINKER CONFIG SECTION  #
###########################

INCLUDE_DIRS = -Iinclude
LIB_DIRS = -Llib

STATIC_LIBS = $(wildcard lib/*.a)
DYNAMIC_LIBS= -lSDL3 -lGL

	
##############################
#    SOURCE FILES SECTION    #
##############################

#Source files
SRC = $(wildcard src/*.cpp)

##############################
#  DIRECTIVE CONFIG SECTION  #
##############################

all:
	@echo $(shell date): Compiling ADWS [local-test] at $(shell hostname) as $(shell whoami)
	@$(CXX) $(CXXFLAGS) $(SRC) $(STATIC_LIBS) -o $(TARGET) $(INCLUDE_DIRS) $(LIB_DIRS) $(DYNAMIC_LIBS)
	@echo ADWS [local-test] compiled! Executable name: $(TARGET)

run:
	@echo $(shell date): Running ADWS [local-test] at $(shell hostname) via make directive as $(shell whoami)
	@./$(TARGET)
	@echo Test completed!
clean:
	@echo $(shell date): Cleaning ADWS [local-test] at $(shell hostname) via make directive as $(shell whoami)
	@rm -f $(TARGET)
	@echo Clean completed!