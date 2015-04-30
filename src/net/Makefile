SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

.PHONY: all

all: $(OBJ)
	cp -f *.o ../../$(BIN)
	
%o: %c
	$(CXX) $(CXXFLAGS) -o $@ $<	\