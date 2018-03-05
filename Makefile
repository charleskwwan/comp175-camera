CPP = "g++"
CPPFLAGS = -Wall -Wextra
GLFLAGS = -lglut -lglui -lGL -lGLU
MACFLAGS = -framework OpenGL -framework GLUT -lglui


mac:
	$(CPP) $(CPPFLAGS) Assignment2.cpp Camera.cpp $(MACFLAGS) -o Assignment2

linux:
	$(CPP) $(CPPFLAGS) Assignment2.cpp Camera.cpp $(GLFLAGS) -o Assignment2

clean:
	rm -rf *.o Assignment2
