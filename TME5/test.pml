



/*
proctype affaicheColor()
{
if
:: feu == 0 -> printf("Le feu est vert\n");
:: feu == 1 -> printf("Le feu est orange\n");
:: feu == 2 -> printf("Le feu est rouge clignote \n");
:: feu == 3 -> printf("Le feu est rouge\n");
:: else -> printf("Le feu est eteint\n");
fi
}

proctype routineFeu(int temp)
{
	int tour = 1;
	do
		:: tour < temp
		run afficheColor();
		if
		:: feu == 1 ->
			feu = 3;
		:: else ->
			feu = (feu + 1) % 4;
		fi
		tour = tour + 1
		:: tour == temp ->
		printf("journée terminer!")
		break;
	od
}
*/

mtype = {GREEN, ORANGE, RED, BUG};

mtype feu = ORANGE;
active proctype routineTricolore()
{
	int tour = 0

	start:
		printf("Le feu est orange clignotant\n");
		feu = BUG;
		goto red;
	do
	:: true

		green:
			feu = GREEN;
			printf("Le feu est vert\n");
			if
			:: true -> goto orange
			:: true -> goto panne
			fi

		orange:
			feu = ORANGE;
			printf("Le feu est orange\n");
			if
			:: true -> goto red
			:: true -> goto panne
			fi

		red:
			feu = RED;
			printf("Le feu est rouge\n");
			if
			:: true -> goto green
			:: true -> goto panne
			fi

		panne:
			feu = BUG;
			printf("Le feu est orange clignotant\n");
			printf("journée terminer\n");
			break;
	od
}

active proctype observeur()
{
	mtype avant = feu;
	mtype apres = feu;

	do 
		::true
		feu != avant
		apres = feu
		if
		:: avant == BUG ->
			assert apres == RED;
		:: avant == GREEN ->
			assert apres == ORANGE;
		:: avant == ORANGE ->
			assert apres == RED;
		:: avant == RED ->
			assert apres == GREEN;
		:: apres == BUG ->
			break;
		fi
	od
}

/*
init 
{
	run routineFeu(10);
}*/
