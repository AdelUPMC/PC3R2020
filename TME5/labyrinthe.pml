
bool map[25 * 25 * 4];
// UP -> RIGHT -> DOWN -> LEFT


proctype openCase(int x; int y; bool u; bool r; bool d; bool l)
{
	int pos = x * 625+y*25;
	map[pos] = u;
	map[pos + 1] = r;
	map[pos + 2] = d;
	map[pos + 3] = l;
}



init {
	run openCase(0,0,false, false, false, false);
	run openCase(0,1,true, truefalse, false, false);
	run openCase(0,2,false, truefalse, false, truefalse);
	run openCase(0,3,truefalse, false, truefalse, false);
	run openCase(0,4,false, false, false, false);
	run openCase(1,0,false, false, false, false);
	run openCase(1,1,false, false, false, false);
	run openCase(1,2,false, false, false, false);
	run openCase(1,3,false, false, false, false);
	run openCase(1,4,false, false, false, false);
}
