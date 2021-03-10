/*
    LISTA 03
    MARIANA NUNES DE OLIVEIRA
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 25

int calc_sad (int *, int *);

struct move
{
    int i, j;
};

int main (int argc, char *argv[])
{
    FILE *data;
    FILE *result;
    int H, W;
    int **F0, **F1;
    int *V1, *V2;
    int **SAD_AUX;
    int highest, lowest;
    struct move mov[MAX], mov2[MAX];
    //counters
    int l, c, L, C, m, contL, contC;

    //open file
    if (argc != 2)
    {
        printf("Invalid operation.\nPlease verify the filename.\n");
        exit(1);
    }

    data = fopen(argv[1], "r");

    if (data == NULL)
    {
        printf("There was a problem opening the file.\n");
        exit(2);
    }

    //read height and width
    fscanf(data, "%d", &H);
    fscanf(data, "%d", &W);

    //allocate memory for arrays and matrices
    F0 = (int **)malloc(H * sizeof(int *));
    F1 = (int **)malloc(H * sizeof(int *));
    SAD_AUX = (int **)malloc((H-1) * sizeof(int *));

    for (l = 0; l < H; l++)
    {
        F0[l] = (int *)malloc(W * sizeof(int));
        F1[l] = (int *)malloc(W * sizeof(int));
    }

    for (l = 0; l < (H-1); l++)
        SAD_AUX[l] = (int *)malloc((W-1) * sizeof(int));

    V1 = (int *)malloc(4 * sizeof(int));
    V2 = (int *)malloc(4 * sizeof(int));

    //copy data to matrix
    for (l = 0; l < H; l++)
        for (c = 0; c < W; c++)
            fscanf(data, "%d", &F0[l][c]);

    for (l = 0; l < H; l++)
        for (c = 0; c < W; c++)
            fscanf(data, "%d", &F1[l][c]);

    //initialize the counter of mov array as zero
    m = 0;

    /*
    L and C are used for matrix F1;
    l and c are used for matrix F0;
    contL and contC are used to handle matrix SAD_AUX.
    */

    for (L = 0; L < (H-1); L += 2)
    {
        for (C = 0; C < (W-1); C += 2)
        {
            V2[0] = F1[L][C];
            V2[1] = F1[L][C+1];
            V2[2] = F1[L+1][C];
            V2[3] = F1[L+1][C+1];

            for (l = 0; l < (H-1); l++)
            {
                for (c = 0; c < (W-1); c++)
                {
                    V1[0] = F0[l][c];
                    V1[1] = F0[l][c+1];
                    V1[2] = F0[l+1][c];
                    V1[3] = F0[l+1][c+1];

                    SAD_AUX[l][c] = calc_sad(V1, V2);
                }
            }

            highest = 0;

            for (contL = 0; contL < (H-1); contL++)
            {
                for (contC = 0; contC < (W-1); contC++)
                {
                    if (SAD_AUX[contL][contC] > highest)
                        highest = SAD_AUX[contL][contC];
                }
            }

            lowest = highest;

            for (contL = 0; contL < (H-1); contL++)
            {
                for (contC = 0; contC < (W-1); contC++)
                {
                    if (SAD_AUX[contL][contC] < lowest)
                    {
                        lowest = SAD_AUX[contL][contC];
                        mov[m].i = contL;
                        mov[m].j = contC;
                    }
                }
            }

            m++;
        }
    }

    //write binary file
    result = fopen("Result.bin", "w+b");
    fwrite(mov, sizeof(struct move), MAX, result);
    rewind(result);

    //open binary file
    fread(mov2, sizeof(struct move), MAX, result);

    //show output on screen
    printf("\ni:\tj:\n\n");
    for (m = 0; m < MAX; m++)
        printf("%d\t%d\n", mov2[m].i, mov2[m].j);

    printf("\n\n");

    //close files
    fclose(data);
    fclose(result);

    //deallocates memory from arrays and matrices
    for (l = 0; l < H; l++)
    {
        free(F0[l]);
        free(F1[l]);
    }

    for (l = 0; l < (H-1); l++)
        free(SAD_AUX[l]);

    free(F0);
    free(F1);
    free(SAD_AUX);
    free(V1);
    free(V2);

    system("PAUSE");
    return 0;
}

int calc_sad(int *v1, int *v2)
{
    int a, sad = 0;

    for (a = 0; a < 4; a++)
        sad += abs(v1[a] - v2[a]);

    return sad;
}
