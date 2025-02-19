.PHONY: all clean config gen_json

all:
	xmake build

clean:
	xmake clean


gen_json:
	xmake project -k compile_commands
