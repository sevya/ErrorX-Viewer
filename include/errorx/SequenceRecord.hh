/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceRecord.hh
@brief A record of one sequence to be error corrected
@details Single nucleotide sequence to be fed to error
correction. Can be initialized from a FASTQ file, TSV file,
or directly from a SequenceQuery.
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef SEQUENCERECORD_HH_
#define SEQUENCERECORD_HH_

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
#include <string>
#include <vector>
#include <memory>

#include "AbSequence.hh"
#include "SequenceQuery.hh"
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"

using namespace std;

namespace errorx {

class ERRORX_API SequenceRecord {

public:
	
	/**
		Empty constructor. Initializes all variables to
		empty string or 0.
	*/	
	SequenceRecord();

	/**
		Destructor - does nothing
	*/
	~SequenceRecord();
	
	/**
		Copy constructor. Copies all member variables
	*/	
	SequenceRecord( SequenceRecord const & copy );

	/**
		Constructs a SequenceRecord from an AbSequence object

		@param sequence AbSequence object
	*/
	SequenceRecord( AbSequence const & sequence );


	/**
		Constructor from vector<string> - this is used
		when building a SequenceRecord from an output ErrorX tsv file
		if the 
	*/
	SequenceRecord( vector<string> const & items );

	/**
		Constructs a SequenceRecord from a SequenceQuery.
		The SequenceQuery contains only the minimal info
		needed for error correction, which is sequence ID,
		NT sequence, germline sequence, and phred score.

		@param query SequenceQuery object to correct
	*/
	SequenceRecord( SequenceQuery & query );

	/**
		Comparison operator
	*/
	bool operator==( SequenceRecord const & other ) const;
	bool operator!=( SequenceRecord const & other ) const;
	bool equals( shared_ptr<SequenceRecord> const & other ) const;

	/**
		Get all the information summarizing a SequenceRecord

		@param fulldata include all fields?
		
		@return vector of strings summarizing the record
	*/
	vector<string> get_summary( bool fulldata=1 ) const;

	/**
		Print key information related to this record
	*/
	void print() const;

	/**
		Correct this SequenceRecord and modify in place with 
		the corrected sequence. Params are generally passed
		by a parent SequenceRecords object.

		@param predictor ErrorPredictor for prediction
		@param options Options for processing
	*/
	void correct_sequence( ErrorPredictor const & predictor, 
						   ErrorXOptions const & options );


	/**
		Get the predicted probability of error for each base.

		@return vector std::pair, where first is the position
		and second is the probability of error
	*/
	vector<pair<int,double>> get_predicted_errors() const;

	/**
		Return whether this record is "good". Records can be called
		"bad" for the following reasons:
		1. IGBlast query does not fit the correct format and couldn't be parsed
		2. V gene Evalue > 1e-3
		3. Sequence is not productive (unless allow_nonproductive_ is True)
		4. PHRED score is not same length as sequence

		@return whether sequence is considered good
	*/
	bool isGood() const;

	/**
		Get the numeric features associated with this record. Params are generally passed
		by a parent SequenceRecords object.

		@param predictor ErrorPredictor for prediction
		@param options Options for processing
	*/
	vector<vector<double>> get_features( ErrorPredictor const & predictor,
		ErrorXOptions const & options );

	/**
		Getters and setters
	*/
	AbSequence sequence() const;

	string full_nt_sequence() const;
	string full_gl_nt_sequence() const;
	string full_nt_sequence_corrected() const;
	string cdr3_aa_sequence() const;
	string full_aa_sequence() const;
	string full_aa_sequence_corrected() const;
	string v_gene() const;
	string d_gene() const;
	string j_gene() const;
	string v_gene_noallele() const;
	string d_gene_noallele() const;
	string j_gene_noallele() const;
	string clonotype() const;
	bool valid_clonotype() const;

	double v_identity() const;
	double d_identity() const;
	double j_identity() const;
	int n_errors() const;
	string sequenceID() const;
	string chain() const;
	bool productive() const;
	string quality_string() const;
	int gl_start() const;
	int translation_frame() const;

	void v_gene( string const & vgene );
	void j_gene( string const & jgene );
	void cdr3_aa_sequence( string const & cdr3_aa_sequence );
	void full_nt_sequence( string const & seq );
	void full_nt_sequence_corrected( string const & seq );
	void full_aa_sequence( string const & seq );
	void full_aa_sequence_corrected( string const & seq );

private:
	
	/**
		Predict error probabilities for each base and modify
		this record in place. Wrapped by correct_sequence.

		@param predictor ErrorPredictor for prediction
		@param options Options for processing
	*/
	void predict_errors( ErrorPredictor const & predictor,
			ErrorXOptions const & options );

	AbSequence sequence_;

	int n_errors_;
	vector<pair<int,double>> predicted_errors_all_;

};

typedef shared_ptr<SequenceRecord> SequenceRecordPtr;

} // namespace errorx


#endif /* SEQUENCERECORD_HH_ */
