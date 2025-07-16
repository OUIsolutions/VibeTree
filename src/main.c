#include "dependencies/CArgvParseOne.c"
#include "dependencies/doTheWorldOne.c"
#include "flags.h"
#include "src/colors.h"

int collect_data(CArgvParse *args) {
    return 0; // Return 0 to indicate success.
}

int main(int argc, char *argv[]) {
    CArgvParse args = newCArgvParse(argc,argv);
    const char *action =  CArgvParse_get_arg(&args, 0);
    if (strcmp(action, "collect") == 0) {
        return collect_data(&args);
    } else {
        printf(ERROR_COLOR"Unknown action: %s\n", action);
        return 1; // Return non-zero to indicate failure.
    }

    return 0;
}