/*
Titel                :      Wuerfel-Simulator
Autor                :
Letzte Aenderung     :      15.06.2009
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fkt(int array[],int laenge, int *wdh)
{

int i,x;

    for(i=0;i<=(*wdh-1);i++)
    {
        x=rand() % 7 +1;

            switch(x)
            {

                case 0:
                     i--;
                     continue;
                case 1:
                    array[0]++;
                    break;
                case 2:
                    array[1]++;
                    break;
                case 3:
                    array[2]++;
                    break;
                case 4:
                    array[3]++;
                    break;
                case 5:
                    array[4]++;
                    break;
                case 6:
                    array[5]++;
                    break;
                default:
                    break;
            }
    }
}

void ausgabe(int array[],int *count)
{
    printf("Relative Haeufigkeit von Augenzahl 1\t:\t%d/%d = %.4f = %.4f%%\n",array[0],*count,(float)array[0]/(*count),((float)array[0]/(*count))*100);
    printf("Relative Haeufigkeit von Augenzahl 2\t:\t%d/%d = %.4f = %.4f%%\n",array[1],*count,(float)array[1]/(*count),((float)array[1]/(*count))*100);
    printf("Relative Haeufigkeit von Augenzahl 3\t:\t%d/%d = %.4f = %.4f%%\n",array[2],*count,(float)array[2]/(*count),((float)array[2]/(*count))*100);
    printf("Relative Haeufigkeit von Augenzahl 4\t:\t%d/%d = %.4f = %.4f%%\n",array[3],*count,(float)array[3]/(*count),((float)array[3]/(*count))*100);
    printf("Relative Haeufigkeit von Augenzahl 5\t:\t%d/%d = %.4f = %.4f%%\n",array[4],*count,(float)array[4]/(*count),((float)array[4]/(*count))*100);
    printf("Relative Haeufigkeit von Augenzahl 6\t:\t%d/%d = %.4f = %.4f%%\n",array[5],*count,(float)array[5]/(*count),((float)array[5]/(*count))*100);
    printf("\n");
}

int main()
{
    srand(time(NULL));

    while(1)
    {
     unsigned int array[6]={0,0,0,0,0,0}, count;

     printf("Erwuenschte Anzahl der Wiederholungen: ");
     scanf("%d", &count);
     fflush(stdin);
     printf("\n");

     fkt(array, 6, &count);
     ausgabe(array, &count);
    }

    system("PAUSE");
    return 0;
}
