CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lXxf86vm -lXi
SRC = ${wildcard *.cpp}

rend: $(SRC)
	clang++ $(CFLAGS) -o rend $(SRC) $(LDFLAGS)

.PHONY: test clean

test: rend
	./rend

clean:
	rm -f rend

