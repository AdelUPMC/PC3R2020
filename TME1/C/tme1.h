#ifndef TME1_H
#define TME1_H






#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

#define NB_THREADS 8
#define SIZEFIFO 42
#define NBPROD 4
#define NBCONS 4
#define CIBLE 3


typedef struct s_fifo
{
	char	**tapis;
	int		start;
	int		end;
	int		sizemax;
	int		size;
}			fifo;


/*
 *	l'ordre  des attribut ne doit pas changer (pour la conversion en  void*)
 */
typedef struct s_compress
{
	fifo	*f;
	const char	*nom;
	int		cible;
}			compress, t_producteur;

typedef struct s_consommateur
{
	fifo	*f;
	int		ref;
}			t_consommateur;




// fifo.h
int		isEmpty(fifo *f);
int		isfull(fifo *f);
void	free_fifo(fifo* f);
fifo*	make_fifo(int size);
void	print_fifo(fifo *f);
void	enfile(fifo* f, char* element);
char	*defile(fifo *f);


//Prdcons.c
void *consommateur(void *args);
void *producteur(void *args);

// utile.c
char*	strcjoin(char *w1, char*w2, char c);
void		error_malloc(const char *s);
#endif
