#include <stdio.h>


	int tempre = 20;
    int tempreMax;
    int tempreMin;

    int feuchtigkeit = 60;
    int feuchtigkeitMax;
    int feuchtigkeitMin;

    int gerausch = 50;
    int gerauschMax;
    int gerauschMin;

    int wasserkontakt = 5;
    int wasserkontaktMax;
    int wasserkontaktMin;
    
    
    int getTempreMax(int tempre) {
    	
    	if (tempreMax == NULL || tempre > tempreMax) {
    		tempreMax = tempre;
		}
		return tempreMax;
	}
	
	int getTempreMin(int tempre) {
    	
    	if (tempreMin == NULL || tempre < tempreMax) {
    		tempreMin = tempre;
		}
		return tempreMin;
	}
	
	int getFeuchtigkeitMax(int feuchtigkeit) {
    	
    	if (feuchtigkeitMax == NULL || feuchtigkeit > feuchtigkeitMax) {
    		feuchtigkeitMax = feuchtigkeit;
		}
		return feuchtigkeitMax;
	}
	
	int getFeuchtigkeitMin(int feuchtigkeit) {
    	
    	if (feuchtigkeitMin == NULL || feuchtigkeit < feuchtigkeitMin) {
    		feuchtigkeitMin = feuchtigkeit;
		}
		return feuchtigkeitMin;
	}
	
	int getGerauschMax(int gerausch) {
    	
    	if (gerauschMax == NULL || gerausch > gerauschMax) {
    		gerauschMax = gerausch;
		}
		return gerauschMax;
	}
	
	int getGerauschMin(int gerausch) {
    	
    	if (gerauschMin == NULL || gerausch < gerauschMin) {
    		gerauschMin = gerausch;
		}
		return gerauschMin;
	}
	
	int getWasserkontaktMax(int wasserkontakt) {
    	
    	if (wasserkontaktMax == NULL || wasserkontakt > wasserkontaktMax) {
    		wasserkontaktMax = wasserkontakt;
		}
		return wasserkontaktMax;
	}
	
	int getWasserkontaktMin(int wasserkontakt) {
    	
    	if (wasserkontaktMin == NULL || wasserkontakt < wasserkontaktMin) {
    		wasserkontaktMin = wasserkontakt;
		}
		return wasserkontaktMin;
	}


int main() {
	
	while (1) {
		printf("Temperatur: ");
		scanf("%d", &tempre);
		printf("Max: %d\n", getTempreMax(tempre));
		printf("Min: %d\n", getTempreMin(tempre));
	}


return 0;
}
