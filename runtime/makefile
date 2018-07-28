OUT = pl.exe
HPATH = ../include
HEAD = $(HPATH)/*.h $(HPATH)/*.hpp
SRC = *.cpp

$(OUT): $(SRC) $(HEAD)
	g++ -std=c++14 -Wall -I$(HPATH) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)