/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/DataChunkFlat.hh
@brief 1-D data chunk to process through Keras model
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef DATACHUNKFLAT_HH_
#define DATACHUNKFLAT_HH_

#include "keras/DataChunk.hh"

using namespace std;

namespace keras {

class DataChunkFlat : public DataChunk {

public:
	/**
		Empty constructor
	*/
	DataChunkFlat();

	/**
		Initialize data vector with fixed size

		@param size size of data vector
	*/
	DataChunkFlat( int size );

	/**
		Initialize data vector with fixed size and initial values

		@param size size of data vector
		@param double fill value for vector

	*/
	DataChunkFlat( int size, double init );
	
	/**
		Get 1D vector of data

		@return 1D vector of data
	*/
	vector<double> const & get_1d() const;

	/**
		Non-const access to data

		@return 1D vector of data
	*/
	vector<double> & get_1d_rw();

	/**
		Set data

		@param data 1D vector of data
	*/
	void set_data( vector<double> const & data );

	void read_from_file( string const & fname );

	/**
	===========================================================
	                    Pure virtual functions 
	===========================================================
	*/
	int get_data_dim() const;
	int num_features() const;
	void show_name() const;
	void show_values() const;
	

private:
	vector<double> data_;
};

} // namespace keras

#endif // DATACHUNKFLAT_HH_
