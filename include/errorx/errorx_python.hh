/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file errorx_python.hh
@brief Python bindings to expose ErrorX interface
@detail These bindings do not rely on boost/python at all,
hopefully they can be more portable
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef INCLUDE_ERRORX_PYTHON_HH_
#define INCLUDE_ERRORX_PYTHON_HH_

#include <Python.h>
#include <string>
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"

using namespace std;

namespace errorx {
namespace python {

// /**
// 	Converts a std::vector to Python list

// 	@param input Templated std::vector
// 	@return Python list consisted of same elements
// */
// template <typename T>
// py::list vector_to_python_list( vector<T> const & input ) {
// 	// initialize python list
// 	PyObject* output = PyList_New( input.size() );

// 	for ( int ii = 0; ii < input.size(); ++ii ) {
// 		int status = PyList_SetItem( output, ii, PyObject *item)
// 	}
// 	return output;
// }

/**
	Runs the ErrorX prediction method on a set of sequences
	and returns the corrected sequences in a Python list.
	Note: the sequences in sequence_list, germline_sequence_list,
	and phred_score_list are matched up element-wise, and must
	be the same length

	Ex: sequence_list = ['ACTG','GTCA']
	germline_sequence_list = ['ATTG','CTCA']
	phred_score_list = ['GGFF','FFFG']

	@param sequence_list List of nucleotide sequences to be corrected
	@param germline_sequence_list List of germline sequences corresponding 
	to each sequence to be corrected
	@param phred_score_list List of PHRED scores as strings to be corrected
	@param options ErrorXOptions python object, to be convert to C++ object

	@throws invalid_argument if lists are not the same length
	
	@return Python list of corrected nucleotide sequences, corresponding 
	element-wise to the input sequence list
*/
static PyObject* correct_sequences( PyObject* self, PyObject* args, PyObject* kwargs );

static PyObject* get_predicted_errors( PyObject* self, PyObject* args, PyObject* kwargs );

static PyObject* run_protocol( PyObject* self, PyObject* args, PyObject* kwargs );

SequenceRecordsPtr submit_query( vector<string> & sequences, 
				   vector<string> & gl_sequences, 
				   vector<string> & phred_scores, 
				   ErrorXOptions & options );

errorx::ErrorXOptions options_from_pyoptions( PyObject* const options );

string extract_string_attr( PyObject* const options, string const & attr );

int extract_int_attr( PyObject* const options, string const & attr );

double extract_double_attr( PyObject* const options, string const & attr );

vector<string> pylist_to_vector( PyObject* const list );

/**
	Utilities functions to convert Python C api from 2.7 to 3.x
	they eliminated PyString and replaced it with PyBytes
*/
PyObject* _PyString_FromString( const char* v );
string _PyString_AsString( PyObject* obj );

} // namespace python
} // namespace errorx

#endif /* INCLUDE_ERRORX_PYTHON_HH_ */
