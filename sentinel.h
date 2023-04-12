#ifndef _SENTINEL_
#define _SENTINEL_

// Macro to insert a sentinel value into a serialized b
// If the object pointer is NULL, the sentinel value is added to the b
#define SENTINEL_INSERTION_CODE(object, b) 
{                                      
    if(!object){                          
        const unsigned int SENTINEL_VALUE = 0xFFFFFFFF;                        
        serialize_data(b, (char *)&SENTINEL_VALUE, sizeof(unsigned int));
        return;                                                    
    }                                                              
}

// Macro to detect a sentinel value in a serialized buffer
// Reads the last four bytes of the buffer and checks if the value matches the sentinel value
// If it does, the function returns NULL, indicating the end of the data structure
// Otherwise, moves the buffer pointer back four bytes so that the next deserialization operation starts at the correct position
#define SENTINEL_DETECTION_CODE(b)     
{                                      
    const unsigned int SENTINEL_VALUE = 0xFFFFFFFF;         
    unsigned int last_value = 0;         
    de_serialize_data((char *)&last_value, b, sizeof(unsigned int)); 
    if(last_value == SENTINEL_VALUE)  return NULL; 
}

#endif /* _SENTINEL_ */
