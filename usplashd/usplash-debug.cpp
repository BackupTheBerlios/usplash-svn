#include "usplash.h"

#include <iostream>
#include <sys/wait.h>

#include "utils.h"

using namespace std;


class UsplashDebug : public Usplash {
	public:
		virtual void set_text( char *);
		virtual void set_progress( int p );
		virtual void set_animfile( char * );
		virtual void quit();
};

void UsplashDebug::set_text( char *ntext )  {
	cout << "TEXT: Message: " << ntext <<  endl;
}

void UsplashDebug::set_progress( int p ) {
	cout << "TEXT: Progress: " << p << endl;
}

void UsplashDebug::set_animfile( char *fn ) {
	cout << "TEXT: Animfile: " << fn << endl;
}

void UsplashDebug::quit()  {
	cout << "TEXT: Quit" << endl;
}



int main( int argc , char *argv ) {
	if (!fork()) {
		UsplashDebug u;
		u.init();
		/* TODO: Drop privs here */
		u.run();
		u.quit();
		exit(0);
	} else {
		wait( NULL );
	}

	unlink( get_socket_name().c_str() );
	return 0;
}
