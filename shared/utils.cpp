#include "utils.h"

std::string get_socket_name() {
	std::string ret;
	if ( getenv( "USPLASH_SOCKET" ) == NULL ) {
		ret = "/etc/usplash/.control";
	} else {
		ret = getenv( "USPLASH_SOCKET" );
	}
	return ret;
}
