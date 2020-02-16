#include "tme2.h"


void		error_malloc(const char *s)
{
	printf("malloc failed in %s\n", s);
	exit(1);
}

char*	strcjoin(char *w1, char*w2, char c)
{
	char *res;

	res = (char*)calloc(strlen(w1)+strlen(w2)+2, sizeof(char));
	res = strcat(res, w1);
	res[strlen(w1)] = c;
	res = strcat(res, w2);
	return res;
}
