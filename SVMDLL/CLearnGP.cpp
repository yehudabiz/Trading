   
#include "stdafx.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "CLearnGP.h"
#include "LearningSet.h"

#define BUFFER_SIZE 10000
#define DEBUG 1

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

#define UNDEFINED_NUMERIC_VALUE  -1


bool WriteToFile(char* fullFileName,char* pcBuffer)
{
     CFile f;

	 CFileException fileException;

	 static int bFlagCreate = false;

	 if(!f.Open(fullFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, &fileException))
	 {
          TRACE ("Cannot open file %s, error = %u\n",fullFileName, fileException.m_cause);
	 	  return false;
     }

     int l = strlen(pcBuffer);
	 pcBuffer[l] = '\n';
	 pcBuffer[l + 1] = '\r';

	 int nBufferLength = strlen(pcBuffer);

	 if(!nBufferLength)
		 return false;

	 DWORD r = f.SeekToEnd( );

     f.Write(pcBuffer,nBufferLength);
	
	 f.Write("\n\r",1);
	 f.Close();

	 return true;
}

bool SaveGlobalResult( char* m_pcPath
					  ,int nNumberOfClasses
					  ,double& dErrorPercent
					  ,double& dAlterPercent
					  ,double& dRejectPercent
					  ,int* pnClassLabelsList
					  ,double* pdErrorMatrix)
{ 
	
	int nBufferSize = BUFFER_SIZE;

	char* pcFileName = new char[nBufferSize];
	memset(pcFileName,0,sizeof(char)*nBufferSize);
	strcat(pcFileName,m_pcPath);
	strcat(pcFileName,"Global.protocol");

	char* pcBuffer = new char[nBufferSize];
	char* pcTmpStr  = new char[nBufferSize];

	double dOK = 1.0 - dErrorPercent - dAlterPercent - dRejectPercent;

    memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	strcat(pcBuffer," OK:     ");
	sprintf(pcTmpStr,"%5.3f", 100*dOK);
	strcat(pcBuffer,pcTmpStr);

	if(!WriteToFile(pcFileName,pcBuffer))
	{
       delete[] pcFileName;
	   delete[] pcBuffer;
	   delete[] pcTmpStr;
	   return false;
    }

    memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	strcat(pcBuffer," Error : ");
	sprintf(pcTmpStr,"%5.3f", 100*dErrorPercent);
	strcat(pcBuffer,pcTmpStr);

	if(!WriteToFile(pcFileName,pcBuffer))
	{
       delete[] pcFileName;
	   delete[] pcBuffer;
	   delete[] pcTmpStr; 
	   return false;
    }
	

	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	strcat(pcBuffer," Alter : ");
	sprintf(pcTmpStr,"%5.3f", 100*dAlterPercent);
	strcat(pcBuffer,pcTmpStr);
	if(!WriteToFile(pcFileName,pcBuffer))
	{
       delete[] pcFileName;
	   delete[] pcBuffer;
	   delete[] pcTmpStr; 
	   return false;
    }
	
	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	strcat(pcBuffer," Reject: ");
	sprintf(pcTmpStr,"%5.3f", 100*dRejectPercent);
	strcat(pcBuffer,pcTmpStr);

	if(!WriteToFile(pcFileName,pcBuffer))
	{
       delete[] pcFileName;
	   delete[] pcBuffer;
	   delete[] pcTmpStr; 
	   return false;
    }
		
	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	
	for(int i = 0; i < nNumberOfClasses; i++)
	{
		memset(pcTmpStr,0,sizeof(char)*nBufferSize);

	    sprintf(pcTmpStr,"%5d",pnClassLabelsList[i]);
		
		strcat(pcBuffer,pcTmpStr);

		strcat(pcBuffer," ");
    }

	if(!WriteToFile(pcFileName,pcBuffer))
	{
       delete[] pcFileName;
	   delete[] pcBuffer;
	   delete[] pcTmpStr; 
	   return false;
    }

	char* pcTmpStr1 = new char[nBufferSize];
	char* pcTmpStr2 = new char[nBufferSize];
	memset(pcTmpStr2,0,sizeof(char)*nBufferSize);
	pcTmpStr2[0] = ' ';
	pcTmpStr2[1] = ' ';

	memset(pcTmpStr,0,sizeof(char)*nBufferSize);

    for(int i =  0; i < 5; i++)
		pcTmpStr[i] = ' ';

	for(int j = 0; j <= nNumberOfClasses; j++)
	{ 
		memset(pcBuffer,0,sizeof(char)*nBufferSize);

		if(j == 0)
			pcTmpStr[2] ='%';
		else
		{  
			memset(pcTmpStr,0,sizeof(char)*nBufferSize);
			sprintf(pcTmpStr,"%5d",pnClassLabelsList[j - 1]);
		}
	
		strcat(pcBuffer,pcTmpStr);

		for(int i = 0; i < nNumberOfClasses; i++)
		{
			memset(pcTmpStr1,0,sizeof(char)*nBufferSize);
			sprintf(pcTmpStr1,"%6.2f", 100*pdErrorMatrix[i + j*nNumberOfClasses]);

	        strcat(pcBuffer,pcTmpStr1);
			strcat(pcBuffer,pcTmpStr2);
		}

		if(!WriteToFile(pcFileName,pcBuffer))
		{
			delete[] pcFileName;
			delete[] pcBuffer;
			delete[] pcTmpStr;
			delete[] pcTmpStr1;
			delete[] pcTmpStr2;
			return false;
		}
    } 
	
	delete[] pcFileName;
	delete[] pcBuffer;
	delete[] pcTmpStr;
	delete[] pcTmpStr1;
	delete[] pcTmpStr2;

	return true;
		
}

void CleanProtocol(char* pcPath,int* pnClassLabels,int nNumberOfClasses)
{
    CFile f;
	CFileException fileException;

	int nBufferSize = BUFFER_SIZE;

	char* pcFileName = new char[nBufferSize];

	char* pcStr = new char[nBufferSize];
	
	for(int i = 0; i < nNumberOfClasses; i++)
	{
		memset(pcFileName,0,sizeof(char)*nBufferSize);
		strcat(pcFileName,pcPath);
		memset(pcStr,0,sizeof(char)*nBufferSize);
		sprintf(pcStr,"%5d",pnClassLabels[i]);
	    strcat(pcFileName,pcStr);
		strcat(pcFileName,".protocol");
		if(!f.Open(pcFileName, CFile::modeCreate | CFile::modeWrite, &fileException))
		{
			delete[] pcFileName;
			delete[] pcStr; 
			return;
		}

		f.Close();
    }

///////////////For Reject Class/////////////////////

	memset(pcFileName,0,sizeof(char)*nBufferSize);
	strcat(pcFileName,pcPath);
	int l = strlen(pcFileName);
	pcFileName[l] = '$';
	strcat(pcFileName,".protocol");
	if(!f.Open(pcFileName, CFile::modeCreate | CFile::modeWrite, &fileException))
	{
		delete[] pcFileName;
        delete[] pcStr; 
		return;
    }
	
	f.Close();

///////////////////////////////////


	memset(pcFileName,0,sizeof(char)*nBufferSize);
	strcat(pcFileName,pcPath);
	strcat(pcFileName,"Global.protocol");
	if(!f.Open(pcFileName, CFile::modeCreate | CFile::modeWrite, &fileException))
	{
		delete[] pcFileName;
        delete[] pcStr; 
		return;
    }

	
	f.Close();

    
	delete[] pcFileName;
    delete[] pcStr; 
    return;
    
}



bool SaveToProtocol( char* pcPath
					,int nNumberOfObjects
					,int nObjectID
					,int nLearnClassLabel
					,int nWinnerClassLabel
					,int nClussificationStatus
					,double dProbability
					,int nNumberOfAlternativeClasses
					,int* pnAlternativeClassLabels)
{


    double a = log((double)nNumberOfObjects);
	double b = log((double)10);
	int c = int(a/b + 0.5);

	a = log((double)nObjectID); 
	int d = int(a/b + 0.5);

	int delta = c - d;

	int nBufferSize = BUFFER_SIZE;

	char* pcFileName = new char[nBufferSize];
	memset(pcFileName,0,sizeof(char)*nBufferSize);
	strcat(pcFileName,pcPath);

	char* pcTmpStr = new char[nBufferSize];
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	sprintf(pcTmpStr,"%5d",nLearnClassLabel);
	strcat(pcFileName,pcTmpStr);
	strcat(pcFileName,".protocol");

	char* pcBuffer = new char[nBufferSize];	
    memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	strcat(pcBuffer," Number: ");
    int l = strlen(pcBuffer);
	for(int i = 0; i < delta; i++)
	{
		pcBuffer[l] = ' ';
		l++;

    }

	sprintf(pcTmpStr,"%d", nObjectID);
	strcat(pcBuffer,pcTmpStr);
	strcat(pcBuffer," Status: ");

	
	if(nClussificationStatus == 0)
	{
		strcat(pcBuffer,"Reject");
		if(!WriteToFile(pcFileName,pcBuffer))
		{
			delete[] pcFileName;
			delete[] pcBuffer;
			delete[] pcTmpStr; 
			return false;
		}

		return true;
    }

	if(nClussificationStatus == 1)
		strcat(pcBuffer,"OK    ");

	if(nClussificationStatus == 2)
		strcat(pcBuffer,"Alt   ");

	if(nClussificationStatus == 3)
		strcat(pcBuffer,"Error ");

	strcat(pcBuffer,"    Recognition:  ");

    memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	sprintf(pcTmpStr,"%5d",nWinnerClassLabel);
	strcat(pcBuffer,pcTmpStr);


    strcat(pcBuffer,"  Confidence:  ");
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	sprintf(pcTmpStr,"%4.2f", dProbability);
	strcat(pcBuffer,pcTmpStr);

	if((nClussificationStatus == 1)||(nClussificationStatus == 3))
	{
	    if(!WriteToFile(pcFileName,pcBuffer))
		{
			delete[] pcFileName;
			delete[] pcBuffer;
			delete[] pcTmpStr;
			return false;
		}
		return true;
    }

	strcat(pcBuffer,"    Alternative: ");

	for(int i =  0; i < nNumberOfAlternativeClasses; i++)
	{
        memset(pcTmpStr,0,sizeof(char)*nBufferSize);
		sprintf(pcTmpStr,"%5d",pnAlternativeClassLabels[i]);
		strcat(pcBuffer,pcTmpStr);
		strcat(pcBuffer," ");
	
    }

    if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcTmpStr; 
		return false;
	} 

	return true;

}


bool SaveStatisticResalt( char* m_pcPath
					     ,int nLearnClassLabel
						 ,ResaltStatistics& pResaltStatistics)

{
    int nBufferSize = BUFFER_SIZE;

	char* pcFileName = new char[nBufferSize];
	memset(pcFileName,0,sizeof(char)*nBufferSize);
	strcat(pcFileName,m_pcPath);

	char* pcBuffer = new char[nBufferSize];
	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	
	sprintf(pcBuffer,"%5d",nLearnClassLabel);
	strcat(pcFileName,pcBuffer);
	strcat(pcFileName,".protocol");

	memset(pcBuffer,0,sizeof(char)*nBufferSize);
    strcat(pcBuffer,"  ");

	if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		return false;
	} 

    if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		return false;
	} 

	char* pcStr = new char[nBufferSize];
	memset(pcStr,0,sizeof(char)*nBufferSize);
	

	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	strcat(pcBuffer," Class Name: ");

	sprintf(pcStr,"%5d",nLearnClassLabel);
	strcat(pcBuffer,pcStr); 
    if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcStr; 
		return false;
	} 

	memset(pcBuffer,0,sizeof(char)*nBufferSize);

	char* pcTmpStr = new char[nBufferSize];
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);

	sprintf(pcTmpStr," OK  =  %6.3f", 100*pResaltStatistics.dOK_Percent);
	strcat(pcBuffer,pcTmpStr);
	if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcStr; 
		delete[] pcTmpStr; 
		return false;
	} 


    memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	sprintf(pcTmpStr," Error  =  %6.3f", 100*pResaltStatistics.dErrorPercent);
	strcat(pcBuffer,pcTmpStr);
	if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcStr; 
		delete[] pcTmpStr; 
		return false;
	} 

	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	sprintf(pcTmpStr," Alt  =  %6.3f", 100*pResaltStatistics.dAlterPercent);
	strcat(pcBuffer,pcTmpStr);
	if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcStr; 
		delete[] pcTmpStr; 
		return false;
	} 

	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	memset(pcTmpStr,0,sizeof(char)*nBufferSize);
	sprintf(pcTmpStr," Reject  =  %6.3f",100*pResaltStatistics.dRejectPercent);
	strcat(pcBuffer,pcTmpStr);
	if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcStr; 
		delete[] pcTmpStr; 
		return false;
	} 

	memset(pcBuffer,0,sizeof(char)*nBufferSize);
	strcat(pcBuffer,"Confidence Error Histogram");
    if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcStr; 
		delete[] pcTmpStr; 
		return false;
	} 

    memset(pcBuffer,0,sizeof(char)*nBufferSize);

	for( int i = 0; i < 10; i++)
	{
        memset(pcTmpStr,0,sizeof(char)*nBufferSize);
		sprintf(pcTmpStr,"   %4.2f", pResaltStatistics.pdConfidenceErrorHist[i]);
		strcat(pcBuffer,pcTmpStr);

    }

	if(!WriteToFile(pcFileName,pcBuffer))
	{
		delete[] pcFileName;
		delete[] pcBuffer;
		delete[] pcStr; 
		delete[] pcTmpStr; 
		return false;
	} 

    return true;
}

ResaltStatistics::ResaltStatistics()
{
   nClassLabel = -1;
   dOK_Percent = 0.0;
   dRejectPercent = 0.0;
   dAlterPercent = 0.0;
   dErrorPercent = 0.0;
  
   nNumberObjectsInClass = 0;

   pdConfidenceErrorHist = new double[10];
   memset(pdConfidenceErrorHist,0,sizeof(double)*10);
}

ResaltStatistics::~ResaltStatistics()
{
	delete[] pdConfidenceErrorHist;
}



CPairwiseRuleGP::CPairwiseRuleGP ()
{
	m_strOutputInfPath   = NULL ; 
	m_pdHyperplane = NULL;
	m_dB = 0.0;
	m_model = NULL;
	m_x_space = NULL;

	m_pdFeatureMeans = NULL;
	m_pdFeatureVariations = NULL;


	m_nNumercalLabelClass1 = -1;
    m_nNumercalLabelClass2 = -1;

	m_nClassLabel1 = -1;
	m_nClassLabel2 = -1;

	m_nNumberOfFeatures = 0;


	m_pbSelectedFeatures = NULL;

}

CPairwiseRuleGP::CPairwiseRuleGP ( char* strOutputInfPath )
{
	m_strOutputInfPath   = strOutputInfPath ; 
	m_pdHyperplane = NULL;
	m_dB = 0.0;
	m_model = NULL;
	m_x_space = NULL;

	m_pdFeatureMeans = NULL;
	m_pdFeatureVariations = NULL;


	m_nNumercalLabelClass1 = -1;
    m_nNumercalLabelClass2 = -1;

	m_nClassLabel1 = -1;
	m_nClassLabel2 = -1;

	m_nNumberOfFeatures = 0;


	m_pbSelectedFeatures = NULL;

}

CPairwiseRuleGP::~CPairwiseRuleGP ()
{
	ClearMemory ();

	if(m_pdHyperplane)
		delete[] m_pdHyperplane;

	if(m_pdFeatureVariations)
		delete[] m_pdFeatureVariations;

	if(m_pdFeatureMeans)
		delete[] m_pdFeatureMeans;


	if(m_pbSelectedFeatures)
		delete[] m_pbSelectedFeatures;

	if(m_model)
		free(m_model);
}

void CPairwiseRuleGP::ClearMemory ()
{	

	if (m_model)
	{
		GP_destroy_model(m_model);
		m_model = NULL;
	}	

	if (m_x_space)
	{
		free(m_x_space);
		m_x_space = NULL;
	}
}

void CPairwiseRuleGP::CalculateFeatureMeansAndVariations(unsigned int* punPairwiseLearnigSetMatrix,int nNumberOfObjects)
{
	int nDefinedFeatures;
	double dFeatVal;
    
	double* m_pdFeatureMeans = new double[m_nNumberOfFeatures];
	memset(m_pdFeatureMeans,0,sizeof(double)*m_nNumberOfFeatures);
	double* m_pdFeatureVariations = new double[m_nNumberOfFeatures];
	memset(m_pdFeatureVariations,0,sizeof(double)*m_nNumberOfFeatures);

	for (int i = 0; i< m_nNumberOfFeatures; i++)
	{	
		nDefinedFeatures = 0;
		for (int j = 0; j < nNumberOfObjects; j++)
		{
			dFeatVal = punPairwiseLearnigSetMatrix[i + j*m_nNumberOfFeatures];

			if (dFeatVal != UNDEFINED_NUMERIC_VALUE)
			{
				nDefinedFeatures ++;
				m_pdFeatureMeans[i]+=	dFeatVal;
				m_pdFeatureVariations[i]+= dFeatVal*dFeatVal;
			}
		}

		m_pdFeatureMeans[i] /= nDefinedFeatures;
		m_pdFeatureVariations[i] /= nDefinedFeatures;
		m_pdFeatureVariations[i] -= m_pdFeatureMeans[i]*m_pdFeatureMeans[i];

		if (m_pdFeatureVariations[i] >= 0.0)
			m_pdFeatureVariations[i] = sqrt(m_pdFeatureVariations[i]);
		else
			m_pdFeatureVariations[i] = 0.0;
	}
}

unsigned int  CPairwiseRuleGP::GetFillHoleFeatureValue ( int unFeatureID 
														 ,unsigned int unFeatureValue)
{
	if ((unFeatureID >= m_nNumberOfFeatures) || (unFeatureID < 0))
	{
		return UNDEFINED_NUMERIC_VALUE;
    }


	if(unFeatureValue == UNDEFINED_NUMERIC_VALUE) 
		unFeatureValue = (unsigned int)(m_pdFeatureMeans[unFeatureID] + 0.5);

	return unFeatureValue;

}

unsigned int  CPairwiseRuleGP::GetNormalFeatureValue ( int unFeatureID 
											           ,unsigned int unFeatureValue)
{
    if ((unFeatureID >= m_nNumberOfFeatures) || (unFeatureID < 0))
	{
		return UNDEFINED_NUMERIC_VALUE;
    }

	if(m_bFillHoles)
	{
		if(unFeatureValue == UNDEFINED_NUMERIC_VALUE) 
			unFeatureValue = (unsigned int)(m_pdFeatureMeans[unFeatureID] + 0.5);
    }

    if(unFeatureValue == UNDEFINED_NUMERIC_VALUE)
          return UNDEFINED_NUMERIC_VALUE;


    if(m_pdFeatureVariations[unFeatureID])
	{
		unFeatureValue = (unsigned int)(1000*(unFeatureValue - m_pdFeatureMeans[unFeatureID]) / (6.0 * m_pdFeatureVariations[unFeatureID]) + 0.5);
		if(unFeatureValue > 1000)
			unFeatureValue = 1000;
		else if (unFeatureValue  < 0)
		    unFeatureValue  = 0;
	}
   
	return unFeatureValue;

}





unsigned int CPairwiseRuleGP::GetFeatureTransformation( int unFeatureID 
											           ,unsigned int unFeatureValue)
{

	
	
	
	if(m_bNormalize)
         unFeatureValue = GetNormalFeatureValue(unFeatureID, unFeatureValue);
	else
	{
        if(m_bFillHoles)
			unFeatureValue = GetFillHoleFeatureValue(unFeatureID,unFeatureValue);
    }

    
	return unFeatureValue;
}


bool CPairwiseRuleGP::ReadRuleFromBinaryList(const char* pcPath)
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

bool CPairwiseRuleGP::WriteRuleToBinaryList(char* pcPath)

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

	 if(!f.Open(pcFileName, CFile::modeCreate | CFile::modeWrite, &fileException))
	 {
          TRACE ("Cannot open file %s, error = %u\n",pcFileName, fileException.m_cause);

		  delete[] pcFileName;
		  delete[] pcString;	  
	 	  return false;
     }

	 DWORD r = f.SeekToEnd( );

	 int nBufferLength = m_nNumberOfFeatures + 2;

	 float* pfBuffer = new float[nBufferLength];

	 memset(pfBuffer,0,sizeof(float)*nBufferLength);

	 pfBuffer[0] = (float)m_nNumberOfFeatures;

	 for( int i = 0; i < m_nNumberOfFeatures; i++)
	 {
         pfBuffer[i + 1] = (float)m_pdHyperplane[i];
	 }

	 pfBuffer[m_nNumberOfFeatures + 1] = (float)m_dB;

     f.Write(pfBuffer,sizeof(float)*nBufferLength);
	
	 f.Close();

	 delete[] pfBuffer;
	 delete[] pcFileName;
	 delete[] pcString;

	 return true;

}

bool CPairwiseRuleGP::MakeLinearPairRule()											       
{
    if((!m_model->SV)||(!m_model->nSV))
		return false;

    if((!m_model->nSV[0])||(!m_model->nSV[1]))
		return false;

	m_pdHyperplane =  new double[m_nNumberOfFeatures]; 
	memset(m_pdHyperplane,0,sizeof(double)*m_nNumberOfFeatures);
	
	double *coef = m_model->sv_coef[0];
	
	for(int k = 0;k < m_model->nSV[0]; k++)
	{
		GP_node *pSV = m_model->SV[k];
		for(int i = 0; i < m_nNumberOfFeatures; i++)
		{	
			m_pdHyperplane[i] +=  coef[k]* pSV->value;
			pSV++;
		}
    }

	for(int k =  0;k < m_model->nSV[1]; k++)
	{
		GP_node *pSV = m_model->SV[m_model->nSV[0] + k];

		for(int i = 0; i < m_nNumberOfFeatures; i++)
		{	
			m_pdHyperplane[i] += coef[m_model->nSV[0] + k] * pSV->value;
			pSV++;
        } 
     }
		
	 m_dB = - m_model->rho[0];

	WriteRuleToBinaryList(m_strOutputInfPath);
	memset(m_pdHyperplane,0,sizeof(double)*m_nNumberOfFeatures);
	m_dB = 0;
	m_nNumberOfFeatures = 0;
	ReadRuleFromBinaryList (m_strOutputInfPath);
     
	return true;
			
}

bool CPairwiseRuleGP::ClassifyObjectByLinearPairRule ( unsigned int* punObjectFeatures 
												       ,int& nWinnerClass
												       ,double& dWinnerClassProbability)
{

	
	nWinnerClass = 0;
	dWinnerClassProbability = 0.0;
	
	if (!punObjectFeatures || m_nNumberOfFeatures < 1)
		return false;


	// classify object
	double dDecValue = 0;

	
    for(int i = 0; i < m_nNumberOfFeatures; i++)
	{
		dDecValue += punObjectFeatures[i]*m_pdHyperplane[i];
    }

	dDecValue += m_dB;


	if(dDecValue > 0)
		nWinnerClass =  1;

	if(dDecValue < 0)
		nWinnerClass = -1;


	dWinnerClassProbability = fabs(dDecValue);


	return true;
}


void CPairwiseRuleGP::SetRule(CPairwiseRuleGP* pRule)
{

	if(m_pbSelectedFeatures)
	   delete[] m_pbSelectedFeatures;

	if(m_pdFeatureMeans)
		delete[] m_pdFeatureMeans;

	if(m_pdFeatureVariations)
		delete[] m_pdFeatureVariations;


    if(m_model)
		GP_destroy_model(m_model);	

	if(m_pdHyperplane)
		delete[] m_pdHyperplane; 

   m_nClassLabel1 = pRule->m_nClassLabel1;
   m_nClassLabel2 = pRule->m_nClassLabel2;
   m_nNumercalLabelClass1 = pRule->m_nNumercalLabelClass1;
   m_nNumercalLabelClass2 = pRule->m_nNumercalLabelClass2;

   m_nNumberOfFeatures = pRule->m_nNumberOfFeatures;
  
   m_bNormalize  = pRule->m_bNormalize;
   m_bFillHoles  = pRule->m_bFillHoles;


   
  
   if(m_nNumberOfFeatures)
   {
    m_pdHyperplane = new double[m_nNumberOfFeatures];
	memcpy(m_pdHyperplane,pRule->m_pdHyperplane,sizeof(double)*m_nNumberOfFeatures);

	m_dB = pRule->m_dB;

    m_pbSelectedFeatures = new bool[m_nNumberOfFeatures];
	memcpy(m_pbSelectedFeatures,pRule->m_pbSelectedFeatures,sizeof(bool)*m_nNumberOfFeatures);
  
	if((pRule->m_bFillHoles)||(pRule->m_bNormalize))
	{
		m_pdFeatureMeans = new double[m_nNumberOfFeatures]; 
		memcpy(m_pdFeatureMeans,pRule->m_pdFeatureMeans,sizeof(double)*m_nNumberOfFeatures);

		m_pdFeatureVariations = new double[m_nNumberOfFeatures]; 
		memcpy(m_pdFeatureVariations,pRule->m_pdFeatureVariations,sizeof(double)*m_nNumberOfFeatures);
	} 

    CopyModel(pRule->m_model);

   } 
}

void CPairwiseRuleGP::CopyModel(GP_model *model)
{
	m_model = Malloc(GP_model,1);
		
		/* these are for training only */
	m_model->param.cache_size =  model->param.cache_size;
	m_model->param.eps =  model->param.eps;
	m_model->param.C =  model->param.C;
	m_model->param.nr_weight =  model->param.nr_weight;
	m_model->param.weight_label = new int[m_model->param.nr_weight];
	memcpy(m_model->param.weight_label,model->param.weight_label,sizeof(int)*m_model->param.nr_weight);
	m_model->param.weight = new double[m_model->param.nr_weight];
	memcpy(m_model->param.weight,model->param.weight,sizeof(double)*m_model->param.nr_weight);
	m_model->param.shrinking =  model->param.shrinking;
	
	int nr_class = model->nr_class;
	m_model->nr_class = model->nr_class; 
	m_model->l = model->l;

	m_model->rho = new double[nr_class*(nr_class-1)/2];
	memcpy(m_model->rho,model->rho,sizeof(double)*nr_class*(nr_class-1)/2);
	
    m_model->label = new int[nr_class];
	memcpy(m_model->label,model->label,sizeof(int)*nr_class);


	m_model->nSV = new int[nr_class];
	memcpy(m_model->nSV,model->nSV,sizeof(int)*nr_class);

	m_model->sv_coef = new double*[nr_class - 1];

	for(int i = 0; i < nr_class - 1; i++)
	{
		m_model->sv_coef[i] = new double[m_model->l];
		memcpy(m_model->sv_coef[i],model->sv_coef[i],sizeof(double)*m_model->l);

	}

	m_model->SV = new GP_node*[m_model->l];


	int nElements=0;
	GP_node * pCurrSV;

	for(int i =  0; i < m_model->l; i++)
	{
		pCurrSV = model->SV[i];

		while (pCurrSV->index != -1)
		{
			nElements++;
			pCurrSV ++;
		}

		nElements ++;	//for the {-1, X} stuctures at the line ends
	}

	// allocate space according to the counted nElements
	m_x_space = new GP_node[nElements];

	// copy SVs into storage
	int nIndex=0;		// Index in m_x_space
	for(int i =  0; i < m_model->l; i++)
	{
		int nSVStartIndex = nIndex;
		pCurrSV = model->SV[i];
		while (pCurrSV->index != -1)
		{
			m_x_space[nIndex] = *pCurrSV;
			pCurrSV ++;
			nIndex++;
		}

		m_x_space[nIndex].index = -1;
		m_model->SV[i] = &m_x_space[nSVStartIndex];
		nIndex++;
	}

}


//////////////////////////////////////Pair Rule Generation Part//////////////////////////////////////////
CPairwiseGP::CPairwiseGP ( PairwiseLearningSet * pPairwiseLearningSet
						    ,char* strOutputInfPath  
							,int nFolds
							,bool bNormalize
							,bool bFillHoles
							,bool bUseWeightsOfClasses
							,int  nCrossValidationType
							)
											  
											
{

   
	m_pPairwiseLearningSet = pPairwiseLearningSet;
	m_nFolds = nFolds;
	m_bUseWeightsOfClasses = bUseWeightsOfClasses;
	m_nCrossValidationType = nCrossValidationType;

	m_pPairwiseRuleGP = new CPairwiseRuleGP( strOutputInfPath );

	m_pPairwiseRuleGP->m_nNumercalLabelClass1 = pPairwiseLearningSet->m_nNumercalLabelClass1; 
	m_pPairwiseRuleGP->m_nNumercalLabelClass2 = pPairwiseLearningSet->m_nNumercalLabelClass2; ; 
	m_pPairwiseRuleGP->m_nClassLabel1 = pPairwiseLearningSet->m_nClassLabel1;
	m_pPairwiseRuleGP->m_nClassLabel2 = pPairwiseLearningSet->m_nClassLabel2;

	m_pPairwiseRuleGP->m_bNormalize  = bNormalize;
	m_pPairwiseRuleGP->m_bFillHoles  = bFillHoles;
				 
    int nNumberOfFeatures = m_pPairwiseLearningSet->m_nNumberOfFeatures;

	m_pPairwiseRuleGP->m_nNumberOfFeatures  = nNumberOfFeatures;
	m_pPairwiseRuleGP->m_pbSelectedFeatures = new bool[nNumberOfFeatures];
    
	for(int j = 0; j < nNumberOfFeatures; j++)
		m_pPairwiseRuleGP->m_pbSelectedFeatures[j] = true;
	
	if((bNormalize)||(bFillHoles))
		m_pPairwiseRuleGP->CalculateFeatureMeansAndVariations(m_pPairwiseLearningSet->m_punPairwiseLearnigSetMatrix,m_pPairwiseLearningSet->m_nNumberOfObjects);

	// initialize GP params
	
	m_param.C = 1e-3;
	m_param.cache_size = 40;
	m_param.eps = 1e-3;
	m_param.shrinking = 1;

	m_param.nr_weight = 0;
	m_param.weight_label = NULL;
	m_param.weight = NULL;

	m_problem.l = 0;
	m_problem.x = NULL;
	m_problem.y = NULL;
	m_x_space = NULL;


	m_nGridLeftCPower = GRID_LEFT_C_POWER;
	m_nGridRightCPower = GRID_RIGHT_C_POWER;
	m_nCStep = C_STEP;
	
	m_dBestParamC = 0.0;
	


}

CPairwiseGP::~CPairwiseGP()
{
    if (m_problem.y)
		free(m_problem.y);
	
	if (m_problem.x)
		free(m_problem.x);

	if (m_x_space)
		free(m_x_space);

	if (m_param.weight_label)
		free(m_param.weight_label);

	if (m_param.weight)
		free(m_param.weight);

	if(m_pPairwiseRuleGP)
		delete m_pPairwiseRuleGP;
}


// Represent learning set in GP library format
bool CPairwiseGP::FillInnerLearningSet()
{
	int nNumberOfObjects = m_pPairwiseLearningSet->m_nNumberOfObjects;
	int nNumberOfFeatures = m_pPairwiseLearningSet->m_nNumberOfFeatures;

	// Count objects with defined values:
	int elements=0;
	for (int i = 0; i < nNumberOfObjects; i++)
	{
		for (int j=0; j < nNumberOfFeatures; j++)
		{
			double dVal = m_pPairwiseLearningSet->m_punPairwiseLearnigSetMatrix[j + i*nNumberOfFeatures];
			if ((dVal != UNDEFINED_NUMERIC_VALUE) && (m_pPairwiseRuleGP->m_pbSelectedFeatures[j]))
				elements++;
		}
	}

	elements += nNumberOfObjects; // for the {-1, X} stuctures at the line ends
	m_problem.l = nNumberOfObjects;
	m_problem.y = Malloc(double,m_problem.l);
	m_problem.x = Malloc(struct GP_node *,m_problem.l);
	m_x_space = Malloc(struct GP_node, elements);

	int nIdx=0;		// Index in m_x_space
	for(int i =  0; i< nNumberOfObjects; i++)
	{
		m_problem.y[i] = m_pPairwiseLearningSet->m_pnPairwiseClassLabelesForObjects[i];
		m_problem.x[i] = &m_x_space[nIdx];
		
		int nNewFeatureIndex = 0;
		for (int j = 0; j < nNumberOfFeatures; j++)
		{
			if (!m_pPairwiseRuleGP->m_pbSelectedFeatures[j])
				continue;

		    unsigned int unVal = m_pPairwiseLearningSet->m_punPairwiseLearnigSetMatrix[j + i*nNumberOfFeatures];

			if((m_pPairwiseRuleGP->m_bNormalize)||(m_pPairwiseRuleGP->m_bFillHoles))
		        unVal = m_pPairwiseRuleGP->GetFeatureTransformation(j,unVal);
		
			
			if (unVal != UNDEFINED_NUMERIC_VALUE)
			{
				m_x_space[nIdx].index = nNewFeatureIndex+1;
				m_x_space[nIdx].value = (double)unVal;
				++nIdx;
			}
			nNewFeatureIndex ++;	
		}
		m_x_space[nIdx++].index = -1;
	}

	// set weights
	m_param.nr_weight = 2;
	m_param.weight = Malloc(double, m_param.nr_weight);
	m_param.weight_label = Malloc(int, m_param.nr_weight);

	memcpy (m_param.weight_label, m_pPairwiseLearningSet->m_pnPairwiseClassLabelesForObjects, m_param.nr_weight*sizeof(int));
	memcpy (m_param.weight,m_pPairwiseLearningSet->m_pdClassWeights,sizeof(double)*m_param.nr_weight);

	const char *error_msg = GP_check_parameter(&m_problem,&m_param);
	if(error_msg)
	{
		//"Error issued by GP_check_parameter()"
		return false;  
	}

	return true;
}



bool CPairwiseGP::EstimateParametersByGrid ()
{
	if(!m_pPairwiseLearningSet)
		return false;
	
	int nDeltaC = m_nGridRightCPower - m_nGridLeftCPower;
	
	double* pdGrid = new double[nDeltaC];
	memset(pdGrid,0,nDeltaC*sizeof(double));

	int nBestCIndex = 0;
	double dRate = 0.0;
	double dBestRate = 0.0;
	for (int i = 0; i < nDeltaC;  i += m_nCStep)					
	{
				
     	m_param.C = pow((double)2,m_nGridLeftCPower + i);
	
		dRate = MakeCrossValidation();
		pdGrid[i] = dRate;
		if (dRate > dBestRate)
		{
				dBestRate = dRate;
				nBestCIndex = i;
		}
		
	}

	int nCorridorLowestValue = int(pdGrid[nBestCIndex]*(1.0-DEFAULT_GREED_CORRIDOR_WIDTH)+0.5);
	int nBestCIndexInCorridor = nBestCIndex;
	for(int i =  nBestCIndex - 1; i >= 0; i--)
	{
		if (pdGrid[i] >= nCorridorLowestValue)
		{
			nBestCIndexInCorridor = i;
		}
	}


	m_dBestParamC = pow((double)2,m_nGridLeftCPower + nBestCIndexInCorridor);

	delete [] pdGrid;

	return true;
}


double CPairwiseGP::MakeCrossValidation ()
{
	double dRate = 0.0;
	double *target = Malloc(double,m_problem.l);

	GP_cross_validation(&m_problem,&m_param,m_nFolds,target);

	for(int i=0;i <m_problem.l; i++)
	{
		if(target[i] == m_problem.y[i])
			dRate += m_pPairwiseLearningSet->GetClassWeight((int)m_problem.y[i]);
	}
	
	free(target);


	return dRate;
}



bool CPairwiseGP::PairwiseLearning ( double dBestParamC)
{
	

	m_pPairwiseLearningSet->InitLearningSet(m_bUseWeightsOfClasses);

	int nNumberOfFeatures = m_pPairwiseLearningSet->m_nNumberOfFeatures;


	FillInnerLearningSet();

	if (m_nCrossValidationType == PAIRWISE_CROSS_VALIDATION)
	{
	    if(!EstimateParametersByGrid())
			return false;
	}
	else
	{
        m_dBestParamC = dBestParamC;
	
	}

	m_param.C = m_dBestParamC;

	m_pPairwiseRuleGP->m_model = GP_train ( &m_problem, &m_param );
	
	// Count number of elements in support vectors
	int nNumberOfGP = m_pPairwiseRuleGP->m_model->l;
	m_pPairwiseLearningSet->m_pbObjectIsInRule = new bool[m_pPairwiseLearningSet->m_nNumberOfObjects];
	memset(m_pPairwiseLearningSet->m_pbObjectIsInRule,0,sizeof(bool)*m_pPairwiseLearningSet->m_nNumberOfObjects);

	int nElements=0;
	GP_node * pCurrSV;

	for (int i = 0; i < nNumberOfGP; i++)
	{
		pCurrSV = m_pPairwiseRuleGP->m_model->SV[i];

		// find vector in problem and save its index in the learning set
		int j;
		for (j = 0; j < m_problem.l; j++)
		{
			if (pCurrSV == m_problem.x[j])
				break;
		}

		if (j < m_problem.l)
			m_pPairwiseLearningSet->m_pbObjectIsInRule[j] = true;

		while (pCurrSV->index != -1)
		{
			nElements++;
			pCurrSV ++;
		}

		nElements ++;	//for the {-1, X} stuctures at the line ends
	}

	// allocate space according to the counted nElements
	m_pPairwiseRuleGP->m_x_space = Malloc(struct GP_node, nElements);

	// copy SVs into storage
	int nIndex=0;		// Index in m_x_space
	for(int i =  0; i < nNumberOfGP; i++)
	{
		int nSVStartIndex = nIndex;
		pCurrSV = m_pPairwiseRuleGP->m_model->SV[i];
		while (pCurrSV->index != -1)
		{
			m_pPairwiseRuleGP->m_x_space[nIndex] = *pCurrSV;
			pCurrSV ++;
			nIndex++;
		}
		m_pPairwiseRuleGP->m_x_space[nIndex].index = -1;
		m_pPairwiseRuleGP->m_model->SV[i] = &m_pPairwiseRuleGP->m_x_space[nSVStartIndex];
		nIndex++;
	}


     m_pPairwiseRuleGP->MakeLinearPairRule();


	return true;
}


CMultiClassRuleGP::CMultiClassRuleGP (double dProbabilityThresholdForAlternative
										,double dRegectSensitivity)
{

	m_dProbabilityThresholdForAlternative = dProbabilityThresholdForAlternative;
	m_dRegectSensitivity = dRegectSensitivity;

    m_dNSigm = 10.0 - dRegectSensitivity;
	if(m_dNSigm < 0.0)
		m_dNSigm = 0.0;
	if(m_dNSigm > 10.0)
		m_dNSigm = 10.0;

	m_nNumberOfClasses = 0;
	m_nKernelType = 0;
	m_bNormalize = false;
	m_bFillHoles = false;
	m_pPairwiseRulesArray = NULL;
	m_nRulesSize = 0;
	m_nMaxRuleSize = 0;

	m_pnWinnerDistributionForClass = NULL;
	m_pnMaxNumberOfWinnersForClass = NULL;
	m_pnMinNumberOfWinnersForClass = NULL;

	m_pdMeanDist = NULL;
	m_pdVarDist  = NULL;
	m_pdWinnerCounter = NULL;

	m_pnListNearestClasses = NULL;
	

}

CMultiClassRuleGP::~CMultiClassRuleGP()
{   
	if(m_pPairwiseRulesArray)
		delete[] m_pPairwiseRulesArray;

	if(m_pnWinnerDistributionForClass)
		delete[] m_pnWinnerDistributionForClass;

	if(m_pnMaxNumberOfWinnersForClass)
		delete[] m_pnMaxNumberOfWinnersForClass;

	if(m_pnMinNumberOfWinnersForClass)
		delete[] m_pnMinNumberOfWinnersForClass;

	if(m_pdMeanDist)
		delete[] m_pdMeanDist;

	if(m_pdVarDist)
		delete[] m_pdVarDist;

	if(m_pdWinnerCounter)
		delete[] m_pdWinnerCounter;
     
	if(m_pnListNearestClasses)
		delete[] m_pnListNearestClasses;

	if(m_pnNumberOfNearestClasses )
		delete[] m_pnNumberOfNearestClasses;



}


bool CMultiClassRuleGP::RejectAnalysis( int nWinnerClassNumber
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

	for(int j =  0; j < m_nNumberOfClasses; j++)
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

double CMultiClassRuleGP::Probability(double dCurrentDist,double dMeanDist, double dVarDist)
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

void CMultiClassRuleGP::MakeArtificialWinnerArray( int nNumberWin
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




bool CMultiClassRuleGP::ProbabilityForAlternativeGroup( int nNumberOfAlternativeClasses
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
	for(int i =  0; i < m_nNumberOfClasses; i++)
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
	for(int i =  0; i < nNumberOfAlternativeClasses; i++)
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


bool CMultiClassRuleGP::AlternativeAnalysis( int nWinnerClassNumber
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
  for(int i =  0; i <  nNumberOfAlternativeClasses; i++)
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

    for(int i =  0; i <  nNumberOfAlternativeClasses; i++)
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
    for(int ll =  0; ll < nNumberOfAlternativeClasses - 1;ll++)
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

	for(int j =  0; j <  nNumberOfAlternativeClasses; j++)
	{ 
		if(pdProbabilityOfAlternativeClasses[j] <= 0.1)
			break;

		nCounter++;

    }

	nNumberOfAlternativeClasses = nCounter;

	return true;
    
}


bool CMultiClassRuleGP::SaveInformationForRegect(char* pcPath)
{

	 CFile f;

     int nBufferSize = BUFFER_SIZE;

	 char* pcFileName = new char[nBufferSize];
	 memset(pcFileName,0,sizeof(char)*nBufferSize);
	 strcat(pcFileName,pcPath);
     
	 strcat(pcFileName,"Regect.rul");

	 CFileException fileException;

	 if(!f.Open(pcFileName, CFile::modeCreate | CFile::modeWrite, &fileException))
	 {
          TRACE ("Cannot open file %s, error = %u\n",pcFileName, fileException.m_cause);
		  delete[] pcFileName;
	 	  return false;
     }

	 
	 float* pfBuffer = new float[m_nNumberOfClasses*m_nNumberOfClasses*(3 + 2*m_nNumberOfClasses) + m_nNumberOfClasses + 2];

     int nBufferLength = sizeof(float)*(m_nNumberOfClasses*m_nNumberOfClasses*(3 + 2*m_nNumberOfClasses) + m_nNumberOfClasses + 2);

	 memset(pfBuffer,0,nBufferLength);

	 pfBuffer[0] = (float)m_nNumberOfClasses;

	 pfBuffer[1] =  (float)m_dNSigm;

	 for( int i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
         pfBuffer[i + 2] = (float)m_pnMaxNumberOfWinnersForClass[i] ;
	 }

	 for(int i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
         pfBuffer[i + 2 + m_nNumberOfClasses*m_nNumberOfClasses] = (float)m_pnMinNumberOfWinnersForClass[i];
	 }

	 for(int i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
          pfBuffer[i + 2 + 2*m_nNumberOfClasses*m_nNumberOfClasses] = (float)m_pnListNearestClasses[i] ;
     }

	 for(int i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
         pfBuffer[i + 2 + 3*m_nNumberOfClasses*m_nNumberOfClasses] = (float)m_pdMeanDist[i] ;
	 }

	 for(int i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses; i++)
	 {
         pfBuffer[i + 2 + 3*m_nNumberOfClasses*m_nNumberOfClasses + m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses] = (float)m_pdVarDist[i] ;
	 }

	 for(int i = 0; i < m_nNumberOfClasses; i++)
	 {
        pfBuffer[i + 2 + 3*m_nNumberOfClasses*m_nNumberOfClasses + 2*m_nNumberOfClasses*m_nNumberOfClasses*m_nNumberOfClasses] = (float)m_pnNumberOfNearestClasses[i];
     }

     f.Write(pfBuffer,nBufferLength);
	
	 f.Close();

	 delete[] pfBuffer;
	 delete[] pcFileName;

	 return true;


}



void CMultiClassRuleGP::SaveParametersToIniFile(char* pcPath)
{
     int nBufferSize = BUFFER_SIZE;
	 char* pcFileName = new char[nBufferSize];
	 memset(pcFileName,0,sizeof(char)*nBufferSize);
	 strcat(pcFileName,pcPath);
    
	 strcat(pcFileName,"Multiclassification.ini");


     CFileException fileException;
	 CFile f;
	 
	 if(!f.Open(pcFileName, CFile::modeCreate | CFile::modeWrite, &fileException))
	 {
		 delete[] pcFileName;
	 	 return;
     }

	 f.Close();
	 	
	 char* pcStr = new char[nBufferSize];	

	 memset(pcStr,0,sizeof(char)*nBufferSize);

	 sprintf(pcStr,"%d", m_nNumberOfFeatures);

	 WritePrivateProfileString ("Features","Number Of Features",pcStr,pcFileName);

	 memset(pcStr,0,sizeof(char)*nBufferSize);

	 sprintf(pcStr,"%d", m_nNumberOfClasses);

	 WritePrivateProfileString ("Class Labels","Number Of Classes",pcStr,pcFileName);
	
    

    char* pcTmp = new char[nBufferSize];
	
	for(int i = 0; i < m_nNumberOfClasses;i++)
	{
        memset(pcStr,0,sizeof(char)*nBufferSize);
		memset(pcTmp,0,sizeof(char)*nBufferSize);
		strcat(pcTmp,"Label");
		sprintf(pcStr,"%d",i + 1);
		strcat(pcTmp,pcStr);
         
		memset(pcStr,0,sizeof(char)*nBufferSize);

		sprintf(pcStr,"%5d",m_pnClassLabels[i]);

		WritePrivateProfileString ("Class Labels",pcTmp,pcStr,pcFileName);
    }

	
	memset(pcStr,0,sizeof(char)*nBufferSize);

	sprintf(pcStr,"%4.2f ", m_dProbabilityThresholdForAlternative);

	WritePrivateProfileString ("Thresholds","Alternative Probability",pcStr,pcFileName);

	memset(pcStr,0,sizeof(char)*nBufferSize);

	sprintf(pcStr,"%4.2f ", m_dRegectSensitivity);

	WritePrivateProfileString("Thresholds","Regect Sensitivity",pcStr,pcFileName);

}

bool CMultiClassRuleGP::InitialForTC( int nNumberOfFeatures
									  ,int nNumberOfClasses
							          ,int* pnClassLabels)
{
	
    if(nNumberOfClasses < 2)
		return false;


	m_nNumberOfClasses = nNumberOfClasses;

	m_nNumberOfFeatures = nNumberOfFeatures;

	m_nMaxRuleSize = m_nNumberOfClasses*(m_nNumberOfClasses -1)/2;

	m_pPairwiseRulesArray = new CPairwiseRuleGP[m_nMaxRuleSize];

	m_pnClassLabels = new int[nNumberOfClasses];
	memcpy(m_pnClassLabels,pnClassLabels,sizeof(int)*nNumberOfClasses);

	m_pnWinnerDistributionForClass = new int[nNumberOfClasses*nNumberOfClasses*nNumberOfClasses]; 
	memset(m_pnWinnerDistributionForClass,0,sizeof(int)*nNumberOfClasses*nNumberOfClasses*nNumberOfClasses);

	m_pnMaxNumberOfWinnersForClass = new int[nNumberOfClasses*nNumberOfClasses];
	memset(m_pnMaxNumberOfWinnersForClass,0,sizeof(int)*nNumberOfClasses*nNumberOfClasses);

    m_pnMinNumberOfWinnersForClass = new int[nNumberOfClasses*nNumberOfClasses]; 
	memset(m_pnMinNumberOfWinnersForClass,0,sizeof(int)*nNumberOfClasses*nNumberOfClasses);

	m_pdMeanDist = new double[nNumberOfClasses*nNumberOfClasses*nNumberOfClasses];
	memset(m_pdMeanDist,0,sizeof(double)*nNumberOfClasses*nNumberOfClasses*nNumberOfClasses);

	m_pdVarDist  = new double[nNumberOfClasses*nNumberOfClasses*nNumberOfClasses];
	memset(m_pdVarDist,0,sizeof(double)*nNumberOfClasses*nNumberOfClasses*nNumberOfClasses);

	m_pdWinnerCounter = new double[nNumberOfClasses*nNumberOfClasses*nNumberOfClasses]; 
	memset(m_pdWinnerCounter,0,sizeof(double)*nNumberOfClasses*nNumberOfClasses*nNumberOfClasses);

	m_pnListNearestClasses = new int [nNumberOfClasses*nNumberOfClasses];
	memset(m_pnListNearestClasses,0,sizeof(int)*nNumberOfClasses*nNumberOfClasses);

    m_pnNumberOfNearestClasses = new int [nNumberOfClasses];
    memset(m_pnNumberOfNearestClasses,0,sizeof(int)*nNumberOfClasses);

	return true;

}

bool CMultiClassRuleGP::AddPairwiseRuleGP(CPairwiseRuleGP* pPairwiseRuleGP)
{
   if(!pPairwiseRuleGP)
	   return false;


   if(m_nRulesSize >= m_nMaxRuleSize)
	   return false; 

   m_pPairwiseRulesArray[m_nRulesSize].SetRule(pPairwiseRuleGP);

   
   m_nRulesSize++;

   return true;

}


int CMultiClassRuleGP::GetTCClassification( RObject* pObject
										   ,int& nWinnerClassLabel
										   ,double &dWinnerProbability
										   ,int& nNumberOfAlternativeClasses
										   ,double* pdProbabilityOfAlternativeClasses
										   ,int* pnAlternativeClassLabels)
{
    
   
   

	nWinnerClassLabel = -1;

	nNumberOfAlternativeClasses = 0;

     
	if(!m_nNumberOfClasses)
		return 0;

	if(!m_pPairwiseRulesArray)
		return 0;

	int nLearnClass = 0;

	for(int ll = 0; ll < m_nNumberOfClasses; ll++)
	{
		if(m_pnClassLabels[ll] == pObject->m_nClassLabel)
			 nLearnClass = ll + 1;
    } 

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

			  if(!m_pPairwiseRulesArray[k].ClassifyObjectByLinearPairRule (pObject->m_punFeaturesArray,d,dWinnerClassProbability))
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
	for(int j =  0; j < m_nNumberOfClasses; j++)
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
	 {
		 nWinnerClassNumber = 0;
     }
    
    

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


int CMultiClassRuleGP::GetTCClassification(unsigned int* punFeaturesArray,int nLearnClassNumber)
{
	if(!punFeaturesArray)
		return 0;
     
	if((!m_nNumberOfClasses)||(!m_pPairwiseRulesArray))
		return 0;

	if(m_nRulesSize != m_nMaxRuleSize)
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

			  if(!m_pPairwiseRulesArray[k].ClassifyObjectByLinearPairRule (punFeaturesArray,d,dWinnerClassProbability))
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
      
	  for(int j =  0; j < m_nNumberOfClasses; j++)
		m_pnWinnerDistributionForClass[(int)pnWinnerArray[j] + j*m_nNumberOfClasses + (nLearnClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses] += 1;


	  for(int i =  0; i < m_nNumberOfClasses; i++)
	  {
		  for(int j =  0; j < m_nNumberOfClasses; j++)
		  {
			  if(pdTournamentArray[i + j*m_nNumberOfClasses])
			  {
				 
				m_pdMeanDist[i + j*m_nNumberOfClasses + (nLearnClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses] += pdTournamentArray[i + j*m_nNumberOfClasses];
				m_pdVarDist [i + j*m_nNumberOfClasses + (nLearnClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses] += pdTournamentArray[i + j*m_nNumberOfClasses]*pdTournamentArray[i + j*m_nNumberOfClasses];
				m_pdWinnerCounter[i + j*m_nNumberOfClasses + (nLearnClassNumber - 1)*m_nNumberOfClasses*m_nNumberOfClasses]++;
              }
          } 
      }


   
     delete[] pdTournamentArray; 
     delete[] pnWinnerArray;	
	 return (nWinnerClassNumber);
}


///////////////////////////////////////Tournament Rules Generation Part//////////////////////////////////////////
CMultiClassGP::CMultiClassGP ( double dProbabilityThresholdForAlternative
							  ,double dRegectSensitivity
							  ,bool bFillHoles 
							  ,bool bNormalize
							  ,bool bUseWeightsOfClasses
							  ,int  nFolds)
									                  
{

    m_dProbabilityThresholdForAlternative = dProbabilityThresholdForAlternative;
	m_dRegectSensitivity = dRegectSensitivity;
	m_dNSigm = 10.0 - dRegectSensitivity;

	if(m_dNSigm < 0.0)
		m_dNSigm = 0.0;
	if(m_dNSigm > 10.0)
		m_dNSigm = 10.0;
	

	m_nFolds = nFolds;
	m_bFillHoles = bFillHoles;
	m_bNormalize = bNormalize;
	m_bUseWeightsOfClasses = bUseWeightsOfClasses;
	
	m_pMultiClassRuleGP = NULL;
	m_pLearningSet = NULL;


		// initialize GP params
	m_param.C = 1e-3;
	m_param.cache_size = 40;
	m_param.eps = 1e-3;
	m_param.shrinking = 1;
	m_param.nr_weight = 0;
	m_param.weight_label = NULL;
	m_param.weight = NULL;

	m_problem.l = 0;
	m_problem.x = NULL;
	m_problem.y = NULL;
	m_x_space = NULL;


	m_nGridLeftCPower = GRID_LEFT_C_POWER;
	m_nGridRightCPower = GRID_RIGHT_C_POWER;
    m_nCStep = C_STEP;



	m_dBestParamC = 0.0;
	m_dBestParamGamma = 0.0;

}

CMultiClassGP::~CMultiClassGP ()
{

	if(m_pMultiClassRuleGP)
		delete m_pMultiClassRuleGP;
	//ER
	if(m_pnClassLabels)
		delete [] m_pnClassLabels;

}


void CMultiClassGP::SetOutputPath(char* strOutputInfPath)
{
	m_strOutputInfPath = strOutputInfPath;
}

bool CMultiClassGP::FillInnerLearningSet()
{
	int nNumberOfObjects = m_pLearningSet->m_nNumberOfObjects;
	int nNumberOfFeatures = m_pLearningSet->m_nNumberOfFeatures;

	// Count objects with defined values:
	int elements=0;
	for (int i = 0; i < nNumberOfObjects; i++)
	{
		for (int j=0; j < nNumberOfFeatures; j++)
		{
			double dVal = m_pLearningSet->m_punLearnigSetMatrix[j + i*nNumberOfFeatures];
			if (dVal != UNDEFINED_NUMERIC_VALUE)
				elements++;
		}
	}

	elements += nNumberOfObjects; // for the {-1, X} stuctures at the line ends
	m_problem.l = nNumberOfObjects;
	m_problem.y = Malloc(double,m_problem.l);
	m_problem.x = Malloc(struct GP_node *,m_problem.l);
	m_x_space = Malloc(struct GP_node, elements);

	int nIdx=0;		// Index in m_x_space
	for(int i =  0; i< nNumberOfObjects; i++)
	{
		m_problem.y[i] = m_pLearningSet->m_pnNumericalClassLabelesForObjects[i];
		m_problem.x[i] = &m_x_space[nIdx];
		
		int nNewFeatureIndex = 0;
		for (int j = 0; j < nNumberOfFeatures; j++)
		{
			double dVal = m_pLearningSet->m_punLearnigSetMatrix[j + i*nNumberOfFeatures];

		
			if (dVal != UNDEFINED_NUMERIC_VALUE)
			{
					m_x_space[nIdx].index = nNewFeatureIndex+1;
					m_x_space[nIdx].value = dVal;
					++nIdx;
			}
			nNewFeatureIndex ++;
			
		}
		m_x_space[nIdx++].index = -1;
	}

	// set weights
	m_param.nr_weight = m_pLearningSet->m_nNumberOfClasses;
	m_param.weight = Malloc(double, m_param.nr_weight);
	m_param.weight_label = Malloc(int, m_param.nr_weight);

	memcpy (m_param.weight_label, m_pLearningSet->m_pnNumericalClassLabelesForObjects, m_param.nr_weight*sizeof(int));
	memcpy (m_param.weight,m_pLearningSet->m_pdClassWeights,sizeof(double)*m_param.nr_weight);

	const char *error_msg = GP_check_parameter(&m_problem,&m_param);
	if(error_msg)
	{
		//"Error issued by GP_check_parameter()"
		return false;  
	}

	return true;
}

double CMultiClassGP::MakeCrossValidation ()
{
	double dRate = 0.0;
	double *target = Malloc(double,m_problem.l);

	GP_cross_validation(&m_problem,&m_param,m_nFolds,target);

	for(int i=0;i<m_problem.l;i++)
	{
		if(target[i] == m_problem.y[i])
			dRate += m_pLearningSet->GetClassWeight((int)m_problem.y[i]);
	}
	
	free(target);


	return dRate;
}

bool CMultiClassGP::EstimateParametersByGrid ()
{   
	if(!m_pLearningSet)
		return false;

   
	int nDeltaC = m_nGridRightCPower - m_nGridLeftCPower;

	double* pdGrid = new double[nDeltaC];
	memset(pdGrid,0,nDeltaC*sizeof(double));
	int nBestCIndex = 0;
	double dRate = 0.0;
	double dBestRate = 0.0;
	for (int i = 0; i < nDeltaC;  i += m_nCStep)					
	{	
		m_param.C = pow((double)2,m_nGridLeftCPower + i);
		dRate = MakeCrossValidation ();
		pdGrid[i] = dRate;
		if (dRate > dBestRate)
		{
			dBestRate = dRate;
			nBestCIndex = i;
		}
		
	}

	int nCorridorLowestValue = (int)(pdGrid[nBestCIndex]*(1.0-DEFAULT_GREED_CORRIDOR_WIDTH)+0.5);
	int nBestCIndexInCorridor = nBestCIndex;
	for(int i =  nBestCIndex - 1; i >= 0; i--)
	{
		if (pdGrid[i] >= nCorridorLowestValue)
		{
			nBestCIndexInCorridor = i;
		}
	}

	m_dBestParamC = pow((double)2,m_nGridLeftCPower + nBestCIndexInCorridor);

	delete [] pdGrid;
	return true;

}





bool CMultiClassGP::MultiClassGPLearningForTC( LearningSet *pLearningSet
						                        ,double& dErrorPercent
												,double& dRejectPercent
						                        ,double* dErrorMatrix
						                        ,int* pnClassLabels
												,int  nCrossValidationType
												,double dBestParamC
												)
{

	 if(!pLearningSet)
		  return false;

      m_nCrossValidationType = nCrossValidationType;

      m_pLearningSet = pLearningSet;

      if(!m_pLearningSet->SetLearningData(m_bUseWeightsOfClasses))
      	  return false;

	  m_nNumberOfFeatures = m_pLearningSet->m_nNumberOfFeatures;

	  m_nNumberOfClasses = m_pLearningSet->m_nNumberOfClasses;

	  int nNumberOfObjects = m_pLearningSet->m_nNumberOfObjects;
	 
	  if(m_pLearningSet->m_pnClassLabels)
	  {
		m_pnClassLabels = new int[m_nNumberOfClasses];
		memcpy(m_pnClassLabels,m_pLearningSet->m_pnClassLabels,sizeof(int)*m_nNumberOfClasses);
      }

	  m_pMultiClassRuleGP = new CMultiClassRuleGP(m_dProbabilityThresholdForAlternative,m_dRegectSensitivity);

	 if(!m_pMultiClassRuleGP->InitialForTC(m_nNumberOfFeatures,m_nNumberOfClasses,m_pnClassLabels))
	 {
		 delete m_pMultiClassRuleGP;
		 m_pMultiClassRuleGP = NULL;
		 return false;
     }		 
    
	 FillInnerLearningSet();

	 if(m_nCrossValidationType == MULTICLASS_CROSS_VALIDATION)
	 {
	    if(!EstimateParametersByGrid())
			return false;
	 }
	 else
	 {
        m_dBestParamC = dBestParamC;
	
	 }


	 int k = 0;
	 bool bFlagStop = false;
	 
	 
	
	 for(int i = 0; i < m_nNumberOfClasses - 1; i++)
	 {
		 if(bFlagStop)
			break;



		 for(int j = i + 1; j < m_nNumberOfClasses; j++)
		 {   
			 if(bFlagStop)
				 break;
           
             
			PairwiseLearningSet* pPairwiseLearningSet = m_pLearningSet->GetPairwiseLearningSetForTC(i + 1,j + 1);


			 if(!pPairwiseLearningSet)
			 {
                 bFlagStop = true;
				 break;
			 } 	 
           
             
			 CPairwiseGP *pPairwiseGP  =  new CPairwiseGP( pPairwiseLearningSet
				                                          ,m_strOutputInfPath 
				                                          ,m_nFolds
														  ,m_bNormalize
														  ,m_bFillHoles
														  ,m_bUseWeightsOfClasses
														  ,m_nCrossValidationType);
				 

			

			 
			 if(!pPairwiseGP->PairwiseLearning(m_dBestParamC))
			 {
                  if(pPairwiseLearningSet)
					  delete pPairwiseLearningSet;

				  if(pPairwiseGP)
					  delete pPairwiseGP;

				  bFlagStop = true;
				  break;
             } 
               
			  
/*/////////////////Debug//////////////////////////////////////////
		   char buffer[40];
			 memset(buffer,0,sizeof(unsigned char)*40);

			 sprintf( buffer, "ClassNumber1 = %d  ClassNumber2 = %d", i,j);

             cout << buffer  << endl;
////////////////////////////////////////////*/
			 if(!m_pMultiClassRuleGP->AddPairwiseRuleGP(pPairwiseGP->m_pPairwiseRuleGP))
				 bFlagStop = true;
             
			 if(pPairwiseGP)
				delete pPairwiseGP;
          
			 if(pPairwiseLearningSet)
				delete pPairwiseLearningSet;
			 
			 k++;
			
         } 

	 }	

	 
	 if(bFlagStop)
	 {
         delete m_pMultiClassRuleGP;
		 m_pMultiClassRuleGP = NULL;
		 return false;
     }

     
	

     if(dErrorMatrix)
		memset(dErrorMatrix,0,sizeof(double)*(m_nNumberOfClasses + 1)*m_nNumberOfClasses); 

	 dErrorPercent = 0;
	 dRejectPercent = 0;
	 
     int* pnNumberObjectsInClass = new int[m_nNumberOfClasses]; 
	 memset(pnNumberObjectsInClass,0,sizeof(int)*m_nNumberOfClasses);

	 for(int i =  0; i < nNumberOfObjects; i++)
	 {

		 if(DEBUG)
		 {
////////////////Debug//////////////////////////////////////////
		   char buffer[BUFFER_SIZE];
		   memset(buffer,0,sizeof(unsigned char)*BUFFER_SIZE);

		   sprintf( buffer, "LearningSet Object %d from %d\n ", i + 1,nNumberOfObjects);

           printf(buffer);
////////////////////////////////////////////
         }

		int nLearnClassNumber = pLearningSet->m_pnNumericalClassLabelesForObjects[i];
		pnNumberObjectsInClass[nLearnClassNumber - 1]++;
		
	
		int nWinnerClassNumber = m_pMultiClassRuleGP->GetTCClassification(&pLearningSet->m_punLearnigSetMatrix[i*m_nNumberOfFeatures],nLearnClassNumber);


		
		if(dErrorMatrix)
			dErrorMatrix[nWinnerClassNumber + (m_nNumberOfClasses + 1)*nLearnClassNumber]++;

		if(nLearnClassNumber != nWinnerClassNumber)
		{
			if(nWinnerClassNumber)
				dErrorPercent++;
            else
				dRejectPercent++;
		   
/////////////////////////////////////////////////////////////////////////////////////
				int nRecognClassLabel = -1;

                int nLearnClassLabel =  pLearningSet->m_pObjectArray[i].m_nClassLabel;
		
				if(nWinnerClassNumber)
					nRecognClassLabel = pLearningSet->m_pnClassLabels[nWinnerClassNumber - 1];


				int nObjectIDInClass =  pLearningSet->m_pObjectArray[i].m_nObjectIDInClass;
               
               
        } 
	
     }

	 
     delete[] pnNumberObjectsInClass;
     
	 double  dCounter = 0.0;
	 double  dMean = 0.0;
	 double  dVar = 0.0;
	 int     nMaxNumberOfWinners = 0;
	 int     nMinNumberOfWinners = 0;
     
	

	 for(int k =  0; k < m_nNumberOfClasses; k++)
	 {
		 int* pnDelta = NULL;

		 for(int j = 0; j < m_nNumberOfClasses; j++)
		 {
			 dCounter = 0.0;
			 dMean = 0.0;
			 dVar = 0.0;

			 nMaxNumberOfWinners = 0;
			 nMinNumberOfWinners = 0;

			 for(int i = 0; i < m_nNumberOfClasses; i++)
			 {
				 double W = m_pMultiClassRuleGP->m_pnWinnerDistributionForClass[i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses];
				 
				 dMean += i*W;
				 dVar  += i*i*W;
				 dCounter += W;

					      
             }

			 if(dCounter)
			 {
				dMean /= dCounter;
				dVar /= dCounter;
				dVar -= dMean*dMean;
			    dVar = sqrt(dVar);
             }

			 

			 int nMaxNumberOfWinners = (int)(dMean + (m_dNSigm + 1)*dVar + 0.5);
			 int nMinNumberOfWinners = (int)(dMean - (m_dNSigm + 1)*dVar + 0.5);

			 if(nMaxNumberOfWinners > (m_nNumberOfClasses - 1))
				 nMaxNumberOfWinners = m_nNumberOfClasses - 1;

			 if(nMinNumberOfWinners < 0)
				 nMinNumberOfWinners = 0;

		

			 m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[j + k*m_nNumberOfClasses] = nMaxNumberOfWinners;
			 m_pMultiClassRuleGP->m_pnMinNumberOfWinnersForClass[j + k*m_nNumberOfClasses] = nMinNumberOfWinners;


         }

		 for(int j =  0; j < m_nNumberOfClasses; j++)
		 {
              if(j == k)
				  continue;

			  if(m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[j + k*m_nNumberOfClasses] >= m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[k + k*m_nNumberOfClasses])
				  m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[j + k*m_nNumberOfClasses] = m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[k + k*m_nNumberOfClasses] - 1;

         }

		 for(int j =  0; j < m_nNumberOfClasses; j++)
		 {
			 if(j == k)
				  continue;

			 if(m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[j + k*m_nNumberOfClasses] == m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[k + k*m_nNumberOfClasses] - 1)
					m_pMultiClassRuleGP->m_pnNumberOfNearestClasses[k]++; 
         }


     
         int l = 0;
        
		 pnDelta = new int[m_pMultiClassRuleGP->m_pnNumberOfNearestClasses[k]]; 

		 for(int j =  0; j < m_nNumberOfClasses; j++)
		 {
			 if(j == k)
				  continue;

			 if(m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[j + k*m_nNumberOfClasses] == m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[k + k*m_nNumberOfClasses] - 1)
			 {
				m_pMultiClassRuleGP->m_pnListNearestClasses[l + k*m_nNumberOfClasses] = j + 1; 

				pnDelta[l] = m_pMultiClassRuleGP->m_pnMaxNumberOfWinnersForClass[j + k*m_nNumberOfClasses] - m_pMultiClassRuleGP->m_pnMinNumberOfWinnersForClass[j + k*m_nNumberOfClasses];

				l++;
             }
		 }

		 for(int ll = 0; ll < m_pMultiClassRuleGP->m_pnNumberOfNearestClasses[k] - 1;ll++)
		 {
			for(int l = 0 ; l < m_pMultiClassRuleGP->m_pnNumberOfNearestClasses[k] - ll - 1; l++)
			{
				if(pnDelta[l] > pnDelta[l + 1])
				{
					int a = pnDelta[l];
					pnDelta[l] = pnDelta[l + 1];
					pnDelta[l + 1] = a;

					int b = m_pMultiClassRuleGP->m_pnListNearestClasses[l + k*m_nNumberOfClasses];
					m_pMultiClassRuleGP->m_pnListNearestClasses[l + k*m_nNumberOfClasses] = m_pMultiClassRuleGP->m_pnListNearestClasses[l + 1 + k*m_nNumberOfClasses];
					m_pMultiClassRuleGP->m_pnListNearestClasses[l + 1 + k*m_nNumberOfClasses] = b;

				}

			} 
		}
		
		if(pnDelta)
			delete[] pnDelta;
     }

	 for(int k =  0; k < m_nNumberOfClasses; k++)
	 {
		 for(int j = 0; j < m_nNumberOfClasses; j++)
		 {
			 for(int i = 0; i < m_nNumberOfClasses; i++)
			 {
				 if(m_pMultiClassRuleGP->m_pdWinnerCounter[i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses])
				 {
					m_pMultiClassRuleGP->m_pdMeanDist[i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses] /= m_pMultiClassRuleGP->m_pdWinnerCounter[i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses];
					m_pMultiClassRuleGP->m_pdVarDist [i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses] /= m_pMultiClassRuleGP->m_pdWinnerCounter[i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses];
					m_pMultiClassRuleGP->m_pdVarDist [i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses] -= m_pMultiClassRuleGP->m_pdMeanDist[i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses]*m_pMultiClassRuleGP->m_pdMeanDist[i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses];
					m_pMultiClassRuleGP->m_pdVarDist [i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses] = sqrt(m_pMultiClassRuleGP->m_pdVarDist [i + j*m_nNumberOfClasses + k*m_nNumberOfClasses*m_nNumberOfClasses]);
                 }
             }
         }
     }

	 
     if((dErrorMatrix)&&((nNumberOfObjects)))
	 {
		for(int i = 0; i < m_nNumberOfClasses*m_nNumberOfClasses;i++)
		{
			dErrorMatrix[i] /=  (double)nNumberOfObjects;
		
		}
     }
	 
	 if(nNumberOfObjects)
	 {
		dErrorPercent /= (double)nNumberOfObjects;
		dRejectPercent /= (double)nNumberOfObjects;
		
     }

     m_pMultiClassRuleGP->SaveInformationForRegect(m_strOutputInfPath);
	 m_pMultiClassRuleGP->SaveParametersToIniFile(m_strOutputInfPath);

	 return true;

}

bool CMultiClassGP::MultiClassTesting( LearningSet *pTestSet
								      ,double& dErrorPercent
									  ,double& dAlterPercent
									  ,double& dRejectPercent
									  ,double* pdErrorMatrix
									  ,bool bProtocolFlag)
 {

	 if(!pTestSet)
		 return false;

	 if(!m_nNumberOfClasses)
		 return false;


	 if(!m_pMultiClassRuleGP)
		 return false;

	 if(bProtocolFlag)
	 {	 
		 CleanProtocol(m_strOutputInfPath,m_pnClassLabels,m_nNumberOfClasses);
     }

	 int nNumberOfObjects = pTestSet->m_nSetSize;

     pdErrorMatrix = new double[(m_nNumberOfClasses + 1)*m_nNumberOfClasses];

	 if(pdErrorMatrix)
		memset(pdErrorMatrix,0,sizeof(double)*(m_nNumberOfClasses + 1)*m_nNumberOfClasses); 


	 ResaltStatistics* pResaltStatistics = new ResaltStatistics[m_nNumberOfClasses];

	 dErrorPercent = 0;
	 dRejectPercent = 0;
	 dAlterPercent = 0;
	 

	
	 double* pdProbabilityOfAlternativeClasses = new double[m_nNumberOfClasses];
	 int* pnAlternativeClassLabels = new int[m_nNumberOfClasses];


	 for(int i = 0; i < nNumberOfObjects; i++)
	 {
		int nWinnerClassLabel = -1;
		double dProbability = 0.0;
		int nNumberOfAlternativeClasses = 0;

		memset(pdProbabilityOfAlternativeClasses,0,sizeof(double)*m_nNumberOfClasses);
		memset(pnAlternativeClassLabels,0,sizeof(int)*m_nNumberOfClasses);

        int nObjectID =  pTestSet->m_pObjectArray[i].m_nObjectIDInClass;

        int nClussificationStatus = 0;

		if(DEBUG)
		{
   ////////////////Debug//////////////////////////////////////////
		   char buffer[BUFFER_SIZE];
		   memset(buffer,0,sizeof(unsigned char)*BUFFER_SIZE);

		   sprintf( buffer, "TestSet Object %d from %d\n", i + 1,pTestSet->m_nSetSize);

           printf( buffer);
        
////////////////////////////////////////////
        }
////////////////////Debug//////////////////
//	st_timer = 0;
//	fn_timer = 0;
//	tm = 0.0;
//	QueryPerformanceCounter((LARGE_INTEGER*)&st_timer);
 ////////////////////////////////////////////////////////

		int nWinnerClassNumber = m_pMultiClassRuleGP->GetTCClassification( &pTestSet->m_pObjectArray[i]
																		   ,nWinnerClassLabel
																		   ,dProbability
																		   ,nNumberOfAlternativeClasses
																		   ,pdProbabilityOfAlternativeClasses
																		   ,pnAlternativeClassLabels);

 /////////////////////////Debug////////////////////////////
//   QueryPerformanceCounter((LARGE_INTEGER*)&fn_timer);  
//	if (0<timer_freq)
//	{
//		 tm = (double)(fn_timer-st_timer)/(double)timer_freq;
//	}
//	float a = (float)tm;
///////////////////////////////////////////////////////////*/
      
	

		int nLearnClassLabel = pTestSet->m_pObjectArray[i].m_nClassLabel;

	

		int nLearnClassNumber = 0;

		for (int j = 0; j <  m_nNumberOfClasses; j++)
		{
			if(m_pnClassLabels[j] == nLearnClassLabel )
			{
                 	nLearnClassNumber = j + 1;
					break;
            }
		}


		if(pdErrorMatrix)
			pdErrorMatrix[(nLearnClassNumber - 1) + m_nNumberOfClasses*nWinnerClassNumber]++;


		if(nWinnerClassNumber)
		{

			if(nNumberOfAlternativeClasses)
			{
               
				if(nLearnClassLabel == nWinnerClassLabel)
				{
					 nClussificationStatus = 2;
				}
				else
				{
					nClussificationStatus = 3;

                    for(int i = 0; i < nNumberOfAlternativeClasses; i++)
					{
						if(nLearnClassLabel == pnAlternativeClassLabels[i])
						{
							nClussificationStatus = 2;
							break;
                        }
                    }

                }

            }
			else
			{

				if(nLearnClassLabel == nWinnerClassLabel)
				{
					 nClussificationStatus = 1;
				}
				else
				{
				     nClussificationStatus = 3;

                }
					

            } 

        }
		else
		{
           nClussificationStatus = 0;

        }

		if(nLearnClassNumber > 0)
		{
			switch(nClussificationStatus)
			{
           
			case 0:
				dRejectPercent++;
				pResaltStatistics[nLearnClassNumber - 1].dRejectPercent++;
				break;
			case 1:
				pResaltStatistics[nLearnClassNumber - 1].dOK_Percent++;
				break;

			case 2:
				dAlterPercent++;
				pResaltStatistics[nLearnClassNumber - 1].dAlterPercent++;
				break;

			case 3:
				dErrorPercent++;
				pResaltStatistics[nLearnClassNumber - 1].dErrorPercent++;
				break;


			}
        
			pResaltStatistics[nLearnClassNumber - 1].nNumberObjectsInClass++;

			if(nClussificationStatus == 3)
			{
				for(int k = 0; k < 10; k++)
				{
					if((dProbability > 0.1*k)&&(dProbability <= 0.1*(k + 1)))
					{
						pResaltStatistics[nLearnClassNumber - 1].pdConfidenceErrorHist[k]++;
					}
				}
			} 
        }

        if(bProtocolFlag)
		{
			
           
				SaveToProtocol( m_strOutputInfPath
					           ,nNumberOfObjects
					           ,nObjectID
							   ,nLearnClassLabel
							   ,nWinnerClassLabel
							   ,nClussificationStatus
							   ,dProbability
							   ,nNumberOfAlternativeClasses
							   ,pnAlternativeClassLabels);
							   
		}
		
	
     }

     delete[] pdProbabilityOfAlternativeClasses;
	 delete[] pnAlternativeClassLabels;

	 for(int i = 0; i < m_nNumberOfClasses;i ++)
	 {
          pResaltStatistics[i].nClassLabel = m_pnClassLabels[i];

		 for(int j = 0; j < 10; j++)
		 {
			 if(pResaltStatistics[i].dErrorPercent)
				pResaltStatistics[i].pdConfidenceErrorHist[j] /= pResaltStatistics[i].dErrorPercent;
         }

		 if(pResaltStatistics[i].nNumberObjectsInClass)
		 {
                pResaltStatistics[i].dOK_Percent   /= pResaltStatistics[i].nNumberObjectsInClass;

                pResaltStatistics[i].dAlterPercent /= pResaltStatistics[i].nNumberObjectsInClass;
		
                pResaltStatistics[i].dErrorPercent /= pResaltStatistics[i].nNumberObjectsInClass;

                pResaltStatistics[i].dRejectPercent /= pResaltStatistics[i].nNumberObjectsInClass;

				SaveStatisticResalt( m_strOutputInfPath,pResaltStatistics[i].nClassLabel,pResaltStatistics[i]);

         }


     }
     
	 if((pdErrorMatrix)&&(nNumberOfObjects))
	 {
		for(int  i = 0; i <= m_nNumberOfClasses;i++)
		{
			for(int j = 0; j < m_nNumberOfClasses;j++)
			{
				if(pResaltStatistics[j].nNumberObjectsInClass)
					pdErrorMatrix[j + i*m_nNumberOfClasses] /= pResaltStatistics[j].nNumberObjectsInClass;
				else
					pdErrorMatrix[j + i*m_nNumberOfClasses] = 0;


            }
         
        }
     }

	
	 if(nNumberOfObjects)
	 {
		dErrorPercent /= (double)nNumberOfObjects;
		dRejectPercent /= (double)nNumberOfObjects;
		dAlterPercent /= (double)nNumberOfObjects;
		
     }

	 if(bProtocolFlag)
	 {
			
		SaveGlobalResult( m_strOutputInfPath
					     ,m_nNumberOfClasses
					     ,dErrorPercent
					     ,dAlterPercent
					     ,dRejectPercent
					     ,m_pnClassLabels
					     ,pdErrorMatrix);
       
     }


    if(pResaltStatistics)
		delete[] pResaltStatistics;

    return true;
 } 

