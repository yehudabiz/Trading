#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H

#include "ipl.h"

const int HEIGHT=32+2;//+2 for white margins
const int WIDTH=16+2;

//for symmetry boxes SVM calc
const int maxboxwidth=3;
const int maxboxheight=3;
const int boxesnumber=5;

const int MAXLABELSNUM=40;

typedef enum {PIXELSONLY, SYMMETRY} FeatureMode;
typedef enum {GRAYSCALE, THRESHOLD} ThresholdMode;
typedef enum {NOEDGEREMOVAL, MAXBLACKPIXELS} EdgeRemovalMode;
typedef enum {NOGRAYNORM, HISTOGRAMEQ} GrayNormMode;

class ProcessImage
{
private:
	 void findblackedges(IplImage*im, int maxblacksinblankrow, int maxblacksinblankcol, int &toprow, int &bottomrow, int&leftcol, int &rightcol, int threshold);
public:
	 char m_alg[200];
	 FeatureMode featuresMode;
	 ThresholdMode thresholdMode;
	 EdgeRemovalMode edgeRemovalMode;
	 GrayNormMode grayNormMode;
	 int maxblackpixelsinmargin;
	 uint *arr;
	 int m_numOfFeatures;
	 //int minblackpixelsaround;
	 IplImage* image2array(IplImage*image, uint* arr,int threshold,const char*strOpt);
	 void GetOptionsStr(char* str);
	 bool SetOptionsStr(const char* str);
	 ProcessImage();
	 ~ProcessImage();
};

#endif