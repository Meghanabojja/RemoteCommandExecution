#ifndef __FUNCTIONS__
#define __FUNCTIONS__

typedef struct serialized_buffer{
    #define BUFFER_DEFAULT_SIZE 512
    void *b;
    int size;
    int next;
} ser_buff_t;

void serialize_data(ser_buff_t *b,  const char *data, int val_size);
void init_serialized_buffer(ser_buff_t **b);

int is_serialized_buffer_empty(const ser_buff_t *b)
void free_serialized_buffer(ser_buff_t *b)
    
#endif
