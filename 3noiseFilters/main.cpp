//
//  main.cpp
//  3noiseFilters
//
//  Created by yin yu lo on 2/17/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class ImageProcessing{
    public:
        int numRows, numCols, minVal, maxVal, newMin, newMax, maskRows, maskCols, maskMin, maskMax;
        int** mirrorFrameAry;
        int** avgAry;
        int** medianAry;
        int** gaussAry;
        int** maskAry;
        ImageProcessing(){
        
        }
        void loadMask(int** maskAry, ifstream &maskFile){
            while(!maskFile.eof()){
                for(int i = 0; i < maskRows; i++)
                    for(int j = 0; j < maskCols; j++)
                        maskFile >> maskAry[i][j];
            }
        }
        void loadImage(int** mirrorFrameAry, ifstream &inFile){
            while(!inFile.eof()){
                for(int i = 1; i < numRows+1; i++)
                    for(int j = 1; j < numCols+1; j++)
                        inFile >> mirrorFrameAry[i][j];
            }
        }
        void mirrorFraming(int** mirrorFrameAry){
            for(int i = 0; i < numRows+2; i++){
                mirrorFrameAry[i][0] = mirrorFrameAry[i][1];
                mirrorFrameAry[i][numCols+1] = mirrorFrameAry[i][numCols];
            }
            for(int j = 0; j < numCols+2; j++){
                mirrorFrameAry[0][j] = mirrorFrameAry[1][j];
                mirrorFrameAry[numRows+1][j] = mirrorFrameAry[numRows][j];
            }
        }
        void computeAvgImg(int** avgAry, int &newMin, int &newMax){
            int neighborAry[9];
            newMin = 9999;
            newMax = 0;
            for(int i = 1; i < numRows+1; i++){
                for(int j = 1; j < numCols+1; j++){
                    loadNeighbors(i, j, neighborAry);
                    avgAry[i][j] = avg3x3(neighborAry);
                    if(newMin > avgAry[i][j])
                        newMin = avgAry[i][j];
                    if(newMax < avgAry[i][j])
                        newMax = avgAry[i][j];
                }
            }
        }
        int avg3x3(int neighborAry[]){
            int avg = 0;
            int sum = 0;
            for(int i = 0; i < 9; i++)
                sum += neighborAry[i];
            avg = sum/9;
            return avg;
        }
        void computeMedianImg(int** medianAry, int &newMin, int &newMax){
            int neighborAry[9];
            newMin = 9999;
            newMax = 0;
            for(int i = 1; i < numRows+1; i++){
                for(int j = 1; j < numCols+1; j++){
                    loadNeighbors(i, j, neighborAry);
                    sort(neighborAry);
                    medianAry[i][j] = neighborAry[4];
                    if(newMin > medianAry[i][j])
                        newMin = medianAry[i][j];
                    if(newMax < medianAry[i][j])
                        newMax = medianAry[i][j];
                }
            }
        }
        void loadNeighbors(int i, int j, int neighborAry[]){
            int p = 0;
            for(int k = i-1; k < i+2; k++)
                for(int l = j-1; l < j+2; l++)
                    neighborAry[p++] = mirrorFrameAry[k][l];
        }
        void sort(int neighborAry[]){
            int temp,j;
            for(int i = 1; i < 9; i++){
                temp = neighborAry[i];
                for(j = i - 1; j >= 0 && neighborAry[j] > temp; j--){
                    neighborAry[j+1] = neighborAry[j];
                }
                neighborAry[j+1] = temp;
            }
        }
        void computeGaussImg(int** gaussAry, int &newMin, int &newMax){
            int neighborAry[9];
            newMin = 9999;
            newMax = 0;
            for(int i = 1; i < numRows+1; i++){
                for(int j = 1; j < numCols+1; j++){
                    gaussAry[i][j] = convolution(i, j, gaussAry, maskAry);
                    if(newMin > gaussAry[i][j])
                        newMin = gaussAry[i][j];
                    if(newMax < gaussAry[i][j])
                        newMax = gaussAry[i][j];
                }
            }
        }
        int convolution(int i, int j, int** gaussAry, int** maskAry){
            int sum = 0;
            int totalWeight = 0;
            int x = 0;
            for(int k = 0; k < 3; k++){
                for(int l = 0; l < 3; l++){
                    sum += maskAry[k][l] * mirrorFrameAry[k+i-1][l+j-1];
                    totalWeight += maskAry[k][l];
                }
            }
            x = sum/totalWeight;
            return x;
        }
        void outputAryToFile(int** ary, ofstream &outFile, int newMin, int newMax){
            outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
            for(int i = 1; i < numRows+1; i++){
                for(int j = 1; j < numCols+1; j++){
                    outFile << ary[i][j] << " ";
                }
                outFile << endl;
            }
        }
};

int main(int argc, char * argv[]) {
    ifstream inFile, maskFile;
    ofstream avgOutImg, medianOutImg, gaussOutImg;
    
    inFile.open(argv[1]);
    maskFile.open(argv[2]);
    avgOutImg.open(argv[3]);
    medianOutImg.open(argv[4]);
    gaussOutImg.open(argv[5]);
    
    ImageProcessing *image = new ImageProcessing();
    inFile >> image->numRows >> image->numCols >> image->minVal >> image->maxVal;
    maskFile >> image->maskRows >> image->maskCols >> image->maskMin >> image->maskMax;
    
    image->mirrorFrameAry = new int* [image->numRows+2];
    for(int i = 0; i < image->numRows+2; i++)
        image->mirrorFrameAry[i] = new int [image->numCols+2];
    
    image->avgAry = new int* [image->numRows+2];
    for(int i = 0; i < image->numRows+2; i++)
        image->avgAry[i] = new int [image->numCols+2];
    
    image->medianAry = new int* [image->numRows+2];
    for(int i = 0; i < image->numRows+2; i++)
        image->medianAry[i] = new int [image->numCols+2];
    
    image->gaussAry = new int* [image->numRows+2];
    for(int i = 0; i < image->numRows+2; i++)
        image->gaussAry[i] = new int [image->numCols+2];
    
    image->maskAry = new int* [image->maskRows];
    for(int i = 0; i < image->maskRows; i++)
        image->maskAry[i] = new int [image->maskCols];
    
    image->loadMask(image->maskAry, maskFile);
    image->loadImage(image->mirrorFrameAry, inFile);
    image->mirrorFraming(image->mirrorFrameAry);
   
    image->computeAvgImg(image->avgAry, image->newMin, image->newMax);
    image->outputAryToFile(image->avgAry, avgOutImg, image->newMin, image->newMax);
    
    image->computeMedianImg(image->medianAry, image->newMin, image->newMax);
    image->outputAryToFile(image->medianAry, medianOutImg, image->newMin, image->newMax);
    
    image->computeGaussImg(image->gaussAry, image->newMin, image->newMax);
    image->outputAryToFile(image->gaussAry, gaussOutImg, image->newMin, image->newMax);
    
    for(int i = 0; i < image->numRows+2; i++)
        delete[] image->mirrorFrameAry[i];
    delete[] image->mirrorFrameAry;
    
    for(int i = 0; i < image->numRows+2; i++)
        delete[] image->avgAry[i];
    delete[] image->avgAry;
    
    for(int i = 0; i < image->numRows+2; i++)
        delete[] image->medianAry[i];
    delete[] image->medianAry;
    
    for(int i = 0; i < image->numRows+2; i++)
        delete[] image->gaussAry[i];
    delete[] image->gaussAry;
    
    for(int i = 0; i < image->maskRows; i++)
        delete[] image->maskAry[i];
    delete[] image->maskAry;
    
    delete image;
    
    inFile.close();
    maskFile.close();
    avgOutImg.close();
    medianOutImg.close();
    gaussOutImg.close();
    
    return 0;
}
