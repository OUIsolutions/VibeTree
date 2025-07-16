#include "../dependencies/CArgvParseOne.c"
#include "../dependencies/doTheWorldOne.c"
#include "flags.h"
#include "colors.h"

void add_to_json(cJSON *out_json,const char *file_path) {
    char *content = dtw_load_string_file_content(file_path);
    if (content) {
        cJSON_AddStringToObject(out_json, file_path, content);
        free(content);
        return;
    } 
    cJSON_AddNullToObject(out_json, file_path);
}

int collect_data(CArgvParse *args) {
    int total_entries = sizeof(FLAG_ENTRIES)/sizeof(char*);
    int entries_flag_size = CArgvParse_get_flag_size(args,FLAG_ENTRIES,total_entries);

    if(entries_flag_size == 0){
        printf(ERROR_COLOR"Error: No entries specified.\n");
        return 1;
    }

    int total_out_instructions_flag = sizeof(OUT_INSTRUCTIONS)/sizeof(char*);
    int out_instructions_flag_size = CArgvParse_get_flag_size(args,OUT_INSTRUCTIONS,total_out_instructions_flag);
    if(out_instructions_flag_size == 0){
        printf(ERROR_COLOR"Error: No out-instructions flag specified.\n");
        return 1;
    } 
    if(out_instructions_flag_size > 1){
        printf(ERROR_COLOR"Error: Multiple out-instructions flags specified.\n");
        return 1;
    }
    const char *json_file_path = CArgvParse_get_flag(args,OUT_INSTRUCTIONS,total_out_instructions_flag,0);

    cJSON *data = cJSON_CreateObject();
    for(int i = 0; i <entries_flag_size; i++){
        const char *current_entrie = CArgvParse_get_flag(args,FLAG_ENTRIES,total_entries,i);
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
             "IMPORTANT: You must respond ONLY with a valid JSON object.\n"
        "Do NOT include any explanatory text, markdown formatting, or code blocks.\n"
        "The response must be parseable JSON that follows this exact structure:\n\n"
        "{\n"
        "  \"file_path_1\": \"content_of_file_1\",\n"
        "  \"file_path_2\": \"content_of_file_2\",\n"
        "  \"file_path_n\": \"content_of_file_n\"\n"
        "}\n\n"
        "Here is the specific JSON structure you must follow:\n%s\n\n"
        "Remember: Respond ONLY with the JSON object, nothing else.";
    
    size_t message_size = strlen(instruction_template) + strlen(json_string) + 1;
    char *whole_message = malloc(message_size);
    snprintf(whole_message, message_size, instruction_template, json_string);

    dtw_write_string_file_content(json_file_path, whole_message);
    
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
    char *json_start = strchr(json_content, '{');
    char *json_end = strrchr(json_content, '}');
    
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
        
        if(json->type != cJSON_Object){
            printf(ERROR_COLOR"Error: JSON file %s is not an object.\n", json_file_path);
            cJSON_Delete(json);
            return 1;
        }

        // Iterate through all object members
        cJSON *item = NULL;
        cJSON_ArrayForEach(item, json) {
            if (item->type == cJSON_String && item->string && item->valuestring) {
                dtw_write_string_file_content(item->string, item->valuestring);
                printf(SUCCESS_COLOR"Written: %s\n", item->string);
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