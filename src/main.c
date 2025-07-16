#include "../dependencies/CArgvParseOne.c"
#include "../dependencies/doTheWorldOne.c"
#include "flags.h"
#include "colors.h"

void add_to_json(cJSON *out_json,const char *file_path) {
    cJSON *file_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(file_obj, "path", file_path);
    
    char *content = dtw_load_string_file_content(file_path);
    if (content) {
        cJSON_AddStringToObject(file_obj, "content", content);
        free(content);
    } else {
        cJSON_AddNullToObject(file_obj, "content");
    }
    
    cJSON_AddItemToArray(out_json, file_obj);
}

int collect_data(CArgvParse *args) {
    int total_entries = sizeof(FLAG_ENTRIES)/sizeof(char*);
    int entries_flag_size = CArgvParse_get_infinity_flag_size(args,FLAG_ENTRIES,total_entries);

    if(entries_flag_size == 0){
        printf(ERROR_COLOR"Error: No entries specified.\n");
        return 1;
    }

    int total_out_instructions = sizeof(OUT_INSTRUCTIONS)/sizeof(char*);
    int out_instructions_flag_size = CArgvParse_get_flag_size(args,OUT_INSTRUCTIONS,total_out_instructions);
    if(out_instructions_flag_size == 0){
        printf(ERROR_COLOR"Error: No Out Instructions flag specified.\n");
        return 1;
    } 
    if(out_instructions_flag_size > 1){
        printf(ERROR_COLOR"Error: Multiple No Out Instructions flags specified.\n");
        return 1;
    }
    const char *out_instruction_path = CArgvParse_get_flag(args,OUT_INSTRUCTIONS,total_out_instructions,0);

    // Check for prompt flag
    int total_prompt = sizeof(FLAG_PROMPT)/sizeof(char*);
    int prompt_flag_size = CArgvParse_get_flag_size(args,FLAG_PROMPT,total_prompt);
    const char *custom_prompt = "";
    if(prompt_flag_size > 0){
        if(prompt_flag_size > 1){
            printf(ERROR_COLOR"Error: Multiple prompt flags specified.\n");
            return 1;
        }
        custom_prompt = CArgvParse_get_flag(args,FLAG_PROMPT,total_prompt,0);
    }

    cJSON *data = cJSON_CreateArray();
    for(int i = 0; i <entries_flag_size; i++){
        const char *current_entrie = CArgvParse_get_infinty_flag(args,FLAG_ENTRIES,total_entries,i);
        int type = dtw_entity_type(current_entrie);
        if(type == DTW_FILE_TYPE) {
            add_to_json(data, current_entrie);
        } 
        if(type == DTW_FOLDER_TYPE) {
            DtwStringArray *files = dtw_list_files_recursively(current_entrie,DTW_CONCAT_PATH);
            for(int j = 0; j < files->size; j++){
                add_to_json(data, files->strings[j]);
            }
            DtwStringArray_free(files);
        }
    }

    // Create the example JSON with proper formatting
    char *json_string = cJSON_Print(data);  // Use formatted print for clarity
    
    // Create a comprehensive instruction message
    const char *instruction_template = 
        "%s\n\n"
        "\nIMPORTANT: do not respond the json with content that you did not modified\n"
        "\nIMPORTANT: only include in the json output, content that you modified \n"
        "\nIMPORTANT: You must respond in these format: .\n"
        "[\n"
        "  {\"path\": \"file_path_1\", \"content\": \"content_of_file_1 or null\"},\n"
        "  {\"path\": \"file_path_2\", \"content\": \"content_of_file_2 or null\"},\n"
        "  {\"path\": \"file_path_n\", \"content\": \"content_of_file_n or null\"}\n"
        "]\n\n"
        "file tree:\n%s\n\n";
    // Calculate message size considering custom prompt
    size_t prompt_size = strlen(custom_prompt) + 2; // +2 for "\n\n"
    size_t message_size = strlen(instruction_template) + strlen(json_string) + prompt_size + 1;
    char *whole_message = malloc(message_size);
    
  
    snprintf(whole_message, message_size, instruction_template, custom_prompt,json_string);

    

    dtw_write_string_file_content(out_instruction_path, whole_message);
    
    free(json_string);
    free(whole_message);

    cJSON_Delete(data);
    return 0;
}

int implement(CArgvParse *args) {
    int total_json_flag = sizeof(FLAG_JSON)/sizeof(char*);
    int json_flag_size = CArgvParse_get_flag_size(args,FLAG_JSON,total_json_flag);
    if(json_flag_size == 0){
        printf(ERROR_COLOR"Error: No JSON flag specified.\n");
        return 1;
    } 
    if(json_flag_size > 1){
        printf(ERROR_COLOR"Error: Multiple JSON flags specified.\n");
        return 1;
    }
    const char *json_file_path = CArgvParse_get_flag(args,FLAG_JSON,total_json_flag,0);

    char *json_content = dtw_load_string_file_content(json_file_path);
    if (json_content == NULL) {
        printf(ERROR_COLOR"Error: Could not load JSON file %s\n", json_file_path);
        return 1;
    }
    
    // Try to extract JSON from the response if it contains extra text
    char *json_start = strchr(json_content, '[');
    char *json_end = strrchr(json_content, ']');
    
    if (json_start && json_end && json_end > json_start) {
        // Extract only the JSON portion
        size_t json_length = json_end - json_start + 1;
        char *extracted_json = malloc(json_length + 1);
        strncpy(extracted_json, json_start, json_length);
        extracted_json[json_length] = '\0';
        
        cJSON *json = cJSON_Parse(extracted_json);
        free(extracted_json);
        free(json_content);
        
        if (json == NULL) {
            printf(ERROR_COLOR"Error: Could not parse extracted JSON from file %s\n", json_file_path);
            return 1;
        }
        
        if(json->type != cJSON_Array){
            printf(ERROR_COLOR"Error: JSON file %s is not an array.\n", json_file_path);
            cJSON_Delete(json);
            return 1;
        }

        // Iterate through all array items
        cJSON *item = NULL;
        cJSON_ArrayForEach(item, json) {
            if (item->type == cJSON_Object) {
                cJSON *path_obj = cJSON_GetObjectItem(item, "path");
                cJSON *content_obj = cJSON_GetObjectItem(item, "content");
                
                if (path_obj && cJSON_IsString(path_obj) && path_obj->valuestring) {
                    if (content_obj && cJSON_IsString(content_obj) && content_obj->valuestring) {
                        dtw_write_string_file_content(path_obj->valuestring, content_obj->valuestring);
                        printf(SUCCESS_COLOR"Written: %s\n", path_obj->valuestring);
                    } else if (content_obj && cJSON_IsNull(content_obj)) {
                        // Handle null content - create empty file or skip
                        dtw_write_string_file_content(path_obj->valuestring, "");
                        printf(SUCCESS_COLOR"Written (empty): %s\n", path_obj->valuestring);
                    }
                }
            }
        }
        
        cJSON_Delete(json);
        return 0;
    } else {
        free(json_content);
        printf(ERROR_COLOR"Error: Could not find valid JSON in file %s\n", json_file_path);
        return 1;
    }
}

int main(int argc, char *argv[]) {
    CArgvParse args = newCArgvParse(argc,argv);
    const char *action = CArgvParse_get_arg(&args, 1);
    if (action == NULL) {
        printf(ERROR_COLOR"Error: No action specified. Use 'collect' or 'implement'.\n");
        return 1;
    }

    if (strcmp(action, "collect") == 0) {
        return collect_data(&args);
    } else if (strcmp(action, "implement") == 0) {
        return implement(&args);
    } else {
        printf(ERROR_COLOR"Unknown action: %s\n", action);
        return 1;
    }

    return 0;
}