#include "logger.h"

void logger::init()
{
	boost::log::add_file_log(LOG_FILE);
	boost::log::core::get()->set_filter
	(
		boost::log::trivial::severity >= boost::log::trivial::trace
	);
}

void logger::getLastError()
{
	DWORD errId = GetLastError();
	LPVOID errBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errBuf, 0, NULL);
	BOOST_LOG_TRIVIAL(error) << errBuf << std::endl;
}
