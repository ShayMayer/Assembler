# Mips Assembler

## Brief
This is an implementation of the mips assembler(partial instruction set).
The program takes a file(or a group of files) and converts them into machine code.

The idea in general is taking the file, validting and the converting its instructions into
machine code.
There are 2 types of instructions, data instrcutions(numbers and strings)
and the logical instructions(adding, subtracting, jumping and such) 

The file itself consists not only of instructions but also consists of labels.
The label is what comes before data instructions and with this, the use can a few things
like deciding which data is external(imported data) and which data is entry(expoted data).
Of course that data can be regular(neither external nor entry).

Every line should not be longer than 80 chars, And there are maximum amount of
"memory cells", which means that if there is to much data to process than the assmbler
says we've reached the limit.

The output consists of 3 files:
1) ob file, which contains the machine code
2) entry file, which contains all exported data
3) extern file, which contains all imported data

## How thing are done
First the user types in the names of the file he wishes to assemble.
Then the assmble checkes whether the files are valid, in a case any of the files
isn't valid then the assembler prints and error messgae.

Now the assembler checks whether the current instruction(not including the comment(the '#' char is the comment symbol))
consists of more than 80 chars, If so then it notifies the user.

If it's all fine then the file goes through a process which is called "first pass".
during the first pass the assembler iterates through every single line of the file and validates it.
the validation process is about looking for syntax errors, wrong amount of arguments and such.
If the assembler detects a wrong instruction then it doesn't stop, It keeps validating in order to
find more errors so it will be easier for the user to fix the code.

If nothing went wrong with the first pass then the file goes through a process which is called "second pass".
during the second pass the assembler tries to convert every instruction into machine code, but before
doing that, the assembler checks whether an error occurred.
An error can occured during the second pass if an entry label is external or vice versa, that's considered
an error becuase a label can't be both belong to data which both imported from another file and exported
to another files.
And like the first pass, If the assembler detects a wrong instruction then it doesn't stop
It keeps validating in order to find more errors so it will be easier for the user to fix the code.

If nothing went wrong with the second pass then the assembler starts producing the output files.
There are 3 output files:

1) We have the ob file, that consists of the machine code(the instrucitons we converted to binary).
   The first line consists of two numbers, The first one is the size of the regular instructions in
   memory and the second one is the size of the data in memory.
   After that there are two section, The 1th is the regular instrcutions section in which   
   each line is a 32 bit address divided into 4 hexa-decimal numbers and each of these 4 numbers
   is an 8 bit number. The number at the beggining of the line is the memory counter(starts from 100).
   
   The 2th section is about the data. like the 1th section, each line consits of 32 bits.
   The last line could often not be full because the data variables' size is not fixed, It 
   can be with the size of 16 bits for example.
   
2) We have the entry file, In which each line consists of the instruction's label name and the 
   instruction's address.
   
3) We have the extern file, In which each line consists of the imported instruction's label name
   and the address it was used in.

## source files
1. `assembler` - Contains functions that tries to assemble every single file given as arguments via the termial.
2. `code` - Contains functions for processing a single line in the first pass and a code line in the first pass.
3. `extern_list` - Contains function for processing a single line in second pass and replacing symbol by it's address.
4. `file_writer` - Contains functions for creating the output files.
5. `first_pass` - Contains functions for the first processing of the instructions.
6. `second_pass` - Contains functions for the second processing of the instructions.
7. `globals.h` - Contains constants and info shared everywhere.
8. 'symbol_table' - Contains info and functions about the symbols in the code
9. `memory_table` - Contains the memory(instruction and data as bytes).
10. `utils` - Contains useful and general-purpose functions.
