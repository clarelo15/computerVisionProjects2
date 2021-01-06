//
//  main.cpp
//  histogram
//
//  Created by yin yu lo on 2/2/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class Image{
    public:
        int numRows, numCols, minVal, maxVal, thresholdValue;
        int* histAry;
    
        Image(){
            
        }
    
        void computeHist (ifstream & inFile, int histAry[]){
            int pixel_val = 0;
            while(!inFile.eof()){
                inFile >> pixel_val;
                histAry[pixel_val]++;
            }
        }

        void printHist(int histAry[], ofstream & outFile1){
            for(int i = 0; i <= maxVal; i++){
                //outFile1 << i << " " << histAry[i] << "\t";
                outFile1 << i << " (" << histAry[i] << ") : \t";
                for(int j = 1; (j <= 60) && ( j <= histAry[i]) ; j++){
                    outFile1 << "+";
                }
                outFile1 << endl;
            }
        }

        void thresholdOp(ifstream & inFile, int thresholdValue, ofstream & outFile2){
            inFile >> numRows >> numCols >> minVal >> maxVal;
            int iRow = 0;
            int jCol = 0;
            int pixel_val = 0;
            
            while(iRow < numRows){
                while(jCol < numCols){
                    inFile >> pixel_val;
                    if(pixel_val >= thresholdValue)
                        outFile2 << 1 << " ";
                    else
                        outFile2 << 0 << " ";
                    jCol++;
                }
                outFile2 << endl;
                iRow++;
                jCol = 0;
            }
        }

        void prettyPrint (ifstream & outFile2, ofstream & outFile3){
            int iRow = 0;
            int jCol = 0;
            int pixel_val;
            outFile2 >> numRows >> numCols >> minVal >> maxVal;
            
            while(iRow < numRows){
                while(jCol < numCols){
                    outFile2 >> pixel_val;
                    if(pixel_val > 0)
                        outFile3 << pixel_val << " ";
                    else
                        outFile3 << "  ";
                    jCol++;
                }
                iRow++;
                jCol = 0;
                outFile3 << endl;
            }
        }
};


int main(int argc, char * argv[]) {
    ifstream inFile;
    ofstream outFile1, outFile2, outFile3;
    
    inFile.open(argv[1]);
    Image * image = new Image();
    inFile >> image->numRows >> image->numCols >> image->minVal >> image->maxVal;
    image->histAry = new int[image->maxVal + 1]();

    outFile1.open(argv[2]);
    outFile1 << image->numRows << " " << image->numCols << " " << image->minVal << " " << image->maxVal << endl;
    outFile2.open(argv[3]);
    outFile2 << image->numRows << " " << image->numCols << " "<< 0 << " "<< 1 << endl;
    outFile3.open(argv[4]);
    
    string a;
    a = argv[5];
    image->thresholdValue = atoi(a.c_str());
    
    image->computeHist (inFile, image->histAry);
    inFile.close();
    
    image->printHist(image->histAry, outFile1);
    
    inFile.open(argv[1]);
    image->thresholdOp(inFile, image->thresholdValue, outFile2);
    
    outFile2.close();
    ifstream outFile2_reopen;
    outFile2_reopen.open(argv[3]);
    
    image->prettyPrint (outFile2_reopen,outFile3);
    
    delete [] image->histAry;
    delete image;
    inFile.close();
    outFile2_reopen.close();
    outFile1.close();
    outFile3.close();
    
    return 0;
}


