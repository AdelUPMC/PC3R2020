#include "tme1.h"

fifo*	make_fifo(int size)
{
	fifo *f;
	if(!(f = (fifo*)calloc(1, sizeof(fifo))))
	{
		printf("Malloc failed (make_fifo) \n");
		return 0x0;
	}
	if (!(f->tapis=(char **) calloc(size+1,sizeof(char*))))
	{
		free(f);
		printf("Malloc failed (make_fifo) \n");
		return 0x0;
	}	
	f->start=0;
	f->end=0;
	f->sizemax=size;
	f->size=0;
	return f;
}

void	free_fifo(fifo* f)
{
	int i;
	if (f)
	{
		for(i = f->start; i < f->end; i++)
		{
			if(f->tapis[i])
				free(f->tapis[i]);
		}
		free(f);
		f = 0x0;
	}
}

int		isfull(fifo *f)
{
	
	if (f->size == f->sizemax)
		return 1;
	return 0;
}

int		isEmpty(fifo *f)
{
	
	if (f->size == 0)
		return 1;
	return 0;
}


void	enfile(fifo* f, char* element)
{
	if (f->size == f->sizemax)
	{
		printf("Plein !!\n");
		return;
	}
	if (f->end >= f->sizemax)
	{
		memmove(f->tapis,&f->tapis[f->start], sizeof(char*) * f->size);
		f->start = 0;
		f->end = f->size;
	}
	f->tapis[f->end] = strdup(element);
	f->end+=1;
	f->size+=1;
}

char*	defile(fifo *f)
{
	char *res;
	
	if(f->size==0)
	{
		printf("Vide !!\n");
		return 0x0;
	}
	res = f->tapis[f->start];
	free(f->tapis[f->start]);

	f->tapis[f->start] = 0x0;
	f->start++;
	f->size--;
	return res;
}

void 	print_fifo(fifo *f)
{
	int i;
	
	if (!f)
	{
		printf("Fifo effacé ou non cree\n");
		return;
	}
	printf("Il y a %d elements dans la file :\n", f->size);
	for(i = f->start ; i < f->end; i++)
	{
		printf("[%s] ", f->tapis[i]);
	}
	printf("\n");
}


