/*  C-Strings:  String utility for C.

    Author: Guilherme Arruda

    Created in: 28/jul/24
    Last Updated: 05/oct/24
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
#include <stdarg.h>


//========================================= DATA STRUCTURES ====================================

// MACROS

#define S(a) str_init(a)
#define END_LIST "-->END_LIST<--"
#define count_t uint16_t

typedef struct STRING_t{
    char *str;
    count_t str_len;
    count_t (*count)(const char* src_str, const char* value);
    count_t (*split)(const char* src_str, const char* value, char*** dest_list);
    count_t (*join)(char** dest_str, char* join_str, char** str_list, count_t item_count);
    count_t (*vjoin)(char** dest_str, char* join_str, ...);
    count_t (*search)(const char* src_str, const char* value, count_t** dest_pos, count_t ocurrency_num);
    count_t (*replace)(char** dest_str, const char* src_str, char* old_value, char* new_value);
    size_t (*from_file)(char** dest_str, char* file_path);
    size_t (*to_file)(const char* src_str, char* file_path, char* mode);
}STRING_t;

//=============(*vjoin)(*vjoin)(*vjoin)(*vjoin)====================== FUNCTIONS ==============================================

count_t str_count(const char* src_str, const char* value){
    count_t val_len = strlen(value), count = 0;
    
    for(count_t i = 0; (i + val_len) <= strlen(src_str); i++){
    	if(!strncmp(&src_str[i], value, val_len)){
    		i += val_len - 1;
    		count++;
		}
	}

    return count;
}

count_t str_split(const char* src_str, const char* value, char*** dest_list){
    count_t val_len = strlen(value);
    count_t count = 0, last_pos = 0, i;
    
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

count_t str_join(char** dest_str, char* join_str, char** str_list, count_t item_count){
    
    count_t count = 0, i;
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

count_t str_vjoin(char** dest_str, char* join_str, ...){
    count_t output_len = 0;
    (*dest_str) = NULL;
    
    va_list args;
    va_start(args, join_str);

    char* tmp = va_arg(args, char*);
    
    while(strcmp(tmp, END_LIST) != 0){
        output_len += strlen(tmp) + strlen(join_str);
        (*dest_str) = (char*)realloc((*dest_str), output_len);
        strcat((*dest_str), tmp);
        strcat((*dest_str), join_str);
        tmp = NULL;
        tmp = va_arg(args, char*);
    };

    output_len -= strlen(join_str);
    (*dest_str) = (char*)realloc((*dest_str), output_len);
    (*dest_str)[output_len] = '\0';

    va_end(args);

    return output_len;
}

count_t str_search(const char* src_str, const char* value, count_t** dest_pos, count_t occurrence_num){
    
    char* buffer = (char*)calloc(strlen(src_str), sizeof(char));
    strcpy(buffer, src_str);

    if(*dest_pos != NULL){
        free(*dest_pos);
        *dest_pos = NULL;
    }

    count_t count = 0;

    if(!occurrence_num){

        buffer = strstr(buffer, value);
        while(buffer != NULL){
            count++;
            (*dest_pos) = (count_t*)realloc((*dest_pos), count);
            (*dest_pos)[count-1] = (count_t)(strlen(src_str) - strlen(buffer));
            buffer = strstr(&buffer[1], value);
        }

        return count;
    }else{

        buffer = strstr(buffer, value);
        while((buffer != NULL) && (count < occurrence_num)){
            count++;
            (*dest_pos) = (count_t*)realloc((*dest_pos), count);
            (*dest_pos)[count - 1] = (count_t)(strlen(src_str) - strlen(buffer));
            buffer = strstr(&buffer[1], value);
        }

        return count;
    }

};

count_t str_replace(char** dest_str, const char* src_str, char* old_value, char* new_value){
    count_t old_val_len = strlen(old_value);
    count_t new_val_len = strlen(new_value);
    count_t dest_len = strlen(src_str);

    count_t dif = new_val_len - old_val_len;
    count_t last_pos_dest = 0, last_pos_src = 0, i;

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

    return dest_len;
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


    rewind(f);
    fseek(f, 0, SEEK_END);
    size_t f_len = ftell(f);

    fclose(f);

    return f_len;
}

STRING_t* str_init(const char* str){
    STRING_t *s = (STRING_t*)calloc(1, sizeof(STRING_t));

    s->str = (char*)calloc(strlen(str), sizeof(char));
    strcpy(s->str, str);

    s->str_len = strlen(str);

    s->count = str_count;
    s->split = str_split;
    s->join = str_join;
    s->vjoin = str_vjoin;
    s->search = str_search;
    s->replace = str_replace;
    s->to_file = str_to_file;
    s->from_file = str_from_file;
}


#endif // END C_strings