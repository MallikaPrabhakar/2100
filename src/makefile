objects = theme.o font.o networking.o menu.o map.o game.o object.o main.o
LIBS = -lSDL2_ttf -lSDL2_image -lpthread
CFLAGS = `sdl2-config --cflags`

run: install
	./game

install: game

game: $(objects)
	g++ -g $(objects) -o game `sdl2-config --libs` $(LIBS)

main.o: main.cpp font.o theme.o networking.o menu.o map.o game.o
	g++ -c $(CFLAGS) main.cpp

menu.o: menu.cpp menu.hpp theme.o font.o map.o networking.o game.o
	g++ -c $(CFLAGS) menu.cpp

theme.o: theme.cpp theme.hpp
	g++ -c $(CFLAGS) theme.cpp

font.o: font.cpp font.hpp theme.o
	g++ -c $(CFLAGS) font.cpp

networking.o: networking.cpp networking.hpp
	g++ -c $(CFLAGS) networking.cpp

map.o: map.cpp map.hpp networking.o
	g++ -c $(CFLAGS) map.cpp

game.o: game.cpp game.hpp object.o map.o theme.o networking.o
	g++ -c $(CFLAGS) game.cpp

object.o: object.cpp object.hpp
	g++ -c $(CFLAGS) object.cpp

clean:
	rm -f $(objects)

reinstall: clean install
