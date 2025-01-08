TARGET = client_program
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -lssl -lcrypto

SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ -lssl -lcrypto

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean