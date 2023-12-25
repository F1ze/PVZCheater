#pragma once
#include "pch.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

//BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
//BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
//BOOST_LOG_TRIVIAL(info) << "An informational severity message";
//BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
//BOOST_LOG_TRIVIAL(error) << "An error severity message";
//BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";


// How to use boost lib: https://levelup.gitconnected.com/the-definite-guide-on-compiling-and-linking-boost-c-libraries-for-visual-studio-projects-c79464d7282d

namespace logger {
	const std::string LOG_FILE = "pvz_logger.log";
	void init();
	void getLastError();
}