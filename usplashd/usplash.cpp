#include "usplash.h"

#include "message.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

Usplash::Usplash() {
}

void Usplash::run() {
	running = true;
	int fd,con;
	struct sockaddr_un local,remote;
	int len;


	/* Create socket */
	if( ( fd = socket( AF_UNIX , SOCK_STREAM , 0 ) ) == -1 ) {
		perror( "socket" );
		return ;
	}

	/* Fill sockaddr */
	local.sun_family = AF_UNIX;
	strcpy( local.sun_path , get_socket_name().c_str() );

	/* Bind socket */
	len = strlen( local.sun_path ) + sizeof( local.sun_family );
	if ( -1 == bind( fd , ( struct sockaddr *)&local , len ) ) {
		perror( "bind" );
		return;
	}

	listen( fd , 5 );

	len = sizeof( struct sockaddr_un );
	
	while( running ) {
		/* Shoud maybe use select here, so that we can timeout */
		if ( ( con = accept( fd , (struct sockaddr *)&remote , (socklen_t*)&len ) ) == -1 ) {
			perror( "accept" );
			return;
		}

		/* Retriev message */
		Message *msg = (Message*)malloc( sizeof( Message ) );
		recv( con , msg , sizeof( Message ) , 0  );
		if( msg->size > 0 ) {
			msg = (Message*)realloc( (void*)msg , sizeof( Message ) + msg->size );
			recv( con , msg+sizeof( Message ) , msg->size , 0 );
		}

		/* Parse message */
		if( msg->cmd == MSG_EXIT ) {
			running = false;
		} else if ( msg->cmd == MSG_TEXT ) {
			set_text( (char*)(msg+sizeof(Message)) );
		}
		
		close( con );
		free( msg );
	}

	close( fd );
}






void Usplash::init() {
}

void Usplash::quit() {
}

void Usplash::set_text( char *ntext ) {
}

void Usplash::set_progress( float p ) {
}
