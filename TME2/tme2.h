#ifndef TME2_H
#define TME2_H






#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

#define NB_THREADS 8
#define SIZEFIFO 42
#define NBPROD 4
#define NBCONS 4
#define NBMESSAGERS 3
#define CIBLE 3


typedef struct s_journal{
	char **log;
	int size;
}journal;

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
typedef struct s_producteur
{
	fifo		*tapis_prod;
	const char	*nom;
	int		cible;
//	ft_scheduler_t 	*schedtprod;
}	t_producteur;

typedef struct s_consommateur
{
	fifo	*tapis_cons;
	int		ref;
//	ft_scheduler_t *schedtcons;
}			t_consommateur;


typedef struct s_messager
{
	int ref;
	fifo	*tapis_prod;
	fifo	*tapis_cons;
	char	*paquet;
	ft_scheduler_t *schedtprod;
	ft_scheduler_t *schedtcons;
}t_messager;



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
