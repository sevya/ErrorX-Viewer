/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/Layer.hh
@brief Generic class for neural network layer
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef LAYER_HH_
#define LAYER_HH_

#include "keras/DataChunk.hh"

#include <string>

using namespace std;
typedef unsigned int uint;

namespace keras {

class Layer {

public:

	Layer( string const name ) : 
		name_( name ) 
		{}

	virtual ~Layer() {}

	string get_name() { return name_; }

	/**
	===========================================================
	                    Pure virtual functions 
	===========================================================
	*/

	/**
		Load weights for this layer based on either a file stream
		or a string stream.
	*/
	virtual void load_weights( istream & fin ) = 0;

	/**
		Compute the output for this layer for an input data chunk

		@param dc pointer to data chunk
	*/
	virtual keras::DataChunk* compute_output( DataChunk* dc ) = 0;

	/**
		Get description of the layer architecture
	*/	
	virtual uint get_input_rows() const = 0;
	virtual uint get_input_cols() const = 0;
	virtual uint get_output_units() const = 0;

private:
	string name_;

};


} // namespace keras

#endif // LAYER_HH_
