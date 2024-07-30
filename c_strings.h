/*  C-Strings:  String utility for C.

    Author: Guilherme Arruda

    Created in: 28/07/24
    Last Updated: 29/07/24
*/

//==================================== HEADERS ==============================================
#pragma once

#ifndef _C_STRINGS_HEADER
#define _C_STRINGS_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>


//========================================= DATA STRUCTURES ====================================

typedef struct{
    char *str;
    uint16_t (*count)(const char* src_str, const char* value);
    uint8_t (*split)(const char* src_str, const char* value, char*** dest_list);
    uint8_t (*join)(char** dest_str, char* join_str, char** str_list, uint8_t item_count);
    uint8_t (*replace)(char** dest_str, const char* src_str, char* old_value, char* new_value);
    size_t (*from_file)(char** dest_str, char* file_path);
    size_t (*to_file)(const char* src_str, char* file_path, char* mode);
}STRING_t;

// MACROS

#define S(a) str_init(a)


//=================================== FUNCTIONS ==============================================

uint16_t str_count(const char* src_str, const char* value){
    uint16_t val_len = strlen(value);
    uint8_t count = 0;
    
    for(int i = 0; (i + val_len) <= strlen(src_str); i++){
    	if(!strncmp(&src_str[i], value, val_len)){
    		i += val_len - 1;
    		count++;
		}
	}

    return count;
}

uint8_t str_split(const char* src_str, const char* value, char*** dest_list){
    uint16_t val_len = strlen(value);
    uint8_t count = 0, last_pos = 0, i;
    (*dest_list) = NULL;
    
    for(i = 0; (i + val_len) <= strlen(src_str); i++){
    	if(!strncmp(&src_str[i], value, val_len)){
            (*dest_list) = (char**)realloc((*dest_list), (count + 1)*sizeof(char*));
            (*dest_list)[count] = (char*)calloc(i - last_pos + 1, sizeof(char));
            strncpy((*dest_list)[count], &src_str[last_pos], i - last_pos);
            last_pos = i + val_len;
    		i += val_len - 1;
            count++;
		}
	}

    if(last_pos < strlen(src_str) - 1){
        (*dest_list) = (char**)realloc((*dest_list), (count + 1)*sizeof(char*));
        (*dest_list)[count] = (char*)calloc(i - last_pos + 1, sizeof(char));
        strcpy((*dest_list)[count], &src_str[last_pos]);
        count++;
    }

    return count;
}

uint8_t str_join(char** dest_str, char* join_str, char** str_list, uint8_t item_count){
    
    uint8_t count = 0, i;
    (*dest_str) = (char*)calloc(1, sizeof(char));

    for(i = 0; i < item_count - 1; i++){
        count += strlen(str_list[i]);
        (*dest_str) = (char*)realloc((*dest_str), count*sizeof(char));
        strcat((*dest_str), str_list[i]);
        strcat((*dest_str), join_str);
    }
    
    count += strlen(str_list[i]);
    (*dest_str) = (char*)realloc((*dest_str), count*sizeof(char));
    strcat((*dest_str), str_list[i]);

    return count;
}

uint8_t str_replace(char** dest_str, const char* src_str, char* old_value, char* new_value){
    uint16_t old_val_len = strlen(old_value);
    uint16_t new_val_len = strlen(new_value);
    uint16_t dest_len = strlen(src_str);

    uint8_t dif = new_val_len - old_val_len;
    uint8_t last_pos_dest = 0, last_pos_src = 0, i;

    //(*dest_str) = (char*)calloc(strlen(src_str) + n_times*dif, sizeof(char));
    (*dest_str) = (char*)calloc(dest_len, sizeof(char));

    for(i = 0; (i + old_val_len) <= strlen(src_str); i++){
    	if(!strncmp(&src_str[i], old_value, old_val_len)){
            dest_len += dif;
            (*dest_str) = (char*)realloc((*dest_str), dest_len*sizeof(char));
            strncat((*dest_str), &src_str[last_pos_src], i - last_pos_src);
            strcat((*dest_str), new_value);
            last_pos_dest = i + old_val_len + dif;
            last_pos_src = i + old_val_len;
    		i += old_val_len - 1;
		}
	}

    if(last_pos_src < strlen(src_str) - 1) strcat(&(*dest_str)[last_pos_dest], &src_str[last_pos_src]);

    return 1;
}

size_t str_from_file(char** dest_str, char* file_path){
    FILE *f = fopen(file_path, "r");

    if (f == NULL) {
        perror("\nErro ao abrir o arquivo.\n");
        return 0;
    }

    fseek(f, 0, SEEK_END);
    size_t f_len = ftell(f);
    rewind(f);

    (*dest_str) = (char*)calloc(f_len, sizeof(char));

    fread((*dest_str), sizeof(char), f_len, f);
    (*dest_str)[f_len] = '\0';

    fclose(f);

    return f_len;
}

size_t str_to_file(const char* src_str, char* file_path, char* mode){
    FILE *f = fopen(file_path, mode);

    if (f == NULL) {
        perror("\nErro ao abrir o arquivo.\n");
        return 0;
    }

    fwrite(src_str, sizeof(char), strlen(src_str), f);
    fflush(f);

    fclose(f);

    return strlen(src_str);
}

STRING_t* str_init(char* str){
    STRING_t *s = (STRING_t*)calloc(1, sizeof(STRING_t));

    s->str = str;

    s->count = str_count;
    s->join = str_join;
    s->replace = str_replace;
    s->split = str_split;
    s->to_file = str_to_file;
    s->from_file = str_from_file;
}


#endif _C_STRINGS_HEADER // END C_strings