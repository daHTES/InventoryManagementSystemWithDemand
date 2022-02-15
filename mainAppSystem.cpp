#include "InterfaceMainAppSystem.h"


int main() 
{
	que = fopen("test", "wt");
	soJournal = fopen("sojount", "wt");

	srand((unsigned)time(0));
	SuperMarket s(74);
	for (total = 0L; total < 8000; total++) 
	{
		s.run();
	}
	fclose(que);
	fclose(soJournal);

	que = fopen("que", "wt");
	soJournal = fopen("soso", "wt");
	entered = 0L;
	rejected = 0L;
	satisfied = 0L;
	numOrders = 0;
	sojAve = 0;
	queAve = 0;

	s.queueExtraLength = s.getLength();
	for (total = 0L; total < 1000; total++) 
	{
		s.run();
	}

	fclose(soJournal);
	fclose(que);
	// Выводы

	return 0;
}
