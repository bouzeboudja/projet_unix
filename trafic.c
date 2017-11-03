#include "polling.h"

/*#-----------------------------------------------AUTOMATE_TRAFIC----------------------------------------------------#*/

/*Dans cet automate on déclenche une boucle infinie qui émet des signaux chaque 3 seconde à la station secondaire associée au pid passé en paramètre*/

void trafic(int pid_sti,int delai_min_requete, int delai_max_requete){
	srand(time(NULL));
	while(FIN==0){
		sleep( (rand() % (delai_max_requete - delai_min_requete + 1)) + delai_min_requete );
		kill(pid_sti,DATA_REQ_TX);
	}
}

int main(int argc, char **argv){

  //tester le nombre d'arguments passer en parametre
  if(argc != 5){
        erreur("le nombre d'arguments de trafic n'est pas correcte \n");
  }
  int i = atoi(argv[1]); //numero de la satation secondaire associée
  int pid_Sti = atoi(argv[2]); //pid de la station secondaire associée
  int delai_min_requete = atoi(argv[3]);
  int delai_max_requete = atoi(argv[4]);
  trafic(pid_Sti, delai_min_requete, delai_max_requete);
  //affichage
	/*
  printf("\tje suis trafic\n \t\t --n° station %d \n\t\t --pid de ma station %d \n\t\t --le délai minimum %d\n\t\t --délai max requete %d \n",
  atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
  printf("\t#----------------------------------#\n");
	*/
  sleep(2);
  exit(EXIT_SUCCESS);
}
