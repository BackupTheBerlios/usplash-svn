#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "message.h"
#include "utils.h"

int main( int argc , char *argv ) {

	int s , len;
	sockaddr_un remote;

	if ((s = socket( AF_UNIX , SOCK_STREAM , 0 )) == -1 ) {
		perror( "socket" );
		exit(1);
	}

	remote.sun_family = AF_UNIX;
	strcpy( remote.sun_path , get_socket_name().c_str() );
	len = strlen( remote.sun_path ) + sizeof( remote.sun_family );

	if ( connect( s , (sockaddr *)&remote , len ) == -1 ) {
		perror( "connect" );
		exit( 1 );
	}

	Message *msg = (Message*)malloc( sizeof( Message ) );
	msg->cmd = MSG_EXIT;
	msg->size = 0;

	if ( send(s, (void*)msg, sizeof(Message)+msg->size , 0 ) == -1 ) {
		perror( "send" );
		exit(1);
	}
	close( s );

	return 0;
}
