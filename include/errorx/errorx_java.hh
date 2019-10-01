/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file errorx_java.hh
@brief Java bindings to expose ErrorX interface
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <jni.h>

#include "SequenceRecords.hh"

#ifndef _Included_ErrorX
#define _Included_ErrorX
#ifdef __cplusplus
extern "C" {
#endif

/**
	JNI interface to get corrected nucleotide sequences using the ErrorX interface

	@param sequence_list String[] of nucleotide sequences to be corrected
	@param germline_sequence_list String[] of germline sequences corresponding 
	to each sequence to be corrected
	@param phred_score_list String[] of PHRED scores as strings to be corrected
	This is set automatically by the Java code
	@param options Java ErrorXOptions object to control processing options

	@throws invalid_argument if lists are not the same length
	
	@return String[] of corrected nucleotide sequences, corresponding 
	element-wise to the input sequence list
*/
JNIEXPORT jobject JNICALL Java_errorx_ErrorX_correctSequences( JNIEnv *env, 
					 jobject thisObj,
					 jobjectArray sequence_list, // String[]
					 jobjectArray germline_sequence_list, // String[]
					 jobjectArray phred_score_list, // String[]
					 jobject options // ErrorXOptions
					 );

/**
	Runs the ErrorX prediction method on a single sequence
	and returns the probability that each base is an error.

	@param sequence_list String of nucleotide sequence to be corrected
	@param germline_sequence_list String of germline sequence corresponding 
	to sequence to be corrected
	@param phred_score_list String of PHRED score to be corrected
	This is set automatically by the Java code
	@param options Java ErrorXOptions object to control processing options
	@throws invalid_argument if lists are not the same length
	
	@return double[] consisting of the error probabilities for each base
	along the input sequence
*/
JNIEXPORT jdoubleArray JNICALL Java_errorx_ErrorX_getPredictedErrors( JNIEnv *env, 
					 jobject thisObj,
					 jstring sequence, // String 
					 jstring germline, // String
					 jstring phred_score, // String
					 jobject options // ErrorXOptions
					 );

/**
	Runs the ErrorX prediction method on sequences from a file
	and writes the output to a file

	@param options ErrorXOptions object with running parameters
*/
JNIEXPORT void JNICALL Java_errorx_ErrorX_runProtocol( JNIEnv *env, jobject thisObj,
					 jobject options // ErrorXOptions 
					 );
#ifdef __cplusplus
}

/**
	Runs the ErrorX prediction method on a set of sequences. Both 
	correctSequences and getPredictedErrors are wrappers for this
	subroutine. Wraps the get_corrected_records function that takes
	std::vectors. First converts each jobjectArray to vector<string>,
	then runs the get_corrected function.

	@param sequence_list String[] of nucleotide sequences to be corrected
	@param germline_sequence_list String[] of germline sequences corresponding 
	to each sequence to be corrected
	@param phred_score_list String[] of PHRED scores as strings to be corrected
	This is set automatically by the Python code
	@param options Java ErrorXOptions object to control processing options

	@throws invalid_argument if lists are not the same length
	
	@return SequenceRecordsPtr with error correction consisting of all input
	sequences
*/
errorx::SequenceRecordsPtr get_corrected_records( JNIEnv* env,
				 jobjectArray & sequence_list, // String[]
				 jobjectArray & germline_sequence_list, // String[]
				 jobjectArray & phred_score_list, // String[]
				 jobject & options // ErrorXOptions
				 );

/**
	Runs the ErrorX prediction method on a set of sequences. Both 
	correctSequences and getPredictedErrors are wrappers for this
	subroutine. 

	@param sequence_list String[] of nucleotide sequences to be corrected
	@param germline_sequence_list String[] of germline sequences corresponding 
	to each sequence to be corrected
	@param phred_score_list String[] of PHRED scores as strings to be corrected
	This is set automatically by the Python code
	@param options ErrorXOptions object to control processing options

	@throws invalid_argument if lists are not the same length
	
	@return SequenceRecordsPtr with error correction consisting of all input
	sequences
*/
errorx::SequenceRecordsPtr get_corrected_records( JNIEnv* env,
				 vector<string> & sequence_list, 
				 vector<string> & germline_sequence_list,
				 vector<string> & phred_score_list,
				 errorx::ErrorXOptions & options 
				 );

/**
	Converts a std::vector full of doubles to a Java double[]. 
	Returns null if there's not enough memory to allocate array
	
	@param in_vector std::vector full of doubles to convert
	
	@return jdoubleArray with elements from in_vector
*/
jdoubleArray vector_to_array( JNIEnv* env, vector<double> & in_vector );

/**
	Converts a std::vector full of strings to a Java String[]. 
	Returns null if there's not enough memory to allocate array
	
	@param in_vector std::vector full of strings to convert

	@return jobjectArray with Strings from in_vector
*/
jobjectArray vector_to_array( JNIEnv* env, vector<string> & in_vector );

/**
	Converts a Java String[] to a std::vector. 
	
	@param array String[] full of strings to convert
	
	@return std::vector with strings from in_vector
*/
vector<string> array_to_vector( JNIEnv* env, jobjectArray & array );

/**
	Converts a Java String or Java object to a std::string.
	
	@param jstr jstring to convert
	
	@return std::string from jstring
*/
string jstring_to_string( JNIEnv* env, jstring & jstr );

/**
	Converts a Java ErrorXOptions object to its C++ counterpart
	
	@param jstr jstring to convert
	
	@return std::string from jstring
*/
errorx::ErrorXOptions joptions_to_options( JNIEnv* env, jobject & options );


#endif
#endif
