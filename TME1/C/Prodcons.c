#include "tme1.h"

int CPT; // partage par tout les consommateur
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


compress*	make_compress(fifo* f, char* nom, int cible)
{
	compress *c = (compress*)malloc(sizeof(compress));
	c->f = f;
	c->nom = nom;
	c->cible = cible;
	return c;
}

t_producteur	make_producteur(fifo *f, char *nom, int cible)
{
	t_producteur p;
	p.f = f;
	p.nom = strdup(nom);
	p.cible = cible;
	return (p);
}

t_producteur	void_to_producteur(void *conso)
{
	t_producteur *tmp = (t_producteur*) conso;
	t_producteur p;
	p.f = tmp->f;
	p.nom = tmp->nom;
	p.cible = tmp->cible;
	return p;
}

t_consommateur	make_consommateur(fifo *f, int ref)
{
	t_consommateur t;
	t.f = f;
	t.ref = ref;
	return (t);
}

t_consommateur	void_to_consommateur(void *conso)
{
	t_consommateur *tmp = (t_consommateur*) conso;
	t_consommateur c;
	c.f = tmp->f;
	c.ref = tmp->ref;
	return c;
}

void *producteur(void *args)
{
	t_producteur p = void_to_producteur(args);
	char *str = calloc(10,1);
	char *res;
	int produits = 0;

	while(produits < p.cible)
	{
		if (!isfull(p.f))
		{
			pthread_mutex_lock(&mutex);
			sprintf(str, "%d", produits);
			res = strcjoin((char*)p.nom, str, ' ');
			enfile(p.f,res);
			free(res);
			pthread_mutex_unlock(&mutex);
			produits++;
		}
		else
		{
			printf("tapis plein\n");
			usleep(10000);
		}
	}
	free(str);
	return 0x0;
}

void *consommateur(void *conso)
{
	t_consommateur c = void_to_consommateur(conso);
	char *s;

	while(CPT > 0)
	{
		if (!isEmpty(c.f))
		{
			pthread_mutex_lock(&mutex);
			s = defile(c.f);
			printf("C%d mange %s \n", c.ref,s);
			CPT--;
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			printf("tapis vide\n");
			usleep(10000);
		}
	}
	printf("consommateur %d a fini sa vie\n", c.ref);
	return 0x0;
}


void	bigtest()
{
	fifo *f= make_fifo(SIZEFIFO);
	pthread_t *prod;
	pthread_t *cons;
	t_consommateur  conso[NBCONS];
	t_producteur  produ[NBPROD];
	int i;
	char *str = calloc(10,1);


	if (!(prod = (pthread_t*)malloc(sizeof(pthread_t) * NBPROD)))
		error_malloc("bigtest");
	if (!(cons = (pthread_t*)malloc(sizeof(pthread_t) * NBCONS)))
		error_malloc("bigtest");

//	printf("create\n");
	for (i = 0; i < NBPROD; i++) {
		sprintf(str, "fruit%d", i);
		produ[i] = make_producteur(f, str, CIBLE);
		pthread_create(&prod[i],NULL,producteur,(void*)(&produ[i]));
	}	
	for (i = 0; i < NBCONS; i++)
	{
		conso[i] = make_consommateur(f, i);
		pthread_create(&prod[i],NULL,consommateur,(void*)(&conso[i]));
	}


	for (i = 0; i < NBCONS; i++)
		pthread_join(cons[i], NULL);
	for (i = 0; i < NBPROD; i++)
		pthread_join(prod[i], NULL);


	printf("finish\n");
	free_fifo(f);
	free(prod);
	free(cons);
	free(str);
}

int main(void)
{
	CPT = CIBLE * NBPROD;
	/*pthread_t threads[NB_THREADS];
	fifo *f= make_fifo(SIZEFIFO);
	void * status;
	pthread_t *prod;
	pthread_t *cons;
	compress *test;
	
	test = make_compress(f, "Pomme", NBPROD);
	pthread_create(&prod,NULL,producteur,(void*)test);
	pthread_join (prod,&status);
	*/
	bigtest();
	return 0;
}
