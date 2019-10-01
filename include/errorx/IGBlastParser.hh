/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file IGBlastParser.hh
@brief Class to run IGBlast on a set of query sequences and parse the output
@details IGBlastParser runs the executable, then breaks the output
file into chunks where each chunk is a single query. SequenceRecord does the
heavy lifting in terms of turning that output into a SequenceRecord object 
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef IGBLASTPARSER_HH_
#define IGBLASTPARSER_HH_

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

#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "util.hh"

using namespace std;

namespace errorx {
	
class ERRORX_API IGBlastParser {

public:
	/**
		Empty constructor
	*/
	IGBlastParser();

	/**
		Runs IGBlast on a set of input sequences based on the 
		information in ErrorXOptions

		@param options ErrorXOptions that dictate what the input
		and output files are 
	*/
	void blast( ErrorXOptions & options );
	
	/**
		Splits the IGBlast output file into chunks so that it
		can be processed and turned into SequenceRecord(s)

		@param options ErrorXOptions to control processing

		@return A SequenceRecords object constructed from the IGBlast output
	*/
	SequenceRecordsPtr parse_output( ErrorXOptions const & options );

	/**
		Parses a section of the IGBlast output file representing
		one sequence. Turns it into an AbSequence that can then be
		used to make a SequenceRecord

		@param options ErrorXOptions to control processing

		@return AbSequence object constructed from the IGBlast output
	*/
	AbSequence parse_lines( vector<string> const & lines, ErrorXOptions const & options );

	/**
		Parses a IGBlast output line representing
		one sequence. Turns it into an AbSequence that can then be
		used to make a SequenceRecord

		@param options ErrorXOptions to control processing

		@return AbSequence object constructed from the IGBlast output
	*/
	AbSequence parse_line( vector<string> const & tokens, ErrorXOptions const & options );


private:
	/**
		Tracks progress of the IGBlast query to make a progress bar

		@param options ErrorXOptions to control processing
	*/
	void track_progress( ErrorXOptions const & options );

	/**
		Execute the query in another thread to allow progress bar

		@param command command to outsource to another thread
	*/
	void exec_in_thread( string command ) ;


	atomic<bool> thread_finished_;
//	string thread_output_;
};

} // namespace errorx

#endif /* IGBLASTPARSER_HH_ */
