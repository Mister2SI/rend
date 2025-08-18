CFLAGS = -std=c++17 -O2 -Iinclude
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lXxf86vm -lXi
SRC = ${wildcard src/*.cpp}

rend_dbg: clean
	clang++ $(CFLAGS) -g -o rend $(SRC) $(LDFLAGS)

rend_rel: clean
	clang++ $(CFLAGS) -o rend $(SRC) $(LDFLAGS)

.PHONY: test clean

test: rend_dbg
	./rend

clean:
	rm -f rend

