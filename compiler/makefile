OUT = pl.exe
HEAD = *.h *.hpp
SRC = *.cpp

$(OUT): $(SRC) $(HEAD)
	g++ -std=c++14 -Wall -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)