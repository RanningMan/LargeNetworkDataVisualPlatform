CC = g++
CPPFLAGS = -g -Wno-deprecated -O2 -c
LDFLAGS	=  -O2 -lboost_thread
MYSQLCFLAGS = `mysql_config --cflags`
MYSQLLIBS = `mysql_config --libs`
SOURCES = src/Utils.cpp src/Graph.cpp src/Generator.cpp src/main.cpp src/Log.cpp src/DBManager.cpp src/DBJob.cpp src/BinaryTree.cpp src/GraphUtil.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE = VisDemoWebservice

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(MYSQLLIBS)

.cpp.o : 
	$(CC) $(CPPFLAGS) $(MYSQLCFLAGS) $< -o $@

rm:
	rm -f src/*.o
	rm -f *.exe
