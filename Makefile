BIN_DIR = ./bin
SRC_DIR = ./src
INCLUDE = ./include

all: $(BIN_DIR)/knight_s_tour $(BIN_DIR)/knight_s_tour_visualize
.PHONY: clean run visualize

run: $(BIN_DIR)/knight_s_tour
	$<

visualize: CXXFLAGS += -DVISUALIZE
visualize: $(BIN_DIR)/knight_s_tour_visualize
	$<

CC = g++
CXXFLAGS = -fdiagnostics-color=always -std=c++17 -g3 -ggdb -Wall -Wextra
HEADERS = $(wildcard $(INCLUDE)/*.h $(INCLUDE)/*.hpp)

$(BIN_DIR): 
	mkdir -p $@

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp $(HEADERS) | $(BIN_DIR)
	$(CC) $(CXXFLAGS) $< -I $(INCLUDE) -o $@

$(BIN_DIR)/%_visualize: $(SRC_DIR)/%.cpp $(HEADERS) | $(BIN_DIR)
	$(CC) $(CXXFLAGS) $< -I $(INCLUDE) -o $@

clean:
	rm -rf $(BIN_DIR)/*