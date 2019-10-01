/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/LayerActivation.hh
@brief Activation layer for neural network
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef LAYERACTIVATION_HH_
#define LAYERACTIVATION_HH_

#include "keras/Layer.hh"
#include "keras/DataChunk.hh"

using namespace std;

namespace keras {


class LayerActivation : public Layer {

public:
	LayerActivation();

	/**
	===========================================================
	                    Pure virtual functions 
	===========================================================
	*/
	void load_weights( istream & fin );
	DataChunk* compute_output( DataChunk* dc );
	uint get_input_rows() const;
	uint get_input_cols() const;
	uint get_output_units() const;

private:
	string activation_type_;

};

} // namespace keras

#endif // LAYERACTIVATION_HH_
