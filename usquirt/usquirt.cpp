#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>

using namespace std;

#include "message.h"
#include "utils.h"

void send_message( Message *msg ) {
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

	if ( send(s, (void*)msg, sizeof(Message)+msg->size , 0 ) == -1 ) {
		perror( "send" );
		exit(1);
	}
	close( s );
}

void print_help() {
	/* TODO: Print helpful use message here */
}
	

int main( int argc , char **argv ) {
	Message *msg = (Message*)malloc( sizeof( Message ) );
	msg->size = 0;

	/* Just checking */
	if ( argc < 2 ) {
		print_help(); exit(1);
	}

	if ( !strcmp( argv[1] , "exit" ) ) {
		msg->cmd = MSG_EXIT;
	} else if ( !strcmp( argv[1] , "text" ) ) {
		if ( argc < 3 ) {
			print_help(); exit(1);
		}
		msg->cmd = MSG_TEXT;
		msg = ( Message *)realloc( msg , sizeof( Message ) +  strlen( argv[2] ) + 1 );
		msg->size = strlen( argv[2] );
		strcpy((char*) msg+sizeof(Message) , argv[2] );

	} else if ( !strcmp( argv[1] , "progress" ) ) {
		if( argc < 3 ) {
			print_help(); exit(1);
		}
		msg->cmd = MSG_PROGRESS;
		msg = (Message*)realloc( msg , sizeof( MsgProgress ) );
		msg->size = sizeof( MsgProgress ) - sizeof( Message );
		((MsgProgress*)msg)->progress = atoi( argv[2] );


	} else {
		print_help(); exit(1);
	}

	send_message( msg );

	return 0;
}
