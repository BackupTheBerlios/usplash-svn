#ifndef __usplash_h
#define __usplash_h

#include <string>

class Usplash {
	public:
		Usplash();
		
		virtual void init();
		virtual void quit();
		virtual void set_text( std::string );
		virtual void set_progress( float );


		void start();
		void run();
		void stop();
		void waitfor();

	protected:
		bool running;
		std::string text;
		float progress;
};

#endif
