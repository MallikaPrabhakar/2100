objects = networking.o main.o menu.o theme.o font.o
LIBS = -lSDL2_ttf -lpthread
CFLAGS = `sdl2-config --cflags`

run: install
	./game

install: game

game: $(objects)
	g++ $(objects) -o game `sdl2-config --libs` $(LIBS)

main.o: main.cpp font.o theme.o menu.o
	g++ -c $(CFLAGS) main.cpp

menu.o: menu.cpp menu.hpp font.o
	g++ -c $(CFLAGS) menu.cpp

theme.o: theme.cpp theme.hpp
	g++ -c $(CFLAGS) theme.cpp

font.o: font.cpp font.hpp theme.o
	g++ -c $(CFLAGS) font.cpp

networking.o: networking.cpp networking.hpp
	g++ -c $(CFLAGS) networking.cpp


clean:
	rm -f $(objects)

reinstall: clean install
