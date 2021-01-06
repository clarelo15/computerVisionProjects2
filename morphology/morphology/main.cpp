//
//  main.cpp
//  morphology
//
//  Created by yin yu lo on 2/23/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

int numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin, rowFrameSize, colFrameSize, extraRows, extraCols;
int** zeroFramedAry;
int** morphAry;
int** structAry;
int** ary;

void zero2DAry(int** ary, int row, int col){
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            ary[i][j] = 0;
}

void loadImg(ifstream &imgFile, int** zeroFrameAry){
    while(!imgFile.eof()){
        for(int i = rowOrigin; i < numImgRows + rowOrigin; i++)
            for(int j = colOrigin; j < numImgCols + colOrigin; j++)
                imgFile >> zeroFrameAry[i][j];
    }
}

void loadStruct(ifstream &structFile, int** structAry){
    while (!structFile.eof()) {
        for(int i = 0; i < numStructRows; i++)
            for(int j = 0; j < numStructCols; j++)
                structFile >> structAry[i][j];
    }
}

void dilation(int i, int j, int ** ary, int ** ary2){
    for(int k = 0; k < numStructRows; k++){
        for(int l = 0; l < numStructCols; l++){
            if(structAry[k][l] == 1)
                ary2[i-rowOrigin+k][j-colOrigin+l] = 1;
        }
    }
}

void computeDilation(int ** ary, int ** ary2){
    for(int i = rowOrigin; i < numImgRows + rowOrigin; i++)
        for(int j = colOrigin; j < numImgCols + colOrigin ; j++)
            if(ary[i][j] > 0)
                dilation(i, j, ary, ary2);
}

void erosion(int i, int j, int ** ary, int ** ary2){
    bool match = true;
    for(int k = 0; k < numStructRows && match; k++){
        for(int l = 0; l < numStructCols && match; l++){
            if(structAry[k][l] == 1 && ary[i-rowOrigin+k][j-colOrigin+l] == 0)
                match = false;
        }
    }
    if(match) {
        ary2[i][j] = 1;
    }
    else
        ary2[i][j] = 0;
}

void computeErosion(int ** ary, int ** ary2){
    for(int i = rowOrigin; i < numImgRows + rowOrigin; i++)
        for(int j = colOrigin; j < numImgCols + colOrigin ; j++)
            if(ary[i][j] > 0)
                erosion(i,j, ary, ary2);
}

void opening(int ** ary, int ** ary2, int ** temp){
    zero2DAry(temp, numImgRows + extraRows, numImgCols + extraCols);
    computeErosion(ary, temp);
    computeDilation(temp, ary2);
}

void computeOpening(int** ary, int ** ary2){
    int ** temp = new int * [numImgRows + extraRows];
    for(int i = 0; i < numImgRows + extraRows; i++)
        temp[i] = new int [numImgCols + extraCols];
    
    opening(ary, ary2, temp);
    
    for(int i = 0; i < numImgRows + extraRows; i++)
        delete [] temp[i];
    delete [] temp;
}

void closing(int ** ary, int ** ary2, int ** temp){
    zero2DAry(temp, numImgRows + extraRows, numImgCols + extraCols);
    computeDilation(ary, temp);
    computeErosion(temp, ary2);
}

void computeClosing(int** ary, int ** ary2){
    int ** temp = new int * [numImgRows + extraRows];
    for(int i = 0; i < numImgRows + extraRows; i++)
        temp[i] = new int [numImgCols + extraCols];
    
    closing(ary, ary2, temp);
    
    for(int i = 0; i < numImgRows + extraRows; i++)
        delete [] temp[i];
    delete [] temp;
}

void aryToFile(int** morphAry, ofstream &outFile){
    outFile << numImgRows << " " << numImgCols << " " << imgMin << " " << imgMax << endl;
    for(int i = rowOrigin; i < numImgRows + rowOrigin; i++){
        for(int j = colOrigin; j < numImgCols + colOrigin; j++){
            outFile << morphAry[i][j] << " ";
        }
        outFile << endl;
    }
}
void prettyPrint(int** ary, ofstream &outFile, int r, int c, int row, int col, string name){
    outFile << name << endl;
    for(int i = r; i < row + r; i++){
        for(int j = c; j < col + c; j++){
            if(ary[i][j] == 0)
                outFile << "." << " ";
            else
                outFile << ary[i][j] << " ";
        }
        outFile << endl;
    }
    outFile << endl;
}

int main(int argc, char * argv[]) {
    ifstream imgFile, structFile;
    ofstream dilateOutFile, erodeOutFile, openingOutFile, closingOutFile, prettyPrintFile;
    
    imgFile.open(argv[1]);
    structFile.open(argv[2]);
    dilateOutFile.open(argv[3]);
    erodeOutFile.open(argv[4]);
    openingOutFile.open(argv[5]);
    closingOutFile.open(argv[6]);
    prettyPrintFile.open(argv[7]);
    
    imgFile >> numImgRows >> numImgCols >> imgMin >> imgMax;
    structFile >> numStructRows >> numStructCols >> structMin >> structMax;
    structFile >> rowOrigin >> colOrigin;
    
    rowFrameSize = numStructRows / 2;
    colFrameSize = numStructCols / 2;
    extraRows = rowFrameSize * 2;
    extraCols = colFrameSize * 2;
    
    int r = numImgRows + extraRows;
    int c = numImgCols + extraCols;
    
    zeroFramedAry = new int* [numImgRows + extraRows];
    for(int i = 0; i < numImgRows + extraRows; i++)
        zeroFramedAry[i] = new int [numImgCols + extraCols];
    
    morphAry = new int* [numImgRows + extraRows];
    for(int i = 0; i < numImgRows + extraRows; i++)
        morphAry[i] = new int [numImgCols + extraCols];
    
    structAry = new int* [numStructRows];
    for(int i = 0; i < numStructRows; i++)
        structAry[i] = new int [numStructCols];
    

    zero2DAry(zeroFramedAry, r, c);
    loadImg(imgFile, zeroFramedAry);
    prettyPrint(zeroFramedAry, prettyPrintFile, rowOrigin, colOrigin, numImgRows, numImgCols, "imgFile");
    
    zero2DAry(structAry,numStructRows, numStructCols);
    loadStruct(structFile, structAry);
    prettyPrint(structAry, prettyPrintFile, 0, 0, numStructRows, numStructCols, "structFile");
    
    zero2DAry(morphAry, r, c);
    computeDilation(zeroFramedAry, morphAry);
    aryToFile(morphAry,dilateOutFile);
    prettyPrint(morphAry, prettyPrintFile, rowOrigin, colOrigin, numImgRows, numImgCols, "dilation");
    
    zero2DAry(morphAry, r, c);
    computeErosion(zeroFramedAry, morphAry);
    aryToFile(morphAry,erodeOutFile);
    prettyPrint(morphAry, prettyPrintFile, rowOrigin, colOrigin, numImgRows, numImgCols, "erosion");
    
    zero2DAry(morphAry, r, c);
    computeOpening(zeroFramedAry, morphAry);
    aryToFile(morphAry, openingOutFile);
    prettyPrint(morphAry, prettyPrintFile, rowOrigin, colOrigin, numImgRows, numImgCols, "opening");
    
    zero2DAry(morphAry, r, c);
    computeClosing(zeroFramedAry, morphAry);
    aryToFile(morphAry, closingOutFile);
    prettyPrint(morphAry, prettyPrintFile, rowOrigin, colOrigin, numImgRows, numImgCols, "closing");
    
    for(int i = 0; i < numImgRows + extraRows; i++)
        delete [] zeroFramedAry[i];
    delete [] zeroFramedAry;
    
    for(int i = 0; i < numImgRows + extraRows; i++)
        delete [] morphAry[i];
    delete [] morphAry;
    
    for(int i = 0; i < numStructRows; i++)
        delete [] structAry[i];
    delete [] structAry;
    
    imgFile.close();
    structFile.close();
    dilateOutFile.close();
    erodeOutFile.close();
    openingOutFile.close();
    closingOutFile.close();
    prettyPrintFile.close();
    
    return 0;
}
