CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
SRC_DIR = src
TEST_DIR = tests
INCLUDE_DIR = include

# Object files for library
SRC_FILES = $(SRC_DIR)/bucket.cpp $(SRC_DIR)/cuckoo_filter.cpp $(SRC_DIR)/dynamic_cuckoo_filter.cpp $(SRC_DIR)/hash_utils.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Test executables
TEST_BUCKET = $(TEST_DIR)/test_bucket
TEST_HASH = $(TEST_DIR)/test_hash

.PHONY: all clean test test_bucket test_hash

all: $(TEST_BUCKET) $(TEST_HASH)

# Compile library object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test_bucket
$(TEST_BUCKET): $(TEST_DIR)/test_bucket.cpp $(SRC_DIR)/bucket.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(SRC_DIR)/bucket.o

# Compile test_hash (sada koristi i Bucket klasu)
$(TEST_HASH): $(TEST_DIR)/test_hash.cpp $(SRC_DIR)/hash_utils.o $(SRC_DIR)/bucket.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(SRC_DIR)/hash_utils.o $(SRC_DIR)/bucket.o

# Run tests
test: test_bucket test_hash
	@echo "Running all tests..."
	@echo ""
	@./$(TEST_BUCKET)
	@echo ""
	@./$(TEST_HASH)

test_bucket: $(TEST_BUCKET)
	./$(TEST_BUCKET)

test_hash: $(TEST_HASH)
	./$(TEST_HASH)

clean:
	rm -f $(OBJ_FILES) $(TEST_BUCKET) $(TEST_HASH) $(TEST_DIR)/*.o $(TEST_DIR)/*.exe

