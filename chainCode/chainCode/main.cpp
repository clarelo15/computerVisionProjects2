//
//  main.cpp
//  chainCode
//
//  Created by yin yu lo on 4/20/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class Image{
    public:
        int numRows, numCols, minVal, maxVal;
        int** imageAry;
        int** ccAry;
        Image(){
        
        }
        void zeroFramed(int** ary){
            for(int i = 0; i < numRows + 2; i++)
                for(int j = 0; j < numCols + 2; j++)
                    ary[i][j] = 0;
        }
        void loadImage(ifstream &inFile, int** ary){
            while(!inFile.eof()){
                for(int i = 1; i < numRows + 1; i++)
                    for(int j = 1; j < numCols + 1; j++)
                        inFile >> ary[i][j];
            }
        }
};

class ConnectCC{
public:
    int label, numbpixels, minRow, minCol, maxRow, maxCol;
    ConnectCC(){
        
    }
    void clearCC(int **ary, int r, int c){
        for(int i = 0; i < r; i++)
            for(int j = 0; j < c; j++)
                ary[i][j] = 0;
    }
    void loadCC(int ccLabel, int **ary, int **ary2, int r, int c){
        for(int i = 0; i < r; i++)
            for(int j = 0; j < c; j++)
                if(ary[i][j] == ccLabel)
                    ary2[i][j] = ccLabel;
    }
};

class ChainCode{
public:
    class Point{
    public:
        int row, col;
        int value;
        Point(){
        }
    };
    Point neighborCoord[8];
    Point startP;
    Point currentP;
    Point nextP;
    int lastQ;
    int nextDir;
    int pChainDir;
    ChainCode(){
        
    }
    void getChainCode(int* ary, int** ary2, ofstream & outFile, ofstream & outFile2){
        int zeroTable[8] = {6, 0, 0, 2, 2, 4, 4, 6};
        int label, minRow, maxRow, minCol, maxCol;
        bool isStartP = false;
        label = ary[0];
        minRow = ary[2];
        maxRow = ary[4];
        minCol = ary[3];
        maxCol = ary[5];
        for(int iRow = minRow; iRow <= maxRow; iRow++){
            for(int jCol = minCol; jCol <= maxCol; jCol++){
                if(ary2[iRow][jCol] == label){
                    startP.row = iRow;
                    startP.col = jCol;
                    outFile << startP.row << " " << startP.col << " " << ary2[iRow][jCol] << " ";
                    outFile2 << startP.row << " " << startP.col << " " << ary2[iRow][jCol] << endl;;
                    currentP.row = iRow;
                    currentP.col = jCol;
                    lastQ = 4;
                    isStartP = true;
                    break;
                }
            }
            if(isStartP)
                break;
        }
        bool starting = true;
        int nextQ = 0;
        int count20 = 0;
        while((!(currentP.row == startP.row && currentP.col == startP.col) )|| starting){
            starting = false;
            nextQ = (lastQ + 1) % 8;
            //cout << nextQ  << "bbb" << endl;
            pChainDir = findNextP(currentP.row, currentP.col, nextQ, nextP.row, nextP.col, ary2);
            currentP.value *= -1;
            outFile << pChainDir;
            if(count20 < 20){
                outFile2 << pChainDir << " ";
                count20++;
            }
            else{
                outFile2 << endl;
                outFile2 << pChainDir << " ";
                count20 = 1;
            }
            lastQ = zeroTable[(pChainDir + 7) % 8];
            currentP.row = nextP.row;
            currentP.col = nextP.col;
            //cout << currentP.row << "fff" << currentP.col << endl;
        }
        outFile << endl;
        outFile2 << endl << endl;
    }
    
    void loadNeighborsCoord(int i, int j, int neighborCoord[], int **ary){
        neighborCoord[0] = ary[i][j+1];
        neighborCoord[1] = ary[i-1][j+1];
        neighborCoord[2] = ary[i-1][j];
        neighborCoord[3] = ary[i-1][j-1];
        neighborCoord[4] = ary[i][j-1];
        neighborCoord[5] = ary[i+1][j-1];
        neighborCoord[6] = ary[i+1][j];
        neighborCoord[7] = ary[i+1][j+1];
    }
    
    int findNextP(int currentPRow, int currentPCol, int nextQ, int &nextPRow, int &nextPCol, int **ary){
        int neighborCoord[9];
        loadNeighborsCoord(currentPRow, currentPCol, neighborCoord, ary);
        int chainDir = 0;
        int loop = 0;
        while(loop < 8){
            if(neighborCoord[nextQ] > 0){
                chainDir = nextQ;
                break;
            }
            else
                nextQ = (++nextQ) % 8;
            /*chainDir = (++chainDir) % 8;
            cout << chainDir<<"nn" << endl;
            switch (chainDir) {
                case 0:
                    if(neighborCoord[5] > 0){
                        chainDir = 0;
                        break;
                    }
                case 1:
                    if(neighborCoord[2] > 0){
                        chainDir = 1;
                        break;
                    }
                case 2:
                    if(neighborCoord[1] > 0){
                        chainDir = 2;
                        break;
                    }
                case 3:
                    if(neighborCoord[0] > 0){
                        chainDir = 3;
                        break;
                    }
                case 4:
                    if(neighborCoord[3] > 0){
                        chainDir = 4;
                        break;
                    }
                case 5:
                    if(neighborCoord[6] > 0){
                        chainDir = 5;
                        break;
                    }
                case 6:
                    if(neighborCoord[7] > 0){
                        chainDir = 6;
                        break;
                    }
                case 7:
                    if(neighborCoord[8] > 0){
                        chainDir = 7;
                        break;
                    }
                    
                default:
                    break;
            }*/
            loop++;
        }
        if(chainDir == 0){
            nextPRow = currentPRow;
            nextPCol = currentPCol + 1;
        }
        else if(chainDir == 1){
            nextPRow = currentPRow - 1;
            nextPCol = currentPCol + 1;
        }
        else if(chainDir == 2){
            nextPRow = currentPRow - 1;
            nextPCol = currentPCol;
        }
        else if(chainDir == 3){
            nextPRow = currentPRow - 1;
            nextPCol = currentPCol - 1;
        }
        else if(chainDir == 4){
            nextPRow = currentPRow;
            nextPCol = currentPCol - 1;
        }
        else if(chainDir == 5){
            nextPRow = currentPRow + 1;
            nextPCol = currentPCol - 1;
        }
        else if(chainDir == 6){
            nextPRow = currentPRow + 1;
            nextPCol = currentPCol;
        }
        else if(chainDir == 7){
            nextPRow = currentPRow + 1;
            nextPCol = currentPCol + 1;
        }
        return chainDir;
    }
    void prettyPrint(){
        
    }
};

int main(int argc, const char * argv[]) {
    ifstream labelFile, propFile;
    ofstream chainCodeFile, deBugFile;
    int numOfCC;
    int CCproperty[6];
    
    labelFile.open(argv[1]);
    propFile.open(argv[2]);
    chainCodeFile.open(argv[3]);
    deBugFile.open(argv[4]);
    
    Image *img = new Image();
    ChainCode *chaincode = new ChainCode();
    ConnectCC *connect = new ConnectCC();
    
    labelFile >> img->numRows >> img->numCols >> img->minVal >> img->maxVal;
    chainCodeFile << img->numRows << " " << img->numCols << " " << img->minVal << " " << img->maxVal << endl;
    deBugFile << img->numRows << " " << img->numCols << " " << img->minVal << " " << img->maxVal << endl;
    
    img->imageAry = new int* [img->numRows + 2];
    for(int i = 0; i < img->numRows + 2; i++)
        img->imageAry[i] = new int [img->numCols + 2];
    
    img->ccAry = new int* [img->numRows + 2];
    for(int i = 0; i < img->numRows + 2; i++)
        img->ccAry[i] = new int [img->numCols + 2];
    
    img->zeroFramed(img->imageAry);
    img->zeroFramed(img->ccAry);
    
    img->loadImage(labelFile, img->imageAry);
    
    propFile >> img->numRows >> img->numCols >> img->minVal >> img->maxVal >> numOfCC;
    int index = 1;
    while(index <= numOfCC){
        for(int i = 0; i < 6; i++)
            propFile >> CCproperty[i];
        connect->clearCC(img->ccAry, img->numRows + 2, img->numCols + 2);
        connect->loadCC(index, img->imageAry, img->ccAry, img->numRows + 2, img->numCols + 2);
        chaincode->getChainCode(CCproperty, img->ccAry, chainCodeFile, deBugFile);
        index++;
        /*for(int i = 0; i < 6; i++)
            cout << CCproperty[i] << " ";
        cout << endl;
        for(int i = 0; i < img->numRows + 2; i++){
            for(int j = 0; j < img->numCols + 2; j++)
                cout << img->ccAry[i][j] << " ";
            cout << endl;
        }*/
    }
    
    for(int i = 0; i < img->numRows + 2; i++)
        delete[] img->imageAry[i];
    delete[] img->imageAry;
    
    for(int i = 0; i < img->numRows + 2; i++)
        delete[] img->ccAry[i];
    delete[] img->ccAry;
    
    delete img;
    delete connect;
    delete chaincode;
    
    labelFile.close();
    propFile.close();
    chainCodeFile.close();
    deBugFile.close();
    
    return 0;
}

