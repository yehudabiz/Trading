#include "stdafx.h"
#include <math.h>
#include <float.h>
#include <direct.h>
#include <iostream>
#include <stdlib.h>
#include "LearningSet.h"


int sortLabeles(const void* elem1,const void * elem2 )
{
	  RObject *ptr1, *ptr2;

	  ptr1=(RObject *)elem1;

	  ptr2=(RObject *)elem2;

	  if(ptr1->m_nClassLabel < ptr2->m_nClassLabel ) return -1;

	  if(ptr1->m_nClassLabel == ptr2->m_nClassLabel)
	  {
		  if(ptr1->m_nObjectIDInClass < ptr2->m_nObjectIDInClass) return -1;
		  if(ptr1->m_nObjectIDInClass == ptr2->m_nObjectIDInClass)return  0;
		  if(ptr1->m_nObjectIDInClass > ptr2->m_nObjectIDInClass) return  1;
			  
      }

	  if(ptr1->m_nClassLabel > ptr2->m_nClassLabel ) return 1;

	  return 1;

 }


RObject::RObject()
{ 
	m_nObjectID = 0;
	m_nObjectIDInClass = 0;
	m_nNumberOfFeatures = 0;
	m_nClassLabel = -1;
	m_punFeaturesArray = NULL;
	m_bFlagObjectIsUse = false;


}

RObject::~RObject()
{
   if(m_punFeaturesArray)
		delete[] m_punFeaturesArray;
}

void RObject::SetObject(RObject* pObject)
{

	m_nObjectID = pObject->m_nObjectID;
	m_nObjectIDInClass = pObject->m_nObjectIDInClass;
	m_nNumberOfFeatures = pObject->m_nNumberOfFeatures;
	m_nClassLabel = pObject->m_nClassLabel;
	m_punFeaturesArray = new unsigned int[m_nNumberOfFeatures];
	memcpy(m_punFeaturesArray,pObject->m_punFeaturesArray,sizeof(unsigned int)*m_nNumberOfFeatures);
	m_bFlagObjectIsUse = pObject->m_bFlagObjectIsUse;

}

bool RObject::MakeProectionalObject(RObject* pObject)
{

	m_nObjectID = pObject->m_nObjectID;
	m_nObjectIDInClass = pObject->m_nObjectIDInClass;
	m_nNumberOfFeatures = int(4*sqrt((double)pObject->m_nNumberOfFeatures));
	m_nClassLabel = pObject->m_nClassLabel;
	m_punFeaturesArray = new unsigned int[m_nNumberOfFeatures];
	memset(m_punFeaturesArray,0,sizeof(unsigned int)*m_nNumberOfFeatures);

	int nImgX = (int)sqrt((double)pObject->m_nNumberOfFeatures);
	int nImgY = (int)sqrt((double)pObject->m_nNumberOfFeatures);
 

    for( int x = 0; x < nImgX; x++)
	{
		bool bFlag = false;	
		int nCounter = 0;

        for( int y = 0; y < nImgY; y++)
		{
			if(!pObject->m_punFeaturesArray[x + y*nImgX])
			{
			   bFlag = true;

			   m_punFeaturesArray[x + 2*nImgX]++;

			   if(y == (nImgY - 1))
				   nCounter++;
			}
			else
			{
				if(bFlag == true)
				{
					nCounter++;
					bFlag = false;	

                }

			}
        }
        
		m_punFeaturesArray[x] = nCounter;
    } 

	for( int y = 0; y < nImgY; y++)
	{
		bool bFlag = false;	
		int nCounter = 0;

        for( int x = 0; x < nImgX; x++)
		{
			if(!pObject->m_punFeaturesArray[x + y*nImgX])
			{
			   bFlag = true;

			   m_punFeaturesArray[y + 3*nImgX]++;

			   if(x == (nImgX - 1))
				   nCounter++;
			}
			else
			{
				if(bFlag == true)
				{
					nCounter++;
					bFlag = false;	

                }

			}
        }
        
		m_punFeaturesArray[y + nImgX] = nCounter;
    } 

	m_bFlagObjectIsUse = pObject->m_bFlagObjectIsUse;

	return true;

}


	


PairwiseLearningSet* LearningSet::GetPairwiseLearningSetForTC( int nNumericalLabelClass1 
	                                                          ,int nNumericalLabelClass2
															  ,bool bUseClassWeights)
 {  
    if((nNumericalLabelClass1 < 1)||(nNumericalLabelClass2 < 1))
		return NULL;

    if((nNumericalLabelClass1 > m_nNumberOfClasses)||(nNumericalLabelClass2 > m_nNumberOfClasses))
		return NULL;

	if((!m_pnNumberObjectsInClasses[nNumericalLabelClass1 - 1])||(!m_pnNumberObjectsInClasses[nNumericalLabelClass2 - 1]))
		return NULL;

   PairwiseLearningSet* pPairwiseLearningSet = new PairwiseLearningSet();

   pPairwiseLearningSet->m_nNumercalLabelClass1 = nNumericalLabelClass1;
   pPairwiseLearningSet->m_nNumercalLabelClass2 = nNumericalLabelClass2;
   pPairwiseLearningSet->m_nClassLabel1 = m_pnClassLabels[nNumericalLabelClass1 - 1];
   pPairwiseLearningSet->m_nClassLabel2 = m_pnClassLabels[nNumericalLabelClass2 - 1];;
   pPairwiseLearningSet->m_nNumberOfObjectsInClass1 = m_pnNumberObjectsInClasses[nNumericalLabelClass1 - 1];
   pPairwiseLearningSet->m_nNumberOfObjectsInClass2 = m_pnNumberObjectsInClasses[nNumericalLabelClass2 - 1];
   pPairwiseLearningSet->m_nNumberOfObjects = pPairwiseLearningSet->m_nNumberOfObjectsInClass1 + pPairwiseLearningSet->m_nNumberOfObjectsInClass2;
   pPairwiseLearningSet->m_nNumberOfFeatures = m_nNumberOfFeatures;
   pPairwiseLearningSet->m_pnPairwiseClassLabelesForObjects = new int[pPairwiseLearningSet->m_nNumberOfObjects];
   pPairwiseLearningSet->m_punPairwiseLearnigSetMatrix = new unsigned int[m_nNumberOfFeatures*pPairwiseLearningSet->m_nNumberOfObjects];

   int nClassBegin = 0;
   

   for(int i = 0; i < nNumericalLabelClass1 - 1; i++)
   {
       nClassBegin += m_pnNumberObjectsInClasses[i]; 
   }

   int nNumberObjectsInClass1 = pPairwiseLearningSet->m_nNumberOfObjectsInClass1;
    
   memcpy(pPairwiseLearningSet->m_punPairwiseLearnigSetMatrix, &m_punLearnigSetMatrix[nClassBegin*m_nNumberOfFeatures], sizeof(unsigned int)*nNumberObjectsInClass1*m_nNumberOfFeatures);

   for(int i = 0; i < nNumberObjectsInClass1; i++)
	   pPairwiseLearningSet->m_pnPairwiseClassLabelesForObjects[i] = 1;



   nClassBegin = 0;
   for(int i =  0; i < nNumericalLabelClass2 - 1; i++)
   {
       nClassBegin += m_pnNumberObjectsInClasses[i]; 
   }

   int nNumberObjectsInClass2 =  pPairwiseLearningSet->m_nNumberOfObjectsInClass2;
    
   memcpy(&pPairwiseLearningSet->m_punPairwiseLearnigSetMatrix[nNumberObjectsInClass1*m_nNumberOfFeatures], &m_punLearnigSetMatrix[nClassBegin*m_nNumberOfFeatures], sizeof(unsigned int)*nNumberObjectsInClass2*m_nNumberOfFeatures);
   
   for(int i = nNumberObjectsInClass1; i < pPairwiseLearningSet->m_nNumberOfObjects; i++)
	   pPairwiseLearningSet->m_pnPairwiseClassLabelesForObjects[i] = -1;

  
   return(pPairwiseLearningSet);

}

 

PairwiseLearningSet::PairwiseLearningSet()
{
	m_nNumercalLabelClass1 = -1;;
	m_nNumercalLabelClass2 = -1;
	m_nClassLabel1 = -1;
	m_nClassLabel2 = -1;

	m_nNumberOfObjects = 0;
	m_nNumberOfObjectsInClass1 = 0;
	m_nNumberOfObjectsInClass2 = 0;
    m_nNumberOfFeatures = 0;


	m_punPairwiseLearnigSetMatrix = NULL;
	m_pnPairwiseClassLabelesForObjects = NULL;

	m_pdFeatureMeans = NULL;
	m_pdFeatureVariations = NULL;
	m_pdClassWeights = NULL;
	m_pbObjectIsInRule = NULL;
}

PairwiseLearningSet::~PairwiseLearningSet()
{
	if(m_punPairwiseLearnigSetMatrix)
	   delete[] m_punPairwiseLearnigSetMatrix;

	if(m_pnPairwiseClassLabelesForObjects)
		delete[] m_pnPairwiseClassLabelesForObjects;

	if(m_pdFeatureMeans)
		delete[] m_pdFeatureMeans;

	if(m_pdFeatureVariations)
		delete[] m_pdFeatureVariations; 

	if(m_pdClassWeights)
		delete[] m_pdClassWeights;

	if(m_pbObjectIsInRule)
		delete[] m_pbObjectIsInRule ;

  
}

void PairwiseLearningSet::CalculateFeatureMeansAndVariations()
{
	int nDefinedFeatures;
	double dFeatVal;
    
    m_pdFeatureMeans = new double[m_nNumberOfFeatures];
	memset(m_pdFeatureMeans,0,sizeof(double)*m_nNumberOfFeatures);
	m_pdFeatureVariations = new double[m_nNumberOfFeatures];
	memset(m_pdFeatureVariations,0,sizeof(double)*m_nNumberOfFeatures);

	for (int i = 0; i< m_nNumberOfFeatures; i++)
	{	
		nDefinedFeatures = 0;
		for (int j = 0; j < m_nNumberOfObjects; j++)
		{
			dFeatVal = m_punPairwiseLearnigSetMatrix[i + j*m_nNumberOfFeatures];

			if (dFeatVal != UNDEFINED_NUMERIC_VALUE)
			{
				nDefinedFeatures ++;
				m_pdFeatureMeans[i]+=	dFeatVal;
				m_pdFeatureVariations[i]+= dFeatVal*dFeatVal;
			}
		}

		if(nDefinedFeatures)
		{
			m_pdFeatureMeans[i] /= nDefinedFeatures;
			m_pdFeatureVariations[i] /= nDefinedFeatures;
			m_pdFeatureVariations[i] -= m_pdFeatureMeans[i]*m_pdFeatureMeans[i];

			if (m_pdFeatureVariations[i] >= 0.0)
				m_pdFeatureVariations[i] = sqrt(m_pdFeatureVariations[i]);
			else
				m_pdFeatureVariations[i] = 0.0;
		}
	}
}


void PairwiseLearningSet::NormalizeFeatureValues()
{
	for (int i=0; i<m_nNumberOfObjects; i++)
	{	
		for (int j=0; j<m_nNumberOfFeatures; j++)
		{
			double dFeatVal = m_punPairwiseLearnigSetMatrix[j+i*m_nNumberOfFeatures];
			if (dFeatVal != UNDEFINED_NUMERIC_VALUE && m_pdFeatureVariations[j])
			{	
				if(m_pdFeatureVariations[j])
					dFeatVal = (dFeatVal - m_pdFeatureMeans[j]) / (6.0 * m_pdFeatureVariations[j]) + 0.5;

				if	(dFeatVal > 1.0)
					dFeatVal = 1.0;
				else if (dFeatVal < 0.0)
					dFeatVal = 0.0;

				m_punPairwiseLearnigSetMatrix[j+i*m_nNumberOfFeatures] = (unsigned int)dFeatVal;
			}
		}
	}

	m_bMatrixIsNormalized = true;
}



double PairwiseLearningSet::GetClassWeight(int nClass)
{


	if (nClass == m_nNumercalLabelClass1)
		return m_pdClassWeights[0];
	else
		return m_pdClassWeights[1];
}

bool PairwiseLearningSet::InitLearningSet(bool bUseWeightsOfClasses,bool bFillHoles, bool bNormalize)
{

	m_pdClassWeights = new double[2];

	if(bUseWeightsOfClasses)
	{
		m_pdClassWeights[0] = m_nNumberOfObjectsInClass2/(m_nNumberOfObjectsInClass2 + m_nNumberOfObjectsInClass1);
		m_pdClassWeights[1] = m_nNumberOfObjectsInClass1/(m_nNumberOfObjectsInClass2 + m_nNumberOfObjectsInClass1);
    }
	else
	{
		m_pdClassWeights[0] = 1;
		m_pdClassWeights[1] = 1;
    } 
	
	if((bFillHoles)||(bNormalize))
		CalculateFeatureMeansAndVariations();

	if (bFillHoles)
	{
 		FillUndefinedFeatureValuesWithMeanValues();
	}

	if (bNormalize)
	{
		NormalizeFeatureValues();
	}


	return true;
}

void PairwiseLearningSet::FillUndefinedFeatureValuesWithMeanValues()
{
	for (int i=0; i< m_nNumberOfObjects; i++)
	{	
		for (int j=0; j<m_nNumberOfFeatures; j++)
		{
			double dFeatVal = m_punPairwiseLearnigSetMatrix[j + i*m_nNumberOfFeatures];
			if (dFeatVal == UNDEFINED_NUMERIC_VALUE)
			{			
				if (m_bMatrixIsNormalized)
					m_punPairwiseLearnigSetMatrix[j + i*m_nNumberOfFeatures] = 500;
				else
					m_punPairwiseLearnigSetMatrix[j + i*m_nNumberOfFeatures] = (unsigned int)(m_pdFeatureMeans[j] + 0.5);
			}
		}
	}
}





LearningSet::LearningSet()
{
	m_nSetSize = 0;
	m_nNumberOfFeatures = 0;
	m_nNumberOfClasses = 0;
	m_nNumberOfObjects = 0;

	m_pObjectArray = NULL;

    m_punLearnigSetMatrix = NULL;
 
    m_pnNumberObjectsInClasses = NULL;
  
    m_pnClassLabels = NULL;

	m_pdClassWeights = NULL;
   
    m_pnNumericalClassLabelesForObjects = NULL;
   
}

LearningSet::~LearningSet()
{
	if(m_pObjectArray)
		delete[] m_pObjectArray;


	if(m_punLearnigSetMatrix)
		delete[] m_punLearnigSetMatrix;
 
	if(m_pnNumberObjectsInClasses)
		delete[] m_pnNumberObjectsInClasses;
  
	if(m_pnNumberObjectsInClasses)
		delete[] m_pnClassLabels;
   
	if(m_pnNumberObjectsInClasses)
		delete[] m_pnNumericalClassLabelesForObjects;

	if(m_pdClassWeights)
		delete[] m_pdClassWeights;

}



bool LearningSet::SetLearningData(bool bUseWeightsOfClasses)
{  
   
   if(!m_nSetSize)
	   return false;

   if(!m_nNumberOfFeatures)
	   return false;

   qsort(m_pObjectArray,m_nSetSize,sizeof(RObject),sortLabeles);

   int nOldLabel = -1;

   m_nNumberOfClasses = 0;
  
   for(int i = 0; i < m_nSetSize; i ++)
   {   
	   if(m_pObjectArray[i].m_nClassLabel !=  nOldLabel)
	   {
		       m_nNumberOfClasses++;
			   nOldLabel = m_pObjectArray[i].m_nClassLabel;   
	   }
   } 

   if(!m_nNumberOfClasses)
	   return false;


   m_punLearnigSetMatrix = new unsigned int[m_nNumberOfFeatures*m_nNumberOfObjects];
   memset(m_punLearnigSetMatrix,0,sizeof(unsigned int)*m_nNumberOfFeatures*m_nNumberOfObjects);
  
   m_pnNumberObjectsInClasses = new int[m_nNumberOfClasses];
   memset(m_pnNumberObjectsInClasses,0,sizeof(int)*m_nNumberOfClasses);

   m_pnClassLabels = new int[m_nNumberOfClasses];

   for(int i =  0; i < m_nNumberOfClasses; i++)
	   m_pnClassLabels[i] = -1;

  
   m_pnNumericalClassLabelesForObjects = new int[m_nNumberOfObjects];
   memset(m_pnNumericalClassLabelesForObjects,0,sizeof(int)*m_nNumberOfObjects);


   nOldLabel = -1;
   int nClassNumber = 0;
  
   for(int i =  0; i < m_nSetSize; i ++)
   {

	   if(m_pObjectArray[i].m_bFlagObjectIsUse)
	   {   
		   memcpy(&m_punLearnigSetMatrix[i*m_nNumberOfFeatures],m_pObjectArray[i].m_punFeaturesArray,sizeof(unsigned int)*m_nNumberOfFeatures);
           
		   if(m_pObjectArray[i].m_nClassLabel !=  nOldLabel)
		   {
			   nClassNumber++; 
			   nOldLabel = m_pObjectArray[i].m_nClassLabel;
			   m_pnClassLabels[nClassNumber - 1] = nOldLabel;
           }

		   m_pObjectArray[i].m_nObjectIDInClass =  m_pnNumberObjectsInClasses[nClassNumber - 1];

           m_pnNumberObjectsInClasses[nClassNumber - 1]++;
		   
		   m_pnNumericalClassLabelesForObjects[i] = nClassNumber;
           
       }
   } 

   m_pdClassWeights = new double[m_nNumberOfClasses]; 

   memset(m_pdClassWeights,0,sizeof(double)*m_nNumberOfClasses);

   if(bUseWeightsOfClasses)
   {   
	   double dSum = 0.0; 

	   for(int i = 0; i < m_nNumberOfClasses; i++)
	   {
		   if(m_pnNumberObjectsInClasses[i])
		   {;
			   dSum += (1/m_pnNumberObjectsInClasses[i]);
			   m_pdClassWeights[i] = 1/m_pnNumberObjectsInClasses[i];
           }
       }

	   if(dSum)
	   {
			for(int i = 0; i < m_nNumberOfClasses; i++)
				m_pdClassWeights[i] /= dSum;
			
	   }
		
   }
   else
   {
       for(int i = 0; i < m_nNumberOfClasses; i++)
		   m_pdClassWeights[i] = 1;
   }

   return true; 
}

double LearningSet::GetClassWeight(int nClass)
{
	return m_pdClassWeights[nClass];
}



bool LearningSet::MakeProectionalLearningSet()
{
	if(!m_nSetSize)
		return false;
    
	RObject* pNewObjectArray = new RObject[m_nSetSize];

    for (int i = 0; i < m_nSetSize; i++)
	{
       pNewObjectArray[i].MakeProectionalObject(&m_pObjectArray[i]);
    }

    m_nNumberOfFeatures = pNewObjectArray[0].m_nNumberOfFeatures;

	delete[] m_pObjectArray;

	m_pObjectArray = pNewObjectArray;

	return true;
}

bool LearningSet::AddObject(unsigned int* punFeaturesArray, int nClassLabel)
{
   if(m_nSetSize >= m_nNumberOfObjects)
	   return false;

	if(nClassLabel == -1)
		return false; 

	m_pObjectArray[m_nSetSize].m_bFlagObjectIsUse = true;
    m_pObjectArray[m_nSetSize].m_nClassLabel = nClassLabel;
	m_pObjectArray[m_nSetSize].m_nObjectID = m_nSetSize;
	m_pObjectArray[m_nSetSize].m_nNumberOfFeatures = m_nNumberOfFeatures; 
	m_pObjectArray[m_nSetSize].m_punFeaturesArray = new unsigned int[m_nNumberOfFeatures];
	memcpy(m_pObjectArray[m_nSetSize].m_punFeaturesArray,punFeaturesArray,sizeof(unsigned int)*m_nNumberOfFeatures);
	m_nSetSize++;
	return true;
	
}

void LearningSet::SetNumberOfFeatures(int nNumberOfFeatures)
{
    m_nNumberOfFeatures = nNumberOfFeatures;
}

void LearningSet::SetNumberOfObjects(int nNumberOfObjects)
{
	m_nNumberOfObjects = nNumberOfObjects;
	m_pObjectArray = new RObject[m_nNumberOfObjects];
}