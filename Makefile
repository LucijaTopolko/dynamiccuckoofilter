# Makefile za Cuckoo Filter Hash funkcije

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = 

# Source files
SOURCES = hash_utils.cpp
TEST_SOURCES = test_hash.cpp
HEADERS = hash_utils.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Executable
TARGET = test_hash

# Default target
all: $(TARGET)

# Build test program
$(TARGET): $(OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build uspješan! Pokreni sa: ./$(TARGET)"

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run tests
test: $(TARGET)
	@echo "Pokrećem testove..."
	@echo ""
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET)
	@echo "Cleaned!"

# Rebuild everything
rebuild: clean all

# Help
help:
	@echo "Dostupne naredbe:"
	@echo "  make          - Kompajlira program"
	@echo "  make test     - Kompajlira i pokreće testove"
	@echo "  make clean    - Briše generirane datoteke"
	@echo "  make rebuild  - Clean + build"
	@echo "  make help     - Prikazuje ovu pomoć"

.PHONY: all test clean rebuild help

