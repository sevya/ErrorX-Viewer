/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ErrorXOptions.hh
@brief Controls the options around running the ErrorX protocol
@details 
@author Alex Sevy (alex@endeavorbio.com)
*/


#ifndef INCLUDE_ERRORXOPTIONS_HH_
#define INCLUDE_ERRORXOPTIONS_HH_

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

#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

#include "ProgressBar.hh"

using namespace std;

namespace errorx {
	
class ERRORX_API ErrorXOptions {
public:
	/**
		Empty constructor. Initializes to default values.
		Infile and format must be set before using the options though
	*/	
	ErrorXOptions();

	/**
		Constructor which initializes all to default values, except 
		for required arguments infile and format 
	*/	
	ErrorXOptions( string infile, string format );

	/**
		Destructor - does nothing though
	*/
	~ErrorXOptions();

	/**
		Assignment operator
	*/
	ErrorXOptions & operator=( ErrorXOptions const & other);

	/**
		Copy constructor
	*/	
	ErrorXOptions( ErrorXOptions const & other );

	/**
		Converts a FASTQ file to FASTA file. Uses the infile_ variable
		to read, and outputs to the same name with the extension .fasta,
		which is then set to the variable infasta_
	*/	
	void fastq_to_fasta();

	/**
		Check that options are properly initialized before running. 

		@throws invalid_argument if either infile or format are not specified
	*/	
	void validate();

	/**
		Counts the number of queries based on infile and saves
		to a member variable
	*/
	void count_queries();
	
	/*
		Getters
	*/
	string outfile() const;
	string format() const;
	string species() const;
	string igtype() const;
	string infile() const;
	string infasta() const;
	string igblast_output() const;
	string errorx_base() const;
	int verbose() const;
	int nthreads() const;
	double error_threshold() const;
	char correction() const;
	bool trial() const;
	int num_queries() const;
	bool allow_nonproductive() const;
	function<void(int,int)> increment() const;
	function<void(void)> reset() const;
	function<void(void)> finish() const;
	function<void(string)> message() const;
	unordered_map<string,string> quality_map() const;
	string get_quality( string const & sequenceID ) const;

	/*
		Setters
	*/
	void outfile( string const & outfile );
	void format( string const & format );
	void species( string const & species );
	void igtype( string const & igtype );
	void infile( string const & infile );
	void infasta( string const & infasta );
	void igblast_output( string const & igblast_output );
	void errorx_base( string const & errorx_base );
	// void errorx_base( boost::filesystem::path const & errorx_base );
	void verbose( int const verbose );
	void nthreads( int const nthreads );
	void error_threshold( double const & error_threshold );
	void correction( char const & correction );
	void trial( bool const trial );
	void num_queries( int const num_queries );
	void allow_nonproductive( bool const allow_nonproductive );
	void increment( function<void(int,int)> const & increment ) ;
	void reset( function<void(void)> const & reset ) ;
	void finish( function<void(void)> const & finish ) ;
	void message( function<void(string)> const & message ) ;
	void quality_map( unordered_map<string,string> const & quality_map );

private:

	void initialize_callback();

	/**
		User specified options:
		
		infile_: input file, either FASTQ or TSV, to use for prediction
		format_: format of infile_, either fastq or tsv
		outfile_: name of the output file to write the final ErrorX predictions. 
		Default out.tsv
		species_: species to use for germline assignment. Currently supported
		are human or mouse. Default human
		igtype_: type of receptor to analyze. Either Ig or TCR. Default Ig
		verbose_: Verbosity level: should ErrorX output extra warnings and messages? 
		0: don't output any message at all
		1: output progress during processing
		2: output progress and debugging messages
		nthreads_: how many threads to use? -1 for all available. Default -1
		error_threshold_: what probability is enough to call a base an error?
		Default 0.8
		allow_nonproductive_: allow sequences through the protocol even if they
		are nonproductive. Default no
		correction_: when ErrorX corrects a sequence it replaces the original base
		with a new character. What character should be used? Default N
	*/
	string infile_;
	string format_;
	string outfile_;
	string species_;
	string igtype_;
	int verbose_;
	int nthreads_;
	double error_threshold_;
	bool allow_nonproductive_;
	char correction_;

	/**
		Automatically generated options:
		These options are set internally to ErrorX based on where it's called 
		from and don't need to be changed.
	*/
	string infasta_;
	string igblast_output_;
	string errorx_base_;
	bool trial_;
	int num_queries_;

	/**
		Callback functions to update progress during error correction
		By default, these will update a progress bar on the command 
		line. Can also be used in a GUI to show a progress window.

		Four callback functions are implemented:
		
		increment_: this increments the number of processed records. 
			Takes in as arguments: (increment value, total records)

		reset_: resets the progress bar to 0. Used in the transition
			from tracking progress on IGBlast to error correction

		finish_: finishes the progress bar. Frequently the thread finishes and
			progress updating is killed before it hits 100%. To make it look
			pretty this function manually updates the bar to 100%.

		message_: sets the message for the currently running step
	*/
	function<void(int,int)> increment_;
	function<void(void)> reset_;
	function<void(void)> finish_;
	function<void(string)> message_;
	ProgressBar _bar;

	/**
		This maps a sequence ID to the corresponding quality string
		this enables us to find the quality based on the IGBlast output later
	*/
	unordered_map<string,string> quality_map_;
	
};

typedef unique_ptr<ErrorXOptions> ErrorXOptionsPtr;
typedef shared_ptr<ErrorXOptions> ErrorXOptionsSP;

} // namespace errorx

#endif /* INCLUDE_ERRORXOPTIONS_HH_ */
