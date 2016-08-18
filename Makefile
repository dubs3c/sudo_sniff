  # the compiler: gcc for C program, define as g++ for C++
  CC = gcc

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  CFLAGS  = -g -Wall

  # The cource code file
  FILE = sudo_sniff.c

  # the build target executable:
  TARGET = sudo

  all: $(TARGET)

  $(TARGET): $(FILE)
	$(CC) $(CFLAGS) -o $(TARGET) $(FILE)

  clean:
	$(RM) $(TARGET)