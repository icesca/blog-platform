#MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
#MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = .

#CXXFLAGS = -I$(MUDUO_INCLUDE)
#LDFLAGS = -L$(MUDUO_LIBRARY) -lmy_http -lmymuduo

all: webapp
clean:
	rm -f webapp

webapp: $(SRC)/main.cc MyWebAppServer.cc 
	g++ -o $@ $^ -lmy_http -lmymuduo

.PHONY: all clean
