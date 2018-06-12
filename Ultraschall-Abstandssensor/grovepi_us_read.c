#include "../grovepi.h"

int main(void)
{
	int adata;
	int port = 4;

	//Exit on failure to start communications with the GrovePi
	if(init()==-1)
		exit(1);

	while(1)
	{
		adata=ultrasonicRead(port);
		if(adata > 300)
            continue;
		else
            printf("Abstand: %d cm\n",adata);
		if(adata==-1)
			printf("IO Error");

		pi_sleep(100);
	}
   	return 1;
}
