/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceFeatures.hh
@brief The features extracted from a nucleotide sequence to be 
used for neural network processing and error correction
@details This class calculates, from an NT sequence, the features
that are used in the NN implementation. It is then given to 
ErrorPredictor to do the actual transformation and prediction.
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef SEQUENCEFEATURES_HH_
#define SEQUENCEFEATURES_HH_

/// manages dllexport and import for windows
/// does nothing on Mac/Linux
#if defined(_WIN32) || defined(_WIN64)
#ifdef ERRORX_EXPORTS
#define ERRORX_API __declspec(dllexport)
#else
#define ERRORX_API __declspec(dllimport)
#endif
#else
#define ERRORX_API 
#endif

#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

// #include "SequenceRecord.hh"
// #include "keras_model.hh"

namespace errorx {

using namespace std;

// forward declare to avoid circular dependencies
class SequenceRecord;

class ERRORX_API SequenceFeatures {

public:	

	/**
		Constructor that uses a SequenceRecord object, as well as which position
		should be predicted as an error.

		@param record SequenceRecord that contains sequence to be corrected
		@param position which position along sequence should be predicted? 0-indexed.
		Should be < record.full_nt_sequence()

		@throws invalid_argument if position is out of bounds
	*/
	SequenceFeatures( SequenceRecord const & record, int position );

	/**
		Copy constructor
	*/
	SequenceFeatures( SequenceFeatures const & other );

	/**
		Destructor - does nothing
	*/
	~SequenceFeatures();

	/**
		Get the features from this sequence as a vector of length 229
	
		@return vector of features for NN processing as doubles
	*/
	vector<double> get_feature_vector() const;

	/**
		Encode a single NT as a binary vector
	
		@return sparse binary vector of length 6
	*/
	vector<int> nt_to_binary( char nt ) const;

	/**
		Get window surrounding the position of interest. 
		Two functions, either for a string or a vector of ints
	
		@return string of vector of ints with the surrounding window
	*/
	string get_window( string const & sequence, int start, int end ) const;
	vector<int> get_window( vector<int> const & array, int start, int end ) const;

	/**
		Calculates metrics related to a DNA sequence. Will compute the 
		GC content and the level of SHM and return a std::pair of those 
		values in that order.

		@param sequence DNA sequence
		@param gl_sequence Germline DNA sequence

		@return pair of <GC_pct,SHM>
	*/	
	pair<double,double> calculate_metrics( string const & sequence, string const & gl_sequence );


	/**
		Encode a full NT sequence as a binary vector
	
		@return sparse binary vector of length 6*sequence.size()
	*/
	vector<int> encode_sequence( string const & sequence ) const;
	
	/**
		Getter
	*/
	bool is_germline() const;
	vector<int> quality_window() const;

private:
	/**
		Translates a character from a PHRED string 
		to its integer counterpart. Assumes an offset
		of 33, as used by Illumina.

		@return int value corresponding to PHRED char
	*/
	int decode( char qual, int base );

	string sequence_window_;
	string gl_sequence_window_;
	vector<int> quality_window_;

	double global_GC_pct_;
	double local_GC_pct_;
	double global_SHM_;
	double local_SHM_;
	double global_quality_avg_;
	double local_quality_avg_;
	int is_germline_;

	vector<int> sequence_window_binary_;
	vector<int> gl_sequence_window_binary_;

};

} // namespace errorx


#endif /* SEQUENCEFEATURES_HH_ */
