#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct s {
    int numbers[9][9];
    int fixed[9][9];
};
typedef struct s Sudoku;

int *rows;
int *cols;
int *solution;
int gaps_to_fill;

bool CheckRow(Sudoku s, int row) {
    int counter[10];

    for(int i=0; i<=9; i++) {
        counter[i] = 0;
    }

    for(int j=0; j<=8; j++) {
        counter[ (s.numbers)[row][j] ]++;
    }

    for(int i=1; i<=9; i++) {
        if(counter[i] > 1)
            return false;
    }

    return true;
}

bool CheckColumn(Sudoku s, int col) {
    int counter[10];

    for(int i=0; i<=9; i++) {
        counter[i] = 0;
    }

    for(int j=0; j<=8; j++) {
        counter[ (s.numbers)[j][col] ]++;
    }

    for(int i=1; i<=9; i++) {
        if(counter[i] > 1)
            return false;
    }

    return true;
}

bool CheckSquare(Sudoku s, int row_shift, int col_shift) {
    int counter[10];

    for(int i=0; i<=9; i++) {
        counter[i] = 0;
    }

    counter[ (s.numbers)[0+row_shift][0+col_shift] ]++;
    counter[ (s.numbers)[0+row_shift][1+col_shift] ]++;
    counter[ (s.numbers)[0+row_shift][2+col_shift] ]++;

    counter[ (s.numbers)[1+row_shift][0+col_shift] ]++;
    counter[ (s.numbers)[1+row_shift][1+col_shift] ]++;
    counter[ (s.numbers)[1+row_shift][2+col_shift] ]++;

    counter[ (s.numbers)[2+row_shift][0+col_shift] ]++;
    counter[ (s.numbers)[2+row_shift][1+col_shift] ]++;
    counter[ (s.numbers)[2+row_shift][2+col_shift] ]++;

    for(int i=1; i<=9; i++) {
        if(counter[i] > 1)
            return false;
    }

    return true;
}

bool CheckAllRows(Sudoku s) {
    for(int i=0; i<=8; i++) {
        if(!CheckRow(s, i))
            return false; 
    }
    return true;
}

bool CheckAllColumns(Sudoku s) {
    for(int i=0; i<=8; i++) {
        if(!CheckColumn(s, i))
            return false; 
    }
    return true;
}

bool CheckAllSquares(Sudoku s) {
    for(int i=0; i<=2; i++) {
        for(int j=0; j<=2; j++) {
            if(!CheckSquare(s, 3*i, 3*j))
                return false;
        }
    }
    return true;
}

bool OK(Sudoku s) {

    bool p = CheckAllRows(s);
    bool q = CheckAllColumns(s);
    bool r = CheckAllSquares(s);

    return (p && q && r);
}

void PrintNumber(Sudoku s, int row, int col) {
    if(s.numbers[row][col] == 0)
        printf(" ");
    else
        printf("%d", s.numbers[row][col]); 
}

void PrintRow(Sudoku s, int row) {
    PrintNumber(s, row, 0);
    printf(" ");
    PrintNumber(s, row, 1);
    printf(" ");
    PrintNumber(s, row, 2);
    printf("|");
    PrintNumber(s, row, 3);
    printf(" ");
    PrintNumber(s, row, 4);
    printf(" ");
    PrintNumber(s, row, 5);
    printf("|");
    PrintNumber(s, row, 6);
    printf(" ");
    PrintNumber(s, row, 7);
    printf(" ");
    PrintNumber(s, row, 8);
}

void PrintSudoku(Sudoku s) {
    PrintRow(s, 0);
    printf("\n");
    PrintRow(s, 1);
    printf("\n");
    PrintRow(s, 2);
    printf("\n");
    printf("-----+-----+-----\n");
    PrintRow(s, 3);
    printf("\n");
    PrintRow(s, 4);
    printf("\n");
    PrintRow(s, 5);
    printf("\n");
    printf("-----+-----+-----\n");
    PrintRow(s, 6);
    printf("\n");
    PrintRow(s, 7);
    printf("\n");
    PrintRow(s, 8);
    printf("\n");
}

bool EvaluateSolution(Sudoku s) {
    for(int i=0; i<=gaps_to_fill-1; i++) {
        s.numbers[rows[i]][cols[i]] = solution[i];
    }
    return OK(s);
}

void ApplyAndPrintSolution(Sudoku s) {
    for(int i=0; i<=gaps_to_fill-1; i++) {
        s.numbers[rows[i]][cols[i]] = solution[i];
    }
    PrintSudoku(s);
}

void PreSolve(Sudoku s) {

    gaps_to_fill = 0;

    for(int i=0; i<=8; i++) {
        for(int j=0; j<=8; j++) {
            if(s.numbers[i][j] == 0)
                gaps_to_fill++;
        }
    }

    //printf("GTF: %d\n", gaps_to_fill);

    solution = malloc(gaps_to_fill*sizeof(int));

    rows = malloc(gaps_to_fill*sizeof(int));
    cols = malloc(gaps_to_fill*sizeof(int));
    int index = 0;

    for(int i=0; i<=8; i++) {
        for(int j=0; j<=8; j++) {
            if(s.numbers[i][j] == 0) {
                rows[index] = i;
                cols[index] = j;
                solution[index] = 0;
                index++;
            }
        }
    }

}

void RecursiveSolve(Sudoku s, int i) {

    if(i == gaps_to_fill) {
        ApplyAndPrintSolution(s);
        return;
    }

    for(int m=i+1; m<=gaps_to_fill-1; m++) {  
        solution[m] = 0;
    }

    for(int j=1; j<=9; j++) {
        solution[i] = j;
        if(EvaluateSolution(s)) {
            //ApplyAndPrintSolution(s);
            //printf("\n\n\n");
            //printf("Still going...\n");
            RecursiveSolve(s, i+1);
        } //else {
            for(int m=i; m<=gaps_to_fill-1; m++) {
                solution[m] = 0;
            }
        //}
    }

}

void Solve(Sudoku s) {
    PreSolve(s);
    RecursiveSolve(s, 0);
}

void ReadFile(char *filename, Sudoku *dest) {
    
    FILE *input = fopen(filename, "r");
    int byte;

	if(input == NULL) {
		printf("ERROR: Couldn't find file %s\n", filename);
		exit(1);
	}

    for(int i=0; i<=8; i++) {
        for(int j=0; j<=8; j++) {
            (dest -> numbers)[i][j] = 0;
            (dest -> fixed)[i][j] = 0;
        }
    }

    for(int i=0; i<=8; i++) {
        for(int j=0; j<=8; j++) {
            byte = fgetc(input);
            (dest -> numbers)[i][j] = byte - 48;
            (dest -> fixed)[i][j] = 1;
        }
        fgetc(input);
    }
    fclose(input);
}

int main(int argc, char **argv) {

    Sudoku s;

    ReadFile(argv[1], &s);

    printf("UNSOLVED: \n");
    PrintSudoku(s);

    printf("\n");

    printf("SOLVED: \n");
    Solve(s);

    return 0;
}
