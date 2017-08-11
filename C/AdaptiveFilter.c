/*
 * @file AdaptiveFilter.c
 *  
 * Adaptive Filter implements a normalized least-mean-square adaptive filter.
 * Routines are provided for "desired signal" input or "error signal" input.
 *
 * Created on: Apr 14, 2014
 * Author: John Bang
 */

/******************************************************************************/
/* include block */
#include "AdaptiveFilter.h"

/******************************************************************************/
/** local definitions **/
static void AdaptWeights(AfData *pData);
static double Filter(double input, AfData *pData);
static double SquaredNorm(double *x, unsigned int length);

/******************************************************************************
 * AdaptiveFilterRun
 *
 * @param[in]     input  input signal sample
 * @param[in]     desired desired signal sample
 * @param[in,out] pData  pointer to AdaptiveFilter parameter/state struct
 *
 * @returns       adaptive filter output (estimate of desired signal)
 *
 * @note          Runs the normalized least mean square adaptive filter and
 *  computes a new output.
 *
 * @warning       none
 */
double AdaptiveFilterRun(double input, double desired, AfData *pData) {
	double output;

	output = Filter(input, pData); /* filter the input */
	pData->Error = desired - output; /* update the error */
	AdaptWeights(pData); /* update adaptive filter weights */

	return output;
}
/* End of AdaptiveFilterRun() */
/******************************************************************************/

/******************************************************************************
 * AdaptiveFilterRunErrorIn
 *
 * @param[in]     input  input signal sample
 * @param[in]     error error signal sample (desired - output)
 * @param[in,out] pData  pointer to AdaptiveFilter parameter/state struct
 *
 * @returns       adaptive filter output (estimate of desired signal)
 *
 * @note          Runs the normalized least mean square adaptive filter and
 *  computes a new output.
 *
 * @warning       none
 */
double AdaptiveFilterRunErrorIn(double input, double error, AfData *pData) {
	double output;

	pData->Error = error; /* update the error */
	AdaptWeights(pData); /* update adaptive filter weights */
	output = Filter(input, pData); /* filter the input */

	return output;
}
/* End of AdaptiveFilterRunErrorIn() */
/******************************************************************************/

/** internal functions **/

/***************************************************************************//**
* AdaptWeights
* 
* @param[in,out]     pData pointer to AdaptiveFilter parameter/state struct
*
* @returns       none
* 
* @note          Updates the filter weights in pData->pWeights using the
*  canonical normalized least mean square algorithm.
* 
* @warning       none
*******************************************************************************/
static void AdaptWeights(AfData *pData) {
	double sn, normStepSize;
	int i;

	sn = SquaredNorm(pData->pBuffer,pData->Length); /* compute norm term */
	normStepSize = (pData->StepSize)/(pData->Regularization + sn); /* normalize step size */

	for ( i = pData->Length - 1; i >= 0; i--) {
        /* wrap index */
        if (pData->BufferIdx >= pData->Length) {
            pData->BufferIdx = 0;
        }
        /* Normalized Least Mean Square update equation */
		pData->pWeights[i] += normStepSize * (pData->Error) * (pData->pBuffer[pData->BufferIdx++]);
	}
}
/* End of AdaptWeights()*/
/******************************************************************************/

/***************************************************************************//**
* Filter
* 
* @param[in]     input input signal sample
* @param[in,out]     pData pointer to AdaptiveFilter parameter/state struct
*
* @returns       new filter output
* 
* @note          Computes a new output sample using the input and current
*  filter weights.
*
* 
* @warning       none
*******************************************************************************/
static double Filter(double input, AfData *pData) {
	double output = 0;
	int i;
    
    /* wrap index */
    if (pData->BufferIdx >= pData->Length) {
        pData->BufferIdx = 0;
    }
    /* overwrite oldest input with new input */
	pData->pBuffer[pData->BufferIdx++] = input;

	for (i = pData->Length - 1; i >= 0; i--) {
        /* wrap index */
        if (pData->BufferIdx >= pData->Length) {
            pData->BufferIdx = 0;
        }
        /* compute inner product of weight vector and buffer */
		output += (pData->pWeights[i]) * (pData->pBuffer[pData->BufferIdx++]);
	}
    
	return output;
}
/* End of Filter()*/
/******************************************************************************/

/***************************************************************************//**
* SquaredNorm
* 
* @param[in]     pInput pointer to a buffer of input samples
* @param[in]     length length of the buffer
*
* @returns       squared L2-norm of the input buffer
* 
* @note          Computes the squared L2-norm of the input buffer which is
*  the sum of each every element squared.
* 
* @warning       none
*******************************************************************************/
static double SquaredNorm(double *pInput, const unsigned int length) {
	double output = 0;
	unsigned int i;

    /* Note: this could be computationally improved, but is currently done
     * this way to definitively avoid numerical error accumulation due to
     * the properties of floating point addition.
     */
	for ( i = 0; i < length; i++ ) {
		output += pInput[i]*pInput[i]; /* accumulate squared elements */
	}
    
	return output;
}
/* End of SquaredNorm()*/
/******************************************************************************/


