/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ProgressBar.hh
@brief Draws a progress bar to the screen during execution
@details 
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef INCLUDE_PROGRESSBAR_HH_
#define INCLUDE_PROGRESSBAR_HH_

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

#include <functional>

using namespace std;

namespace errorx {
	
class ERRORX_API ProgressBar {

public:
	/**
		Empty constructor
	*/	
	ProgressBar();

	/**
		Constructor takes in total number of records
	*/	
	ProgressBar( int total );

	/**
		Copy constructor
	*/	
	ProgressBar( ProgressBar const & other );

	/**
		Increment the progress bar

		@param value value to increment the progress bar
		@param total total number of records
	*/
	void increment( int value, int total );

	/**
		Set the value of the progress bar, either providing a mutex if a multithreaded
		situation, or without

		@param value value to set the progress bar
		@param m pointer to mutex to synchronize progress
	*/
	void message( string s );
	void reset();
	void finish();

	/*
		Getters
	*/
	int total() const;
	int processed() const;

	/*
		Setters
	*/
	void total( int const & total );
	void processed( int const & processed );
	
private:
	void draw();

	int total_;
	int processed_;
	
};

} // namespace errorx

#endif /* INCLUDE_PROGRESSBAR_HH_ */
