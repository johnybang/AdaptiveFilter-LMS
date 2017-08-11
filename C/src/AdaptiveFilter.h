/*
 * @file AdaptiveFilter.h
 *
 * Header file for AdaptiveFilter.c
 *
 * Created on: Apr 14, 2014
 * Author: John Bang
 */

#ifndef ADAPTIVEFILTER_H_
#define ADAPTIVEFILTER_H_

/* Contains Adaptive Filter parameters (StepSize,Regularization,Length)
 * and state info (Buffer, BufferIdx, Weights, and Error)
 */
typedef struct {
	const double StepSize; /* adaptive filter step size */
    const double Regularization; /* regularization constant */
	const unsigned int Length; /* length of filter */
	double *pBuffer; /* pointer to input buffer */
    unsigned int BufferIdx; /* circular index into input buffer */
	double *pWeights; /* pointer to adaptive filter weights */
	double Error; /* pointer to output error (desired - output) state */
} AfData;

double AdaptiveFilterRun (double input, double desired, AfData *pData);
double AdaptiveFilterRunErrorIn(double input, double error, AfData *pData);

#endif /* ADAPTIVEFILTER_H_ */
