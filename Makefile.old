LDFLAGS=-L/usr/local/lib/ -lglfw -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11

gltest:
	g++ -o gltest gltest.cpp shader.cpp $(LDFLAGS)

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

cleang:
	rm gltest

all: triangle uniform colored gltest

clean: cleant cleanu cleanc cleang
