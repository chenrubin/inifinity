#include <stdio.h>

void Bursa(int *buy, int *sell, int *profit, int *prices, int size)
{
	int buy_i = 0;
	int sell_i = 0;
	int *sell_run = prices;
	int *buy_run = prices;
	
	while (sell_run < (prices + size))
	{
		if (*sell_run > *buy_run)
		{
			if (*sell_run - *buy_run > *profit)
			{
				*profit = *sell_run - *buy_run;
				sell_i = sell_run - prices;
				buy_i = buy_run - prices;
			}
		}
		else
		{
			buy_run = sell_run;
		}
		
		++sell_run;
	}
	
	*buy = buy_i;
	*sell = sell_i;
}


int main()
{
	int prices[] = {6,12,3,5,1,4,9,2};
	int buy = 0;
	int sell = 0;
	int profit = 0;
	
	Bursa(&buy, &sell, &profit, prices, 8);
	
	printf("Profit = %d after buying in %d and selling in %d\n", profit, buy, sell);
	
 	return 0;
}
