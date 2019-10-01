/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file AbSequence.hh
@brief Represents an antibody sequence
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef ABSEQUENCE_HH_
#define ABSEQUENCE_HH_

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

#include <string>
#include <vector>
#include "ErrorXOptions.hh"

using namespace std;

namespace errorx { 


class ERRORX_API AbSequence { 

public:
	/**
		Empty constructor
	*/
	AbSequence();

	/**
		Copy constructor
	*/
	AbSequence( AbSequence const & other );

	/**
		Destructor - does nothing
	*/
	~AbSequence();
	
	/**
		Comparison operator
	*/
	bool operator==( AbSequence const & other ) const;
	bool operator!=( AbSequence const & other ) const;

	void build( ErrorXOptions const & options );

	void print() const;

	////// Getters
	bool isGood() const;
	string why() const;
	string sequenceID() const;
	string v_gene() const;
	string d_gene() const;
	string j_gene() const;
	double v_identity() const;
	double d_identity() const;
	double j_identity() const;

	string v_identity_fmt() const;
	string d_identity_fmt() const;
	string j_identity_fmt() const;

	string v_evalue_fmt() const;
	string d_evalue_fmt() const;
	string j_evalue_fmt() const;

	string chain() const;
	bool productive() const;
	string productive_fmt() const;
	string strand() const;

	string quality_string_trimmed() const;
	string quality_string_untrimmed() const;
	string full_nt_sequence() const;
	string full_gl_nt_sequence() const;
	string full_aa_sequence() const;
	string cdr1_nt_sequence() const;
	string cdr1_aa_sequence() const;
	string cdr2_nt_sequence() const;
	string cdr2_aa_sequence() const;
	string cdr3_nt_sequence() const;
	string cdr3_aa_sequence() const;
	string full_nt_sequence_corrected() const;
	string full_aa_sequence_corrected() const;
	int gl_start() const;
	int translation_frame() const;

	///// Setters
	void v_gene( string const & vgene );
	void j_gene( string const & jgene );
	void cdr3_aa_sequence( string const & cdr3_aa_sequence );

	void sequenceID( string const & seqID );
	void quality_string_trimmed( string const & phred );
	void full_nt_sequence( string const & seq );
	void full_aa_sequence( string const & seq );
	void full_gl_nt_sequence( string const & seq );
	
	void full_nt_sequence_corrected( string const & seq );
	void full_aa_sequence_corrected( string const & seq );


private:
	// IGBlastParser and SequenceRecord can directly set these params
	friend class IGBlastParser;
	friend class SequenceRecord;

	// private subroutines that build sequence
	void build_nt_sequence();
	void build_phred();
	void translate_sequence( bool allow_nonproductive );

/** 
============================= 
	  Member variables
=============================
*/
	string sequenceID_;

	string v_gene_;
	string d_gene_;
	string j_gene_;

	bool hasV_;
	bool hasD_;
	bool hasJ_;

	string v_nts_;
	string d_nts_;
	string j_nts_;

	vector<string> jxn_nts_;

	string v_gl_nts_;
	string d_gl_nts_;
	string j_gl_nts_;

	double v_identity_;
	double d_identity_;
	double j_identity_;

	double v_evalue_;
	double d_evalue_;
	double j_evalue_;

	string chain_;
	bool productive_;
	string strand_;

	int query_start_;
	int gl_start_;
	int translation_frame_;

	string phred_;
	string phred_trimmed_;

	string full_nt_sequence_;
	string full_gl_nt_sequence_;
	string full_aa_sequence_;

	string cdr1_nt_sequence_;
	string cdr1_aa_sequence_;
	string cdr2_nt_sequence_;
	string cdr2_aa_sequence_;
	string cdr3_nt_sequence_;
	string cdr3_aa_sequence_;

	string full_nt_sequence_corrected_;
	string full_aa_sequence_corrected_;

	bool good_;
	string failure_reason_;


};

} // namespace errorx

#endif // ABSEQUENCE_HH_
