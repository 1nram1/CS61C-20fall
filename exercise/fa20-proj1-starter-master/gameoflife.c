/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int ring(int n, int m) {
	return (n + m) % m;
}

 	int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
 	int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};


//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.




// Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
// {
// 	//YOUR CODE HERE
// 	Color *nextsate = (Color*)malloc(sizeof(Color));
// 	int isAliveR = 0, isAliveG = 0, isAliveB = 0;
// 	int AliveNeighborsR = 0, AliveNeighborsG = 0, AliveNeighborsB = 0;
// 	int idR = 0, idG = 0, idB = 0;
// 	Color *pixel = image->image[row * image->cols + col];
// 	isAliveR = (pixel->R == 255);
// 	isAliveG = (pixel->G == 255);
// 	isAliveB = (pixel->B == 255);
// 	for(int i = 0; i < 8; ++i) {
// 		int newcol = ring(col + dy[i], image->cols);
// 		int newrow = ring(row + dx[i], image->rows);
// 		Color *neighbor = image->image[newrow * image->cols + newcol];
// 		if (neighbor->R == 255){
// 			AliveNeighborsR += 1;
// 		}
// 		if (neighbor->G == 255){
// 			AliveNeighborsG += 1;
// 		}
// 		if (neighbor->B == 255){
// 			AliveNeighborsB += 1;
// 		}
// 	}
// 	idR = isAliveR * 9 + AliveNeighborsR;
// 	idG = isAliveG * 9 + AliveNeighborsG;
// 	idB = isAliveB * 9 + AliveNeighborsB;
// 	if(rule & (1 << idR)){
// 		nextsate->R = 255;
// 	} else {
// 		nextsate->R = 0;
// 	}
// 	if(rule & (1 << idG)){
// 		nextsate->G = 255;
// 	} else {
// 		nextsate->G = 0;
// 	}
// 	if(rule & (1 << idB)){
// 		nextsate->B = 255;
// 	} else {
// 		nextsate->B = 0;
// 	}
// 	return nextsate;
// }


Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
    Color *nextState = (Color*)malloc(sizeof(Color));
    uint8_t aliveNeighboursR[8] = {0};  // 统计红色分量每个位的活着邻居数量
    uint8_t aliveNeighboursG[8] = {0};  // 绿色分量
    uint8_t aliveNeighboursB[8] = {0};  // 蓝色分量
    uint8_t isAliveR[8], isAliveG[8], isAliveB[8];

    // 读取当前像素每个位的状态
    for (int bit = 0; bit < 8; bit++) {
        isAliveR[bit] = ((*(image->image + row * image->cols + col))->R >> bit) & 1;
        isAliveG[bit] = ((*(image->image + row * image->cols + col))->G >> bit) & 1;
        isAliveB[bit] = ((*(image->image + row * image->cols + col))->B >> bit) & 1;
    }

    // 统计邻居的活着状态
    for (int i = 0; i < 8; i++) {
        int newrow = ring(row + dx[i], image->rows);
        int newcol = ring(col + dy[i], image->cols);
        for (int bit = 0; bit < 8; bit++) {
            aliveNeighboursR[bit] += ((*(image->image + newrow * image->cols + newcol))->R >> bit) & 1;
            aliveNeighboursG[bit] += ((*(image->image + newrow * image->cols + newcol))->G >> bit) & 1;
            aliveNeighboursB[bit] += ((*(image->image + newrow * image->cols + newcol))->B >> bit) & 1;
        }
    }

    // 根据规则计算下一状态
    nextState->R = 0;
    nextState->G = 0;
    nextState->B = 0;

    for (int bit = 0; bit < 8; bit++) {
        int idxR = 9 * isAliveR[bit] + aliveNeighboursR[bit];
        int idxG = 9 * isAliveG[bit] + aliveNeighboursG[bit];
        int idxB = 9 * isAliveB[bit] + aliveNeighboursB[bit];

        if (rule & (1 << idxR)) {
            nextState->R |= (1 << bit);
        }
        if (rule & (1 << idxG)) {
            nextState->G |= (1 << bit);
        }
        if (rule & (1 << idxB)) {
            nextState->B |= (1 << bit);
        }
    }

    return nextState;
}





//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image *newing = (Image*) malloc (sizeof(Image));
	newing->cols = image->cols;
	newing->rows = image->rows;
	newing->image = (Color**) malloc(sizeof(Color*) * (newing->cols * newing->rows));
	for(int i = 0; i < newing->cols; ++i){
		for(int j = 0; j < newing->rows; ++j){
			newing->image[i * newing->cols + j] = evaluateOneCell(image,i,j,rule);
		}
	}
	return newing;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (!(argc == 3)){
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.");
		return -1;
	}
	uint32_t rule = strtol(argv[2], NULL,16) & 0xFFFFFFFF ;
	Image *Input_Image = readData(argv[1]);
	Image *Output_Image = life(Input_Image, rule);
	writeData(Output_Image);
	freeImage(Input_Image);
	freeImage(Output_Image);
	return 0;
}
