#### C-strings
##### Some string utility for C.

- #### Headers
    - stdio
    - stdlib
    - stdint
    - string
    - ctype

- #### Datatypes
    - STRING_t

    ```C
    STRING_t

    Alternative abstraction to use this library functions
    
    Attributes:
        str

    Functions:
        count, replace, split, join, from_file, to_file
    
    ```

- #### Macros
    - S(x) ```Abstraction for str_init() function.```

- #### Functions

    ```C
    uint16_t str_count(const char* src_str, const char* value)

    Counts the number of times a given element appears in the string.

    Parameters:
        src_str: Source string.
        value  : value to be counted.
    
    Return:
        The number of times this value was found.

    ```

    ```C
    uint8_t str_split(const char* src_str, const char* value, char*** dest_list)

    Splits a string using a value as a reference.

    Paremeters:
        src_str  : Source string.
        value    : Reference value.
        dest_list: Destination char** array (passed by reference).

    Return:
        The number of elements of Destination list.

    ```

    ```C
    uint8_t str_join(char** dest_str, char* join_str, char** str_list, uint8_t item_count)

    Join a list of strings with a 'join string'.

    Parameters:
        dest_str : Destination string (passed by reference).
        join_str : Used to be appended between the strings.
        str_list : List of strings to be joined.

    Return:
        The new length of dest_str.
    ```

    ```C
    uint8_t str_replace(char** dest_str, const char* src_str, char* old_value, char* new_value)

    Replace all the old values for a new value.

    Parameters:
        dest_str  : Destination string (passed by reference).
        src_str   : Source string.
        old_value : Value to be changed.
        new_value : New value.

    Return:
        The new length of dest_str.
    ```

    ```C
    size_t str_from_file(char** dest_str, char* file_path)

    Copy all content of a text file to a string.

    Parameters:
        dest_str  : Destination string (passed by reference).
        file_path : File path

    Return:
        The new length of dest_str.
    ```

    ```C
    size_t str_to_file(const char* src_str, char* file_path, char* mode)

    Write a string to a file.

    Parameters:
        dest_str  : Destination string (passed by reference).
        file_path : File path

    Return:
        Total of characters written.
    ```

    ```C
    STRING_t* str_init(char* str)

    Create a new STRING_t* with size 1 (dynamically allocated).

    Parameters:
        str : Input string (Can be NULL).

    Return:
        A new STRING_t* with size 1 (dynamically allocated).
    ```