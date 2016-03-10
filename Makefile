EXEFILE = tree
LIBS = -lglfw -lGL -lGLU

$(EXEFILE): main.o Tree.o Leaf.o
	g++ $(LIBS) -o $(EXEFILE) main.o Tree.o Leaf.o 

main.o: main.cpp
	g++ -c main.cpp
	

Tree.o: Tree.cpp Tree.h
	g++ -c Tree.cpp
	
Leaf.o: Leaf.cpp Leaf.h
	g++ -c Leaf.cpp
	
clean:
	rm $(EXEFILE) main.o Tree.o Leaf.o

