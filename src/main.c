#include "../dependencies/CArgvParseOne.c"
#include "../dependencies/doTheWorldOne.c"
#include "flags.h"
#include "colors.h"
void add_to_json_if_not_binary(cJSON *out_json,const char *file_path) {
    char *content = dtw_load_string_file_content(file_path);
    if (content) {
        cJSON_AddStringToObject(out_json, file_path, content);
        free(content);
    } else {
        printf(ERROR_COLOR"Warning: Skipping binary file %s\n", file_path);
    }
}

int collect_data(CArgvParse *args) {


    const char* entries[] = {"entries","e"};
    int total_entries = sizeof(entries)/sizeof(char*);
    int entries_flag_size = CArgvParse_get_flag_size(args,entries,total_entries);

    if(entries_flag_size == 0){
        printf(ERROR_COLOR"Error: No entries specified.\n");
        return 1; // Return non-zero to indicate failure.
    }
    const char *json_flag[] = {"json","j"};
    int total_json_flag = sizeof(json_flag)/sizeof(char*);
    int json_flag_size = CArgvParse_get_flag_size(args,json_flag,total_json_flag);
    if(json_flag_size == 0){
        printf(ERROR_COLOR"Error: No JSON flag specified.\n");
        return 1; // Return non-zero to indicate failure.
    } 
    if(json_flag_size > 1){
        printf(ERROR_COLOR"Error: Multiple JSON flags specified.\n");
        return 1; // Return non-zero to indicate failure.
    }
    const char *json_file_path = CArgvParse_get_flag(args,json_flag,total_json_flag,0);


    cJSON *data = cJSON_CreateObject();
    for(int i = 0; i <entries_flag_size; i++){
        const char *current_entrie = CArgvParse_get_flag(args,entries,total_entries,i);
        int type = dtw_entity_type(current_entrie);
        if(type == DTW_FILE_TYPE) {
            add_to_json_if_not_binary(data, current_entrie);
        } 
        if(type == DTW_FOLDER_TYPE) {
            DtwStringArray *files = dtw_list_files_recursively(current_entrie,DTW_CONCAT_PATH);
            for(int j = 0; j < files->size; j++){
                add_to_json_if_not_binary(data, files->strings[j]);
            }
        }
    }
    char *json_string = cJSON_PrintUnformatted(data);
    dtw_write_string_file_content(json_file_path, json_string);
    free(json_string);
    cJSON_Delete(data);
    return 0; // Return 0 to indicate success.
}
int implement(CArgvParse *args) {
    const char *json_flag[] = {"json","j"};
    int total_json_flag = sizeof(json_flag)/sizeof(char*);
    int json_flag_size = CArgvParse_get_flag_size(args,json_flag,total_json_flag);
    if(json_flag_size == 0){
        printf(ERROR_COLOR"Error: No JSON flag specified.\n");
        return 1; // Return non-zero to indicate failure.
    } 
    if(json_flag_size > 1){
        printf(ERROR_COLOR"Error: Multiple JSON flags specified.\n");
        return 1; // Return non-zero to indicate failure.
    }
    const char *json_file_path = CArgvParse_get_flag(args,json_flag,total_json_flag,0);

    char *json_content = dtw_load_string_file_content(json_file_path);
    if (json_content == NULL) {
        printf(ERROR_COLOR"Error: Could not load JSON file %s\n", json_file_path);
        return 1; // Return non-zero to indicate failure.
    }
    cJSON *json = cJSON_Parse(json_content);
    free(json_content);
    if (json == NULL) {
        printf(ERROR_COLOR"Error: Could not parse JSON file %s\n", json_file_path);
        return 1; // Return non-zero to indicate failure.   
    }

    if(json->type != cJSON_Object){
        printf(ERROR_COLOR"Error: JSON file %s is not an object.\n", json_file_path);
        cJSON_Delete(json);
        return 1; // Return non-zero to indicate failu
    }

    for(int i= 0; cJSON_GetArraySize(json) > i; i++){
        cJSON *item = cJSON_GetArrayItem(json, i);
        if (item == NULL || item->type != cJSON_String) {
            printf(ERROR_COLOR"Error: Invalid JSON item at index %d\n", i);
            cJSON_Delete(json);
            return 1; // Return non-zero to indicate failure.
        }
        
        const char *key = item->string;
        const char *value = item->valuestring;
        dtw_write_string_file_content(key, value);    
    }

}

int main(int argc, char *argv[]) {
    CArgvParse args = newCArgvParse(argc,argv);
    const char *action =  CArgvParse_get_arg(&args, 1);
    if (strcmp(action, "collect") == 0) {
        return collect_data(&args);
    }
    if (strcmp(action, "implement") == 0) {
        return implement(&args);
    }
    else {
        printf(ERROR_COLOR"Unknown action: %s\n", action);
        return 1; // Return non-zero to indicate failure.
    }

    return 0;
}