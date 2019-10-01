/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ErrorPredictor.hh
@brief Runs neural network prediction on DNA sequence features
@details Features of a DNA sequence are generated by SequenceFeatures,
then this class is used to make a NN prediction
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef ERRORPREDICTOR_HH_
#define ERRORPREDICTOR_HH_

#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

// #include "keras_model.hh"
#include "keras/KerasModel.hh"
#include "ErrorXOptions.hh"
#include "SequenceFeatures.hh"

using namespace std;

namespace errorx {

class ErrorPredictor {

public:
	/**
		Default constructor
	*/
	ErrorPredictor( ErrorXOptions const & options );

	/**
		Copy constructor
	*/	
	ErrorPredictor( ErrorPredictor const & other );

	
	/**
		Applies neural network to features contained in
		SequenceFeatures object

		@param features SequenceFeatures object containing features
		of interest

		@return double representing NN prediction for this base
	*/
	double apply_model( SequenceFeatures const & features ) const;

	/**
		Applies neural network to features contained in
		a vector of double vectors. Expects 228 features per vector

		@param features vector of vectors containing features
		of interest

		@return vector of doubles representing NN prediction for each set of features
	*/
	vector<double> apply_model( vector<vector<double>> const features ) const;

private:

	ErrorXOptions options_;
	keras::KerasModel keras_model_;
};

typedef unique_ptr<ErrorPredictor> ErrorPredictorPtr;


} // namespace errorx


#endif /* ERRORPREDICTOR_HH_ */
