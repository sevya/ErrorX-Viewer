/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceRecords.hh
@brief Collection of SequenceRecord objects that holds all
data associated with input sequences and corrected sequences
@details Holds all SequenceRecord objects associated with an
ErrorX query. In addition it holds ErrorXOptions that has 
settings for processing, and ErrorPredictor that does the error correction itself.
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef SEQUENCERECORDS_HH_
#define SEQUENCERECORDS_HH_

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

#include <vector>
#include <iostream>
#include <memory>
#include <map>
#include <functional>
#include <mutex>

#include "SequenceQuery.hh"
#include "SequenceRecord.hh"
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"
#include "ClonotypeGroup.hh"
#include "util.hh"

using namespace std;

namespace errorx {

class ERRORX_API SequenceRecords {

public:
	/**
		Destructor. Deletes references to member variables options_ 
		and predictor_. Also deletes references to every 
		SequenceRecord contained in records_. 
		Every time a SequenceRecords object is copied a deep copy is 
		made, so none of these pointers are shared.
	*/
	~SequenceRecords();

	/**
		Constructor using ErrorXOptions as argument.
	*/
	SequenceRecords( ErrorXOptions const & options );

	/**
		Copy constructor. Deep copy of all member variables.
	*/
	SequenceRecords( SequenceRecords const & other );

	/**
		Constructor from vector of SequenceRecords. Useful for
		concatenating all of these SequenceRecords into a single object. 
		Deep copy of all member variables so they can be safely 
		deleted after. Uses the ErrorXOptions member variable from
		the first item in the vector to get its options.
	*/
	SequenceRecords( vector<unique_ptr<SequenceRecords>> const & others );

	/**
		Constructor from SequenceRecord vector. Useful for
		concatenating many individual SequenceRecord objects
		into a single object. Deep copy of all member variables 
		so they can be safely deleted after.
	*/
	SequenceRecords( vector<SequenceRecordPtr> const & record_vector, ErrorXOptions const & options );

	/**
		Comparison operators
	*/
	bool operator==( SequenceRecords const & other ) const;
	bool operator!=( SequenceRecords const & other ) const;
	bool equals( unique_ptr<SequenceRecords> const & other ) const;
	bool equals( shared_ptr<SequenceRecords> const & other ) const;

	/**
		Populates SequenceRecords object with elements from
		a TSV file. TSV file should contain the following 
		columns, in order: 
		SequenceID, NTSequence, GermlineSequence, PhredScore
	*/
	void import_from_tsv();

	/**
		Populates SequenceRecords object with elements from
		a vector of SequenceQuery objects

		@param queries Vector of SequenceQuery objects to 
		use to fill SequenceRecords
	*/
	void import_from_list( vector<SequenceQuery> & queries );

	/**
		After correcting the member SequenceRecord objects, 
		calculates the error rate over the whole set 

		@return double value of estimated error rate
	*/
	double estimate_error_rate() const;

	/**
		Adds a SequenceRecord to the records_ member variable.
		Does not copy the record, just assigns it

		@param record SequenceRecord to add to records_
	*/
	void add_record( SequenceRecordPtr & record );

	/**
		Get the number of SequenceRecord objects held internally

		@return size of records_ variable
	*/
	int size() const;

	/**
		Get the SequenceRecord objects held internally

		@return records_ member variable
	*/
	vector<SequenceRecordPtr> get_records() const;

	/**
		Get a specific SequenceRecord

		@param i index of desired SequenceRecord

		@return SequenceRecord at index i
	*/
	SequenceRecordPtr get( int i ) const;

	/**
		Get number of "good" SequenceRecord objects
		SequenceRecords are defined as "good" by the
		IGBlastParser if they fit certain criteria,
		such as confident V assignment, productive, etc.
		Whether a nonproductive record is considered 
		"good" is determined by option allow_nonproductive
		in ErrorXOptions

		@return number of "good" records
	*/
	int good_records() const;

	/** 
		Get number of productive SequenceRecord objects

		@return number of productive records
	*/
	int productive_records() const;

	/**
		Returns a vector of string vectors, where each
		string vector is a summary of a SequenceRecord
		
		@param bool fulldata return all fields?
		@return vector of summaries of each SequenceRecord
	*/
	vector<vector<string>> get_summary( bool fulldata=1 ) const;

	/**
		Returns a vector of labels, where the labels describe
		the entries from get_summary()

		@param bool fulldata return all fields?
		@return vector of labels
	*/
	static vector<string> get_summary_labels( bool fulldata=1 );

	/**
		Wraps the get_summary() function and writes to 
		std out.
	*/
	void print_summary() const;

	/**
		Wraps the get_summary() function and writes to 
		the outfile_ variable of ErrorXOptions.
	*/
	void write_summary() const;

	/**
		Runs "mock" error correction protocol. When given a FASTA file
		I can't actually do error correction. So I just put the NT sequence
		in the "corrected" column
	*/
	void mock_correct_sequences();

	/**
		Runs error correction protocol on each SequenceRecord
		object. Modifies "records" in-place. Static qualifier
		allows it to be used easily in multi-threading.

		@param records Collection of SequenceRecord objects to be
		error corrected
	*/
	static void correct_sequences( unique_ptr<SequenceRecords> & records );
	
	/**
		For debugging purposes. Gets features from each SequenceRecord 
		object and writes to a file.
	*/
	void write_features();

	/**
		Get the options file used for these records
	*/
	ErrorXOptionsPtr get_options() const;

	
	/**
		Group all SequenceRecord objects into clonotypes, where 
		each SequenceRecords object is a clonotype. This allow you
		to figure out clonal lineages within clonotypes. Makes an
		assignment of each child SequenceRecord, not a copy
	*/
	void count_clonotypes();
	vector<ClonotypeGroup> clonotypes();

	/**
		Get the lengths of CDR loops in the dataset. Returns a 
		map in the format:
		{"CDR1": vector<int>{ 11,11,10 }, "CDR2": {}, "CDR3": {} }
	*/
	map<string,vector<int>> cdr_lengths();


	/**
		Functions to return the count of unique sequences and clonotypes
		of different types

		@param corrected Should unique sequences be calculated after 
		correction?
	*/
	int unique_nt_sequences( bool corrected );
	int unique_aa_sequences( bool corrected );

	int unique_clonotypes();
	map<string,int> vgene_counts();
	map<string,int> jgene_counts();
	map<string,int> vjgene_counts();
	// map<string,int,function<bool(string,string)>> clonotype_counts() const;
	
private:
	/**
		Chunks the current SequenceRecords object into N parts,
		where N is the number of threads used in multithreading.
		Deep copies of each member variable are made, so the 
		parent object can be safely deleted after chunking.
	*/
	vector<unique_ptr<SequenceRecords>> chunk_records();
	
	/**
		Corrects a single SequenceRecords object in one thread.

		@param records SequenceRecords object to be corrected
		@param update callback function to update progress that takes in
		(done,total,mutex*)
		@param m mutex* to keep track of threads
		@param total total number of records over all threads
	*/
	static void correct_sequences_threaded( unique_ptr<SequenceRecords> & records, function<void(int,int)>* update, mutex* m, int total);

	/** 
	============================= 
		  Member variables
	=============================
	*/
	vector<SequenceRecordPtr> records_;
	ErrorXOptionsPtr options_;
	ErrorPredictorPtr predictor_;

	/**
		Holds the different clonotypes contained in the dataset
	*/
	vector<ClonotypeGroup> clonotypes_;

};

typedef unique_ptr<SequenceRecords> SequenceRecordsPtr;
typedef shared_ptr<SequenceRecords> SequenceRecordsSP;


} // namespace errorx

#endif /* SEQUENCERECORDS_HH_ */
