#ifndef __usplash_h
#define __usplash_h

#include <string>

class Usplash {
	public:
		Usplash();
		
		virtual void init();
		virtual void quit();
		virtual void set_text( char * );
		virtual void set_progress( int );


		void start();
		void run();
		void stop();
		void waitfor();

	private:
		bool running;
};

#endif
