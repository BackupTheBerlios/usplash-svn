#ifndef __message_h
#define __message_h

#define MSG_EXIT 0
#define MSG_TEXT 1
#define MSG_PROGRESS 2
#define MSG_ANIMFILE 3


struct Message {
	short size;
	short cmd;
};

struct MsgProgress {
	short size;
	short cmd;
	int progress;
};



#endif
