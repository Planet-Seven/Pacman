CXX = g++
LD = g++
CXXFLAGS = -Wall -pedantic -o2 -std=c++20
BUILD_DIR = build
SRC_DIR = src

all: compile doc


compile: $(BUILD_DIR)/main.o $(BUILD_DIR)/CGhost.o $(BUILD_DIR)/CManhattan.o $(BUILD_DIR)/CMax.o $(BUILD_DIR)/CEuclid.o $(BUILD_DIR)/CCollectible.o $(BUILD_DIR)/CCoin.o $(BUILD_DIR)/CPowerUp.o $(BUILD_DIR)/CGameState.o $(BUILD_DIR)/CPos.o
	mkdir -p $(BUILD_DIR)
	make deps
	$(LD) $(CXXFLAGS) $^ -lSDL2 -lSDL2_ttf -o pacman
	touch $(BUILD_DIR)/highscores.txt


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	./pacman

doc:
	doxygen

deps:
	$(CXX) -MM $(SRC_DIR)/*cpp > $(BUILD_DIR)/Makefile.d

clean:
	make -i cleandoc
	make -i cleancompile

cleandoc:
	rm -r doc

cleancompile:
	rm -r $(BUILD_DIR)
	rm pacman

-include $(BUILD_DIR)/Makefile.d

.PHONY: clean