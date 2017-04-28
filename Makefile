LDFLAGS=-L/usr/local/lib/ -lglfw -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11

triangle:
	g++ -o triangle triangle.cpp $(LDFLAGS)

uniform:
	g++ -o uniform uniform.cpp $(LDFLAGS)

colored:
	g++ -o colored colored.cpp $(LDFLAGS)

cleant:
	rm triangle

cleanu:
	rm uniform

cleanc:
	rm colored

all: triangle uniform colored

clean: cleant cleanu cleanc
