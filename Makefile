LDFLAGS=-L/usr/local/lib/ -lglfw -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11

triangle:
	g++ -o triangle triangle.cpp $(LDFLAGS)

clean:
	rm triangle
