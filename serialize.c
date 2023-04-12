#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>


void serialize_data(ser_buff_t *b, const char *data, int nbytes){
    
// Check if buffer is valid
    if (b == NULL) assert(0);

    ser_buff_t *buff = (ser_buff_t *)(b);
    int Remaining_size = buff->size - buff->next;
    char issize = 0;

 // Check if buffer has enough space
    while(Remaining_size < nbytes){
        buff->size = buff->size * 2;
        Remaining_size = buff->size - buff->next;
        issize = 1;
    }

    if(issize == 0){
        memcpy((char *)buff->b + buff->next, data, nbytes);
        buff->next += nbytes;
        return;
    }

    // resize of the buffer
buff->b = realloc(buff->b, buff->size); 

// Copy data into buffer
    memcpy((char *)buff->b + buff->next, data, nbytes);
    buff->next += nbytes;
    return;
}

//De-serialization
void de_serialize_data(char *dest, ser_buff_t *buffer, int data_size){
	assert(buffer && buffer->data && data_size);
	if(data_size == 0) return;
	assert((buffer->total_size - buffer->next) >= data_size);

	memcpy(dest, buffer->data + buffer->next, data_size);
	buffer->next += data_size;
}