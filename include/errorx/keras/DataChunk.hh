/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/DataChunk.hh
@brief Generic class for a data chunk to be processed in a keras model
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef DATACHUNK_HH_
#define DATACHUNK_HH_

#include <vector>

#include "exceptions.hh"

using namespace std;
typedef unsigned int uint;

namespace keras {

class DataChunk {

public:

	virtual ~DataChunk() {};
	/**
		Functions to access data held by data chunk. Implementation
		and return type depend on shape of data
	*/
	virtual vector<double> const & get_1d() const { 
		throw FunctionNotImplemented("function get_1d not implemented"); 
	}
	virtual vector<vector<vector<double>>> const & get_3d() const { 
		throw FunctionNotImplemented("function get_3d not implemented"); 
	}
	
	/**
		Functions to set data held by data chunk. Implementation
		depends on shape of data
	*/
        virtual void set_data( vector<vector<vector<double>>> const & ) {};
        virtual void set_data( vector<double> const & ) {};


	/**
	===========================================================
	                    Pure virtual functions 
	===========================================================
	*/

	/**
		Get dimensions of data 

		@return data dimensions
	*/
	virtual int get_data_dim() const = 0;

	/**
		Get number of features

		@return num features
	*/
	virtual int num_features() const = 0;

	/**
		Print name of data chunk to stdout
	*/
	virtual void show_name() const = 0;

	/**
		Print the values contained by layer to stdout
	*/
	virtual void show_values() const = 0;

	/**
		Read data from file. Depending on the shape of the data 
		chunk this will be implemented differently

		@param fname input file name
	*/
	virtual void read_from_file( string const & fname ) = 0;
};

} // namespace keras

#endif // DATACHUNKFLAT_HH_
