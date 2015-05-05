export CXX = g++
export CXXFLAGS = -fPIC -Wall -g -O2
export CXXLDFLAGS = -fPIC -shared
export BIN = bin

export TARGET = $(BIN)/libnsresolve.so

subdirs = 	src/net \
			src/parser \
			src/resolver \
			src/export \
			src/record \
			bin
			
.PHONY: all
all:$(TARGET)
	
$(TARGET):
	@for dir in $(subdirs); do $(MAKE) -C $$dir || exit 1; done \
	
.PHONY: clean
clean:
	rm -f `find . -name "*.o"`
	rm -f $(TARGET)