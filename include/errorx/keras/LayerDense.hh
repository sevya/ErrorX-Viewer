/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/LayerDense.hh
@brief Dense layer for a neural network
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef LAYERDENSE_HH_
#define LAYERDENSE_HH_

#include "Layer.hh"
#include "DataChunk.hh"

using namespace std;

namespace keras {

class LayerDense : public Layer {

public:
	LayerDense();

	void print_weights();

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
	vector<vector<double>> weights_; //input, neuron
	vector<double> bias_; // neuron
	int input_cnt_;
	int neurons_;

};

} // namespace keras

#endif // LAYERDENSE_HH_
