//
//  main.cpp
//  Thinning
//
//  Created by yin yu lo on 3/28/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class ThinningSkeleton{
    public:
        int numRows, numCols, minVal, maxVal, changeflag, cycleCount;
        bool isSekeleton;
        int** firstAry;
        int** secondAry;
    
        ThinningSkeleton(){
        
        }
        void zeroFrame(int** ary){
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
        void doThinning(int** ary1, int** ary2, int &changeFlag, bool &isSekeleton){
            isSekeleton = true;
            northThinning(ary1, ary2, changeFlag, isSekeleton);
            copyArys(ary1, ary2);
            
            southThinning(ary1, ary2, changeFlag, isSekeleton);
            copyArys(ary1, ary2);
            
            westThinning(ary1, ary2, changeFlag, isSekeleton);
            copyArys(ary1, ary2);
            
            eastThinning(ary1, ary2, changeFlag, isSekeleton);
            copyArys(ary1, ary2);
        }
        void northThinning(int** ary1, int** ary2, int &changeFlag, bool &isSekeleton){
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    ary2[i][j] = ary1[i][j];
                    if(ary1[i][j] > 0 && ary1[i - 1][j] <= 0){
                        if(check3n4ConditionsForNorthandSouth(ary1, i, j)){
                            ary2[i][j] = 0;
                            changeFlag++;
                            isSekeleton = false;
                        }
                    }
                }
            }
        }
        void southThinning(int** ary1, int** ary2, int &changeFlag, bool &isSekeleton){
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    ary2[i][j] = ary1[i][j];
                    if(ary1[i][j] > 0 && ary1[i + 1][j] <= 0){
                        if(check3n4ConditionsForNorthandSouth(ary1, i, j)){
                            ary2[i][j] = 0;
                            changeFlag++;
                            isSekeleton = false;
                        }
                    }
                }
            }
        }
        void westThinning(int** ary1, int** ary2, int &changeFlag, bool &isSekeleton){
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    ary2[i][j] = ary1[i][j];
                    if(ary1[i][j] > 0 && ary1[i][j - 1] <= 0){
                        if(check3n4ConditionsForWestandEast(ary1, i, j)){
                            ary2[i][j] = 0;
                            changeFlag++;
                            isSekeleton = false;
                        }
                    }
                }
            }
        }
        void eastThinning(int** ary1, int** ary2, int &changeFlag, bool &isSekeleton){
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    ary2[i][j] = ary1[i][j];
                    if(ary1[i][j] > 0 && ary1[i][j + 1] <= 0){
                        if(check3n4ConditionsForWestandEast(ary1, i, j)){
                            ary2[i][j] = 0;
                            changeFlag++;
                            isSekeleton = false;
                        }
                    }
                }
            }
        }
        void copyArys(int** ary1, int** ary2){
            for(int i = 0; i < numRows + 2; i++)
                for(int j = 0; j < numCols + 2; j++)
                    ary1[i][j] = ary2[i][j];
        }
        /*bool check3n4ConditionsForNorthandSouth(int** ary, int i, int j){
            bool result = false;
            int totalCount = 0;
            int count = 0;
            for(int r = i-1; r < i+2; r++)
                for(int c = j-1; c < j+2; c++)
                    if(ary[r][c] > 0)
                        totalCount++;
            if(ary[i-1][j] == 1)
                count++;
            if(ary[i][j-1] == 1)
                count++;
            if(ary[i+1][j] == 1)
                count++;
            if(ary[i][j+1] == 1)
                count++;
            totalCount = totalCount-1;
            if(totalCount < 4)
                result = false;
            if(totalCount >= 4){
                if(count == 1)
                    result = false;
                else if(count == 2){
                    if(ary[i][j-1] == 1 && ary[i][j+1] == 1)
                        result = false;
                    else if(ary[i-1][j] == 1 && ary[i+1][j] == 1)
                        result = false;
                    else if(ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i-1][j+1] == 1)
                        result = false;
                    else if(ary[i][j+1] == 1 && ary[i+1][j] == 1 && ary[i-1][j-1] == 1)
                        result = false;
                    else if(ary[i-1][j] == 1 && ary[i][j+1] == 1 && ary[i+1][j-1] == 1)
                        result = false;
                    else if(ary[i][j-1] == 1 && ary[i-1][j] == 1 && ary[i+1][j+1] == 1)
                        result = false;
                    else
                        result = true;
                }
                else if(count == 3)
                    result = true;
            }
            return result;
        }
        bool check3n4ConditionsForWestandEast(int** ary, int i, int j){
            bool result = false;
            int totalCount = 0;
            int count = 0;
            for(int r = i-1; r < i+2; r++)
                for(int c = j-1; c < j+2; c++)
                    if(ary[r][c] > 0)
                        totalCount++;
            if(ary[i-1][j] == 1)
                count++;
            if(ary[i][j-1] == 1)
                count++;
            if(ary[i+1][j] == 1)
                count++;
            if(ary[i][j+1] == 1)
                count++;
            totalCount = totalCount-1;
            if(totalCount < 3)
                result = false;
            if(totalCount >= 3){
                if(count == 1)
                    if(ary[i-1][j-1] == 1 && ary[i-1][j] == 1 && ary[i-1][j+1] == 1)
                        result = true;
                    else if(ary[i+1][j-1] == 1 && ary[i+1][j] == 1 && ary[i+1][j+1] == 1)
                        result = true;
                    else if(ary[i-1][j-1] == 1 && ary[i][j-1] == 1 && ary[i+1][j-1] == 1)
                        result = true;
                    else if(ary[i-1][j+1] == 1 && ary[i][j+1] == 1 && ary[i+1][j+1] == 1)
                        result = true;
                    else
                        result = false;
                else if(count == 2){
                    if(ary[i][j-1] == 1 && ary[i][j+1] == 1)
                        result = false;
                    else if(ary[i-1][j] == 1 && ary[i+1][j] == 1)
                        result = false;
                    else if(ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i-1][j+1] == 1)
                        result = false;
                    else if(ary[i][j+1] == 1 && ary[i+1][j] == 1 && ary[i-1][j-1] == 1)
                        result = false;
                    else if(ary[i-1][j] == 1 && ary[i][j+1] == 1 && ary[i+1][j-1] == 1)
                        result = false;
                    else if(ary[i][j-1] == 1 && ary[i-1][j] == 1 && ary[i+1][j+1] == 1)
                        result = false;
                    else
                        result = true;
                }
                else if(count == 3)
                    result = true;
            }
            return result;
        }*/
        bool check3n4ConditionsForWestandEast(int** ary, int i, int j){
            int totalCount = 0;
            for(int r = i-1; r < i+2; r++)
                for(int c = j-1; c < j+2; c++)
                    if(ary[r][c] > 0)
                        totalCount++;
            totalCount = totalCount-1;
            if(totalCount < 3)
                return false;
            if(totalCount >= 3){
                if(((ary[i-1][j-1] == 1 && ary[i-1][j] == 1 && ary[i-1][j+1] == 1) && (ary[i+1][j-1] == 0 && ary[i+1][j] == 0 && ary[i+1][j+1] == 0)) || ((ary[i+1][j-1] == 1 && ary[i+1][j] == 1 && ary[i+1][j+1] == 1) && (ary[i-1][j-1] == 0 && ary[i-1][j] == 0 && ary[i-1][j+1] == 0))|| ((ary[i-1][j-1] == 1 && ary[i][j-1] == 1 && ary[i+1][j-1] == 1) && (ary[i-1][j+1] == 0 && ary[i][j+1] == 0 && ary[i+1][j+1] == 0)) || ((ary[i-1][j+1] == 1 && ary[i][j+1] == 1 && ary[i+1][j+1] == 1) && (ary[i-1][j-1] == 0 && ary[i][j-1] == 0 && ary[i+1][j-1] == 0)) || (ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i-1][j+1] == 0) || (ary[i][j+1] == 1 && ary[i+1][j] == 1 && ary[i-1][j-1] == 0) || (ary[i-1][j] == 1 && ary[i][j+1] == 1 && ary[i+1][j-1] == 0) || (ary[i][j-1] == 1 && ary[i-1][j] == 1 && ary[i+1][j+1] == 0) || (ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i-1][j] == 1) || (ary[i][j+1] == 1 && ary[i+1][j] == 1 && ary[i-1][j] == 1) || (ary[i-1][j] == 1 && ary[i][j+1] == 1 && ary[i][j-1] == 1) || (ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i][j+1] == 1))
                    return true;
            }
            return false;
        }
        bool check3n4ConditionsForNorthandSouth(int** ary, int i, int j){
            int totalCount = 0;
            for(int r = i-1; r < i+2; r++)
                for(int c = j-1; c < j+2; c++)
                    if(ary[r][c] > 0)
                        totalCount++;
            totalCount = totalCount-1;
            if(totalCount < 4)
                return false;
            if(totalCount >= 4){
                if(((ary[i-1][j-1] == 1 && ary[i-1][j] == 1 && ary[i-1][j+1] == 1) && (ary[i+1][j-1] == 0 && ary[i+1][j] == 0 && ary[i+1][j+1] == 0)) || ((ary[i+1][j-1] == 1 && ary[i+1][j] == 1 && ary[i+1][j+1] == 1) && (ary[i-1][j-1] == 0 && ary[i-1][j] == 0 && ary[i-1][j+1] == 0))|| ((ary[i-1][j-1] == 1 && ary[i][j-1] == 1 && ary[i+1][j-1] == 1) && (ary[i-1][j+1] == 0 && ary[i][j+1] == 0 && ary[i+1][j+1] == 0)) || ((ary[i-1][j+1] == 1 && ary[i][j+1] == 1 && ary[i+1][j+1] == 1) && (ary[i-1][j-1] == 0 && ary[i][j-1] == 0 && ary[i+1][j-1] == 0)) || (ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i-1][j+1] == 0) || (ary[i][j+1] == 1 && ary[i+1][j] == 1 && ary[i-1][j-1] == 0) || (ary[i-1][j] == 1 && ary[i][j+1] == 1 && ary[i+1][j-1] == 0) || (ary[i][j-1] == 1 && ary[i-1][j] == 1 && ary[i+1][j+1] == 0) || (ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i-1][j] == 1) || (ary[i][j+1] == 1 && ary[i+1][j] == 1 && ary[i-1][j] == 1) || (ary[i-1][j] == 1 && ary[i][j+1] == 1 && ary[i][j-1] == 1) || (ary[i][j-1] == 1 && ary[i+1][j] == 1 && ary[i][j+1] == 1))
                    return true;
            }
            return false;
        }
        
        void prettyPrint(int** ary, ofstream &outFile){
            for(int i = 1; i < numRows + 1; i++){
                for(int j = 1; j < numCols + 1; j++){
                    if(ary[i][j] > 0)
                        outFile << ary[i][j] << " ";
                    else
                        outFile << "  ";
                }
                outFile << endl;
            }
        }
        
};

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile1, outFile2;
    
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    
    ThinningSkeleton *thinning = new ThinningSkeleton();
    
    inFile >> thinning->numRows >> thinning->numCols >> thinning->minVal >> thinning->maxVal;
    outFile1 << thinning->numRows << " " << thinning->numCols << " " << thinning->minVal << " " << thinning->maxVal << endl;
    
    thinning->firstAry = new int* [thinning->numRows + 2];
    for(int i = 0; i < thinning->numRows + 2; i++)
        thinning->firstAry[i] = new int [thinning->numCols + 2];
    
    thinning->secondAry = new int* [thinning->numRows + 2];
    for(int i = 0; i < thinning->numRows + 2; i++)
        thinning->secondAry[i] = new int [thinning->numCols + 2];
    
    thinning->zeroFrame(thinning->firstAry);
    thinning->zeroFrame(thinning->secondAry);
    
    thinning->loadImage(inFile, thinning->firstAry);
    outFile2 << "prettyPrint of input image " << endl;
    thinning->prettyPrint(thinning->firstAry, outFile2);
    
    thinning->changeflag = 0;
    thinning->cycleCount = 0;
    //thinning->doThinning(thinning->firstAry, thinning->secondAry, thinning->changeflag);
    //outFile2 << "result of thinning : cycle – " << thinning->cycleCount << endl;
    //thinning->prettyPrint(thinning->firstAry, outFile2);
    //cout << thinning->changeflag << endl;
    while(thinning->changeflag >= 0 && !thinning->isSekeleton){
        thinning->doThinning(thinning->firstAry, thinning->secondAry, thinning->changeflag, thinning->isSekeleton);
        thinning->cycleCount++;
        //if(!thinning->isSekeleton){
            outFile2 << "result of thinning : cycle – " << thinning->cycleCount << endl;
            thinning->prettyPrint(thinning->firstAry, outFile2);
        //}
        
    }
    
    for(int i = 1; i < thinning->numRows + 1; i++){
        for(int j = 1; j < thinning->numCols + 1; j++)
            outFile1 << thinning->firstAry[i][j] << " ";
        outFile1 << endl;
    }
    
    
    for(int i = 0; i < thinning->numRows + 2; i++)
        delete[] thinning->firstAry[i];
    delete[] thinning->firstAry;
    
    for(int i = 0; i < thinning->numRows + 2; i++)
        delete[] thinning->secondAry[i];
    delete[] thinning->secondAry;
    
    delete thinning;
    
    inFile.close();
    outFile1.close();
    outFile2.close();
    
    return 0;
}
