CXX = g++
CXXFLAGS = -g -Wall -Werror
TARGET = frupal
OBJS = main.o item_tiles.o map.o hero.o
LIBS = -lncurses
$(TARGET): $(OBJS)
		$(CXX) $(OBJS) -o $(TARGET) $(CXXFLAGS) $(LIBS)

main.o: main.cpp

item_tiles.o: item_tiles.cpp

map.o: map.cpp

hero.o: hero.cpp

clean:
		rm *.o $(TARGET)
