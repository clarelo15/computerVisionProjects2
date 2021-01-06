//
//  main.cpp
//  textLineDetection
//
//  Created by yin yu lo on 4/27/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class ImagePP{
public:
    int numRows, numCols, minVal, maxVal;
    class Box{
    public:
        int minR, minC, maxR, maxC;
    };
    class BoxNode{
    public:
        int boxType;
        Box bBox;
        BoxNode* next;
    };
    class BoxQ{
    public:
        BoxNode* qFront;
        BoxNode* qBack;
        BoxQ(){
            qFront = new BoxNode;
            qBack = new BoxNode;
            qBack = qFront;
        }
        void insert(BoxNode* newBoxNode){
            qBack->next = newBoxNode;
            qBack = newBoxNode;
        }
    };
    int** imgAry;
    int thrVal, direction;
    int* hPP;
    int* vPP;
    int* hPPbin;
    int* vPPbin;
    int* hPPMorph;
    int* vPPMorph;
    int* hppCC;
    int* vppCC;
    int hPPruns, vPPruns;
    BoxNode* boxAry;
    BoxQ boxQ;
    
    void zeroFramed(int** ary){
        for(int i = 0; i < numRows+2; i++)
            for(int j = 0; j < numCols+2; j++)
                ary[i][j] = 0;
    }
    void loadImage(int** ary, ifstream& inFile){
        while (!inFile.eof()) {
            for(int i = 1; i < numRows+1; i++)
                for(int j = 1; j < numCols+1; j++)
                    inFile >> ary[i][j];
        }
    }
    void computeHPP(int** ary, int* aryH){
        for(int i = 0; i < numRows+2; i++)
            for(int j = 0; j < numCols+2; j++)
                if(ary[i][j] > 0)
                    aryH[i]++;
    }
    void computeVPP(int** ary, int* aryV){
        for(int i = 0; i < numRows+2; i++)
            for(int j = 0; j < numCols+2; j++)
                if(ary[i][j] > 0)
                    aryV[j]++;
    }
    void arrayToFileH(int*ary, ofstream& outFile){
        for(int i = 0; i < numRows+2; i++)
            outFile << i << " " << ary[i] << endl;
        outFile << endl << endl;
    }
    void arrayToFileV(int*ary, ofstream& outFile){
        for(int i = 0; i < numCols+2; i++)
            outFile << i << " " << ary[i] << endl;
        outFile << endl << endl;
    }
    void thresholdHPP(int* ary, int thrval, int* binHPP){
        for(int i = 0; i < numRows+2; i++){
            if(ary[i] < thrVal)
                binHPP[i] = 0;
            else
                binHPP[i] = 1;
        }
    }
    void thresholdVPP(int* ary, int thrval, int* binVPP){
        for(int i = 0; i < numCols+2; i++){
            if(ary[i] < thrVal)
                binVPP[i] = 0;
            else
                binVPP[i] = 1;
        }
    }
    void morphClosing(int* ary, int size, int* arybin){
        //int structElem[3] = {1,1,1};
        for(int i = 1; i < size-1; i++){
            if(arybin[i] == 0 && arybin[i-1] == 1 && arybin[i+1] == 1){
                ary[i-1] = 1;
                ary[i] = 1;
                ary[i+1] = 1;
            }
            else if(arybin[i] == 0 && arybin[i-1] == 1 && arybin[i+1] == 0 && arybin[i+2] == 1){
                ary[i-1] = 1;
                ary[i] = 1;
                ary[i+1] = 1;
            }
            else if(arybin[i] == 1)
                ary[i] = arybin[i];
        }
    }
    void readingDirection(int* hppMorph, int* vppMorph, ofstream& outFile, int* hppCC, int* vppCC){
        int factor = 3;
        int i = 0;
        int index = 0;
        while(i < numRows+2){
            if(hppMorph[i] == 1){
                hppCC[index++] = i;
                for(int j = i+1; j < numRows+2; j++)
                    if(hppMorph[j] == 0){
                        hPPruns++;
                        i = j - 1;
                        hppCC[index++] = i;
                        break;
                    }
            }
            i++;
        }
        int k = 0;
        index = 0;
        while(k < numCols+2){
            if(vppMorph[k] == 1){
                vppCC[index++] = k;
                for(int l = k+1; l < numCols+2; l++)
                    if(vppMorph[l] == 0){
                        vPPruns++;
                        k = l - 1;
                        vppCC[index++] = k;
                        break;
                    }
            }
            k++;
        }
        cout << hPPruns << endl;
        cout << vPPruns << endl;
        if(hPPruns >= factor * vPPruns){
            outFile << "The reading direction is Horizontal " << endl << endl;
            direction = 1;
        }
        else if(vPPruns >= factor * hPPruns){
            outFile << "The reading direction is Vertical " << endl << endl;
            direction = 2;
        }
        else
            outFile << "Can not determine the reading direction " << endl << endl;
    }
    
    void findLineBoxes(int* ary1, int* ary2, BoxQ &boxQ){
        int i = 0;
        int j = 0;
        int index = 0;
        bool isCycle = false;
        bool isCycle2 = false;
        while(index < hPPruns*vPPruns){
            if(ary1[i] > 0 && isCycle == false){
                boxAry[index].bBox.minR = ary1[i++];
                boxAry[index].bBox.maxR = ary1[i++];
                if((ary2[j] > 0 && isCycle2 == false) || j == 0){
                    boxAry[index].bBox.minC = ary2[j++];
                    boxAry[index].bBox.maxC = ary2[j++];
                    if(ary1[i] > 0)
                        j=j-2;
                }
                else if (ary2[j] == 0 || isCycle2){
                    boxAry[index].bBox.maxC = ary2[--j];
                    boxAry[index].bBox.minC = ary2[--j];
                    isCycle2 = true;
                }
            }
            else if (ary1[i] == 0 || isCycle){
                boxAry[index].bBox.maxR = ary1[--i];
                boxAry[index].bBox.minR = ary1[--i];
                isCycle = true;
                if((ary2[j] > 0 && isCycle2 == false) || j == 0){
                    boxAry[index].bBox.minC = ary2[j++];
                    boxAry[index].bBox.maxC = ary2[j++];
                    if(ary1[i] > 0)
                        j=j-2;
                }
                else if (ary2[j] == 0 || isCycle2){
                    boxAry[index].bBox.maxC = ary2[--j];
                    boxAry[index].bBox.minC = ary2[--j];
                    isCycle2 = true;
                }
            }
            /*if(ary2[j] > 0 && isCycle2 == false){
                boxAry[index].bBox.minC = ary2[j++];
                boxAry[index].bBox.maxC = ary2[j++];
            }
            else if (ary2[j] == 0 || isCycle2){
                boxAry[index].bBox.maxC = ary2[--j];
                boxAry[index].bBox.minC = ary2[--j];
                //isCycle2 = true;
            }*/
            boxQ.insert(&boxAry[index]);
            index++;
        }
    }
    void printBoxInfo(ofstream& outFile){
        int index = 0;
        int boxType = 3;
        outFile << "Each bounding box info" << endl;
        while(index < hPPruns*vPPruns){
            outFile << boxType << endl;
            outFile << boxAry[index].bBox.minR << " " << boxAry[index].bBox.minC << " " << boxAry[index].bBox.maxR << " " << boxAry[index].bBox.maxC << endl;
            index++;
        }
    }
    void drawBoxes(int** imgAry){
        /*int index = 0;
        while(index < hPPruns*vPPruns){
            for(int i = boxAry[index].bBox.minR; i <= boxAry[index].bBox.maxR; i++){
                imgAry[i][boxAry[index].bBox.minC] = 1;
                imgAry[i][boxAry[index].bBox.maxC] = 1;
            }
            for(int j = boxAry[index].bBox.minC; j <= boxAry[index].bBox.maxC; j++){
                imgAry[boxAry[index].bBox.minR][j] = 1;
                imgAry[boxAry[index].bBox.maxR][j] = 1;
            }
            index++;
        }*/
        BoxNode *boxNode;
        boxNode = boxQ.qFront->next;
        while(boxNode){
            for(int i = boxNode->bBox.minR; i <= boxNode->bBox.maxR; i++){
                imgAry[i][boxNode->bBox.minC] = 1;
                imgAry[i][boxNode->bBox.maxC] = 1;
            }
            for(int j = boxNode->bBox.minC; j <= boxNode->bBox.maxC; j++){
                imgAry[boxNode->bBox.minR][j] = 1;
                imgAry[boxNode->bBox.maxR][j] = 1;
            }
            boxNode = boxNode->next;
        }
    }
    void printBoxQueue(int** ary, ofstream& outFile){
        for(int i = 0; i < numRows+2; i++){
            for(int j = 0; j < numCols+2; j++){
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
    
    ImagePP img;
    
    inFile.open(argv[1]);
    
    string a;
    a = argv[2];
    img.thrVal = atoi(a.c_str());
    
    outFile1.open(argv[3]);
    outFile2.open(argv[4]);
    
    inFile >> img.numRows >> img.numCols >> img.minVal >> img.maxVal;
    
    img.imgAry = new int*[img.numRows+2];
    for(int i = 0; i < img.numRows+2; i++)
        img.imgAry[i] = new int[img.numCols+2];
    
    img.hPP = new int[img.numRows+2];
    img.vPP = new int[img.numCols+2];
    img.hPPbin = new int[img.numRows+2];
    img.vPPbin = new int[img.numCols+2];
    img.hPPMorph = new int[img.numRows+2];
    img.vPPMorph = new int[img.numCols+2];
    img.hppCC = new int[img.numRows+2];
    img.vppCC = new int[img.numCols+2];
    
    img.zeroFramed(img.imgAry);
    img.loadImage(img.imgAry, inFile);
    
    img.computeHPP(img.imgAry, img.hPP);
    img.computeVPP(img.imgAry, img.vPP);
    outFile2 << "Horizontal Projection Profile" << endl;
    img.arrayToFileH(img.hPP, outFile2);
    outFile2 << "Vertical Projection Profile" << endl;
    img.arrayToFileV(img.vPP, outFile2);
    
    img.thresholdHPP(img.hPP, img.thrVal, img.hPPbin);
    img.thresholdVPP(img.vPP, img.thrVal, img.vPPbin);
    outFile2 << "Horizontal Projection Profile (Binary)" << endl;
    img.arrayToFileH(img.hPPbin, outFile2);
    outFile2 << "Vertical Projection Profile (Binary)" << endl;
    img.arrayToFileV(img.vPPbin, outFile2);
    
    img.morphClosing(img.hPPMorph, img.numRows+2, img.hPPbin);
    img.morphClosing(img.vPPMorph, img.numCols+2, img.vPPbin);
    
    img.arrayToFileH(img.hPPMorph, outFile2);
    img.arrayToFileV(img.vPPMorph, outFile2);
    
    img.readingDirection(img.hPPMorph, img.vPPMorph, outFile2, img.hppCC, img.vppCC);
    img.boxAry = new ImagePP::BoxNode[img.hPPruns*img.vPPruns];
    
    for(int i = 0; i < img.numRows+2; i++)
        cout << img.hppCC[i] << " ";
    cout << endl;
    for(int i = 0; i < img.numCols+2; i++)
        cout << img.vppCC[i] << " ";
    cout << endl;
    
    img.findLineBoxes(img.hppCC, img.vppCC, img.boxQ);
    
    img.printBoxInfo(outFile2);
    img.drawBoxes(img.imgAry);
    img.printBoxQueue(img.imgAry, outFile1);
    
    for(int i = 0; i < img.numRows+2; i++)
        delete [] img.imgAry[i];
    delete [] img.imgAry;
    
    delete [] img.hPP;
    delete [] img.vPP;
    delete [] img.hPPbin;
    delete [] img.vPPbin;
    delete [] img.hPPMorph;
    delete [] img.vPPMorph;
    delete [] img.hppCC;
    delete [] img.vppCC;
    delete [] img.boxAry;
    
    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}
