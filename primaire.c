#include "polling.h"

static int nb_req_rx = 0;
int  state ;
char * string_state;

sigset_t mask_initial, mask_signaux, sigaux_pendants;

void mask_signal_Data_Rx(){
    sigemptyset(&mask_signaux);
    sigaddset(&mask_signaux, DATA_RX);
    sigprocmask(SIG_SETMASK, &mask_signaux, &mask_initial);
}

void  attente_req(int delai_polling){
    mask_signal_Data_Rx();
    sleep(delai_polling);
    sigemptyset(&sigaux_pendants);
    sigpending(&sigaux_pendants);
    if (sigismember(&sigaux_pendants, DATA_RX) == 1){
        nb_req_rx=1;
    }
    else{
        nb_req_rx=0;
    }
    return;
}

void  primaire(int nb_polling, int delai_polling, int n, int *st){
    state = W_REQ;
    string_state = string_w_req;
    int i, j, k;
    for(i=0; i < nb_polling; i++){
        for (j=0; j < n; j++) {
            do{
                switch (state){
                        case W_REQ:
                        printf("Prim %s st%d %d s \n",string_state,j,delai_polling);
                        kill(st[j] ,POLL_TX);
                        attente_req(delai_polling);
                        if(nb_req_rx==1){
                            printf("Prim %s St%d Data_Rx\n",string_state, j);
                            kill(st[j], ACK_TX);
                            state = BC_DATA;
                            string_state = string_bc_data;
                        }else{
                            printf("Prim %s St%d No_Data\n",string_state, j);
                        }
                        break;
                        case BC_DATA:
                        signal(DATA_RX,SIG_IGN);
                        printf("Prim %s St%d\n",string_state, j);
                        for(k=0; k < n;k++){
                            if(k != j){
                                kill(st[k],DATA_TX);
                            }
                        }
                        state = W_REQ;
                        string_state = string_w_req;
                        break;
                    }
                }while(state==BC_DATA);
            }
        }
    return ;
}

int main(int argc, char **argv){
    //tester le nombre d'arguments passer en parametre
    if(argc < 4){
        erreur("le nombre d'arguments de primaire n'est pas correcte \n");
    }
    int i;
    int nb_polling = atoi(argv[1]);
    int delai_polling = atoi(argv[2]);
    int n = atoi(argv[3]);
    int st[n];
    for(i=0; i < n; i++){
        st[i] = atoi(argv[i+4]);
    }

    primaire(nb_polling, delai_polling, n, st);
    sleep(5);
    for (i = 0; i < n; i++) {
        //printf("station numéro %d pid = %d\n",i,st[i]);
        kill(st[i], SIGTERM);
    }
      //affichage
      /*
      printf("\tje suis primaire  \n");
      printf("\t\tle nombre polling est %d \n",atoi(argv[1]));
      printf("\t\tle delai polling est %d \n", atoi(argv[2]));
      printf("\t\tle nombre de stations secondaire est %d \n", atoi(argv[3]));

      printf("\t\tle pid de mes stations secondaire est: \n\t\t");
      for(i=0;i < atoi(argv[3]);i++){
    	   printf("%d)%d\t",i,atoi(argv[i+4]));
      }
      printf("\n");
      printf("\t#----------------------------------#\n");
      */
    //sleep(2);
    exit(EXIT_SUCCESS);
}
