#MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
#MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = .

#CXXFLAGS = -I$(MUDUO_INCLUDE)
#LDFLAGS = -L$(MUDUO_LIBRARY) -lmy_http -lmymuduo

all: webapp

webapp: $(SRC)/main.cc MyWebAppServer.cc 
	g++ -o $@ $^ -lmy_http -lmymuduo 

webapp_static: $(SRC)/main.cc MyWebAppServer.cc 
	g++ -o $@ $^ -Wl,-Bstatic -lmy_http -Wl,-Bstatic -lmymuduo -Wl,-Bdynamic -lmysqlclient

# this one doesn't work
static: $(SRC)/main.cc MyWebAppServer.cc 
	g++ -static -o $@ $^ -lmy_http -lmymuduo -lmysqlclient

clean:
	rm -f webapp webapp_static static

.PHONY: all clean
