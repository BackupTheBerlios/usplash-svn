#include "usplash.h"

#include <iostream>
#include <sys/wait.h>

#include "utils.h"

using namespace std;


class UsplashText : public Usplash {
	public:
		virtual void set_text( char *);
		virtual void set_progress( int p );
		virtual void quit();
};

void UsplashText::set_text( char *ntext )  {
	cout << "TEXT: Message( \"" << ntext << "\" ) "<< endl;
}

void UsplashText::set_progress( int p ) {
	cout << "TEXT: Progress: " << p << endl;
}

void UsplashText::quit()  {
	cout << "TEXT: Quit" << endl;
}



int main( int argc , char *argv ) {
	if (!fork()) {
		UsplashText ut;
		ut.init();
		/* TODO: Drop privs here */
		ut.run();
		ut.quit();
		exit(0);
	} else {
		wait( NULL );
	}

	unlink( get_socket_name().c_str() );
	return 0;
}
