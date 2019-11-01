/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file constants.hh
@brief Numerical constants used during the ErrorX protocol
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef CONSTANTS_HH_
#define CONSTANTS_HH_

namespace errorx {
namespace constants {

/** 
	Optimized threshold for best balance between precision and recall
	this value may change in future releases but I want it to be 
	defined here
*/
const double OPTIMIZED_THRESHOLD = 0.730736;
const double OPTIMIZED_PRECISION =  0.75;
const double OPTIMIZED_RECALL = 0.3267;

const std::vector<double> OPTIMIZED_THRESHOLD_VECTOR = {
		0.413772, // precision 0.50, recall 0.49
		0.606544, // precision 0.66, recall 0.39
		OPTIMIZED_THRESHOLD, 
		0.865236, // precision 0.85, recall 0.23
		0.973858  // precision 0.95, recall 0.09
		};

/**
	Sequence window used for feature calculation
*/
const int WINDOW = 8;

/**
	E value cutoffs when assigning V, D, and J genes
*/
const double V_EVALUE_CUTOFF = 1e-3;
const double D_EVALUE_CUTOFF = 0.01;
const double J_EVALUE_CUTOFF = 0.01;


/**
	The number of queries you can run for free without
	a license
*/
const int FREE_QUERIES = 100;

} // namespace constants
} // namespace errorx

#endif // CONSTANTS_HH_
