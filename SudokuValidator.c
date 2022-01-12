#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CHILD_THREADS 27

typedef int bool;

#define TRUE 1
#define FALSE 0

void *validateColumn(void *param);
void *validateRow(void *param);
void *validateGrid(void *param);

typedef struct {
int topRow;
int bottomRow;
int leftColumn;
int rightColumn;
} run_param;

// open sudoku puzzle file and fill array with contents


int sudokuPuzzle[9][9];
int i,j;
int x = 0; //thrParam iterator
int l = 0; //tid_grid iterator
int m = 0; //tid_row iterator
int n = 0; //tid_column iterator

  

// create T/F arrays for each column, row, and subgrid
bool booleanColumn[9] = {0};
bool booleanRow[9] = {0};
bool booleanGrid[9] = {0};

int main(void){

    FILE *myFile;
    myFile = fopen("SudokuPuzzle.txt", "r");

    for ( i = 0; i < 9; i++){
        for (j = 0; j < 9; j++){
            fscanf(myFile, "%d", &sudokuPuzzle[i][j]);
        }
    }

    //Print array to console
    printf("\n");
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++) {
            printf("%d", sudokuPuzzle[i][j]);
            printf("\t");
        }
        printf("\n");
        printf("\n");
    }
    printf("\n");

    pthread_t tid_column[9]; /* the thread identifier for child threads */
    pthread_t tid_row[9];
    pthread_t tid_grid[9];
	pthread_attr_t attr[NUM_CHILD_THREADS]; /* set of attributes for the thread */
	run_param thrParam[NUM_CHILD_THREADS];

    for (i = 0; i < NUM_CHILD_THREADS; i++)
		pthread_attr_init(&(attr[i]));

// Create three types of threads, one for 3 x 3 grids when in the correct spot, one for rows, and one for columns
    for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {						
			if ((i==0 || i==3 || i==6)&&(j==0 || j ==3 || j==6)) {
                thrParam[x].topRow = i;
		        thrParam[x].bottomRow = i+2;
                thrParam[x].leftColumn = j;
		        thrParam[x].rightColumn = j+2;
                pthread_create(&(tid_grid[l]),&(attr[x]),validateGrid, &(thrParam[x]));
                x++;
                l++;
            }
            if (j == 0){
                thrParam[x].topRow = i;
		        thrParam[x].bottomRow = i;
                thrParam[x].leftColumn = j;
		        thrParam[x].rightColumn = j+8;
                pthread_create(&(tid_row[m]),&(attr[x]),validateRow, &(thrParam[x]));
                x++;
                m++;
            }
            if (i == 0){
                thrParam[x].topRow = i;
		        thrParam[x].bottomRow = i+8;
                thrParam[x].leftColumn = j;
		        thrParam[x].rightColumn = j;
                pthread_create(&(tid_column[n]),&(attr[x]),validateColumn, &(thrParam[x]));
                x++;
                n++;
            }
        }
    }
// wait for threads to exit    
    for (i = 0; i < 9; i++) {
		pthread_join(tid_grid[i],NULL);
	}
    
    for (i = 0; i < 9; i++) {
		pthread_join(tid_row[i],NULL);
	}

    for (i = 0; i < 9; i++) {
		pthread_join(tid_column[i],NULL);
	}
    int z;
    int validCount = 0;
    for (z = 0; z < 9; z++){
        if (booleanColumn[z] == 1){
            printf("Column: %lX valid\n", (unsigned long)(tid_column[z]));
            validCount++;
        }
        else{
            printf("Column: %lX invalid\n", (unsigned long)(tid_column[z]));
        }
    }

    for (z = 0; z < 9; z++){
        if (booleanRow[z] == 1){
            printf("Row: %lX valid\n", (unsigned long)(tid_row[z]));
            validCount++;
        }
        else{
            printf("Row: %lX invalid\n", (unsigned long)(tid_row[z]));
        }
    }

    for (z = 0; z < 9; z++){
        if (booleanGrid[z] == 1){
            printf("Grid: %lX valid\n", (unsigned long)(tid_grid[z]));
            validCount++;
        }
        else{
            printf("Grid: %lX invalid\n", (unsigned long)(tid_grid[z]));
        }
    }

    if (validCount == 27){
        printf("Sudoku Puzzle: Valid\n");
    }
    else{
        printf("Sudoku Puzzle: Invalid\n");
    }
}
    
    
// Read each column to see if each number only appears once and set T/F array for respective column
void *validateColumn(void *param){
    run_param *inP;
    int tRow;
    int bRow;
    int lCol;
    int rCol;
    pthread_t self;

    inP = (run_param *)param;
    tRow = inP->topRow;
    bRow = inP->bottomRow;
    lCol = inP->leftColumn;
    rCol = inP->rightColumn;
    bool numsRead[9] = {0};
    int k;
    self = pthread_self();

    for (k = tRow; k <= bRow; k++){
        int numRead = sudokuPuzzle[k][lCol];
        if (numsRead[numRead - 1] == 1){
            printf("%lX TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid!\n", (unsigned long)self, tRow+1, bRow+1, lCol+1, rCol+1);
            booleanColumn[k] = 0;
            pthread_exit(0);
        }
        else{
            numsRead[numRead - 1] = 1;
            
        }
    }
    booleanColumn[lCol] = 1;
    printf("%lX TRow: %d, BRow: %d, LCol: %d, RCol: %d valid!\n", (unsigned long)self, tRow+1, bRow+1, lCol+1, rCol+1);
    pthread_exit(0);
}

// Read each row to see if each number only appears once and set T/F array for respective row
void *validateRow(void *param){
    run_param *inP;
    int tRow;
    int bRow;
    int lCol;
    int rCol;
    pthread_t self;

    inP = (run_param *)param;
    tRow = inP->topRow;
    bRow = inP->bottomRow;
    lCol = inP->leftColumn;
    rCol = inP->rightColumn;
    bool numsRead[9] = {0};
    int k;
    self = pthread_self();

    for (k = lCol; k <= rCol; k++){
        int numRead = sudokuPuzzle[tRow][k];
        if (numsRead[numRead - 1] == 1){
            printf("%lX TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid!\n", (unsigned long)self, tRow+1, bRow+1, lCol+1, rCol+1);
            booleanRow[k] = 0;
            pthread_exit(0);
        }
        else{
            numsRead[numRead - 1] = 1;
            
        }
    }
    booleanRow[tRow] = 1;
    printf("%lX TRow: %d, BRow: %d, LCol: %d, RCol: %d valid!\n", (unsigned long)self, tRow+1, bRow+1, lCol+1, rCol+1);
    pthread_exit(0);
}    

// Read each 3 x 3 grid to see if each number only appears once and set T/F array for respective grid
void *validateGrid(void *param){
    run_param *inP;
    int tRow;
    int bRow;
    int lCol;
    int rCol;
    pthread_t self;

    inP = (run_param *)param;
    tRow = inP->topRow;
    bRow = inP->bottomRow;
    lCol = inP->leftColumn;
    rCol = inP->rightColumn;
    bool numsRead[9] = {0};
    int k,l;
    self = pthread_self();

    for (k = tRow; k <= bRow; k++){
        for (l = lCol; l <= rCol; l++){
            int numRead = sudokuPuzzle[k][l];
            if (numsRead[numRead - 1] == 1){
                printf("%lX TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid!\n", (unsigned long)self, tRow+1, bRow+1, lCol+1, rCol+1);
                booleanGrid[k] = 0;
                pthread_exit(0);
            }
            else{
                numsRead[numRead - 1] = 1;
                
            }    
        }
    }
    booleanGrid[(tRow / 3) + lCol] = 1;
    printf("%lX TRow: %d, BRow: %d, LCol: %d, RCol: %d valid!\n", (unsigned long)self, tRow+1, bRow+1, lCol+1, rCol+1);
    pthread_exit(0);
}    
