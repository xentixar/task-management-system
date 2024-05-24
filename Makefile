run: compile
	./build/main.out
	
compile:
	gcc main.c -o build/main.out -lmysqlclient