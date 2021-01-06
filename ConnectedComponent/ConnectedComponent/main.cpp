//
//  main.cpp
//  ConnectedComponent
//
//  Created by yin yu lo on 3/21/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class CCLabel{
    public:
        int numRows, numCols, minVal, maxVal, newMin, newMax, trueNumCC;
        int newLabel;
        int** zeroFramedAry;
        static const int numNb = 5;
        int nonZeroNeighborAry[numNb];
        int* EQAry;
        int label, numpixels, upperLftR, upperLftC, lowerRgtR, lowerRgtC;
        int** ccproperty;
    
    
        CCLabel(){
            
        }
        void zero2D(int** ary){
            for(int i = 0; i < numRows + 2; i++)
                for(int j = 0; j < numCols + 2; j++)
                    ary[i][j] = 0;
        }
    
        void loadImage(ifstream & inFile, int** ary){
            while(!inFile.eof()){
                for(int i = 1; i < numRows + 1; i++)
                    for(int j = 1; j < numCols + 1; j++)
                        inFile >> ary[i][j];
            }
        }
        int connectPass1(int connectness, int** ary, int nonZeroAry[]){
            newLabel = 0;
            int minLabel;
            bool diffFlag = false;
            int nonZeroCount = 0;
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    if(ary[i][j] > 0){
                        minLabel = loadNonZeroPass1(ary, connectness, i, j, nonZeroAry, diffFlag, nonZeroCount);
                        if(minLabel == 0){
                            newLabel++;
                            ary[i][j] = newLabel;
                        }
                        else if (minLabel > 0){
                            ary[i][j] = minLabel;
                            if(diffFlag == true)
                                updateEQ(EQAry, nonZeroAry, minLabel, nonZeroCount);
                        }
                    }
                }
            }
            return newLabel;
        }
        int loadNonZeroPass1(int** ary, int connectness, int i, int j, int nonZeroAry[], bool &diffFlag, int &nonZeroCount){
            minus1D(nonZeroAry);
            int minLabel;
            nonZeroCount = 0;
            if(ary[i - 1][j] > 0){
                nonZeroAry[nonZeroCount] = ary[i - 1][j];
                nonZeroCount++;
            }
            if(ary[i][j - 1] > 0){
                nonZeroAry[nonZeroCount] = ary[i][j - 1];
                nonZeroCount++;
            }
            if(connectness == 8){
                if(ary[i - 1][j - 1] > 0){
                    nonZeroAry[nonZeroCount] = ary[i - 1][j - 1];
                    nonZeroCount++;
                }
                if(ary[i - 1][j + 1] > 0){
                    nonZeroAry[nonZeroCount] = ary[i - 1][j + 1];
                    nonZeroCount++;
                }
            }
            if(nonZeroCount <= 0)
                return 0;
            minLabel = nonZeroAry[0];
            diffFlag = false;
            int index = 1;
            while(index < nonZeroCount){
                if(minLabel != nonZeroAry[index])
                    diffFlag = true;
                if(minLabel > nonZeroAry[index])
                    minLabel = nonZeroAry[index];
                index++;
            }
            return minLabel;
        }
        void minus1D(int nonZeroAry[]){
            for(int i = 0; i < numNb; i++)
                nonZeroAry[i] = -1;
        }
        void updateEQ(int* EQAry, int nonZeroAry[], int minLabel, int nonZeroCount){
            for(int index = 0; index < nonZeroCount && nonZeroAry[index] != -1; index++)
                EQAry[nonZeroAry[index]] = minLabel;
        }
        void connectPass2(int connectness, int** ary, int nonZeroAry[]){
            int minLabel;
            bool diffFlag = false;
            int nonZeroCount = 0;
            for(int i = numRows; i > 0; i--)
                for(int j = numCols; j > 0; j--)
                    if(ary[i][j] > 0){
                        minLabel = loadNonZeroPass2(ary, connectness, i, j, nonZeroAry, diffFlag, nonZeroCount);
                        if(minLabel != ary[i][j])
                            ary[i][j] = minLabel;
                        if(diffFlag == true)
                            updateEQ(EQAry, nonZeroAry, minLabel, nonZeroCount);
                    }
        }
        int loadNonZeroPass2(int** ary, int connectness, int i, int j, int nonZeroAry[], bool &diffFlag, int &nonZeroCount){
            minus1D(nonZeroAry);
            int minLabel;
            nonZeroCount = 0;
        
            nonZeroAry[nonZeroCount] = ary[i][j];
            nonZeroCount++;
            if(ary[i+1][j] > 0){
                nonZeroAry[nonZeroCount] = ary[i+1][j];
                nonZeroCount++;
            }
            if(ary[i][j+1] > 0){
                nonZeroAry[nonZeroCount] = ary[i][j+1];
                nonZeroCount++;
            }
            if(connectness == 8){
                if(ary[i+1][j-1] > 0){
                    nonZeroAry[nonZeroCount] = ary[i+1][j-1];
                    nonZeroCount++;
                }
                if(ary[i+1][j+1] > 0){
                    nonZeroAry[nonZeroCount] = ary[i+1][j+1];
                    nonZeroCount++;
                }
            }
            minLabel = nonZeroAry[0];
            diffFlag = false;
            int index = 1;
            while(index < nonZeroCount){
                if(minLabel != nonZeroAry[index])
                    diffFlag = true;
                if(minLabel > nonZeroAry[index])
                    minLabel = nonZeroAry[index];
                index++;
            }
            return minLabel;
        }
        void manageEQAry(int* EQAry, int newLabel){
            trueNumCC = 0;
            int index = 1;
            while(index <= newLabel){
                if(index != EQAry[index])
                    EQAry[index] = EQAry[EQAry[index]];
                else{
                    trueNumCC++;
                    EQAry[index] = trueNumCC;
                }
                index++;
            }
        }
        void connectPass3(int** ary, int** ccproperty){
            newMin = minVal;
            newMax = trueNumCC;
            int temp = 0;
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    ary[i][j] = EQAry[ary[i][j]];
                    temp = ary[i][j];
                    if(temp > 0){
                        ccproperty[temp-1][1]++;
                        if(i < ccproperty[temp-1][2])
                            ccproperty[temp-1][2] = i;
                        if(j < ccproperty[temp-1][3])
                            ccproperty[temp-1][3] = j;
                        if(i > ccproperty[temp-1][4])
                            ccproperty[temp-1][4] = i;
                        if(j > ccproperty[temp-1][5])
                            ccproperty[temp-1][5] = j;
                    }
                }
            }
            for(int i =0; i < newMax; i++)
                ccproperty[i][0] = i+1;
            
        }
        void prettyPrint(ofstream &prettyPrintFile){
            prettyPrintFile << "zeroFramedAry" << endl;
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    if(zeroFramedAry[i][j] > 0)
                        prettyPrintFile << zeroFramedAry[i][j] << " ";
                    else
                        prettyPrintFile << "  ";
                }
                prettyPrintFile << endl;
            }
            prettyPrintFile << endl;
        }
        void printEQAry(int newLabel, ofstream &prettyPrintFile){
            prettyPrintFile <<"EQAry" << endl;
            prettyPrintFile << "index: ";
            for(int i = 0; i < (numRows * numCols) / 2; i++){
                if(EQAry[i] <= newLabel){
                    prettyPrintFile << i << " ";
                }
            }
            prettyPrintFile << endl;
            prettyPrintFile << "value: ";
            for(int i = 0; i < (numRows * numCols) / 2; i++){
                if(EQAry[i] <= newLabel)
                    prettyPrintFile << EQAry[i] << " ";
            }
            prettyPrintFile << endl;
            prettyPrintFile << endl;
        }
        void printImg(ofstream &labelFile){
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    if(zeroFramedAry[i][j] > 0)
                        labelFile << zeroFramedAry[i][j] << " ";
                    else
                        labelFile << "  ";
                }
                labelFile << endl;
            }
            labelFile << endl;
        }
        void printCCproperty(ofstream &propertyFile){
            propertyFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
            propertyFile << newMax << endl;
            for(int i = 0; i < newMax; i++){
                propertyFile << ccproperty[i][0] << endl;
                propertyFile << ccproperty[i][1] << endl;
                propertyFile << ccproperty[i][2] << " " << ccproperty[i][3] << endl;
                propertyFile << ccproperty[i][4] << " " << ccproperty[i][5] << endl;
                
            }
        }
        void drawBoxes(int** zeroFramedAry, int** ccproperty){
            int index = 1;
            int minRow = 0;
            int minCol = 0;
            int maxRow = 0;
            int maxCol = 0;
            int label = 0;
            while(index <= trueNumCC){
                minRow = ccproperty[index-1][2];
                minCol = ccproperty[index-1][3];
                maxRow = ccproperty[index-1][4];
                maxCol = ccproperty[index-1][5];
                label = ccproperty[index-1][0];
                for(int i = minRow; i <= maxRow; i++){
                    zeroFramedAry[i][minCol] = label;
                    zeroFramedAry[i][maxCol] = label;
                }
                for(int i = minCol; i <= maxCol; i++){
                    zeroFramedAry[minRow][i] = label;
                    zeroFramedAry[maxRow][i] = label;
                }
                index++;
            }
        }
    
};

int main(int argc, char * argv[]) {
    ifstream inFile;
    ofstream prettyPrintFile, labelFile, propertyFile;
    int connectness;
    
    inFile.open(argv[1]);
    prettyPrintFile.open(argv[3]);
    labelFile.open(argv[4]);
    propertyFile.open(argv[5]);
    CCLabel *cclabel = new CCLabel();
    inFile >> cclabel->numRows >> cclabel->numCols >> cclabel->minVal >> cclabel->maxVal;
    
    cclabel->zeroFramedAry = new int* [cclabel->numRows + 2];
    for(int i = 0; i < cclabel->numRows + 2; i++)
        cclabel->zeroFramedAry[i] = new int [cclabel->numCols + 2];
    
    cclabel->EQAry = new int [(cclabel->numRows * cclabel->numCols)/2];
    for(int i = 0; i < (cclabel->numRows * cclabel->numCols)/2; i++)
        cclabel->EQAry[i] = i;
    
    
    
    cclabel->zero2D(cclabel->zeroFramedAry);
    cclabel->loadImage(inFile, cclabel->zeroFramedAry);
    
    
    string c;
    c = argv[2];
    connectness = atoi(c.c_str());
    
    cclabel->newLabel = cclabel->connectPass1(connectness, cclabel->zeroFramedAry, cclabel->nonZeroNeighborAry);
    cclabel->prettyPrint(prettyPrintFile);
    cclabel->printEQAry(cclabel->newLabel, prettyPrintFile);
    
    cclabel->connectPass2(connectness, cclabel->zeroFramedAry, cclabel->nonZeroNeighborAry);
    cclabel->prettyPrint(prettyPrintFile);
    cclabel->printEQAry(cclabel->newLabel, prettyPrintFile);
    
    cclabel->manageEQAry(cclabel->EQAry, cclabel->newLabel);
    cclabel->printEQAry(cclabel->trueNumCC, prettyPrintFile);
    
    cclabel->ccproperty = new int* [cclabel->trueNumCC + 1];
    for(int i = 0; i < cclabel->trueNumCC + 1; i++)
        cclabel->ccproperty[i] = new int [6];
    for(int i = 0; i < cclabel->trueNumCC + 1; i++){
        cclabel->ccproperty[i][2] = 10000;
        cclabel->ccproperty[i][3] = 10000;
    }
    
    cclabel->connectPass3(cclabel->zeroFramedAry, cclabel->ccproperty);
    cclabel->prettyPrint(prettyPrintFile);
    cclabel->printEQAry(cclabel->trueNumCC, prettyPrintFile);
    
    labelFile << cclabel->numRows << " " << cclabel->numCols << " " <<  cclabel->newMin << " " << cclabel->newMax << endl;
    
    cclabel->printImg(labelFile);
    cclabel->printCCproperty(propertyFile);
    
    cclabel->drawBoxes(cclabel->zeroFramedAry, cclabel->ccproperty);
    cclabel->prettyPrint(prettyPrintFile);
    
    for(int i = 0; i < cclabel->numRows + 2; i++)
        delete[] cclabel->zeroFramedAry[i];
    delete[] cclabel->zeroFramedAry;
    
    for(int i = 0; i < cclabel->trueNumCC; i++)
        delete[] cclabel->ccproperty[i];
    delete[] cclabel->ccproperty;
    
    delete[] cclabel->EQAry;
    
    delete cclabel;
    
    inFile.close();
    prettyPrintFile.close();
    labelFile.close();
    propertyFile.close();
    
    return 0;
}
