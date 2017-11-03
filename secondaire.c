#include "polling.h"

//déclaration des variables globales
static char *string_state;
static int state = IDLE;
static int nb_data_req_rx = 0;
static int numero_station;

struct sigaction action_DATA_REQ_RX;
struct sigaction action_POLL_RX;
struct sigaction action_ACK_RX;
struct sigaction action_SD_DATA;
struct sigaction action_DATA_RX;
struct sigaction action_KILL;


sigset_t mask_initial, mask_signaux;

void mask_signal_Data_Rx(){
    sigemptyset(&mask_signaux);
    sigemptyset(&mask_initial);
    sigaddset(&mask_signaux, DATA_RX);
    sigprocmask(SIG_SETMASK, &mask_signaux, &mask_initial);
}

void mask_signaux_Initial(){
    sigprocmask(SIG_SETMASK, &mask_initial, NULL);
}


void tabulation(int n){
    for(int i=0; i<= ((3*n)+2);i++){
        printf("\t");
    }
}

/*------------------------------------------------------------------------------*/
//gestion du signal DATA_REQ_RX

void gestionnaire_recepetion_requete(int singum){
    nb_data_req_rx++;
    tabulation(numero_station);
    printf("St%d %s Data_Req_Rx %d\n", numero_station, string_state, nb_data_req_rx);
    if(state == IDLE){
        state = W_POLL;
        sigaction(POLL_RX,&action_POLL_RX,NULL);
        string_state = string_w_poll;
    }
    return;
}//sig_set set_IDLE

/*------------------------------------------------------------------------------*/
//gestion du signal POLL_RX

void gestionnaire_recepetion_invitation(int signum){
    tabulation(numero_station);
    printf("St%d %s Poll_Rx\n",numero_station, string_state);
    state = SD_DATA;
    string_state = string_sd_data;
     mask_signal_Data_Rx();
  return;
}


/*------------------------------------------------------------------------------*/
//gestion du signal DATA_RX

void gestionnaire_recepetion_Data_Rx(int signum){
    tabulation(numero_station);
    printf("St%d %s Data_Rx\n",numero_station, string_state);
    return;
}

/*------------------------------------------------------------------------------*/
// gestion du l'acquitement

void gestionnaire_reception_Ack_Rx(int signum){
    tabulation(numero_station);
    printf("St%d %s Ack_Rx %d\n",numero_station, string_state, nb_data_req_rx);
    nb_data_req_rx--;
    string_state = string_sd_data;
    if(nb_data_req_rx == 0){
        state = IDLE;
        string_state = string_idle;
    }else{
        state = W_POLL;
        sigaction(POLL_RX,&action_POLL_RX,NULL);
        string_state = string_w_poll;
    }
    mask_signaux_Initial();
    return;
}


/*---------------------AUTOMATE-SECONDAIRE-------------------------*/

void secondaire(int numero_station, int pid_primaire){
    string_state = string_idle;
    sigaction(DATA_REQ_RX,&action_DATA_REQ_RX,NULL);
    //signal(DATA_REQ_RX, gestionnaire_recepetion_requete);
    sigaction(DATA_RX,&action_DATA_RX,NULL);
    //signal(DATA_RX, gestionnaire_recepetion_Data_Rx);
    for EVER{
    switch (state){
        case IDLE:
            signal(POLL_RX,SIG_IGN);
            tabulation(numero_station);
            printf("St%d %s\n",numero_station, string_idle);
            while (nb_data_req_rx == 0)
                pause();
            break;
        case W_POLL:
            //signal(POLL_RX,gestionnaire_recepetion_invitation);
            tabulation(numero_station);
            printf("St%d %s Attente\n",numero_station, string_state);
            pause();
            break;
        case W_ACK:
            //signal(ACK_RX, gestionnaire_reception_Ack_Rx);
            tabulation(numero_station);
            printf("St%d %s\n", numero_station, string_w_ack);
            pause();
            break;
        case SD_DATA:
            tabulation(numero_station);
            printf("St%d %s \n",numero_station, string_sd_data);
            //signal(DATA_RX, SIG_IGN);
            kill(pid_primaire, DATA_TX);
            state = W_ACK;
            sigaction(ACK_RX,&action_ACK_RX,NULL);
            string_state = string_w_ack;
            break;
        default: erreur("erreur au niveau de switch de la satation secondaire !");
        }
    }
    return;
}
/*----------------------------------------#PROGRAMMME PRINCIPAL SECONDAIRE#----------------------------------------*/

int main(int argc, char **argv){
    string_state = string_idle;
    //tester le nombre d'arguments passer en parametre
    if(argc < 2){
         printf("  ******* argc SECONDAIRE = %d \n \n ",argc);
         perror("le nombre d'arguments de secondaire n'est pas correcte \n");
    exit(EXIT_FAILURE);
    }
    //récupération des paramètres
    int pid_primaire = atoi(argv[2]);
    numero_station = atoi(argv[1]);
    //instatlation du gestionnaire pour chaque etat
    action_DATA_REQ_RX.sa_handler = gestionnaire_recepetion_requete;
    action_POLL_RX.sa_handler = gestionnaire_recepetion_invitation;
    action_ACK_RX.sa_handler = gestionnaire_reception_Ack_Rx;
    action_DATA_RX.sa_handler= gestionnaire_recepetion_Data_Rx;
    //action_SD_DATA.sa_handler = ;

    secondaire(numero_station, pid_primaire);

    //affichage
    /*
    printf("\tje suis secondaire \n \t\t --mon numéro de station est: %d \n \t\t --le pid de ma station primaire est: %d \n",atoi(argv[1]),atoi(argv[2]));
    printf("\t#----------------------------------#\n");
    */
    exit(EXIT_SUCCESS);
    }
