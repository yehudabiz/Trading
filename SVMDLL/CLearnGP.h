

#ifndef __CLEARNGP_H__
#define __CLEARNGP_H__

#include "gpLib.h"
#define DEFAULT_NUMBER_OF_FOLDS 2
							


#define CROSS_VALIDATION_IS_NOT_USE 0
#define MULTICLASS_CROSS_VALIDATION 1
#define PAIRWISE_CROSS_VALIDATION   2


#define DEFAULT_GREED_CORRIDOR_WIDTH 0.05
#define GRID_LEFT_C_POWER -5
#define GRID_RIGHT_C_POWER 15
#define C_STEP 1




///////////////////////////////////////Pair Rule Storage Part//////////////////////////////////////////
struct ResaltStatistics
{
   int nClassLabel;
   double dOK_Percent;
   double dRejectPercent;
   double dAlterPercent;
   double dErrorPercent;
   double* pdConfidenceErrorHist;
   int nNumberObjectsInClass;

   ResaltStatistics();
   ~ResaltStatistics();


};

struct LearningSet;
struct PairwiseLearningSet;
struct RObject;

class CPairwiseRuleGP
{
public:

    char* m_strOutputInfPath;  
	int m_nNumercalLabelClass1;
    int m_nNumercalLabelClass2;

	int m_nClassLabel1;
	int m_nClassLabel2;

	int m_nNumberOfFeatures;
	bool *m_pbSelectedFeatures;
	
    double* m_pdHyperplane;
	double m_dB;

										// Kernel type
	GP_model* m_model;								   // Rule in GP library format
	GP_node* m_x_space;	                               // Vectors in GP library format

	bool	m_bNormalize;						       // Normalization flag 
	bool	m_bFillHoles;	                           // Holes filling flag

	double * m_pdFeatureMeans;						  // Used for normalization
	double * m_pdFeatureVariations;	

	CPairwiseRuleGP();
	CPairwiseRuleGP(char*strOutputInfPath);
	~CPairwiseRuleGP();

	void SetRule(CPairwiseRuleGP* pRule);
	void CopyModel(GP_model *model);
	
	void ClearMemory ();

    void CalculateFeatureMeansAndVariations(unsigned int* punPairwiseLearnigSetMatrix,int nNumberOfObjects);

	unsigned int GetFeatureTransformation( int unFeatureID 
										  ,unsigned int unFeatureValue);

	unsigned int GetFillHoleFeatureValue ( int unFeatureID 
										  ,unsigned int unFeatureValue);

	unsigned int GetNormalFeatureValue ( int unFeatureID 
									    ,unsigned int unFeatureValue);



	bool ClassifyObjectByLinearPairRule ( unsigned int* punObjectFeatures 
								         ,int& nWinnerClass
								         ,double& dWinnerClassProbability);

	bool MakeLinearPairRule();		

	bool ReadRuleFromBinaryList (const char* pcPath);
	bool WriteRuleToBinaryList(char* pcPath);

};


///////////////////////////////////////Pair Rule Generation Part//////////////////////////////////////////
class CPairwiseGP
{
public:

                                
	bool m_bUseWeightsOfClasses;
	int m_nFolds;
	int m_nCrossValidationType;
	     
	struct GP_problem m_problem;									// Learning set in GP library format
	struct GP_node * m_x_space;									
	struct GP_parameter m_param;									// Parameters in GP library format

    int m_nGridLeftCPower;											// Grid bounds
	int m_nGridRightCPower;
	int m_nCStep;

	double m_dBestParamC;											// Best C chosen by grid
									// Best gamma chosen by grid
	

    PairwiseLearningSet* m_pPairwiseLearningSet;

	CPairwiseRuleGP* m_pPairwiseRuleGP;

	bool InitLearningSet ();
	bool InitRule ();

	double MakeCrossValidation ();

	bool FillInnerLearningSet ();
	bool EstimateParametersByGrid ();								// C and gamma determination by grid


	CPairwiseGP( PairwiseLearningSet* pPairwiseLearningSet
		         ,char* m_strOutputInfPath  
		         ,int nFolds
				 ,bool bNormalize
				 ,bool bFillHoles		
				 ,bool bUseWeightsOfClasses
				 ,int  nCrossValidationType);

	~CPairwiseGP();

	bool PairwiseLearning ( double dBestParamC = 0.0);

	
};

///////////////////////////////////////Tournament Rule Generation Part//////////////////////////////////////////

// The class stores rules for pairs of classes
class CMultiClassRuleGP
{
public:
	int                 m_nNumberOfFeatures;   
	int					m_nNumberOfClasses;	                        
	int					m_nKernelType;							
	bool				m_bNormalize;						      
	bool				m_bFillHoles;


	double m_dProbabilityThresholdForAlternative;
	double m_dRegectSensitivity;
	double m_dNSigm;
	
	CPairwiseRuleGP*   m_pPairwiseRulesArray;

	int* m_pnWinnerDistributionForClass;
	int* m_pnMaxNumberOfWinnersForClass;
	int* m_pnMinNumberOfWinnersForClass;

    double* m_pdMeanDist;
	double* m_pdVarDist;
	double* m_pdWinnerCounter;  

	int* m_pnListNearestClasses;
	int *m_pnNumberOfNearestClasses;
	
	int m_nRulesSize; 
	int m_nMaxRuleSize;

	int* m_pnClassLabels;
								

	CMultiClassRuleGP ( double dProbabilityThreshold
		                ,double mdRegectSensitivity);
	
	~CMultiClassRuleGP ();

	bool InitialForTC( int nNumberOfFeatures 
		              ,int nNumberOfClasses
					  ,int* pnClassLabeles);
    bool AddPairwiseRuleGP(CPairwiseRuleGP* pPairwiseRuleGP);
	int GetTCClassification(unsigned int* punFeaturesArray,int nLearnClassNumber);
	int GetTCClassification( RObject* pObject
							,int& nWinnerClassLabel
							,double &Probability
							,int& nNumberOfAlternativeClasses
							,double* pdProbabilityOfAlternativeClasses
							,int* pnAlternativeClassLabels);

	bool RejectAnalysis( int nWinnerClassNumber
						,int nWinner2 
						,int* nWinnerArray
						,double* dTournamentArray
						,double& dProbability);

	bool AlternativeAnalysis( int nWinnerClassNumber
		                     ,int* pnWinnerArray
						     ,double* dTournamentArray
							 ,int& nNumberOfAlternativeClasses
							 ,double* pdProbabilityOfAlternativeClasses
							 ,int* nListAlternativeClasses
							 ,double dProbabilityThreshold);

	void MakeArtificialWinnerArray( int nNumberWin
							       ,int nArtificialWinnerClassNumber
							       ,double* pdTournamentArray
							       ,int* pdArtificialWinnerArray
							       ,double dProbabilityThreshold);

	bool ProbabilityForAlternativeGroup( int nNumberOfAlternativeClasses
										,int* pnListAlternativeClasses
										,int nWinnerClassNumber
										,double* pdTournamentArray
										,double& Probability);

	double Probability(double dCurrentDist,double dMeanDist, double dVarDist);


	bool SaveInformationForRegect(char* pcPath);	
	void SaveParametersToIniFile(char* pcPath);


	
};

///////////////////////////////////////Multi Class Rule Generation Part//////////////////////////////////////////

class CMultiClassGP
{
  
	char* m_strOutputInfPath;
	bool m_bNormalize;													// Normalization flag
	bool m_bFillHoles;                                                  // Holes Filling flag
	bool m_bUseWeightsOfClasses;
	
	LearningSet *m_pLearningSet;

    double m_dProbabilityThresholdForAlternative;
	double m_dRegectSensitivity;
	double m_dNSigm;


	int  m_nFolds;
	int  m_nCrossValidationType;
	
	int m_nNumberOfFeatures;
	int m_nNumberOfClasses;

	struct GP_problem m_problem;									// Learning set in GP library format
	struct GP_node *m_x_space;									
	struct GP_parameter m_param;                                    // Parameters in GP library format

	int m_nGridLeftCPower;											// Grid bounds
	int m_nGridRightCPower;
	int m_nCStep;

	int m_nGridLeftGammaPower;
	int m_nGridRightGammaPower;
	int m_nGStep;

	double m_dBestParamC;											// Best C chosen by grid
	double m_dBestParamGamma;										// Best gamma chosen by grid
	

	int* m_pnClassLabels;



	CMultiClassRuleGP* m_pMultiClassRuleGP;

	bool InitLearningSet ();

	bool InitRule ();

	double MakeCrossValidation ();

	bool FillInnerLearningSet();

    bool EstimateParametersByGrid();
										 
public:

	CMultiClassGP ( double dProbabilityThresholdForAlternative = 0.5
		            ,double dRegectSensitivity = 7
					,bool bFillHoles = false 
					,bool bNormalize = false
					,bool bUseWeightsOfClasses = false
					,int  nFolds = DEFAULT_NUMBER_OF_FOLDS);

	~CMultiClassGP();

	void SetOutputPath(char* strOutputInfPath);

	bool MultiClassGPLearningForTC( LearningSet *pLearningSet
									,double& dErrorPercent
									,double& dRejectPercent
									,double* dErrorMatrix = NULL
									,int* pnClassLabels = NULL
									,int  nCrossValidationType = CROSS_VALIDATION_IS_NOT_USE
									,double dBestParamC = 0.03125);

	bool MultiClassTesting( LearningSet *pTestSet
						   ,double& dErrorPercent
						   ,double& dAlterPercent
						   ,double& dRejectPercent
						   ,double* pdErrorMatrix
						   ,bool bProtocolFlag);

};


#endif

