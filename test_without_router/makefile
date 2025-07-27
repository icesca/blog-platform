#MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
#MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = .

#CXXFLAGS = -I$(MUDUO_INCLUDE)
#LDFLAGS = -L$(MUDUO_LIBRARY) -lmy_http -lmymuduo

all: webapp
clean:
	rm -f webapp static

webapp: $(SRC)/main.cc
	g++ -o $@ $^ -lmy_http -lmymuduo

static: $(SRC)/main.cc
	g++ -o $@ $^ -Wl,-Bstatic -lmy_http -Wl,-Bstatic -lmymuduo -Wl,-Bdynamic -lmysqlclient

.PHONY: all clean
