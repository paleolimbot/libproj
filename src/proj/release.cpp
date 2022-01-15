/* <<< Release Notice for library >>> */

#include "R-libproj/proj.h"
#include "R-libproj/proj_internal.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

char const pj_release[] =
    "Rel. "
    STR(PROJ_VERSION_MAJOR)"."
    STR(PROJ_VERSION_MINOR)"."
    STR(PROJ_VERSION_PATCH)", "
    "January 1st, 2022";

const char *pj_get_release() {
    return pj_release;
}
