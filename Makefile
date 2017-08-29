# Opções do compilador
CXX = g++
CXXFLAGS = -g -Wall

FONTES = main.cpp
OBJETOS = $(FONTES:.cpp=.o)

main: $(OBJETOS)
	$(CXX) $(OBJETOS) -o main -lGL -lGLU -lglut -ljpeg

clean:
	-@ rm -f $(OBJETOS) ${OBJAPP1} ${OBJAPP2}

depend:
	makedepend -- ${CFLAGS} -- ${FONTES}

# DO NOT DELETE


