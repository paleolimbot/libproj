/* For full ANSI compliance of global variable */

#include "R-libproj/proj.h"
#include "R-libproj/proj_internal.h"

int pj_errno = 0;

/************************************************************************/
/*                          pj_get_errno_ref()                          */
/************************************************************************/

int *pj_get_errno_ref()

{
    return &pj_errno;
}

/* end */
