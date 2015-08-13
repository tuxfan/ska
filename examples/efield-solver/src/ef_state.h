#ifndef EF_STATE_H
#define EF_STATE_H


typedef struct {
	double *phi;
	double *eps;
	double *debye;
	double *rhs;
	double *sol;
} ef_state;


#endif
