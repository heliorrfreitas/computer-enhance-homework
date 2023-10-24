#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// this function reads a binary file and decode its instructions
// for this case we only expect copy instructions 
// from register to register
char* decodeBinaryFile(char *filename){
	printf("Decoding instructions for the file named %s\n", filename); 
	char *W1[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
	char *W0[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
	unsigned char buffer[100];
	int index = 0;
	FILE *fileP;
	fileP = fopen(filename, "rb"); // r for read, b for binary
	fread(buffer, sizeof(buffer), 1, fileP);

	char *returnValue = NULL;
	returnValue = malloc(1500);
	// returnValue[0]='\0';	

	while(buffer[index] != '\0' ){
		if(buffer[index] >> 2 == 0b100010){
			// retrieve w and d bits 
			int wBit = buffer[index] & 1;
			int dBit = (buffer[index] >> 1) &1;

			index += 1;

			int mod = buffer[index] >> 6;	
			int rm = buffer[index] & 7;	
			int reg = (buffer[index] >> 3) & 7;	
			char *valueOfReg = wBit == 0b1 ? W1[reg] : W0[reg];
			char *valueOfRm = wBit == 0b1 ? W1[rm] : W0[rm];


			char result[15];
			result[0]='\0';

			// MOV destination, source
			if(0b1 == dBit){
				// MOV reg, rm
				printf("mov %s, %s", valueOfReg, valueOfRm);
				strcat(result, "mov ");
				strcat(result, valueOfReg);
				strcat(result, ", ");
				strcat(result, valueOfRm);
				strcat(result, "\n");
				printf("instruction created reg, rm  ==> %s \n", result);
			}else{
				// MOV rm, reg
				printf("mov %s, %s", valueOfRm, valueOfReg);
				strcat(result, "mov ");
				strcat(result, valueOfRm);
				strcat(result, ", ");
				strcat(result, valueOfReg);
				strcat(result, "\n");
				printf("instruction created rm, reg ==> %s \n", result);
			}
			strcat(returnValue, result);
			printf("\n");
		}
		index += 1;
	}

	fclose(fileP);	
	printf("\n\n");
	return returnValue;
}

void createAsmFile(char *filename, char *content){
	FILE *file;
	int filenameSize = strlen(filename) + 10;
	char fullFilename[filenameSize];
	fullFilename[0]='\0';
	strcat(fullFilename, filename);
	strcat(fullFilename, ".asm");
	
	file = fopen(fullFilename, "w");
	fwrite("bits 16\n\n", 1, 10, file);
	fwrite(content, 1, 1500, file);
	fclose(file);
	printf("\nFile %s created!\n", fullFilename);
}

// this part will receive only one filename
// then it will be the orchestrator of all the functions
// which includes
// 	* decoding the binary file
// 	* creating a .asm file with the decoded information
int main(int argc, char **argv){
	char *asmCode = decodeBinaryFile(argv[1]);
	createAsmFile(argv[1], asmCode);		

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
