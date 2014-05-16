#ifndef __GP_H__
#define __GP_H__



#define UNDEFINED_NUMERIC_VALUE  -1 
/*
struct RObject
{
	int m_nObjectID;
	int m_nObjectIDInClass;
	int m_nNumberOfFeatures;
	int m_nClassLabel;
	unsigned int *m_punFeaturesArray;
	bool m_bFlagObjectIsUse;
    
	RObject();

	~RObject();

};
*/

class CPairwiseGeneralizedPortraitRule
{
public:

	int m_nNumercalLabelClass1;
    int m_nNumercalLabelClass2;

	int m_nClassLabel1;
	int m_nClassLabel2;

	int m_nNumberOfFeatures;
	
    double* m_pdHyperplane;
	double m_dB;
	
    bool ReadRuleFromBinaryList(const char* pcPath);

	public:
   
	bool ClassifyObjectByLinearPairRule ( unsigned int* punObjectFeatures 
								         ,int& nWinnerClass
								         ,double& dWinnerClassProbability);

	bool LoadRule( const char* pcPath
		         ,int nNumercalLabelClass1
				 ,int nNumercalLabelClass2
				 ,int nClassLabel1
				 ,int nClassLabel2
				 ,int nMaxNumberOfFeatures
				 );

	CPairwiseGeneralizedPortraitRule();
	~CPairwiseGeneralizedPortraitRule();
};


class CMultiGeneralizedPortraitRule
{	    
	int                 m_nNumberOfFeatures;   
	int					m_nNumberOfClasses;	                        
	double              m_dProbabilityThresholdForAlternative;
	double              m_dNSigm;


	
	CPairwiseGeneralizedPortraitRule*   m_pPairwiseGeneralizedPortraitRulesArray;

	int* m_pnMaxNumberOfWinnersForClass;
	int* m_pnMinNumberOfWinnersForClass;

    double* m_pdMeanDist;
	double* m_pdVarDist;

	int* m_pnListNearestClasses;
    int* m_pnNumberOfNearestClasses;

	int m_nRulesSize; 
	int* m_pnClassLabels;



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
							       ,int* pnArtificialWinnerArray
							       ,double dProbabilityThreshold);

	bool ProbabilityForAlternativeGroup( int nNumberOfAlternativeClasses
										,int* pnListAlternativeClasses
										,int nWinnerClassNumber
										,double* pdTournamentArray
										,double& dProbability);


	double Probability(double dCurrentDist,double dMeanDist, double dVarDist);


	bool ReadInformationForRegect(const char* pcPath);
	bool ReadParametersFromIniFile(const char* pcPath);

  

public:							
	char m_pcPath[200];

	CMultiGeneralizedPortraitRule();
	
	~CMultiGeneralizedPortraitRule ();

	bool LoadRules(const char* pcPath);

  
int GetObjectClassification(const char* rulespath 
							,unsigned int* punFeaturesArray
							,int& nWinnerClassLabel
							,double &dWinnerProbability
							,int& nNumberOfAlternativeClasses
							,double* pdProbabilityOfAlternativeClasses
						    ,int* pnAlternativeClassLabels);

	
};


#endif

