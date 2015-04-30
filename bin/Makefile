SRC = $(wildcard *.o)
TARGET_THIS = $(notdir $(TARGET))

.PHONY: all
all:$(TARGET_THIS)

$(TARGET_THIS):
	$(CXX) $(CXXLDFLAGS) -o $(TARGET_THIS) $(SRC)