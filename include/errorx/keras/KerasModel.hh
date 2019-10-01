/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/KerasModel.hh
@brief Class representing a Keras model for a neural network
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef KERASMODEL_HH_
#define KERASMODEL_HH_

#include "ErrorXOptions.hh"
#include "keras/DataChunk.hh"
#include "keras/Layer.hh"

using namespace std;
typedef unsigned int uint;

namespace keras {

class KerasModel {

public:
	/**
		Initialize a Keras model based on an ErrorXOptions object
		Model file is assumed to be located at (errorx_base)/model.nnet

		@param options ErrorXOptions object
	*/	
	KerasModel( errorx::ErrorXOptions const & options );
	
	KerasModel( string const & file );

	KerasModel();

	KerasModel( KerasModel const & other );

	~KerasModel();

	vector<double> compute_output( DataChunk* dc ) const;

	uint get_input_rows() const;
	uint get_input_cols() const;
	int get_output_length() const;

	Layer* layer( int index ) const;
	int no_layers() const;

	void load_weights( string const & input_fname );
	void load_weights_from_string( string const & input_fname );

	int verbose() const;
	void verbose( int verbose );

private:
	void load_weights_from_stream( istream & fin );
	Layer* create_layer( string const & layer_name );

	int layers_cnt_; // number of layers
	vector<Layer*> layers_; // container with layers
	int verbose_;

};

} // namespace keras

#endif // KERASMODEL_HH_