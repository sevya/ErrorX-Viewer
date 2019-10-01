/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file errorx.hh
@brief Interface to ErrorX error correction
@details Public interface to ErrorX processing. Input 
sequences can be given as a FASTQ file, a TSV file,
or a std::vector of SequenceQuery objects
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef INCLUDE_ERRORX_HH_
#define INCLUDE_ERRORX_HH_

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
#include "SequenceQuery.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "util.hh"

using namespace std;

namespace errorx {

/**
	Runs the ErrorX prediction method on sequences from a file.
	ErrorXOptions contains all necessary options to run the protocol,
	including the input file name, input file format, species, error
	threshold, etc.

	@param options ErrorXOptions object with all necessary options for 
	running the ErrorX protocol

	@throws invalid_argument if either infile or format are not provided
	in options	

	@return SequenceRecordsPtr containing query sequences and their
	corrected versions
*/
ERRORX_API SequenceRecordsPtr run_protocol( ErrorXOptions & options );

/**
	Runs the ErrorX prediction method on a std::vector of 
	SequenceQuery objects. Uses the processing options 
	specified by the options argument.

	@param queries Vector of SequenceQueries to be processed
	@param options Options to use in processing

	@return SequenceRecordsPtr containing query sequences and their
	corrected versions
*/
ERRORX_API SequenceRecordsPtr run_protocol( vector<SequenceQuery> & queries, ErrorXOptions & options );

/**
	Wrapper for the run_protocol( ErrorXOptions & options ) function.
	This function takes the SequenceRecords output by that function
	and writes them to the file specified by the outfile member 
	of ErrorXOptions

	@param options ErrorXOptions object with all necessary options for 
	running the ErrorX protocol

	@throws invalid_argument if either infile or format are not provided
	in options	
*/
ERRORX_API void run_protocol_write( ErrorXOptions & options );

/**
	Debugging function to output features of input sequences as well as
	the corrected sequences.

	@param options ErrorXOptions object with all necessary options for 
	running the ErrorX protocol

	@throws invalid_argument if either infile or format are not provided
	in options	
*/
void run_protocol_write_features( ErrorXOptions & options );

} // namespace errorx

#endif /* INCLUDE_ERRORX_HH_ */
