objects = main.o menu.o theme.o font.o
LIBS = -lSDL2_ttf -lpthread

run: install
	./game

install: game

game: $(objects)
	g++ $(objects) -o game `sdl2-config --libs` $(LIBS)

main.o: main.cpp
	g++ -c `sdl2-config --cflags` main.cpp

menu.o: menu.cpp menu.hpp
	g++ -c `sdl2-config --cflags` menu.cpp

theme.o: theme.cpp theme.hpp
	g++ -c `sdl2-config --cflags` theme.cpp

font.o: font.cpp font.hpp
	g++ -c `sdl2-config --cflags` font.cpp

clean:
	rm -f $(objects)

reinstall: clean install
