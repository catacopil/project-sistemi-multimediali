CC = g++
CPPFLAGS = 
LDFLAGS =




OBJ =  Parser.o  ImageFilter.o ImagePNG.o BitMapRGB.o Header.o main.o 


%.o: %.cpp
		$(CC) $(CPPFLAGS) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) lodepng.cpp $(OBJ)  -o elaboraimmagine 

clean:
		rm -rf $(OBJ) elaboraimmagine

.PHONY: clean
 
