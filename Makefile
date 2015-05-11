export CXX = g++
export CXXFLAGS = -fPIC -Wall -O2
export CXXLDFLAGS = -fPIC -shared
export BIN = bin

export TARGET = $(BIN)/libnsresolve.so

INSTALLPATH = /usr/lib/libnsresolve.so

subdirs = 	src/net \
			src/parser \
			src/resolver \
			src/export \
			src/record \
			bin
			
.PHONY: all
all:$(TARGET)
	
$(TARGET):
	@for dir in $(subdirs); do $(MAKE) -C $$dir || exit 1; done
	
.PHONY: install
install:$(TARGET)
	sudo cp -f $(TARGET) $(INSTALLPATH)
	
.PHONY: clean
clean:
	rm -f `find . -name "*.o"`
	rm -f $(TARGET)
	sudo rm -f $(INSTALLPATH)