#include "polling.h"


#define MAX 100

int main(int argc, char **argv){

  /* déclaration des variables */
  int primaire,trafic,i,j;
  int n=atoi(argv[4]);      //récuperation de n "nombre de stations secondaires"
  int stat[n];              //tableau de stations secondaire
  char i_string[10];        //des tampos pour récuperer les pids des stations secondaire en string
  char pid_string[10];
  char pidp_string[10];

  primaire=fork();          //création du processus primaire
  if (primaire==-1){
    //tester la création de primaire
    perror("erreur fork p");
    exit(EXIT_FAILURE);
  }
  if(primaire==0){
      //création de secondaire
	    for(i=0;i<n;i++){
	      sprintf(i_string,"%d",i);
	      stat[i]=fork();
	      if (stat[i] == -1){
        	 perror("erreur fork p");
        	  exit(EXIT_FAILURE);
	      }//fin if

		    if (stat[i]==0){
            //création de trafic
       			trafic=fork();
        		if (trafic == -1){
                //tester la création de trafic
          			perror("erreur fork p");
          			exit(EXIT_FAILURE);
        		}//fin if

        		if(trafic==0){
          			//trafic
				        int pid=getppid();  //récupération des pids de la station trafic
				        sprintf(pid_string,"%d",pid);//récuperer les pids des stations trafic en string
                //execution du programme trafic
				        execl("trafic.exe","trafic.exe",i_string,pid_string,argv[7],argv[8],NULL);
	        	}
            else{
	          		//fun_secondaire();
				        stat[i]=getpid();   //récupération des pids des stations secondaire
				        int pidp=getppid(); //récupération du pids de la station primaire
				        sprintf(pidp_string,"%d",pidp);//récuperer des pids des stations secondaire en string
				        //wait();            //attente la fin des processus trafic
                //execution du programme secondaire
				        execl("secondaire.exe","secondaire.exe",i_string,pidp_string,NULL);
                        printf("execl a foiré \n");//a suprimer c'est pour test
	        	}
	      }//fin if
        //wait();//attente la fin des processus secondaire
    	}//fin for

	char *tab_param[n+5];
	char buff[10],buff1[10],buff2[10],buff3[10],buff4[10];

  //récuparation des paramètres de poll_config dans le tableau tab_param
	tab_param[0] = argv[1];
	tab_param[1] = argv[5];
	tab_param[2] = argv[6];
	tab_param[3] = argv[4];

  //stockage des parametres en chaine de caracteres
  sprintf(buff,"%d",stat[0]);
	tab_param[4] = buff;
	sprintf(buff1,"%d",stat[1]);
	tab_param[5] = buff1;
	sprintf(buff2,"%d",stat[2]);
	tab_param[6] = buff2;
	sprintf(buff3,"%d",stat[3]);
	tab_param[7] = buff3;
	sprintf(buff4,"%d",stat[4]);
	tab_param[8] = buff4;


  //mettre le reste du tableau a null
  tab_param[4+n]=NULL;

  //éxecution de primaire avec les arguments passe comme tableau
  execv("primaire.exe",tab_param);

  }else{
      //attente la fin du processus primaire
  	  //wait();
      wait(NULL);
      sleep(5);
      /*for (i = 0; i < n; i++) {
          printf("station numéro %d pid = %d\n",i,stat[i]);
         // kill(stat[i], SIGKILL);
     }*/ // a suprimmer
      //exit(EXIT_SUCCESS);
      /*
      //affichage de poll_config
      printf("\tje suis poll_config %d \n",getpid());
      printf("\t\t # ma station primaire est:   %d\n",primaire);
      printf("\t\t # le nombre de stations est: %d\n",atoi(argv[4]));
      printf("\t\t # le nombre de polling est:  %d\n",atoi(argv[5]));
      printf("\t\t # le délai polling est:      %d\n",atoi(argv[6]));
      printf("\t\t # le délai min requete est:  %d\n",atoi(argv[7]));
      printf("\t\t # le délai max requete est:  %d\n",atoi(argv[8]));
      */
  }
  exit(EXIT_SUCCESS);


}//fin main
