assembler: assembler.o utils.o extern_list.o code.o memory_table.o file_writer.o symbol_table.o first_pass.o second_pass.o
	gcc -g -ansi -Wall -pedantic assembler.o utils.o extern_list.o code.o memory_table.o file_writer.o symbol_table.o first_pass.o second_pass.o -o assembler
assembler.o: assembler.c utils.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
utils.o: utils.c utils.h globals.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
extern_list.o: extern_list.c extern_list.h
	gcc -c -ansi -Wall -pedantic extern_list.c -o extern_list.o
code.o: code.c code.h utils.h
	gcc -c -ansi -Wall -pedantic code.c -o code.o
memory_table.o: memory_table.c memory_table.h
	gcc -c -ansi -Wall -pedantic memory_table.c -o memory_table.o
file_writer.o: file_writer.c file_writer.h utils.h extern_list.h symbol_table.h memory_table.h globals.h
	gcc -c -ansi -Wall -pedantic file_writer.c -o file_writer.o
symbol_table.o: symbol_table.c symbol_table.h utils.h globals.h
	gcc -c -ansi -Wall -pedantic symbol_table.c -o symbol_table.o
first_pass.o: first_pass.c first_pass.h utils.o code.o globals.h symbol_table.h memory_table.h
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o
second_pass.o: second_pass.c second_pass.h utils.h code.h memory_table.h symbol_table.h globals.h extern_list.h
	gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o
