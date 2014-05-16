#ifndef _GPLIB_H
#define _GPLIB_H

#ifdef __cplusplus
extern "C" {
#endif

struct GP_node
{
	int index;
	double value;
};

struct GP_problem
{
	int l;
	double *y;
	struct GP_node **x;
};


struct GP_parameter
{
	
	/* these are for training only */
	double cache_size; /* in MB */
	double eps;	/* stopping criteria */
	double C;
	int nr_weight;		
	int *weight_label;	
	double* weight;		
	int shrinking;	/* use the shrinking heuristics */
	
};

struct GP_model
{
	GP_parameter param;	// parameter
	int nr_class;		// number of classes, = 2 in regression/one class GP
	int l;			// total #SV
	GP_node **SV;		// SVs (SV[l])
	double **sv_coef;	// coefficients for SVs in decision functions (sv_coef[n-1][l])
	double *rho;		// constants in decision functions (rho[n*(n-1)/2])


	// for classification only

	int *label;		// label of each class (label[n])
	int *nSV;		// number of SVs for each class (nSV[n])
				// nSV[0] + nSV[1] + ... + nSV[n-1] = l

	
};
struct GP_model *GP_train(const struct GP_problem *prob, const struct GP_parameter *param);
void GP_cross_validation(const struct GP_problem *prob, const struct GP_parameter *param, int nr_fold, double *target);

int GP_get_nr_class(const struct GP_model *model);
void GP_get_labels(const struct GP_model *model, int *label);


void GP_predict_values(const struct GP_model *model, const struct GP_node *x, double* dec_values);
double GP_predict(const struct GP_model *model, const struct GP_node *x);


void GP_destroy_model(struct GP_model *model);
void GP_destroy_param(struct GP_parameter *param);

const char *GP_check_parameter(const struct GP_problem *prob, const struct GP_parameter *param);

#ifdef __cplusplus
}
#endif

#endif /* _LIBGP_H */
