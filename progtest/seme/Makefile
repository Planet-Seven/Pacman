all: build/main.o build/CGhost.o build/CCollectible.o build/CGameState.o
	mkdir -p build
	g++ -Wall -pedantic build/main.o build/CGhost.o build/CCollectible.o build/CGameState.o -lSDL2 -o pac.out
	./pac.out

build/main.o: build/CGhost.o build/CCollectible.o build/CGameState.o
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/main.cpp -o build/main.o

build/CGhost.o:
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/CGhost.cpp -o build/CGhost.o

build/CCollectible.o:
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/CCollectible.cpp -o build/CCollectible.o

build/CGameState.o:
	mkdir -p build
	g++ -Wall -pedantic -std=c++20 -c src/CGameState.cpp -o build/CGameState.o

clean:
	rm -r build
	rm pac.out

.PHONY: clean