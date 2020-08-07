#include "cpp-compat.h"
/* print projection's list of parameters */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "proj.h"
#include "proj_internal.h"

#define LINE_LEN 72
	static int
pr_list(PJ *P, int not_used) {
	paralist *t;
	int l, n = 1, flag = 0;

	(void)cpp_compat_putchar('#');
	for (t = P->params; t; t = t->next)
		if ((!not_used && t->used) || (not_used && !t->used)) {
			l = (int)strlen(t->param) + 1;
			if (n + l > LINE_LEN) {
				(void)cpp_compat_printf("\n#");
				n = 2;
			}
			(void)cpp_compat_putchar(' ');
			if (*(t->param) != '+')
				(void)cpp_compat_putchar('+');
			(void)cpp_compat_printf(t->param);
			n += l;
		} else
			flag = 1;
	if (n > 1)
		(void)cpp_compat_putchar('\n');
	return flag;
}
	void /* print link list of projection parameters */
pj_pr_list(PJ *P) {
	char const *s;

	(void)cpp_compat_putchar('#');
	for (s = P->descr; *s ; ++s) {
		(void)cpp_compat_putchar(*s);
		if (*s == '\n')
			(void)cpp_compat_putchar('#');
	}
	(void)cpp_compat_putchar('\n');
	if (pr_list(P, 0)) {
		(void)cpp_compat_printf("#--- following specified but NOT used\n");
		(void)pr_list(P, 1);
	}
}

/************************************************************************/
/*                             pj_get_def()                             */
/*                                                                      */
/*      Returns the PROJ.4 command string that would produce this       */
/*      definition expanded as much as possible.  For instance,         */
/*      +init= calls and +datum= definitions would be expanded.         */
/************************************************************************/

char *pj_get_def( PJ *P, int options )

{
    paralist *t;
    int l;
    char *definition;
    size_t def_max = 10;
    (void) options;

    definition = (char *) pj_malloc(def_max);
    if (!definition)
        return nullptr;
    definition[0] = '\0';

    for (t = P->params; t; t = t->next)
    {
        /* skip unused parameters ... mostly appended defaults and stuff */
        if (!t->used)
            continue;

        /* grow the resulting string if needed */
        l = (int)strlen(t->param) + 1;
        if( strlen(definition) + l + 5 > def_max )
        {
            char *def2;

            def_max = def_max * 2 + l + 5;
            def2 = (char *) pj_malloc(def_max);
            if (def2) {
                strcpy( def2, definition );
                pj_dalloc( definition );
                definition = def2;
            }
            else {
                pj_dalloc( definition );
                return nullptr;
            }
        }

        /* append this parameter */
        strcat( definition, " +" );
        strcat( definition, t->param );
    }

    return definition;
}
