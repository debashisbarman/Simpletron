/* simulation.c */
#include <stdio.h>			/* Required */
#include <stdlib.h>			/* Required for `exit()` */

/* Input/Output Operations */
#define READ		10		/* Read from terminal and store into memory */
#define WRITE		11		/* Read from memory and write to terminal */

/* Load/Store Operations */
#define LOAD		20		/* Load from memory and store into accumulator */
#define	STORE		21		/* Store into memory from accumulator */

/* Arithmetic Operations */
#define ADD		30		/* Add to accumulator from memory */
#define SUBTRACT	31		/* Subtract to accumulator from memory */
#define DIVIDE		32		/* Divide accumulator from memory */
#define MULTIPLY	33		/* Multiply to accumulator from memory */

/* Transfer of Control Operations */
#define	BRANCH		40		/* Branch to a particular location in memory */
#define	BRANCHNEG	41		/* Branch to a particular location in memory if accumulator is negative */
#define	BRANCHZERO	42		/* Branch to a particular location in memory if accumulator is zero */
#define	HALT		43		/* Termination of the program */

/* Size of memory */
#define MEMSIZE		100		/* Total size of memory */

/* Sentinel */
#define SENTINEL	-99999		/* Sentinel to control input */

/* Validation of User Input */
#define UPLIMIT		9999		/* Upper limit of the input */
#define LOWLIMIT	-9999		/* Lower limit of the input */
#define VALID		1		/* Return 1, true */
#define INVALID		(word >= LOWLIMIT && word <= UPLIMIT)	/* Return 0, false */

/* Modules */
void welcome(void );			/* To display welcome screen */
void dump(const int [], int , int );	/* To dump memory contents */
int validate(int );			/* To validate user input */

/* void main(void ) : Main program handler */
void main(void ) {

	/* Memory initialization and other variables */
	int memory[MEMSIZE] = { 0 };

	int accumulator = 0;
	int instructionCounter = 0;
	int instructionRegister = 0;
	int operationCode = 0;
	int operand = 0;

	/* Display welcome scene */
	welcome();

	/* Load instructions into the memory */
	for (instructionCounter = 0; instructionCounter < MEMSIZE; instructionCounter++) {

		input : {
			printf("%.2d ? ", instructionCounter);
			scanf("%d", &memory[instructionCounter]);
		}

		if (!validate(memory[instructionCounter])) {
			printf("*** Unknown instruction ***\n");
			printf("*** Enter again or exit ***\n");
			goto input;
		}
		
		if (memory[instructionCounter] == SENTINEL) {
			memory[instructionCounter] = 0;
			instructionCounter = MEMSIZE;
			break;
		}
	}

	printf("*** Program loading completed ***\n");

	/* Excute the instructions from the memory */
	printf("*** Program execution begins  ***\n");

	for (instructionCounter = 0; instructionCounter < MEMSIZE; instructionCounter++) {

		instructionRegister = memory[instructionCounter];

		operationCode = instructionRegister / 100;
		operand = instructionRegister % 100;

		switch (operationCode) {

			/* Input/Output Operations */
			case READ :	printf("? ");
					scanf("%d", &memory[operand]);

					/* Fatal Error */
					if (memory[operand] < LOWLIMIT || memory[operand] > UPLIMIT) {
						printf("*** Out of memory ***\n");
						printf("*** Simpletron execution abnormally terminated ***\n");
						exit(EXIT_FAILURE);
					}
					break;
			case WRITE :	printf("%d\n", memory[operand]);
					break;

			/* Load/Store Operations */
			case LOAD :	accumulator = memory[operand];
					break;
			case STORE :	memory[operand] = accumulator;
					break;

			/* Arithmetic Operations */
			case ADD :	accumulator += memory[operand];

					/* Fatal Error */
					if (accumulator < LOWLIMIT || accumulator > UPLIMIT) {
						printf("*** Out of accumulator limit ***\n");
						printf("*** Simpletron execution abnormally terminated ***\n");
						exit(EXIT_FAILURE);
					}
					break;
			case SUBTRACT :	accumulator -= memory[operand];

					/* Fatal Error */
					if (accumulator < LOWLIMIT || accumulator > UPLIMIT) {
						printf("*** Out of accumulator limit ***\n");
						printf("*** Simpletron execution abnormally terminated ***\n");
						exit(EXIT_FAILURE);
					}
					break;
			case DIVIDE :	if (memory[operand] != 0)
						accumulator /= memory[operand];

					/* Fatal Error */
					else {
						printf("*** Attempt to divide by zero ***\n");
						printf("*** Simpletron execution abnormally terminated ***\n");
						exit(EXIT_FAILURE);
					}
					break;
			case MULTIPLY : accumulator *= memory[operand];

					/* Fatal Error */
					if (accumulator < LOWLIMIT || accumulator > UPLIMIT) {
						printf("*** Out of accumulator limit ***\n");
						printf("*** Simpletron execution abnormally terminated ***\n");
						exit(EXIT_FAILURE);
					}
					break;

			/* Transfer of Control Operations */
			case BRANCH :	instructionCounter = operand;
					break;
			case BRANCHNEG :	if (accumulator < 0)
							instructionCounter = operand;
						break;
			case BRANCHZERO :	if (accumulator == 0)
							instructionCounter = operand;
						break;
			case HALT :	printf("*** Simpletron execution terminated ***\n");
					dump(memory, accumulator, instructionCounter);
					break;
		}	
	}
	
	/* End of run */					
	exit(EXIT_SUCCESS);
}

/* void welcome(void ) : to display a welcome screen to the user */
void welcome(void ) {

	/* Display Welcome Screen */
	printf("*** Welcome to Simpletron! ***\n");
	printf("*** Please enter your program one instruction ***\n");
	printf("*** (or data word) at a time. I will type the ***\n");
	printf("*** location number and a question mark (?).  ***\n");
	printf("*** You then type the word for that location. ***\n");
	printf("*** Type the sentinel -99999 to stop entering ***\n");
	printf("*** your program. ***\n");
}

/* void dump(const int memory[], int accumulator, int instructionCounter) : to dump memory content */
void dump(const int memory[], int accumulator, int instructionCounter) {
	
	int i, j;

	/* Display Registers */
	printf("REGISTERS:\n");
	printf("accumulator \t\t%c%.4d\n", accumulator < 0 ? '-' : '+', accumulator);
	printf("instructionCounter \t%3c%.2d\n", ' ', instructionCounter);
	printf("instructionRegister \t%c%.4d\n", memory[instructionCounter] < 0 ? '-' : '+', memory[instructionCounter]);
	printf("operationCode \t\t%3c%.2d\n", ' ', memory[instructionCounter] / 100);
	printf("operand \t\t%3c%.2d\n\n", ' ', memory[instructionCounter] % 100);

	/* Display Memory */
	printf("MEMORY : \n");

	/* Display Memory Headers */
	printf("%3c", ' ');

   	for(i = 0; i < 10; i++)
		printf("%5d ", i);

	printf("\n");

	/* Display Memory Contents */
	for(i = 0; i < MEMSIZE; i += 10) {
		printf("%.2d", i);
		
		for(j = i; j < i+10; j++)
			printf(" %c%.4d", memory[j] < 0 ? '-' : '+', memory[j] < 0 ? -memory[j] : memory[j]);
      
		printf("\n");
   	}

	printf("\n");
}

/* int validate(int word) : to validate user input */
int validate(int word) {

	if (word == SENTINEL)
		return VALID;
	else
		return INVALID;
}
