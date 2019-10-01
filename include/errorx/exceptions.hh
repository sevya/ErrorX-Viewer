/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file exceptions.hh
@brief Class holds custom exception types used in ErrorX
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef EXCEPTIONS_HH_
#define EXCEPTIONS_HH_

#include <iostream>
#include <exception>
#include <string>
#include "constants.hh"

using namespace std;

namespace errorx {

/**
	Exception is thrown when there is no valid license
	file found, and ErrorX is therefore running in 
	trial mode
*/	
class InvalidLicenseException : public exception {
public:
	const char* what () const throw () {
		string message = 
		"You are currently running the trial version of ErrorX, "
				"which can only process "+
				to_string(constants::FREE_QUERIES)+
				" sequences at a time. "
				"Please limit your query size or contact alex@endeavorbio.com "
				"to request access to the full version.";
		return message.c_str();
	}
};


/**
	Exception is thrown when a bad license is given on the command line
*/	
class BadLicenseException : public exception {
public:
	const char* what () const throw () {
		return "License is not valid. Please contact alex@endeavorbio.com for assistance";
	}
};

/**
	Exception is thrown when an invalid file is passed
*/	
class BadFileException : public exception {
public:
	BadFileException() : message_("Input file is invalid. Please check the format and run again") {}
	BadFileException( string const & message ) : message_(message) {}
	
	const char* what () const throw () {
		return message_.c_str();
	}

	string message_;
};


/**
	Exception is thrown when an invalid input is present
*/	
class BadInputException : public exception {
public:
	BadInputException() : message_("Invalid input encountered. Please check your input data.") {}
	BadInputException( string const & message ) : message_(message) {}
	
	const char* what () const throw () {
		return message_.c_str();
	}

	string message_;
};

} // namespace errorx


/** 
	Exceptions thrown by the Keras module
*/
namespace keras {

/**
	Exception is thrown when an unimplemented function is called
*/	
class FunctionNotImplemented : public exception {
public:
	FunctionNotImplemented() : message_("Sorry, you called a function that's not implemented yet!") {}
	FunctionNotImplemented( string const & message ) : message_( message ) {}

	const char* what() const throw() {
		return message_.c_str();
	}

	string message_;
};

/**
	Exception is thrown when a layer type is used that is not support
*/	
class InvalidLayer : public exception {
public:
	InvalidLayer( string const & layer ) : layer_( layer ) {}

	const char* what() const throw() {
		string message = "Layer type "+layer_+" not implemented";
		return message.c_str();
	}

	string layer_;
};


/**
	Exception is thrown when an uninitialize object is used
*/	
class ObjectNotInitialized : public exception {
public:
	ObjectNotInitialized() : message_("Sorry, you used an object that's not been initialized!") {}
	ObjectNotInitialized( string const & message ) : message_( message ) {}

	const char* what() const throw() {
		return message_.c_str();
	}

	string message_;
};


/**
	Exception is thrown when an uninitialize object is used
*/	
class BadModel : public exception {
public:
	BadModel() : message_("Model is malformed.") {}
	BadModel( string const & message ) : message_( message ) {}

	const char* what() const throw() {
		return message_.c_str();
	}

	string message_;
};

} // namespace keras


#endif // EXCEPTIONS_HH_