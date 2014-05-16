#include "stdafx.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <iostream>
#include "CGP.h"
#include <direct.h>

///////////////debug////////////////////////////
//extern LONGLONG timer_freq;

//LONGLONG timer_freq;

//LONGLONG st_timer, fn_timer ;


//double tm;

///////////////////////////////////////////////

#define BUFFER_SIZE 10000



CPairwiseGeneralizedPortraitRule::CPairwiseGeneralizedPortraitRule()
{

	m_nNumercalLabelClass1 = 0;
    m_nNumercalLabelClass2 = 0;

	m_nClassLabel1 = -1;
	m_nClassLabel2 = -1;

	m_nNumberOfFeatures = 0;
	
    m_pdHyperplane = NULL;

	m_dB = 0.0;

////////////////Debug////////////////////////////////////////////////
//  get frequency of timer
//  BOOL b = QueryPerformanceFrequency((LARGE_INTEGER*)&timer_freq);
//  if (!b) {timer_freq=0;}
//  else {timer_freq/=1000;}
//////////////////////////////////////////////////////////////////
}

CPairwiseGeneralizedPortraitRule::~CPairwiseGeneralizedPortraitRule()
{
    if(m_pdHyperplane)
		delete[] m_pdHyperplane;
}





bool CPairwiseGeneralizedPortraitRule::ClassifyObjectByLinearPairRule ( unsigned int* punObjectFeatures
												                       ,int& nWinnerClass
												                       ,double& dWinnerClassProbability)
{

	
	nWinnerClass = 0;
	dWinnerClassProbability = 0.0;

	
	if (!punObjectFeatures || m_nNumberOfFeatures < 1)
		return false;

	double dDecValue = 0;
	
    for(int i = 0; i < m_nNumberOfFeatures; i++)
	{
		dDecValue += (double)punObjectFeatures[i]*m_pdHyperplane[i];
    }

	dDecValue += m_dB;


	if(dDecValue > 0)
		nWinnerClass =  1;

	if(dDecValue < 0)
		nWinnerClass = -1;


	dWinnerClassProbability = fabs(dDecValue);


	return true;
}





bool CPairwiseGeneralizedPortraitRule::LoadRule( const char* pcPath
											   ,int nNumercalLabelClass1
												,int nNumercalLabelClass2
												,int nClassLabel1
												,int nClassLabel2
												,int nMaxNumberOfFeatures)
{

	m_nNumercalLabelClass1 = nNumercalLabelClass1;
	m_nNumercalLabelClass2 = nNumercalLabelClass2;
	m_nClassLabel1 = nClassLabel1;
	m_nClassLabel2 = nClassLabel2;

	bool bStatus = ReadRuleFromBinaryList(pcPath);

	return bStatus;

}


bool CPairwiseGeneralizedPortraitRule::ReadRuleFromBinaryList(const char* pcPath)
{

	 CFile f;

	 int nBufferSize = BUFFER_SIZE;

	 char* pcFileName = new char[nBufferSize];

	 char* pcString = new char[nBufferSize];

	 memset(pcFileName,0,sizeof(char)*nBufferSize);
     memset(pcString,0,sizeof(char)*nBufferSize);
     

	 strcat(pcFileName,pcPath);
	 sprintf(pcString,"%d", m_nNumercalLabelClass1);
	 strcat(pcFileName,pcString);
	 
	 strcat(pcFileName,"_");

     memset(pcString,0,sizeof(char)*nBufferSize);
	 sprintf(pcString,"%d", m_nNumercalLabelClass2);
	 strcat(pcFileName,pcString);
	 strcat(pcFileName,".rul");



	 CFileException fileException;

	 if(!f.Open(pcFileName, CFile::modeRead , &fileException))
	 {
          TRACE ("Cannot open file %s, error = %u\n",pcFileName, fileException.m_cause);
          delete[] pcFileName;
		  delete[] pcString;

	 	  return false;
     }

///////////////////////////
     float a = 0.0;

     f.Read(&a,sizeof(float));

	 m_nNumberOfFeatures = (int)a;

	 f.SeekToBegin();

     nBufferSize = m_nNumberOfFeatures + 2;

	 float* pfBuffer = new float[nBufferSize];

	 memset(pfBuffer,0,sizeof(float)*nBufferSize);

	 f.Read(pfBuffer,sizeof(float)*nBufferSize);

	 m_pdHyperplane = new double[m_nNumberOfFeatures];

	 for( int i = 0; i < m_nNumberOfFeatures; i++)
	 {
		 m_pdHyperplane[i] = (double) pfBuffer[i + 1];
	 }

	 m_dB = (double)pfBuffer[m_nNumberOfFeatures + 1];

	 delete[] pcFileName;
	 delete[] pcString;
	 delete[] pfBuffer;

     return true; 
      

}

CMultiGeneralizedPortraitRule::CMultiGeneralizedPortraitRule()
{

	
	m_nNumberOfFeatures = 0;   
	m_nNumberOfClasses =  0;
	
	m_dProbabilityThresholdForAlternative = 0;

	m_nRulesSize = 0; 
	
	m_pPairwiseGeneralizedPortraitRulesArray = NULL;


	m_pnMaxNumberOfWinnersForClass = NULL;
	m_pnMinNumberOfWinnersForClass = NULL;
    m_pdMeanDist = NULL;

    m_pnListNearestClasses = NULL;
    m_pnNumberOfNearestClasses = NULL;
	m_pnClassLabels = NULL;



								

}

CMultiGeneralizedPortraitRule::~CMultiGeneralizedPortraitRule()
{
    if(m_pPairwiseGeneralizedPortraitRulesArray)
		delete[] m_pPairwiseGeneralizedPortraitRulesArray;

	if(m_pnMaxNumberOfWinnersForClass)
	   delete[]	m_pnMaxNumberOfWinnersForClass;

	if(m_pnMinNumberOfWinnersForClass)
	   delete[]	m_pnMinNumberOfWinnersForClass;

	if(m_pdMeanDist)
		delete[] m_pdMeanDist;

	if(m_pnListNearestClasses)
	   delete[] m_pnListNearestClasses;

    if(m_pnNumberOfNearestClasses) 
      delete[] m_pnNumberOfNearestClasses;
	
    if(m_pnClassLabels)
		delete[] m_pnClassLabels;

}

bool CMultiGeneralizedPortraitRule::LoadRules(const char* pcPath)
{
	strcpy_s(m_pcPath,200,pcPath);

	if(!ReadParametersFromIniFile(pcPath))
		return false;

	if(!ReadInformationForRegect(pcPath))
		return false;


	int m_nRulesSize = m_nNumberOfClasses*(m_nNumberOfClasses -1)/2;

	m_pPairwiseGeneralizedPortraitRulesArray = new CPairwiseGeneralizedPortraitRule[m_nRulesSize];


	int nCounter = 0;
	for( int i = 0; i < m_nNumberOfClasses - 1; i++)
	{
		for( int j = i + 1; j < m_nNumberOfClasses; j++)
		{
			int nClassLabel1 = m_pnClassLabels[i];
			int nClassLabel2 = m_pnClassLabels[j];

			m_pPairwiseGeneralizedPortraitRulesArray[nCounter].LoadRule( pcPath
				                                                       ,i + 1
																	   ,j + 1
																	   ,nClassLabel1
																	   ,nClassLabel2
																	   ,m_nNumberOfFeatures);

            nCounter++;


        }
    }

	return true;

}


bool CMultiGeneralizedPortraitRule::ReadParametersFromIniFile(const char* pcPath)
{
     

     int nBufferSize = BUFFER_SIZE;
	 char* pcFileName = new char[nBufferSize];
	 memset(pcFileName,0,sizeof(char)*nBufferSize);
	 strcat(pcFileName,pcPath);
	 strcat(pcFileName,"Multiclassification.ini");


	char* pcStr = new char[nBufferSize];
	memset(pcStr,0,sizeof(char)*nBufferSize);

	if(!GetPrivateProfileString("Features","Number Of Features","0",pcStr,4,pcFileName))
	{
		delete[] pcFileName;
		delete[] pcStr;
		return false;
    }
	m_nNumberOfFeatures = atoi(pcStr);

    memset(pcStr,0,sizeof(char)*nBufferSize);
	if(!GetPrivateProfileString("Class Labels","Number Of Classes","0",pcStr,4,pcFileName))
	{
		delete[] pcFileName;
		delete[] pcStr;
		return false;
    }

	m_nNumberOfClasses = atoi(pcStr);

    m_pnClassLabels = new int[m_nNumberOfClasses];

	char* pcTmp = new char[nBufferSize];
	
	for(int i = 0; i < m_nNumberOfClasses;i++)
	{
        memset(pcStr,0,sizeof(char)*nBufferSize);
		memset(pcTmp,0,sizeof(char)*nBufferSize);
		strcat(pcTmp,"Label");
		sprintf(pcStr,"%d",i + 1);
		strcat(pcTmp,pcStr);

		if(!GetPrivateProfileString("Class Labels",pcTmp,"-1",pcStr,100,pcFileName))
        {
           delete[] pcFileName;
		   delete[] pcStr;
		   delete[] pcTmp;
		   return false;
        }
         
        m_pnClassLabels[i] = atoi(pcStr);

		if(m_pnClassLabels[i] == -1)
        {
			delete[] pcFileName;
		    delete[] pcStr;
		    delete[] pcTmp;
			return false;
        }
    }


	memset(pcStr,0,sizeof(char)*nBufferSize);

	if(!GetPrivateProfileString("Probability Threshold for Reject","Probability","0.5",pcStr,5,pcFileName))
	{
		delete[] pcFileName;
		delete[] pcStr;
		delete[] pcTmp;
		return false;
    }

	m_dProbabilityThresholdForAlternative = atof(pcStr);

	delete[] pcFileName;
	delete[] pcStr;
	delete[] pcTmp;

	return true;

}

bool CMultiGeneralizedPortraitRule::ReadInformationForRegect(const char* pcPath)
{

	
	 CFile f;
     
	 int nBufferSize = BUFFER_SIZE;

	 char* pcFileName = new char[nBufferSize];
	 
	 memset(pcFileName,0,sizeof(char)*nBufferSize);

	 strcat(pcFileName,pcPath);
     
	 strcat(pcFileName,"Regect.rul");

	 CFileException fileException;

	 if(!f.Open(pcFileName,CFile::modeRead,&fileException))
	 {
          TRACE ("Cannot open file %s, error = %u\n",pcFileName, fileException.m_cause);
		  delete[] pcFileName;
	 	  return false;
     }
	 
   
	 int nBufferLengthB = m_nNumberOfClasses*m_nNumberOfClasses*(3 + 2*m_nNumberOfClasses) + m_nNumberOfClasses + 2;
	 
	 int nBufferLength = sizeof(float)*(nBufferLengthB);

	 float* pfBuffer = new float[nBufferLengthB];

	 memset(pfBuffer,0,nBufferLength);

	 f.Read(pfBuffer,nBufferLength);

	 int nNumberOfClasses = (int)pfBuffer[0];

	 m_dNSigm = (double)pfBuffer[1];

	 if(m_nNumberOfClasses != nNumberOfClasses)
	 {
		 delete[] pfBuffer;
		 return false;
     }

	 m_pnMaxNumberOfWinnersForClass = new int[m_nNumberOfClasses*m_nNumberOfClasses];
	 
	 for(int i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
         m_pnMaxNumberOfWinnersForClass[i] = (int)pfBuffer[i + 2];
	 }

	 m_pnMinNumberOfWinnersForClass = new int[m_nNumberOfClasses*m_nNumberOfClasses];

	 for(int  i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
         m_pnMinNumberOfWinnersForClass[i] = (int)pfBuffer[i + 2 + m_nNumberOfClasses*m_nNumberOfClasses];
	 }

     m_pnListNearestClasses = new int [nNumberOfClasses*nNumberOfClasses];

	 for(int  i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
          m_pnListNearestClasses[i] = (int)pfBuffer[i + 2 + 2*m_nNumberOfClasses*m_nNumberOfClasses] ;
     }


	 m_pdMeanDist = new double[m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses];

	 for(int  i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
         m_pdMeanDist[i] = (double)pfBuffer[i + 2 + 3*m_nNumberOfClasses*m_nNumberOfClasses] ;
	 }

	 m_pdVarDist = new double[m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses];

	 for(int  i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
        m_pdVarDist[i] = (double)pfBuffer[i + 2 + 3*m_nNumberOfClasses*m_nNumberOfClasses + m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses];
	 }


     m_pnNumberOfNearestClasses = new int [nNumberOfClasses];

	 for(int  i = 0; i < m_nNumberOfClasses; i++)
	 {
        m_pnNumberOfNearestClasses[i] =  (int)pfBuffer[i + 2 + 3*m_nNumberOfClasses*m_nNumberOfClasses + 2*m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses] ;
     }
    
	 f.Close();

	 delete[] pfBuffer;
	 delete[] pcFileName;

	 return true;
}




int CMultiGeneralizedPortraitRule::GetObjectClassification(const char* rulespath 
														   ,unsigned int* punFeaturesArray
											               ,int& nWinnerClassLabel
											               ,double &dWinnerProbability
											               ,int& nNumberOfAlternativeClasses
											               ,double* pdProbabilityOfAlternativeClasses
											               ,int* pnAlternativeClassLabels)
{
    
   if(_stricmp(rulespath,m_pcPath)!=0)
   {
	   printf("cgp error\n");
	   exit(1);
   }

	nWinnerClassLabel = -1;

	nNumberOfAlternativeClasses = 0;

     
	if((!m_nNumberOfClasses)||(!m_pPairwiseGeneralizedPortraitRulesArray))
		return 0;


	
	int* pnWinnerArray = new int[m_nNumberOfClasses];
	memset(pnWinnerArray,0,sizeof(int)*m_nNumberOfClasses);
	double* pdTournamentArray = new double[m_nNumberOfClasses*m_nNumberOfClasses];
	memset(pdTournamentArray,0,sizeof(double)*m_nNumberOfClasses*m_nNumberOfClasses);

	int nResalt = 0;

    int k = 0;
	for(int i = 0; i < m_nNumberOfClasses - 1; i++)
	{	
		 for(int j = i + 1; j < m_nNumberOfClasses; j++)
		 {  
		      int d = 0;
		      double dWinnerClassProbability = 0.0;

			  if(!m_pPairwiseGeneralizedPortraitRulesArray[k].ClassifyObjectByLinearPairRule (punFeaturesArray,d,dWinnerClassProbability))
			  {
				   d = 0;
				   nResalt = 0;
			  }
			 
              if(d > 0)
				  nResalt = 1;

			  if(d < 0)
				  nResalt = -1;


			  if(nResalt > 0)
			  {
				  pdTournamentArray[i + j*m_nNumberOfClasses] = dWinnerClassProbability;

				  pnWinnerArray[i] += 1;
              }

			  if(nResalt < 0)
			  {
				 pdTournamentArray[j + i*m_nNumberOfClasses] =  dWinnerClassProbability;
				 pnWinnerArray[j] += 1;
              }

			  k++;
         } 
	 }

	 int nWinnerClassNumber = 0; 
	 int nMax = -1;
	
	 for(int j = 0; j < m_nNumberOfClasses; j++)
	 {
		 

		 if(nMax < pnWinnerArray[j])
		 {
            nWinnerClassNumber = j + 1;

			nMax = pnWinnerArray[j];
         }

     }

	 if(nMax == 0)
	 {
		 delete[] pnWinnerArray;
		 delete[] pdTournamentArray;
		 return 0;
     }

	int nWinner2 = 0;
	nMax = -1;
	for(int j = 0; j < m_nNumberOfClasses; j++)
	{
		if(nWinnerClassNumber == (j + 1))
			continue;

		if(pnWinnerArray[j] > nMax)
		{
            nMax = pnWinnerArray[j];
			nWinner2 = j + 1;
        }
		
	}


	 dWinnerProbability = 0;	 

	 int nClassNumberForReject = nWinnerClassNumber;


	 if(!RejectAnalysis(nClassNumberForReject,nWinner2,pnWinnerArray,pdTournamentArray,dWinnerProbability))
		 nWinnerClassNumber = 0;
         
	  	
	 if(nWinnerClassNumber)
	 {
		  
		nWinnerClassLabel = m_pnClassLabels[nWinnerClassNumber - 1];

		double dCurrentDist = pdTournamentArray[(nWinnerClassNumber - 1) + (nWinner2 - 1)*m_nNumberOfClasses];
		double dMeanDist    = m_pdMeanDist[(nWinnerClassNumber - 1) + (nWinner2 - 1)*m_nNumberOfClasses + (nWinnerClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses];

		if(dMeanDist)
			dWinnerProbability = dCurrentDist/dMeanDist;
		else
			return false;
	
		if(dWinnerProbability < 0.0)
			dWinnerProbability  = 0.0;

		if(dWinnerProbability > 1.0)
			dWinnerProbability  = 1.0;

		if(dWinnerProbability < m_dProbabilityThresholdForAlternative)
		{
		

			nNumberOfAlternativeClasses = m_pnNumberOfNearestClasses[nWinnerClassNumber - 1];

			memset(pdProbabilityOfAlternativeClasses,0,sizeof(double)*nNumberOfAlternativeClasses);

			int* pnListAlternativeClasses = new int[m_nNumberOfClasses];
			memset(pnListAlternativeClasses,0,sizeof(int)*m_nNumberOfClasses);

			
            AlternativeAnalysis( nWinnerClassNumber
				                ,pnWinnerArray
								,pdTournamentArray
								,nNumberOfAlternativeClasses
								,pdProbabilityOfAlternativeClasses
								,pnListAlternativeClasses
								,m_dProbabilityThresholdForAlternative);

			
             ProbabilityForAlternativeGroup( nNumberOfAlternativeClasses
										    ,pnListAlternativeClasses
											,nWinnerClassNumber
											,pdTournamentArray
											,dWinnerProbability);


            for(int i = 0; i < nNumberOfAlternativeClasses;i++)
			{
				pnAlternativeClassLabels[i] = m_pnClassLabels[pnListAlternativeClasses[i] - 1];
            }

			delete[] pnListAlternativeClasses;
        }
	 }

     delete[] pnWinnerArray;
	 delete[] pdTournamentArray; 
	
	 return (nWinnerClassNumber);
	
}



bool CMultiGeneralizedPortraitRule::ProbabilityForAlternativeGroup( int nNumberOfAlternativeClasses
										                           ,int* pnListAlternativeClasses
													               ,int nWinnerClassNumber
													               ,double* pdTournamentArray
											                       ,double& dAltGroupProbability)
{
	if(!nNumberOfAlternativeClasses)
		return false;
    
	nNumberOfAlternativeClasses++;

    int* pnTmpListAlternativeClasses =  new int[m_nNumberOfClasses]; 
	memset(pnTmpListAlternativeClasses,0,sizeof(int)*m_nNumberOfClasses);

	pnTmpListAlternativeClasses[0] = nWinnerClassNumber;

	for( int i = 1; i < nNumberOfAlternativeClasses; i++)
       pnTmpListAlternativeClasses[i] = pnListAlternativeClasses[i - 1];


	int* pnListOfRestClasses = new int[m_nNumberOfClasses]; 
	memset(pnListOfRestClasses,0,sizeof(int)*m_nNumberOfClasses);

	int nCounter = 0;
	for(int i = 0; i < m_nNumberOfClasses; i++)
	{
        bool bFlag = true; 
			
		for( int j = 0; j < nNumberOfAlternativeClasses; j ++)
		{
			if(pnTmpListAlternativeClasses[j]== i + 1)
			{
				bFlag = false;
				break;
            }
        }

        if(bFlag)
		{
			pnListOfRestClasses[nCounter] = i + 1;
			nCounter++;
		}

    }

	int nNumberOfRestClasses = nCounter;

    double dMinProbability = 1;
	for(int i = 0; i < nNumberOfAlternativeClasses; i++)
	{
		int nAlt = pnTmpListAlternativeClasses[i] - 1;
        for( int j = 0; j < nNumberOfRestClasses; j ++)
		{
			 int nRest = pnListOfRestClasses[j] - 1; 
			 double dCurrentDist = pdTournamentArray[nAlt  + nRest*m_nNumberOfClasses];
             double dMeanDist = m_pdMeanDist[nAlt  + nRest*m_nNumberOfClasses + nAlt*m_nNumberOfClasses*m_nNumberOfClasses];
			 double dVarDist = m_pdVarDist[nAlt  + nRest*m_nNumberOfClasses + nAlt*m_nNumberOfClasses*m_nNumberOfClasses];
   
			 double dCurrentProbability = Probability(dCurrentDist,dMeanDist,dVarDist);

             if(dCurrentProbability < dMinProbability)
				   dMinProbability = dCurrentProbability;
        }
    }

	dAltGroupProbability = dMinProbability;

	delete[] pnListOfRestClasses;
	delete[] pnTmpListAlternativeClasses;

	return true;

}




bool CMultiGeneralizedPortraitRule::AlternativeAnalysis( int nWinnerClassNumber
											            ,int* pnWinnerArray
								                        ,double* pdTournamentArray
										                ,int& nNumberOfAlternativeClasses
										                ,double* pdProbabilityOfAlternativeClasses
										                ,int* pnListAlternativeClasses
											            ,double dProbabilityThreshold)
{

    int* pnTmpWinnerArray = new int[m_nNumberOfClasses];
	memset(pnTmpWinnerArray,0,sizeof(int)*m_nNumberOfClasses);

	int* pnTmpListAlternativeClasses = new int[m_nNumberOfClasses];
    memset(pnTmpListAlternativeClasses,0,sizeof(int)*m_nNumberOfClasses);
	memset(pnListAlternativeClasses,0,sizeof(int)*m_nNumberOfClasses);

    int* pnTmpListAlternativeClasses1 = new int[m_nNumberOfClasses];
    memset(pnTmpListAlternativeClasses1,0,sizeof(int)*m_nNumberOfClasses);


    ////////////////////Compute Current alternative classes//////////////////////////////////////////////////
	
	
	if(!nNumberOfAlternativeClasses)
		return true;

    int nCounter = 0;

	for(int i = 0; i < m_nNumberOfClasses; i++)
	{
	   if(i == nWinnerClassNumber - 1)
		   continue;

       pnTmpWinnerArray[nCounter] = pnWinnerArray[i];
       pnTmpListAlternativeClasses[nCounter] = i + 1;
	   nCounter++;
    }
 
	for(int ll = 0; ll <  m_nNumberOfClasses - 1;ll++)
	{
		for(int l = 0 ; l <  m_nNumberOfClasses - ll - 1; l++)
		{
			if(pnTmpWinnerArray[l] < pnTmpWinnerArray[l + 1])
			{
				int a = pnTmpWinnerArray[l];
				pnTmpWinnerArray[l] = pnTmpWinnerArray[l + 1];
				pnTmpWinnerArray[l + 1] = a;

				char b = pnTmpListAlternativeClasses[l];
				pnTmpListAlternativeClasses[l] = pnTmpListAlternativeClasses[l + 1];
				pnTmpListAlternativeClasses[l + 1] = b;

				
			}

		} 
	}

//////////////////Intersection betweeen Current alternative classes and Nearest classes//////////////////////////////////////
  nCounter = 0;
  for(int i = 0; i <  nNumberOfAlternativeClasses; i++)
  {
		int nClassAlt = pnTmpListAlternativeClasses[i];

		for(int j = 0; j <  nNumberOfAlternativeClasses; j++)
		{
			if(m_pnListNearestClasses[j + (nWinnerClassNumber - 1)*m_nNumberOfClasses] == nClassAlt)
			{
				pnTmpListAlternativeClasses1[nCounter] = nClassAlt;
				nCounter++;
            }

        }
  }

  nNumberOfAlternativeClasses = nCounter;
////////////////////Template Filter///////////////////////////////////

    
    nCounter = 0;

    for(int i = 0; i <  nNumberOfAlternativeClasses; i++)
	{
		int nClassAlt = pnTmpListAlternativeClasses1[i];
		bool nFlagAlt = true;

	    for(int j = 0; j < m_nNumberOfClasses; j++)
		{
		

			memset(pnTmpWinnerArray,0,sizeof(int)*m_nNumberOfClasses);

			MakeArtificialWinnerArray( pnWinnerArray[nClassAlt - 1]
									  ,nClassAlt
									  ,pdTournamentArray
									  ,pnTmpWinnerArray
									  ,dProbabilityThreshold);

			int nDelta = 0;

			int nDeltaPlus = pnTmpWinnerArray[j] - m_pnMaxNumberOfWinnersForClass[j + (nClassAlt - 1)*m_nNumberOfClasses];
			int nDeltaMinus = m_pnMinNumberOfWinnersForClass[j + (nClassAlt - 1)*m_nNumberOfClasses] - pnTmpWinnerArray[j];

			if((nDeltaPlus > nDeltaMinus)&&(nDeltaPlus > 0))
				nDelta = nDeltaPlus;

			if((nDeltaMinus > nDeltaPlus)&&(nDeltaMinus > 0))
				nDelta = nDeltaMinus;

			if(nDelta >= 1)
			{
				nFlagAlt = false;
				break;
			}
			
        
		}   
	
		if(nFlagAlt)
		{
			pnListAlternativeClasses[nCounter] = nClassAlt;
            nCounter++;
        } 
	}

	nNumberOfAlternativeClasses = nCounter;

   	delete[] pnTmpListAlternativeClasses;
	delete[] pnTmpListAlternativeClasses1;
	delete[] pnTmpWinnerArray;  
////////////////////Compute Probability/////////////////////////////////////////////////////

	double dProbability = 0;


    for(int j = 0; j <  nNumberOfAlternativeClasses; j++)
	{
		int nClassAlt = pnListAlternativeClasses[j];
		double dCurrentDist = pdTournamentArray[nWinnerClassNumber - 1 + (nClassAlt - 1)*m_nNumberOfClasses];
	    double dMeanDist    = m_pdMeanDist[nWinnerClassNumber - 1  + (nClassAlt - 1)*m_nNumberOfClasses + (nWinnerClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses];
        double dVarDist    = m_pdVarDist[nWinnerClassNumber - 1  + (nClassAlt - 1)*m_nNumberOfClasses + (nWinnerClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses];

		dProbability = Probability(dCurrentDist,dMeanDist,dVarDist);

		pdProbabilityOfAlternativeClasses[j] = 1 - dProbability;
	
	}
////////////////////////////////Sorting by Probability////////////////////////////////
    for(int ll = 0; ll < nNumberOfAlternativeClasses - 1;ll++)
	{
		for(int l = 0 ; l < nNumberOfAlternativeClasses - ll - 1; l++)
		{
			if(pdProbabilityOfAlternativeClasses[l] < pdProbabilityOfAlternativeClasses[l + 1])
			{
				double a = pdProbabilityOfAlternativeClasses[l];
				pdProbabilityOfAlternativeClasses[l] = pdProbabilityOfAlternativeClasses[l + 1];
				pdProbabilityOfAlternativeClasses[l + 1] = a;

				int c = pnListAlternativeClasses[l];
				pnListAlternativeClasses[l] = pnListAlternativeClasses[l + 1];
				pnListAlternativeClasses[l + 1] = c;
		
			}

		} 
	}

	nCounter = 0;

	for(int j = 0; j <  nNumberOfAlternativeClasses; j++)
	{ 
		if(pdProbabilityOfAlternativeClasses[j] <= 0.1)
			break;

		nCounter++;

    }

	nNumberOfAlternativeClasses = nCounter;

	return true;
    
}

void CMultiGeneralizedPortraitRule::MakeArtificialWinnerArray( int nNumberWin
												              ,int nArtificialWinnerClassNumber
												              ,double* pdTournamentArray
												              ,int* pnArtificialWinnerArray
												              ,double dProbabilityThreshold)

{

	double* pdTmpTournamentArray = new double[m_nNumberOfClasses*m_nNumberOfClasses];
	memcpy(pdTmpTournamentArray,pdTournamentArray,sizeof(double)*m_nNumberOfClasses*m_nNumberOfClasses);
    memset(pnArtificialWinnerArray,0,sizeof(int)*m_nNumberOfClasses);

	int nDelta = m_nNumberOfClasses - 1 - nNumberWin;
   
	double dProbability = 0;
	int k = 0;
	while((nDelta > 0)&&(k < m_nNumberOfClasses))
	{
		if(pdTmpTournamentArray[(nArtificialWinnerClassNumber - 1) + k*m_nNumberOfClasses] == 0)
		{
           ///////////////////////////////////////////////////////////////////

           	double dCurrentDist = pdTmpTournamentArray[k + (nArtificialWinnerClassNumber - 1)*m_nNumberOfClasses];

			double dMeanDist    = m_pdMeanDist[k + (nArtificialWinnerClassNumber - 1)*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses];
			double dVarDist    = m_pdVarDist[k + (nArtificialWinnerClassNumber - 1)*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses];

            dProbability = Probability(dCurrentDist,dMeanDist,dVarDist);

            if(dProbability < dProbabilityThreshold)
			{
				pdTmpTournamentArray[(nArtificialWinnerClassNumber - 1) + k*m_nNumberOfClasses] = pdTmpTournamentArray[k + (nArtificialWinnerClassNumber - 1)*m_nNumberOfClasses];
				pdTmpTournamentArray[k + (nArtificialWinnerClassNumber - 1)*m_nNumberOfClasses] = 0;
				nDelta--;
            }
        }

		k++;
	}
    
	for(int i = 0; i < m_nNumberOfClasses; i++)
	{
       	for(int j = 0; j < m_nNumberOfClasses; j++)
		{
           if(pdTmpTournamentArray[i + j*m_nNumberOfClasses])
			   pnArtificialWinnerArray[i]++;
        }

    }

	delete[] pdTmpTournamentArray;

}

bool CMultiGeneralizedPortraitRule::RejectAnalysis( int nWinnerClassNumber
										           ,int nWinner2 
										           ,int* pnWinnerArray
								                   ,double* pdTournamentArray
										           ,double& dProbability)
{

    if(pnWinnerArray[nWinnerClassNumber -1] == pnWinnerArray[nWinner2 - 1] )
			  return false;

	int nFlagReject = false;
	for(int j = 0; j < m_pnNumberOfNearestClasses[nWinnerClassNumber - 1]; j++)
	{
		if(m_pnListNearestClasses[j + (nWinnerClassNumber - 1)*m_nNumberOfClasses] == nWinner2)
		{
			nFlagReject = true;
			break;
        }
	}
   
	if(!nFlagReject)
		return false;
    
	int nDelta = 0;

	for(int j = 0; j < m_nNumberOfClasses; j++)
	{
		nDelta = 0;

		int nDeltaPlus = pnWinnerArray[j] - m_pnMaxNumberOfWinnersForClass[j + (nWinnerClassNumber - 1)*m_nNumberOfClasses];
		int nDeltaMinus = m_pnMinNumberOfWinnersForClass[j + (nWinnerClassNumber - 1)*m_nNumberOfClasses] - pnWinnerArray[j];

		if((nDeltaPlus > nDeltaMinus)&&(nDeltaPlus > 0))
			nDelta = nDeltaPlus;

		if((nDeltaMinus > nDeltaPlus)&&(nDeltaMinus > 0))
			nDelta = nDeltaMinus;

		if(nDelta >= 1)
		{
			nFlagReject = false;
			
        }
    }

	if(nFlagReject)
	{
		double dCurrentDist = pdTournamentArray[(nWinnerClassNumber - 1) + (nWinner2 - 1)*m_nNumberOfClasses];
		double dMeanDist    = m_pdMeanDist[(nWinnerClassNumber - 1) + (nWinner2 - 1)*m_nNumberOfClasses + (nWinnerClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses];
		double dVarDist     = m_pdVarDist[(nWinnerClassNumber - 1) + (nWinner2 - 1)*m_nNumberOfClasses + (nWinnerClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses];
		if(dCurrentDist > dMeanDist + m_dNSigm*dVarDist)
			nFlagReject = false;

    }

	if(nFlagReject)
	  return true;
	else
      return false;
   
}

double CMultiGeneralizedPortraitRule::Probability(double dCurrentDist,double dMeanDist, double dVarDist)
{

      double dProbability = 0.0;


		if(dMeanDist)
			dProbability = dCurrentDist/dMeanDist;
		else
			return 0.0;


		if(dProbability < 0.0)
			dProbability  = 0.0;

		if(dProbability > 1.0)
			dProbability  = 1.0;

		return dProbability;
}