/* 
 * Name: Jiaqi Luo
 * ID : jiaqiluo
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    REQUIRES(M > 0);
    REQUIRES(N > 0);
	
//	int blocksize; // bs is blocksize.
	int row1, col1, row2;
	int var1, var2, var3, var4, var5, var6, var7, var8;
	 if (M == 32 && N ==32){
		
		for (row1 = 0; row1 < 32; row1 += 8){
			for(col1 = 0; col1 < 32; col1 += 8){
				for (row2 = 0; row2 < 8; row2++){
					var1 = A[row1 + row2][0 + col1];	
					var2 = A[row1 + row2][1 + col1];
					var3 = A[row1 + row2][2 + col1];
					var4 = A[row1 + row2][3 + col1];
					var5 = A[row1 + row2][4 + col1];
					var6 = A[row1 + row2][5 + col1];
					var7 = A[row1 + row2][6 + col1];
					var8 = A[row1 + row2][7 + col1];
					B[0 + col1][row1 + row2] = var1; 
					B[1 + col1][row1 + row2] = var2; 
					B[2 + col1][row1 + row2] = var3; 
					B[3 + col1][row1 + row2] = var4; 
					B[4 + col1][row1 + row2] = var5;
				        B[5 + col1][row1 + row2] = var6;
					B[6 + col1][row1 + row2] = var7;
					B[7 + col1][row1 + row2] = var8;
								
					
				} 
			}
		}
	}
	
	if ( M == 61){
		for (row1 = 0; row1 < 64; row1 += 8){
			for (col1 = 0; col1 < 56; col1 += 8){
				for(row2 = 0; row2 < 8; row2++){
					var1 = A[row1 + row2][0 + col1];
                                        var2 = A[row1 + row2][1 + col1];
                                        var3 = A[row1 + row2][2 + col1];
                                        var4 = A[row1 + row2][3 + col1];
                                        var5 = A[row1 + row2][4 + col1];
                                        var6 = A[row1 + row2][5 + col1];
                                        var7 = A[row1 + row2][6 + col1];
                                        var8 = A[row1 + row2][7 + col1];
                                        B[0 + col1][row1 + row2] = var1;
                                        B[1 + col1][row1 + row2] = var2;
                                        B[2 + col1][row1 + row2] = var3;
                                        B[3 + col1][row1 + row2] = var4;
                                        B[4 + col1][row1 + row2] = var5;
                                        B[5 + col1][row1 + row2] = var6;
                                        B[6 + col1][row1 + row2] = var7;
                                        B[7 + col1][row1 + row2] = var8;
				}
			} 
		}
		for (row1 = 0; row1 < 64; row1 += 8){
			for(row2 = 0; row2 < 8; row2++ ){
				var1 = A[row1 + row2][56];
                        	var2 = A[row1 + row2][57];
                        	var3 = A[row1 + row2][58];
                        	var4 = A[row1 + row2][59];
                        	var5 = A[row1 + row2][60];
				B[56][row1 + row2] = var1;
                                B[57][row1 + row2] = var2;
                                B[58][row1 + row2] = var3;
                                B[59][row1 + row2] = var4;
                                B[60][row1 + row2] = var5;
                        }             
		} 
		for (col1 = 0; col1 < 56; col1 += 8){
			for (row1 = 64; row1 < 67; row1++){
				var1 = A[row1][col1 + 0];
                        	var2 = A[row1][col1 + 1];
                        	var3 = A[row1][col1 + 2];
				var4 = A[row1][col1 + 3];
                                var5 = A[row1][col1 + 4];
                                var6 = A[row1][col1 + 5];
				var7 = A[row1][col1 + 6];
                                var8 = A[row1][col1 + 7];
                      		B[col1 + 0][row1] = var1;
                       		B[col1 + 1][row1] = var2;
                        	B[col1 + 2][row1] = var3;
				B[col1 + 3][row1] = var4;
                                B[col1 + 4][row1] = var5;
                                B[col1 + 5][row1] = var6;
				B[col1 + 6][row1] = var7;
                                B[col1 + 7][row1] = var8;
                 
			}	
		}
		for (row1 = 64; row1 < 67; row1++){
			var1 = A[row1][56];
			var2 = A[row1][57];
			var3 = A[row1][58];
			var4 = A[row1][59];
			var5 = A[row1][60];
			B[56][row1] = var1;
			B[57][row1] = var2;
			B[58][row1] = var3;
			B[59][row1] = var4;
			B[60][row1] = var5;	
		}
	}

	if (M == 64){
		for (row1 = 0; row1 < 56; row1 += 8){
			for (col1 = 0; col1 < 56; col1 += 8){	
				for (row2 = 0; row2 < 8; row2++){
					var1 = A[row1 + row2][0 + col1];
					var2 = A[row1 + row2][1 + col1];
					var3 = A[row1 + row2][2 + col1];
					var4 = A[row1 + row2][3 + col1];
					var5 = A[row1 + row2][4 + col1];
					var6 = A[row1 + row2][5 + col1];
					var7 = A[row1 + row2][6 + col1];
					var8 = A[row1 + row2][7 + col1];
					B[0 + col1][row1 + row2] = var1;
					B[1 + col1][row1 + row2] = var2;
					B[2 + col1][row1 + row2] = var3;
					B[3 + col1][row1 + row2] = var4;
					B[60][row2 + 56] = var5;
					B[61][row2 + 56] = var6;
					B[62][row2 + 56] = var7;
					B[63][row2 + 56] = var8;									
				}
				for (row2 = 0; row2 < 8; row2++){
					B[4 + col1][row1 + row2] = B[60][row2 + 56];
					B[5 + col1][row1 + row2] = B[61][row2 + 56];
					B[6 + col1][row1 + row2] = B[62][row2 + 56];
					B[7 + col1][row1 + row2] = B[63][row2 + 56];
				}
			} 
		}
		
		// part 2 
		for(col1 = 0; col1 < 64; col1 += 8){
			for (row2 = 56; row2 < 64; row2++){
				var1 = A[row2][0 + col1];
				var2 = A[row2][1 + col1];
				var3 = A[row2][2 + col1];
				var4 = A[row2][3 + col1];
				var5 = A[row2][4 + col1];
				var6 = A[row2][5 + col1];
				var7 = A[row2][6 + col1];
				var8 = A[row2][7 + col1];
				B[0 + col1][row2] = var1;
				B[1 + col1][row2] = var2;
				B[2 + col1][row2] = var3;
				B[3 + col1][row2] = var4;
				B[60][row2 - 8] = var5;
				B[61][row2 - 8] = var6;
				B[62][row2 - 8] = var7;
				B[63][row2 - 8] = var8;
			}
			for (row2 = 56; row2 < 64; row2++){
				B[4 + col1][row2] = B[60][row2 - 8];
				B[5 + col1][row2] = B[61][row2 - 8];
				B[6 + col1][row2] = B[62][row2 - 8];
				B[7 + col1][row2] = B[63][row2 - 8];
			}	
		}
		for(row1 = 0; row1 < 48; row1 += 8){
			for(row2 = 0; row2 < 8; row2 ++){
				var1 = A[row1 + row2][56];
				var2 = A[row1 + row2][57];
				var3 = A[row1 + row2][58];
				var4 = A[row1 + row2][59];
				var5 = A[row1 + row2][60];
				var6 = A[row1 + row2][61];
				var7 = A[row1 + row2][62];
				var8 = A[row1 + row2][63];
				B[56][row1 + row2] = var1;
				B[57][row1 + row2] = var2;
				B[58][row1 + row2] = var3;
				B[59][row1 + row2] = var4;
				B[60][row2 + 48] = var5;
				B[61][row2 + 48] = var6;
				B[62][row2 + 48] = var7;
				B[63][row2 + 48] = var8;
			}
			for(row2 = 0; row2 < 8; row2++){	
				B[60][row1 + row2] = B[60][row2 + 48];
				B[61][row1 + row2] = B[61][row2 + 48];
				B[62][row1 + row2] = B[62][row2 + 48];
				B[63][row1 + row2] = B[63][row2 + 48];
			}
		}
		for (col1 = 63; col1 > 55; col1 --){
			B[col1][48] = A[48][col1];
			B[col1][49] = A[49][col1];
			B[col1][50] = A[50][col1];
			B[col1][51] = A[51][col1];
			B[col1][52] = A[52][col1];
			B[col1][53] = A[53][col1];
			B[col1][54] = A[54][col1];
			B[col1][55] = A[55][col1];
			
		}
		
	}

    ENSURES(is_transpose(M, N, A, B));
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

