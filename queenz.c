/* N-Queens C Solution
 *
 * CSC 322: Programming Languages
 * Dr. Mossinghoff
 *
 * Authors: Melech Maglasang and Jimmy Plaut
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Boolean macros
#define FALSE 0
#define TRUE -1

//Macros for specified mode
#define COUNT 1
#define DISPLAY 0
#define EXIT 0xe

int inputMode(int*);
int* initializeBoard(int);
int queenChecker(int*,int);
int queenMover(int*,int,int,int*,int*);
void writeBoard(int*, int);

int main(){
	int size = 0;
	time_t begin, end;
	while(TRUE){
		//Clear out the buffer
		fflush(stdout);
		//Reset active values
		int count = 0;
		//Whether not queenMover counts or returns true
		int mode;
		//Becomes true if there's no solution
		int noSolution = FALSE;

		size = inputMode(&mode);

		if(mode == EXIT)
			return TRUE;

		//begin timer
		begin = time(0);

		int* board = initializeBoard(size);

		if(!queenMover(board, 0, size, &count, &mode)){
			//Since this will always return false if in count mode, it's only if the counter is still 0 that it's unsolvable
			if((mode == COUNT) && count == 0){
				printf("No solutions found!\n");
				noSolution = TRUE;
			}
			else if(mode != COUNT){
				printf("No solutions found!\n");
				noSolution = TRUE;
			}
		}

		//If there is no solution, ignore this section and free the currently allocated array
		if(!noSolution){
			if(mode == DISPLAY)
				writeBoard(board, size);
			else if(mode == COUNT)
				printf("Total number of solutions: %d\n", count);
			end = time(0);

			printf("Elapsed time: %ld seconds\n", (end - begin));
		}

		free(board);
		printf("\n");
	}
	return TRUE;
}

/* Print options to the user, and accept a number for size
 *
 * Parameter:
 * 	mode - int pointer back to main indicating the mode chosen by the user
 * Return:
 * 	Size given by the user
 *
 */
int inputMode(int* mode){
	int size = 0;
	//Making sure a valid command is entered
	int validCommand = FALSE;
	//Is the number entered >0
	int validNumber = FALSE;

	char option;

	//Ensure the command given is valid
	while(!validCommand){
		printf("Enter c for count, d for display or q to quit: ");
		scanf("%s", &option);
		if((option != 'q') && (option != 'c') && (option != 'd')
				&& (option != 'Q') && (option != 'C') && (option != 'D')){
			printf("Error, invalid command\n");
		}
		else
			validCommand = TRUE;
	}

	if(option == 'q' || option == 'Q'){
		printf("Thank you for playing!\n");
		*mode = EXIT;
		exit(-1);
	}

	//Set mode to count
	if(option == 'c' || option == 'C')
		*mode = COUNT;
	else
		*mode = DISPLAY;

	//Accept a number
	while(!validNumber){
		printf("Specify a size: ");
		scanf("%d", &size);
		if(size <= 0)
			printf("Error, invalid number\n");
		else
			validNumber = TRUE;
	}


	return size;
}

/* Allocates a size n int array to be used as the board and sets all board values to -1 (no queen)
 *
 * Parameters:
 * 	size - number of spaces to allocate
 * Return - pointer to the beginning of the array
 */
int* initializeBoard(int size){
	//allocate the board
	int* board = (int*) malloc(size * sizeof(int));
	//make the board empty of queens
	for(int i = 0; i < size; i++){
		board[i] = -1;
	}
	return board;
}

/* Helper function that checks all indices before the current index for same row or diagonal
 *
 * Parameters:
 * 	board - pointer to the preallocated int array which contains the board state
 * 	curIndex - the current column to check against
 *
 * Return - TRUE or FALSE macro indicating whether the move is valid
 */
int queenChecker(int* board, int curIndex){
	//Check if the program is in the first column
	if(!curIndex)
		return TRUE;
	//Iterate backwards through the board
	for(int i = 1; i < curIndex+1; i ++){
		if( (board[curIndex-i] == board[curIndex])		//check if the queen is in the same row
				|| (board[curIndex-i] == board[curIndex]-i)		//check if the queen is in the bottom diagonal
						|| (board[curIndex-i] == board[curIndex] + i) )		//check if the queen is in the upper diagonal
			return FALSE;
	}
	return TRUE;
}

/* Recursive function that adds and moves Queens around
 *
 * Parameters:
 * 	board - pointer to the preallocated int array which contains the board state
 * 	index - current index to work on
 * 	size - size of the board
 * 	counter - only used when counting, pointer to a lasting counter
 * 	countMode - boolean value declaring whether to count or not
 * Return - Boolean value indicating whether it's solvable or not, will always return false if in count mode
 */
int queenMover(int* board, int index, int size, int* counter, int* countMode){
	//If current index doesn't have a queen, add one at position 0
	if(board[index] == -1)
		board[index] = 0;

	//if you are in the last index
	if(index == size-1){
		//increment through the last index
		for(int i = 1; i < size+1; i++){
			if(queenChecker(board, index)){
				//check if counting, if so never return TRUE but add to the counter instead
				if(*countMode == COUNT){
					*counter = *counter + 1;
				}
				//if not counting, return true
				else
					return TRUE;
			}
			//increment
			board[index] = i;
		}
		//reset column and backtrack
		board[index] = -1;
		return FALSE;
	}

	//Increment through the current index
	for(int i = 1; i < size+1; i++){
		//check current placement, move on if valid
		if(queenChecker(board, index)){
			if(queenMover(board, index + 1, size, counter, countMode)){
				return TRUE;
			}
		}
		//increment
		board[index] = i;
	}
	//Reset row to -1
	board[index] = -1;

	//Backtrack
	return FALSE;
}

/* Print out board state if on display mode
 *
 * Parameters:
 * 	board - pointer to the preallocated int array which contains the board state
 * 	size - board size
 */
void writeBoard(int* board, int size){
	for(int i = 0; i < size; i++){
		printf("__");
	}
	printf("_\n");
	for(int i = 0; i < size; i++){
		printf("|");
		for(int j = 0; j < size; j++){
			if(board[j] == i)
				printf("Q|");
			else
				printf("_|");
		}
		printf("\n");
	}

}



