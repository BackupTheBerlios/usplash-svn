#include "usplash.h"

#include <iostream>

using namespace std;


class UsplashText : public Usplash {
	public:
		virtual void set_text( string ntext );
		virtual void set_progress( float p );
		virtual void quit();
};

void UsplashText::set_text( string ntext )  {
	text = ntext;
	cout << "TEXT: Message( \"" << text << "\" ) "<< endl;
}

void UsplashText::set_progress( float p ) {
	progress = p;
	cout << "TEXT: Progress: " << progress << endl;
}

void UsplashText::quit()  {
	cout << "TEXT: Quit" << endl;
}



int main( int argc , char *argv ) {
	/* TODO: Fork here */
	UsplashText ut;
	ut.init();
	ut.run();
	ut.quit();
	return 0;
}
