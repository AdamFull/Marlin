#include "command_queue.h"

void command_queue::push(const char* str)
{
    strcpy(buffer[queue_index_w], str);
    queue_index_w = (queue_index_w + 1) % BUFSIZE;
	if (count<BUFSIZE) count++;
}

const char* command_queue::pop()
{
    const char* result = buffer[queue_index_r];
	queue_index_r = (queue_index_r + 1) % BUFSIZE;
	count--;
	return result;
}