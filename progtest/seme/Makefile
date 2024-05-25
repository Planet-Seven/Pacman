CXX = g++
CXXFLAGS = -Wall -pedantic -o2 -std=c++20
BUILD_DIR = build
SRC_DIR = src

all:
	make compile
	make doc

compile: $(BUILD_DIR)/main.o $(BUILD_DIR)/CGhost.o $(BUILD_DIR)/CManhattan.o $(BUILD_DIR)/CMax.o $(BUILD_DIR)/CEuclid.o $(BUILD_DIR)/CCollectible.o $(BUILD_DIR)/CCoin.o $(BUILD_DIR)/CPowerUp.o $(BUILD_DIR)/CGameState.o $(BUILD_DIR)/CPos.o
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -lSDL2 -lSDL2_ttf -o pacman
	touch $(BUILD_DIR)/highscores.txt

run:
	./pacman

doc:
	doxygen

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/CManhattan.h $(SRC_DIR)/CMax.h $(SRC_DIR)/CEuclid.h $(SRC_DIR)/CGhost.h $(SRC_DIR)/CCoin.h $(SRC_DIR)/CPowerUp.h $(SRC_DIR)/CCollectible.h $(SRC_DIR)/CPos.h $(SRC_DIR)/CGameState.h $(SRC_DIR)/CGameMap.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CGhost.o: $(SRC_DIR)/CGhost.cpp $(SRC_DIR)/CPos.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CCollectible.o: $(SRC_DIR)/CCollectible.cpp $(SRC_DIR)/CPos.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CCoin.o: $(SRC_DIR)/CCoin.cpp $(SRC_DIR)/CCollectible.h $(SRC_DIR)/CPos.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CPowerUp.o: $(SRC_DIR)/CPowerUp.cpp $(SRC_DIR)/CCollectible.h $(SRC_DIR)/CPos.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CMax.o: $(SRC_DIR)/CMax.cpp $(SRC_DIR)/CGhost.h $(SRC_DIR)/CPos.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CEuclid.o: $(SRC_DIR)/CEuclid.cpp $(SRC_DIR)/CGhost.h $(SRC_DIR)/CPos.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CManhattan.o: $(SRC_DIR)/CManhattan.cpp $(SRC_DIR)/CGhost.h $(SRC_DIR)/CPos.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CGameState.o: $(SRC_DIR)/CGameState.cpp $(SRC_DIR)/CPos.h $(SRC_DIR)/CGameMap.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CPos.o: $(SRC_DIR)/CPos.cpp 
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -r $(BUILD_DIR)
	rm pacman
	rm -r doc

.PHONY: clean