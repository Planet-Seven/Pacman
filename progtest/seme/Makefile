all: build/main.o build/CGhost.o build/CManhattan.o build/CMax.o build/CEuclid.o build/CCollectible.o build/CCoin.o build/CPowerUp.o build/CGameState.o build/CPos.o
	mkdir -p build
	g++ -Wall -pedantic -o2 build/main.o build/CGhost.o build/CManhattan.o build/CMax.o build/CEuclid.o build/CCollectible.o build/CCoin.o build/CPowerUp.o build/CGameState.o build/CPos.o -lSDL2 -lSDL2_ttf -o pacman
	touch build/highscores.txt
	doxygen

compile: build/main.o build/CGhost.o build/CManhattan.o build/CMax.o build/CEuclid.o build/CCollectible.o build/CCoin.o build/CPowerUp.o build/CGameState.o build/CPos.o
	mkdir -p build
	g++ -Wall -pedantic -o2 build/main.o build/CGhost.o build/CManhattan.o build/CMax.o build/CEuclid.o build/CCollectible.o build/CCoin.o build/CPowerUp.o build/CGameState.o build/CPos.o -lSDL2 -lSDL2_ttf -o pacman
	touch build/highscores.txt

run:
	./pacman

doc:
	doxygen

build/main.o: build/CGhost.o build/CCollectible.o build/CGameState.o build/CPos.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/main.cpp -o build/main.o

build/CGhost.o: build/CPos.o build/CGameState.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CGhost.cpp -o build/CGhost.o

build/CCollectible.o: build/CPos.o build/CGameState.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CCollectible.cpp -o build/CCollectible.o

build/CCoin.o: build/CPos.o build/CGameState.o build/CCollectible.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CCoin.cpp -o build/CCoin.o

build/CPowerUp.o: build/CPos.o build/CGameState.o build/CCollectible.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CPowerUp.cpp -o build/CPowerUp.o

build/CMax.o: build/CPos.o build/CGameState.o build/CGhost.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CMax.cpp -o build/CMax.o

build/CEuclid.o: build/CPos.o build/CGameState.o build/CGhost.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CEuclid.cpp -o build/CEuclid.o

build/CManhattan.o: build/CPos.o build/CGameState.o build/CGhost.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CManhattan.cpp -o build/CManhattan.o

build/CGameState.o: build/CPos.o
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20  -c src/CGameState.cpp -o build/CGameState.o

build/CPos.o:
	mkdir -p build
	g++ -Wall -pedantic -o2 -std=c++20 -c src/CPos.cpp -o build/CPos.o

clean:
	rm -r build
	rm pacman
	rm -r doc

.PHONY: clean