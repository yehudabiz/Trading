#ifndef SVMDLL_H
#define SVMDLL_H

#include "xipl.h"
#include "processimage.h"

#ifdef SVMDLL_EXPORTS
#define SVMDLLAPI	__declspec( dllexport)
#else
#define SVMDLLAPI	__declspec( dllimport)
#endif

extern "C"
{
SVMDLLAPI IplImage* getImage(char*ImagePath);
SVMDLLAPI void svmInitLearn(char* characters, char* algorithm, int nNumberOfObjects, char* strOutputInfPath);
SVMDLLAPI void svmLearnClose();
SVMDLLAPI bool svmAddImageToLearningSet(IplImage* image, char character, int threshold);
SVMDLLAPI bool svmAddImageDataToLearningSet(IplImage* charImage, int prefixedThreshold, char truevalue,int debug, char*fname);
SVMDLLAPI bool svmMultiClassGPLearningForTC( /*LearningSet *pLearningSet
						                     ,*/double& dErrorPercent
											 ,double& dRejectPercent
						                     ,double* pdErrorMatrix
						                     ,int* pnClassLabels
										     ,int  nCrossValidationType
											 ,double dBestParamC);

SVMDLLAPI bool convertSvmAlgString(const char* src,char *dst);
SVMDLLAPI int initSVMclassifiers(const char* svmrulesbasedir);
SVMDLLAPI void ReleaseSVMclassifiers();
SVMDLLAPI int svmDisambiguate(IplImage*image,int threshold, const char* rulesdirname, const char*labels, const char*alg,int debug, char*fname);
SVMDLLAPI int svmDisambiguate2(IplImage*image,int threshold, const char* svmrulesbasedirname, const char*labels, const char*algorithm,int debug, char*fname);
//SVMDLLAPI int svmDisambiguate3(ImageData*imagedata, const char* svmrulesbasedirname, const char*labels, const char*algorithm);
SVMDLLAPI int svmDisambiguate4(IplImage*plateImage, int threshold2use, const char* svmrulesbasedirname, const char*labels, const char*algorithm, int l, int t, int r, int b,int debug, char*fname);
}

#endif