#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include "functions.h"


void serialize(buffer_t *b,  char *data, int nbytes){
    
// Check if buffer is valid
    if (b == NULL) assert(0);

    buffer_t *buff = (buffer_t *)(b);
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
void deserialize(char *dest, buffer_t *b, int size){
	if(!b || !b->b) assert(0);
	if(!size) return;	
	if((b->size - b->next)< size) assert(0);	 

	memcpy(dest, b->b + b->next, size);
	b->next += size;
}


// Declare a function to initialize a serialized buffer of a defined size
void init_buffer_of_size(buff_t **b, int size){
    // Allocate memory for the buffer struct
    (*b) = (buff_t *)calloc(1, sizeof(buff_t));
    
    // Allocate memory for the buffer data using the specified size
    (*b)->b = calloc(1, size);
    
    // Set the buffer size to the specified size
    (*b)->size = size;
    
    (*b)->next = 0;
}
// Declare a function to initialize a serialized buffer
void init_buffer(buffer_t **b){
    // Allocate memory for the buffer struct
    (*b) = (buffer_t *)calloc(1, sizeof(buffer_t));
    
    // Allocate memory for the buffer data
    (*b)->b = calloc(1, BUFFER_DEFAULT_SIZE);
    
    // Set the buffer size
    (*b)->size = BUFFER_DEFAULT_SIZE;
    
    // Set the initial next index to 0
    (*b)->next = 0;
   
    return;
}
// This function skips a certain number of bytes in a serialized buffer.
void buffer_skip(buffer_t *buffer, int size){

    // Calculate the amount of available space in the buffer.
    int available_space = buffer->size - buffer->next;

    if(available_space >= size){
        buffer->next += size;
        return;
    }

    while(available_space < size){
        buffer->size = buffer->size * 2;
        available_space = buffer->size - buffer->next;
    }   

    // After creating enough space, use `realloc()` function to reallocate
    // memory for the buffer and assign it to the pointer `b` inside the buffer_t struct.
    buffer->b = realloc(buffer->b, buffer->size);    

    // Finally, increment the `next` variable by the size of bytes to be skipped,
    // indicating that the skipped bytes are no longer part of the buffer.
    buffer->next += size;
}
// Declare a function to free the memory allocated for a serialized buffer
void free_buffer(buffer_t *b){
    if (b != NULL) { // Check if buffer exists
        free(b->b); // Free the memory allocated for the buffer data
        free(b);    // Free the memory allocated for the buffer struct
    }
}

// Declare a function to check if a serialized buffer is empty

char buffer_empty(buffer_t *buffer){
	// Check if the buffer is empty
	if(buffer->next == 0)	
		return 1;
	return 0;
}
int get_buffer_length(buffer_t *buffer){
	// Get the length/size of the serialized buffer
	return buffer->size;
}
int get_buffer_current_pointer_offset(buffer_t *buffer){
	// Get the current pointer offset of the serialized buffer
	if(!buffer) return -1;
	return  buffer->next;
}

char* get_buffer_current_pointer(buffer_t *buffer){
	// Get the current pointer of the serialized buffer
	if(!buffer) return NULL;
	return  buffer->b + buffer->next;
}

int get_buffer_datasize(buffer_t *buffer){
	// Get the data size of the serialized buffer
	return buffer->next;
}
void copy_in_buffer_by_offset(buffer_t *b, 
                                         int size, 
                                         char *value, 
                                         int offset){
	if(offset > b->size){
		printf("%s(): Error : Attemt to write outside buffer boundaries\n", __FUNCTION__);
		return;
	}
	
	memcpy(b->b + offset, value, size);
}

void truncate_buffer(buffer_t **b){
	buffer_t *clone = NULL;

	if((*b)->next == (*b)->size){
		return;
	}
	init_buffer_of_size(&clone, (*b)->next);
	memcpy(clone->b, (*b)->b, (*b)->next);
	clone->next = clone->size;
	free_buffer(*b);
	*b = clone;
}

void reset_buffer(buffer_t *b){
	b->next = 0;	
}

void buffer_details(buffer_t *b, const char *fn, int lineno){

    printf("%s():%d : starting address = 0x%x\n", fn, lineno, (unsigned int)b);
    printf("size = %d\n", b->size);
    printf("next = %d\n", b->next);
}

void reset_serialize_buffer(ser_buff_t *b){
	b->next = 0;	
}
