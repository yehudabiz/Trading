// SVMDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include <direct.h>
#include <time.h>
#include <stdlib.h>
#include "ipl.h"
#include "xipl.h"
#include <string>
#include <map>
#include <list>
#include <vector>
#include <utility>

#include "svmdll.h"
#include "CGP.H"
#include "LearningSet.h"
#include "ClearnGP.H"
#include "processimage.h"

using namespace std;

LARGE_INTEGER start,end,freq;
double elapsed;

IplImage* CloneByRect(IplImage*im,int leftcol,int toprow,int rightcol,int bottomrow)
{
	IplImage*clone=(IplImage*)xiplCreate(rightcol-leftcol+1,bottomrow-toprow+1);

	for (int r=toprow; r<=bottomrow; r++)
	{
		for(int c=leftcol; c<=rightcol; c++)
		{
			uchar p;
			iplGetPixel(im,c,r,&p);
			iplPutPixel(clone,c-leftcol,r-toprow,&p);
		}
	}
	return clone;
}

////////////////////////////// TRAINING ////////////////////////////
SVMDLLAPI IplImage* getImage(char*ImagePath)
{
	IplImage* image=xiplLoad(ImagePath);
	return image;
}

ProcessImage* processImage;
LearningSet* LearnSet;
CMultiClassGP* MultiClassGP;
char chars[10];

SVMDLLAPI void svmInitLearn(char* characters, char* algorithm, int nNumberOfObjects, char* strOutputInfPath)
{
	LearnSet= new LearningSet;
	MultiClassGP= new CMultiClassGP;
	MultiClassGP->SetOutputPath(strOutputInfPath);
	strcpy_s(chars,characters);

	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	processImage=new ProcessImage;
	processImage->SetOptionsStr(algorithm);
	LearnSet->SetNumberOfObjects(nNumberOfObjects);
	LearnSet->SetNumberOfFeatures(processImage->m_numOfFeatures);    
}

SVMDLLAPI void svmLearnClose()
{
	delete MultiClassGP;
	delete LearnSet;
	delete processImage;
}

SVMDLLAPI bool svmAddImageToLearningSet(IplImage* image, char character, int threshold)
{	
	char algorithm[200];
	processImage->GetOptionsStr(algorithm);

	IplImage* resizedimage=processImage->image2array(image,processImage->arr,threshold,algorithm);
	xiplFree((long)resizedimage);

	int nClassLabel=strchr(chars,character)-chars;
	
	if(LearnSet->AddObject(processImage->arr,nClassLabel))
		return true;
	else
		return false;
}


SVMDLLAPI bool svmAddImageDataToLearningSet(IplImage* charImage, int prefixedThreshold, char truevalue, int debug, char*fname)
{	
	char algorithm[200];
	processImage->GetOptionsStr(algorithm);

	IplImage* resizedimage=processImage->image2array(charImage,processImage->arr,prefixedThreshold,algorithm);
	if (debug==1)
	{
		xiplSaveJPG(fname,resizedimage,100);
	}
	xiplFree((long)resizedimage);

	int nClassLabel=strchr(chars,truevalue)-chars;

	if(LearnSet->AddObject(processImage->arr,nClassLabel))
		return true;
	else
		return false;
}


SVMDLLAPI bool svmMultiClassGPLearningForTC( /*LearningSet *pLearningSet
						                     ,*/double& dErrorPercent
											 ,double& dRejectPercent
						                     ,double* pdErrorMatrix
						                     ,int* pnClassLabels
										     ,int  nCrossValidationType
											 ,double dBestParamC)
{         
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(MultiClassGP->MultiClassGPLearningForTC(LearnSet
						                      ,dErrorPercent
								              ,dRejectPercent
						                      ,pdErrorMatrix
						                      ,pnClassLabels
								              ,nCrossValidationType
								              ,dBestParamC))
		return true;
	else
		return false;
}


////////////////////////////// TESTING ////////////////////////////
SVMDLLAPI bool convertSvmAlgString(const char* src,char *dst)
{
	ProcessImage tmp;
	if(!tmp.SetOptionsStr(src))
		return false;
	tmp.GetOptionsStr(dst);
	return true;
}

map<string,CMultiGeneralizedPortraitRule*> GPsMap;
map<string,ProcessImage*> processImageMap;

SVMDLLAPI int initSVMclassifiers(const char* svmrulesbasedir)
{
	int dirsloaded=0;

	if(svmrulesbasedir==NULL)
		return 0;

	char filespec1[1000];
	WIN32_FIND_DATAA finddata1;
	strcpy_s(filespec1,svmrulesbasedir);
	strcat_s(filespec1,"\\*");
	HANDLE handle1=FindFirstFileA(filespec1,&finddata1);
	if (handle1==INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	do 
	{
		if ((finddata1.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY && 
			(strcmp(finddata1.cFileName,".")!=0) && 
			(strcmp(finddata1.cFileName,"..")!=0)
		)
		{
			char filespec2[1000];
			WIN32_FIND_DATAA finddata2;

			sprintf_s(filespec2,1000,"%s\\%s\\svmrules\\*",svmrulesbasedir,finddata1.cFileName);
			HANDLE handle2=FindFirstFileA(filespec2,&finddata2);
			if (handle2==INVALID_HANDLE_VALUE)
			{
				return 0;
			}

			do 
			{
				if ((finddata2.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY && 
					(strcmp(finddata2.cFileName,".")!=0) && 
					(strcmp(finddata2.cFileName,"..")!=0)
				)
				{
					char svmrulespath[1000];										
					sprintf_s(svmrulespath,1000,"%s\\%s\\svmrules\\%s\\",svmrulesbasedir,finddata1.cFileName,finddata2.cFileName);
					string svmrulespathstr(svmrulespath);

					CMultiGeneralizedPortraitRule *gpObject=new CMultiGeneralizedPortraitRule;

					bool success;
					success=gpObject->LoadRules(svmrulespath);
					if (!success)
					{
						printf("svm dir %s failed to load!\n",svmrulespath);
						continue;
					}

					dirsloaded++;
					
					GPsMap[svmrulespathstr]=gpObject;

					ProcessImage *processImage=new ProcessImage;
					processImage->SetOptionsStr(finddata2.cFileName);
					
					processImageMap[svmrulespathstr]=processImage;
				}
			} while (FindNextFileA(handle2,&finddata2));
		}

	} while (FindNextFileA(handle1,&finddata1));

	return dirsloaded;
}

SVMDLLAPI int svmDisambiguate4(IplImage*plateImage, int threshold2use, const char* svmrulesbasedirname, const char*labels, const char*algorithm, int l, int t, int r, int b, int debug, char*fname)
{
	if (plateImage==NULL)
	{
		return -1;
	}
	else
	{
		IplImage* charImage=CloneByRect(plateImage,l,t,r,b);
		int bestlabel= svmDisambiguate2(charImage,threshold2use,svmrulesbasedirname,labels,algorithm,debug,fname);
		xiplFree((long)charImage);
		return bestlabel;
	}
}
/*
SVMDLLAPI int svmDisambiguate3(ImageData*imagedata, const char* svmrulesbasedirname, const char*labels, const char*algorithm)
{
	if (imagedata->image==NULL)
	{
		return -1;
	}
	else
	{
		return svmDisambiguate2(imagedata->image,imagedata->threshold,svmrulesbasedirname,labels,algorithm);
	}	
}
*/
SVMDLLAPI int svmDisambiguate2(IplImage*image,int threshold2use, const char* svmrulesbasedirname, const char*labels, const char*algorithm, int debug, char*fname)
{
	char svmrulesdirname[200];
	sprintf_s(svmrulesdirname,200,"%s\\%s\\svmrules\\%s\\",svmrulesbasedirname,labels,algorithm);
	return svmDisambiguate(image,threshold2use,svmrulesdirname,labels,algorithm,debug,fname);
}

SVMDLLAPI int svmDisambiguate(IplImage*image,int threshold2use, const char* rulesdirname, const char*labels, const char*algorithm, int debug, char*fname)
{
	int dWinnerClassLabel = -1;
	double dWinnerProbability = -1;
	int nNumberOfAlternativeClasses = 0;
	double pdProbabilityOfAlternativeClasses[MAXLABELSNUM];
	int pcAlternativeClassLabels[MAXLABELSNUM]; 
	int bestlabel;
	string rulesdirnamestr(rulesdirname);

	ProcessImage *processImage;
	if(processImageMap.find(rulesdirnamestr)!=processImageMap.end())
	{
		processImage=processImageMap.find(rulesdirnamestr)->second;
		IplImage* resizedimage=processImage->image2array(image,processImage->arr,threshold2use,algorithm);
		if (debug==1)
		{
			xiplSaveJPG(fname,resizedimage,100);
		}
		xiplFree((long)resizedimage);
	}
	else
	{
		//printf("No data defined for processImage algorithm %s\n",rulesdirname);
		return -1;
	}

	if (GPsMap.find(rulesdirnamestr)!=GPsMap.end())
	{
		if(GPsMap[rulesdirnamestr]->GetObjectClassification(rulesdirname, processImage->arr, dWinnerClassLabel, dWinnerProbability, nNumberOfAlternativeClasses, pdProbabilityOfAlternativeClasses, pcAlternativeClassLabels)==0)
			bestlabel= 1000;
		else
			bestlabel=dWinnerClassLabel;
	}
	else
	{
		//printf("No data defined for SVM algorithm %s\n",rulesdirname);
		return -1;
	}	

	if (bestlabel==1000)
	{
		return '$';
	}
	else
	{
		return labels[bestlabel];
	}	
}

SVMDLLAPI void ReleaseSVMclassifiers()
{
	for (map<string,CMultiGeneralizedPortraitRule*>::iterator it=GPsMap.begin(); it!=GPsMap.end(); it++)
	{
		delete it->second;
	}
	GPsMap.clear();

	for (map<string,ProcessImage*>::iterator it=processImageMap.begin(); it!=processImageMap.end(); it++)
	{
		delete it->second;
	}
	processImageMap.clear();	
}