objects = theme.o font.o sound.o networking.o map.o game.o object.o modes.o main.o
LIBS = -lSDL2_ttf -lSDL2_image -lSDL2_mixer
CFLAGS = `sdl2-config --cflags`

run: install
	./game

install: game

game: $(objects)
	g++ -g $(objects) -o game `sdl2-config --libs` $(LIBS)

main.o: main.cpp font.o theme.o networking.o map.o game.o modes.o
	g++ -c $(CFLAGS) main.cpp

theme.o: theme.cpp theme.hpp
	g++ -c $(CFLAGS) theme.cpp

font.o: font.cpp font.hpp theme.o
	g++ -c $(CFLAGS) font.cpp

sound.o: sound.cpp sound.hpp
	g++ -c $(CFLAGS) sound.cpp

modes.o: modes.cpp modes.hpp theme.o font.o sound.o game.hpp
	g++ -c $(CFLAGS) modes.cpp

networking.o: networking.cpp networking.hpp
	g++ -c $(CFLAGS) networking.cpp

map.o: map.cpp map.hpp networking.o
	g++ -c $(CFLAGS) map.cpp

game.o: game.cpp game.hpp object.o theme.o map.o networking.o sound.o
	g++ -c $(CFLAGS) game.cpp

object.o: object.cpp object.hpp sound.o
	g++ -c $(CFLAGS) object.cpp

clean:
	rm -f $(objects) game

reinstall: clean install
