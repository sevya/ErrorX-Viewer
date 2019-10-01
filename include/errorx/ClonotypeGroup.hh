/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ClonotypeGroup.hh
@brief Collection of SequenceRecord objects that represent a single clonotype
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef CLONOTYPEGROUP_HH_
#define CLONOTYPEGROUP_HH_

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
#include <string>

#include "SequenceRecord.hh"
#include "ErrorXOptions.hh"

using namespace std;

namespace errorx {

class ERRORX_API ClonotypeGroup {

public:
	/**
		Destructor. Does not delete references to individual
		SequenceRecord pointers bc these are owned by the 
		SequenceRecords object that spawns the ClonotypeGroup
	*/
	~ClonotypeGroup();

	/**
		Constructor with ErrorXOptions
	*/
	ClonotypeGroup( ErrorXOptions const & options );

	/**
		Copy constructor. Deep copy of all member variables.
	*/
	ClonotypeGroup( ClonotypeGroup const & other );

	/**
		Equality operator. Counts clonotypes as the same if
		they differ only by X amino acids in the CDR3
	*/
	bool operator==( ClonotypeGroup const & other );
	bool operator!=( ClonotypeGroup const & other );

	/**
		Get number of somatic variants within the clonotype, on the 
		nucleotide or amino acid level, either
		using corrected sequences or uncorrected

		@param corrected Calculate variants after correction?

		@return Number of somatic variants
	*/
	int somatic_variants( bool corrected ) const;
	int somatic_variants_aa( bool corrected ) const;


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
		Get a specific SequenceRecord

		@param i index of desired SequenceRecord

		@return SequenceRecord at index i
	*/
	SequenceRecordPtr get( int i ) const;

	/**
		Getters and setters
	*/
	string v_gene() const;
	string j_gene() const;
	string cdr3() const;
	string toString() const;

	void v_gene( string const & vgene );
	void j_gene( string const & gene );
	void cdr3( string const & cdr3 );

private:
	string vgene_;
	string jgene_;
	string cdr3_;

	vector<SequenceRecordPtr> records_;
	ErrorXOptions options_;
};

} // namespace errorx

#endif /* CLONOTYPEGROUP_HH_ */
