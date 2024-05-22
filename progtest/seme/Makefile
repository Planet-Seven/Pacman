all: build/main.o build/CGhost.o build/CCollectible.o build/CGameState.o build/CPos.o
	mkdir -p build
	g++ -Wall -pedantic -fsanitize=address -g build/main.o build/CGhost.o build/CCollectible.o build/CGameState.o build/CPos.o -lSDL2 -lSDL2_ttf -o pac.out
	pwd
	./pac.out

build/main.o: build/CGhost.o build/CCollectible.o build/CGameState.o build/CPos.o
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/main.cpp -o build/main.o

build/CGhost.o: build/CPos.o build/CGameState.o
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/CGhost.cpp -o build/CGhost.o

build/CCollectible.o: build/CPos.o build/CGameState.o
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/CCollectible.cpp -o build/CCollectible.o

build/CGameState.o: build/CPos.o
	mkdir -p build
	g++ -Wall -pedantic -std=c++20  -c src/CGameState.cpp -o build/CGameState.o

build/CPos.o:
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/CPos.cpp -o build/CPos.o

clean:
	rm -r build
	rm pac.out

.PHONY: clean