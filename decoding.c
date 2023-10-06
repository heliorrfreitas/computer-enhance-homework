#include <stdio.h>
#include <string.h>


// this function reads a binary file
int readBinaryFile(char *filename){
	printf("Decoding instructions for the file named %s\n", filename); 
	unsigned char buffer[100];
	FILE *fileP;
	fileP = fopen(filename, "rb"); // r for read, b for binary
	fread(buffer, sizeof(buffer), 1, fileP);

	int index = 0;
	
	while(buffer[index] != '\0' ){
		if(buffer[index] >> 2 == 0b100010){
			// retrieve w and d bits 
			int wBit = buffer[index] & 1;
			int dBit = (buffer[index] >> 1) &1;

			// go to second byte
			index += 1;

			// GET mod, rm, reg values
			int mod = buffer[index] >> 6;	
			int rm = buffer[index] & 7;	
			int reg = (buffer[index] >> 3) & 7;	
		
			char *W1[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
			char *W0[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

			char *valueOfReg = wBit == 0b1 ? W1[reg] : W0[reg];
			char *valueOfRm = wBit == 0b1 ? W1[rm] : W0[rm];

			// calculating destination
			// Example  of how it is 
			// MOV destination, source
			if(0b1 == dBit){
				// MOV reg, rm
				printf("mov %s, %s", valueOfReg, valueOfRm);
			}else{
				// MOV rm, reg
				printf("mov %s, %s", valueOfRm, valueOfReg);
			}
			printf("\n");
		}
		// go to next byte block
		index += 1;
	}
	
	printf("\n\n");

	return 0;
}

int main(){
	char filename37Single[] = "listing_0037_single_register_mov";
	readBinaryFile(filename37Single);

	char filename38Many[] = "listing_0038_many_register_mov";
	readBinaryFile(filename38Many);

	return 0;
}

  // What I need to do is to to decode the MOV instructions
	// buffer[0] - first byte
	// 	first x bits are for the mov instruction MOV = 0b100010
	// 	the other bit is the D bit
	// 	the other is the W bit
	// 
	// buffer[1] - second byte
	// 	MOD (2 bits) reg(3 bits) r/m field(3)
	// 	MOD -> tells us what kind of move it is (since we are only looking at register to register moves, the value will always be 11 (2 1 bits, not eleven))
	// 	reg and r/m fields encode the two registers involved in the move
	// 	To know which is the source and which is the destination we need to look at the bit D
	// 	IF d = 1 then reg = destination; IF d = 0 then reg = source;
	// The W bit says whether the instruction is going to be wide
	// 	meaning that it will operate on 16 bits
	// 	IF w = 0; mov will copy 8 bits; IF w = 1 move will copy 16 bits;
	//



// restarted on september 19 2023
// continue reading this https://www.computerenhance.com/p/instruction-decoding-on-the-8086#footnote-1-105901699
// from there: The first byte — eight bits — has the pattern 100010 in its high six bits. That is what indicates that it will be a mov instruction specifically.


// References
// https://www.computerenhance.com/p/instruction-decoding-on-the-8086
// https://www.w3schools.com/cpp/showcpp.asp?filename=c_demo_files_null2
// https://stackoverflow.com/questions/17598572/read-and-write-to-binary-files-in-c
// https://stackoverflow.com/questions/5488377/converting-an-integer-to-binary-in-c
// https://stackoverflow.com/questions/5172107/how-to-concatenate-string-and-int-in-c
// https://www.log2base2.com/c-examples/loop/split-a-number-into-digits-in-c.html
// https://www.educative.io/blog/concatenate-string-c
// https://www.includehelp.com/c/how-to-assign-binary-value-in-a-variable-directly.aspx
// this is important to help with the comparison of strings:  https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/
