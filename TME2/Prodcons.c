#include "tme2.h"
int CPT; // partagé par les consommateurs et les messagers, donc à protéger avec un mutex

//apthread_mutex_t cptmut=PTHREAD_MUTEX_INITIALIZER;


//ft_thread_t    consommateurs[NB_CONS], producteurs[NB_CONS],messagers[NB_MESSAGERS];
ft_scheduler_t	schedcons, schedprod;
ft_event_t      peut_produire_prod,peut_consommer_cons,peut_produire_msg,peut_consommer_msg;
/*pas besoin de mutex pour les journaux journalProd et journalCons
car ils ne sont pas partagés entre thread liées et non liées*/
//journal journalProd,journalCons,journalMsg;
//pthread_mutex_t jmsgmut=PTHREAD_MUTEX_INITIALIZER;
//ft_thread_mutex_lock(jmsg)
//ft_thread_mutex_unlock(jmsg)


/*
journal makeJournal(){
	char **l=(char**) malloc(sizeof(char*));
	journal j= (journal*)malloc(sizeof(journal));
	j->log=l;
	j->size=0;
}

void free_journal(journal* j){
	if(j){
		free(j->l);
		free(j);
	}
}

void addLog(journal * j,char* msg){
	j->log[j->size]=msg;
	j->size++;
}

void readLogs(journal* j){
	int i;
	for(i=0;i<j->size;j++){
		printf(" log[%d]: %s\n",i,(j->log)[i]);
	}
}
*/
t_messager	make_messager(int ref,fifo *tapis_prod,fifo *tapis_cons,ft_scheduler_t schedtprod,ft_scheduler_t schedtcons)
{
	t_messager m;
	m.ref=ref;
	m.tapis_prod=tapis_prod;
	m.tapis_cons=tapis_cons;
	m.schedtcons=&schedtcons;
	m.schedtprod=&schedtprod;
	m.paquet=(char*)malloc(sizeof(char));
	return (m);
}

t_messager	void_to_messager(void *conso)
{
	t_messager *tmp = (t_messager*) conso;
	t_messager m;
	m.ref=tmp->ref;
	m.tapis_prod=tmp->tapis_prod;
	m.tapis_cons=tmp->tapis_cons;
	m.schedtcons=tmp->schedtcons;
	m.schedtprod=tmp->schedtprod;
	m.paquet=tmp->paquet;
	return m;
}

t_producteur	make_producteur(fifo *tapis_prod, char *nom, int cible, ft_scheduler_t schedtprod)
{
	t_producteur p;
	p.tapis_prod = tapis_prod;
	p.nom = strdup(nom);
	p.cible = cible;
	ft_thread_link(schedtprod);
	return (p);
}

t_producteur	void_to_producteur(void *conso)
{
	t_producteur *tmp = (t_producteur*) conso;
	t_producteur p;
	p.tapis_prod = tmp->tapis_prod;
	p.nom = tmp->nom;
	p.cible = tmp->cible;
	return p;
}

t_consommateur	make_consommateur(fifo *tapis_cons, int ref, ft_scheduler_t schedtcons)
{
	t_consommateur t;
	t.tapis_cons = tapis_cons;
	t.ref = ref;
	ft_thread_link(schedtcons);
	return (t);
}

t_consommateur	void_to_consommateur(void *conso)
{
	t_consommateur *tmp = (t_consommateur*) conso;
	t_consommateur c;
	c.tapis_cons = tmp->tapis_cons;
	c.ref = tmp->ref;
	return c;
}

void producteur(void *conso)
{
	t_producteur p = void_to_producteur(conso);
	char *str = calloc(10,1);
	char *res;
	//char *enfilesuccess;
	int produits = 0;

	printf("GO\n");
	while(produits < p.cible)
	{
		if (isfull(p.tapis_prod))
		{
			printf("tapis plein, attente de l'évènement peut_produire_prod\n");
			ft_thread_await(peut_produire_prod);
			printf("reveille TOI !\n");
		}
		sprintf(str, "%d", produits);
		res = strcjoin((char*)p.nom, str, ' ');
		enfile(p.tapis_prod , res);
	//	enfilesuccess=strcjoin("succès de l'enfilage: ",res,' ');
	//	addLog(journalProd,enfilesuccess);
		free(res);
	//	free(enfilesuccess);
		produits++;
		ft_thread_generate(peut_consommer_msg);//réveil un messager en attente sur le scheduler de production
		ft_thread_cooperate();
	}
	free(str);
}

void consommateur(void *conso)
{
	t_consommateur c = void_to_consommateur(conso);
	char *s;
	//char *defilesuccess;

	while(CPT > 0)
	{
		if (isEmpty(c.tapis_cons))
		{
			printf("tapis vide, dort\n");
			ft_thread_await(peut_consommer_cons);
		}
		s = defile(c.tapis_cons);
		printf("C%d mange %s \n", c.ref,s);
	/*	defilesuccess=strcjoin("succès du défilage: ",s,' ');
		addLog(journalCons,defilesuccess);
		ft_thread_mutex_lock(cptmut);
	*/	CPT--;
	//	ft_thread_mutex_unlock(cptmut);
	//	free(s);
	//	free(defilesuccess);
		ft_thread_generate(peut_produire_msg);//réveil un messager en attente sur le scheduler de consommation
		ft_thread_cooperate();
	}
	printf("consommateur %d a fini sa vie\n", c.ref);
}

void _messager(void *conso)
{
	t_messager m= void_to_messager(conso);
	char *s;
	//char *defilesuccess;
	//char *enfilesuccess;

	while(CPT > 0)
	{
		ft_thread_link(*m.schedtprod);
		if (isEmpty(m.tapis_prod))
		{
			printf("tapis de production vide\n");
			ft_thread_await(peut_consommer_msg);
		}
		//pas besoin de mutex car il est forcément lié au scheduler schedprod à ce moment là
		m.paquet = defile(m.tapis_prod);
		ft_thread_generate(peut_produire_prod);// réveil des producteurs en attente sur (tapis_prod plein)
		ft_thread_unlink();
		//defilesuccess=strcjoin("Messager: defile un paquet dans le tapis de production: ",m.paquet,' ');
		/*debut écriture du trajet*/
	/*
	  ft_thread_mutex_lock(jmsgmut);
		addLog(journalMsg,defilesuccess);
		ft_thread_mutex_unlock(jmsgmut);
		*/
		//free(defilesuccess);
		/*fin écriture du trajet*/

		ft_thread_link(*m.schedtcons);
		if (isfull(m.tapis_cons))
		{
			printf("tapis de consommation vide\n");
			ft_thread_await(peut_produire_msg);
		}
		//pas besoin de mutex car il est forcément lié au scheduler schedcons à ce moment là
		enfile(m.tapis_cons ,m.paquet);
		//enfilesuccess=strcjoin("Messager: enfile un paquet dans le tapis de consommation:  ",m.paquet,' ');
		/*debut écriture du trajet*/
		/*
		ft_thread_mutex_lock(jmsgmut);
		addLog(journalMsg,enfilesuccess);
		ft_thread_mutex_unlock(jmsgmut);
		free(enfilesuccess)
		*/
		/*fin écriture du trajet*/
		ft_thread_generate(peut_consommer_cons);// réveil des consommateurs en attente sur (tapis_cons vide)
		ft_thread_unlink();
	}
	printf("messager %d a fini sa vie\n", m.ref);
}
/*
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
}*/




void		test()
{
	fifo	*tapisprod= make_fifo(SIZEFIFO);
	fifo	*tapiscons= make_fifo(SIZEFIFO);
	int		i;
	char	*str = calloc(42, 1);

	t_consommateur  conso[NBCONS];
	t_producteur  produ[NBPROD];
	t_messager mess[NBMESSAGERS];

	ft_thread_t *prod;
	ft_thread_t *cons;
	ft_thread_t * msg;

	ft_scheduler_t schedtprod = ft_scheduler_create();
	ft_scheduler_t schedtcons = ft_scheduler_create();

	peut_produire_prod= ft_event_create(schedtprod);
	peut_consommer_cons= ft_event_create(schedtcons);
	peut_produire_msg= ft_event_create(schedtcons);
	peut_consommer_msg= ft_event_create(schedtprod);

	CPT = CIBLE * NBPROD;
	if (!(prod = (ft_thread_t*)malloc(sizeof(ft_thread_t) * NBPROD)))
		error_malloc("test_fair_threads");
	if (!(cons = (ft_thread_t*)malloc(sizeof(ft_thread_t) * NBCONS)))
		error_malloc("test_fair_threads");
	if (!(msg = (ft_thread_t*)malloc(sizeof(ft_thread_t) * NBMESSAGERS)))
		error_malloc("test_fair_threads");
	for (i = 0; i < NBPROD; i++) {
		sprintf(str, "fruit%d", i);
		printf("%s\n", str);
		produ[i] = make_producteur(tapisprod, str, CIBLE,schedprod);
		prod[i]=ft_thread_create(schedprod, producteur,NULL,(void*)(&produ[i]));
	}

	for (i = 0; i < NBCONS; i++)
	{
		conso[i] = make_consommateur(tapiscons, i,schedcons);
		cons[i]=ft_thread_create(schedcons, consommateur,NULL,(void*)(&conso[i]));
	}

	for (i = 0; i < NBMESSAGERS; i++)
	{
		mess[i] = make_messager(i,tapisprod,tapiscons,schedprod,schedcons);
		msg[i]=ft_thread_create(schedtprod,_messager,NULL,(void*)(&mess[i]));
	}
/*
	for (i = 0; i < NBCONS; i++)
		ft_thread_join(cons[i]);

	for (i = 0; i < NBPROD; i++)
		ft_thread_join(prod[i]);

	for (i = 0; i < NBMESSAGERS; i++)
		ft_thread_join(msg[i]);
*/
	ft_scheduler_start (schedtprod);
	ft_scheduler_start (schedtcons);
	printf("START\n");
	free_fifo(tapisprod);
	free_fifo(tapiscons);
}









void	test_fair_threads()
{
	fifo *tapisprod= make_fifo(SIZEFIFO);
	fifo *tapiscons= make_fifo(SIZEFIFO);
	ft_thread_t *prod;
	ft_thread_t *cons;
	ft_thread_t * msg;
	t_consommateur  conso[NBCONS];
	t_producteur  produ[NBPROD];
	t_messager mess[NBMESSAGERS];
	int i;
	char *str = calloc(10,1);

	ft_scheduler_t schedtprod = ft_scheduler_create();
	ft_scheduler_t schedtcons = ft_scheduler_create();
	ft_scheduler_t schedtmp = ft_scheduler_create();//scheduler initial des messagers
	//events pour les producteurs et consommateurs:
	ft_event_t peut_produire_prod= ft_event_create(schedtprod);
	ft_event_t peut_consommer_cons= ft_event_create(schedtcons);

	/*Rappel : un messager consomme dans le tableau de prod et produit dans le tableau de cons
	events pour les messagers: */
	ft_event_t peut_produire_msg= ft_event_create(schedtcons);
	ft_event_t peut_consommer_msg= ft_event_create(schedtprod);
/*
	journalProd=make_journal();
	journalCons=make_journal();
	journalMsg=make_journal();
*/

/*création des fair_threads*/
/*
	if (!(prod = (ft_thread_t*)malloc(sizeof(ft_thread_t) * NBPROD)))
		error_malloc("test_fair_threads");
	if (!(cons = (ft_thread_t*)malloc(sizeof(ft_thread_t) * NBCONS)))
		error_malloc("test_fair_threads");
	if (!(msg = (ft_thread_t*)malloc(sizeof(ft_thread_t) * NBMESSAGERS)))
		error_malloc("test_fair_threads");


	for (i = 0; i < NBPROD; i++) {
		sprintf(str, "fruit%d", i);
		produ[i] = make_producteur(tapisprod, str, CIBLE,schedprod);
		prod[i]=ft_thread_create(schedprod,_producteur,NULL,(void*)(&produ[i]));
	}

	for (i = 0; i < NBCONS; i++)
	{
		conso[i] = make_consommateur(tapiscons, i,schedcons);
		cons[i]=ft_thread_create(schedcons,_consommateur,NULL,(void*)(&conso[i]));
	}

	for (i = 0; i < NBMESSAGERS; i++)
	{
		mess[i] = make_messager(i,tapisprod,tapiscons,schedprod,schedcons);
		msg[i]=ft_thread_create(schedtmp,_messager,NULL,(void*)(&mess[i]));
	}


	for (i = 0; i < NBCONS; i++)
		ft_thread_join(cons[i]);

	for (i = 0; i < NBPROD; i++)
		ft_thread_join(prod[i]);

	for (i = 0; i < NBMESSAGERS; i++)
		ft_thread_join(msg[i]);



	free_fifo(tapisprod);
	free_fifo(tapiscons);
	free_journal(journalProd);
	free_journal(journalCons);
	free_journal(journalMsg);

	free(prod);
	free(cons);
	free(msg);
	free(str);
	printf("finish\n");
*/}

int main(void)
{
	CPT = CIBLE * NBPROD;
	test();
	//bigtest();
	//test_fair_threads();
	return 0;
}
