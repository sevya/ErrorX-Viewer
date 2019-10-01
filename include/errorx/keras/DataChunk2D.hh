/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/DataChunk2D.hh
@brief 2-D data chunk to process through Keras model
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef DATACHUNK2D_HH_
#define DATACHUNK2D_HH_

#include "keras/DataChunk.hh"

using namespace std;

namespace keras {

class DataChunk2D : public DataChunk {

public:
	/**
		Empty constructor
	*/
	DataChunk2D();

	/**
		Get 3d vector of data

		@return 3D vector of data
	*/	
	vector<vector<vector<double>>> const & get_3d() const;
	
	/**
		Set 3d vector of data

		@param data 3d vector of data
	*/
	void set_data( vector<vector<vector<double>>> const & data );

	/**
		Read data from file. Depending on the shape of the data 
		chunk this will be implemented differently

		@param fname input file name
	*/
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
	/**
		Read a 1D array from an open file stream. Useful 
		for multi-dimensional data to get a single dimension array

		@param fin file stream
		@param cols how many columns are present for the 1D array
	*/
	vector<double> read_1d_array( ifstream & fin, int cols );

	vector<vector<vector<double>>> data_; // depth, rows, cols

	int depth_;
	int rows_;
	int cols_;

};

} // namespace keras

#endif // DATACHUNK2D_HH_
