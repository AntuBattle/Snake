#include <stdio.h>
#include <stdlib.h>


int partition(int array[], int min, int max);
void quickSort(int array[], int min, int max) 
{
	if(min<max)
	{
		int pi=partition(array, min, max);
		quickSort(array, min, pi-1);
		quickSort(array, pi+1, max);
	}
}

int partition(int array[], int min, int max)
{
	int i=min-1, change; //Indica l'attuale posizione del pivot
	int j, pivot=array[max];
	for(j=min; j<max; j++)
	{
		if(array[j]<pivot)
		{
			i++;
			change=array[i];
			array[i]=array[j];
			array[j]=change;
		}
	}
	change=array[i+1];
	array[i+1]=array[max];
	array[max]=change;
	return (i+1);
}

struct ciao 
{
	int cc;
	char a;
};
int main()
{
struct ciao* array[10];
struct ciao* saluto;
struct ciao ciao;
saluto=&ciao;
saluto->cc = 3;
array[2]=saluto;
printf("%d", (array[2])->cc);
return 0;
	
}
