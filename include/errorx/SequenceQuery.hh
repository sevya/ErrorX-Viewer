/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceQuery.hh
@brief A nucleotide sequence along with the minimal 
information needed for error correction
@details A single nucleotide sequence along with the inferred
germline sequence, sequence ID, and PHRED (quality) score as 
a string of characters
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef SEQUENCEQUERY_HH_
#define SEQUENCEQUERY_HH_

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

using namespace std;

namespace errorx {

class ERRORX_API SequenceQuery {

public:

	/**
		Constructor with all the necessary information

		@param sequenceID Name of sequence
		@param sequence NT sequence
		@param germline_sequence Inferred germline sequence (on DNA level)
		@param phred_string PHRED (quality) scores straight from the sequencer 
	*/	
	SequenceQuery( string sequenceID, 
				   string sequence, 
				   string germline_sequence, 
				   string phred_string );
	/**
		Copy constructor
	*/	
	SequenceQuery( SequenceQuery const & other );
	
	/**
		Destructor - does nothing
	*/
	~SequenceQuery();

	/**
		Setters
	*/
	void sequenceID( string const & sequenceID );
	void sequence( string const & sequence );
	void germline_sequence( string const & germline_sequence );
	void phred_string( string const & phred_string );

	/**
		Getters
	*/
	string sequenceID() const;
	string sequence() const;
	string germline_sequence() const;
	string phred_string() const;

private:
	string sequenceID_;
	string sequence_;
	string germline_sequence_;
	string phred_string_;
};

} // namespace errorx

#endif /* SEQUENCEQUERY_HH_ */
