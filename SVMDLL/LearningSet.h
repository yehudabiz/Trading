#ifndef _LEARNINGSET_H
#define _LEARNINGSET_H


#define UNDEFINED_NUMERIC_VALUE  -1 
struct RObject
{
	int m_nObjectID;
	int m_nObjectIDInClass;
	int m_nNumberOfFeatures;
	int m_nClassLabel;
	unsigned int *m_punFeaturesArray;
	bool m_bFlagObjectIsUse;

	void SetObject(RObject* pObject);
	bool MakeProectionalObject(RObject* pObject);
    
	RObject();

	~RObject();

};


struct PairwiseLearningSet
{
	int   m_nNumercalLabelClass1;
	int   m_nNumercalLabelClass2;
	int   m_nClassLabel1;
	int   m_nClassLabel2;

	int   m_nNumberOfObjects;
	int   m_nNumberOfObjectsInClass1;
	int   m_nNumberOfObjectsInClass2;
    int   m_nNumberOfFeatures;

	double* m_pdClassWeights;

	bool m_bMatrixIsNormalized;


	unsigned int  *m_punPairwiseLearnigSetMatrix;
	int           *m_pnPairwiseClassLabelesForObjects;
	bool          *m_pbObjectIsInRule;

	double* m_pdFeatureMeans;
	double* m_pdFeatureVariations;

	void CalculateFeatureMeansAndVariations();
	void NormalizeFeatureValues();

	bool InitLearningSet(bool bUseWeightsOfClasses = false,bool bFillHoles = false, bool bNormalize = false);
	void FillUndefinedFeatureValuesWithMeanValues();
	double GetClassWeight(int nClass);
    PairwiseLearningSet();
	~PairwiseLearningSet();
	

};

struct LearningSet
{
	int m_nSetSize;
	int m_nNumberOfFeatures;
	int m_nNumberOfClasses;
	int m_nNumberOfObjects;

	RObject* m_pObjectArray;

	unsigned int* m_punLearnigSetMatrix;
 
    int* m_pnNumberObjectsInClasses;
  
    int* m_pnClassLabels;
   
    int* m_pnNumericalClassLabelesForObjects;

	double* m_pdClassWeights;		
	
     LearningSet();
	~LearningSet();
	

	void Init(int nLearningSetSize,int nNumberOfFeatures);

	bool SetLearningData(bool bUseWeightsOfClasses = false);

	double GetClassWeight(int nClass);

	PairwiseLearningSet* GetPairwiseLearningSetForTC( int nNumericalLabelClass1 
	                                                 ,int nNumericalLabelClass2
													 ,bool bUseClassWeights = false);



	bool AddNewLearningSet(LearningSet* pLearningSet);
	bool AddObject(unsigned int* punFeaturesArray, int nClassLabel);
	void SetNumberOfFeatures(int nNumberOfFeatures);
	void SetNumberOfObjects(int nNumberOfObjects);

	bool MakeProectionalLearningSet();


};


#endif