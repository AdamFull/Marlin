#ifndef QUEUE_H
#define QUEUE_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#define BUFSIZE 8
#define MAX_CMD_SIZE 32

class queue
{
public:
    void push(const char* str);
    const char* pop();
	inline bool isEmpty() { return count > 0 ? false : true; }
    inline uint8_t length() { return count; }

private:
	char buffer[BUFSIZE][MAX_CMD_SIZE];

	uint8_t queue_index_r = 0,
		queue_index_w = 0,
		count;
};

#endif