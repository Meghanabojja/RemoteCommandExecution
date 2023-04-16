#ifndef __FUNCTIONS__
#define __FUNCTIONS__

/* Data types supported*/

typedef struct serialized_buffer{
    #define BUFFER_DEFAULT_SIZE 512
    void *b;
    int size;
    int next;
} buffer_t;

/* init functions*/
void init_buffer(buffer_t **b);
void init_buffer_of_size(buffer_t **b, int size);

/* Serialize functions*/

char buffer_empty(buffer_t *b);

/* get functions*/
int  get_buffer_datasize(buffer_t *b);
int get_buffer_length(buffer_t *b);
int get_buffer_current_pointer_offset(buffer_t *b);
char *get_buffer_current_pointer(buffer_t *b);
void buffer_skip(buffer_t *b, int size);
void copy_in_buffer_by_offset(buffer_t *b, int size, char *value, int offset);

/* De-Serialize function */

void serialize(buffer_t *b, char *data, int val_size);
void deserialize(char *dest, buffer_t *b, int val_size);

/* free Resourse*/
void free_buffer(buffer_t *b);

/reset function/
void truncate_buffer(buffer_t **b);
void reset_buffer(buffer_t *b);
/* Details*/
void buffer_details(buffer_t *b, const char *fn, int lineno);
void  reset_serialize_buffer(buffer_t *b);

#endif
