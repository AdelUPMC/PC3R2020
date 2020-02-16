#include "tme1.h"

void	test1()
{
	fifo *f = make_fifo(5);
	enfile(f,"1");
	print_fifo(f);
	enfile(f,"2");
	print_fifo(f);
	enfile(f,"3");
	print_fifo(f);
	enfile(f,"4");
	print_fifo(f);
	free_fifo(f);
}

void	test2()
{
	fifo *f = make_fifo(5);
	enfile(f,"1");
	print_fifo(f);
	enfile(f,"2");
	print_fifo(f);
	enfile(f,"3");
	print_fifo(f);
	enfile(f,"4");
	print_fifo(f);
	enfile(f,"5");
	print_fifo(f);
	enfile(f,"6");
	print_fifo(f);
	free_fifo(f);
}
void	test3()
{
	fifo *f = make_fifo(5);
	defile(f);
	free_fifo(f);
}
void	test5()
{
	fifo *f = make_fifo(5);
	enfile(f,"1");
	enfile(f,"2");
	enfile(f,"3");
	enfile(f,"4");
	enfile(f,"5");
	enfile(f,"6");
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("supp %s \n", defile(f));
	printf("strat = %d", f->start);
	enfile(f,"4");
	enfile(f,"5");
	enfile(f,"6");
	printf("strat = %d", f->start);
	free_fifo(f);
}



