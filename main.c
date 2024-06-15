#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BUSES 10
#define MAX_SEATS 50
#define MAX_PASSENGERS 100
#define RESERVATION_FILE "reservations.txt"
#define PAYMENT_FILE "payment_proofs.txt"

// Définition des villes
const char *cities[] = {
    "Lubumbashi", "Kalemie", "Kolwezi", "Likasi", "Fungurume",
    "Moba", "Kinshasa", "Kisangani", "Kilwa", "Kongolo"
};
int main() {
    int choice;
    //srand(time(NULL)); // Initialize random number generator

    initializeBuses();
    system("CLS");

    while (1) {

        displayMenu();

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("CLS");
                reserveSeat();
                break;
            case 2:
                system("CLS");
                viewReservations();
                break;
            case 3:
                system("CLS");
                searchReservation();
                break;
            case 4:
                system("CLS");
                printf("Au revoir!\n");
                return 0;
            default:
                system("CLS");
                printf("Choix invalide. Réessayez.\n");
        }
    }

    return 0;
}



typedef struct {
    int seat_number;
    int is_reserved;
    char passenger_name[50];
} Seat;

typedef struct {
    int bus_id;
    char departure[50];
    char destination[50];
    char date[20];
    Seat seats[MAX_SEATS];
} Bus;

typedef struct {
    char name[50];
    char contact[50];
    int bus_id;
    int seat_number;
    char reservation_code[10];
} Passenger;

Bus buses[MAX_BUSES];
Passenger passengers[MAX_PASSENGERS];
int passenger_count = 0;


int phrase_existe(const char *nom_fichier, const char *phrase_recherchee) {
    char ligne[1024];
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, phrase_recherchee) != NULL) {
            fclose(fichier);
            return 1; // La phrase existe
        }
    }

    fclose(fichier);
    return 0; // La phrase n'existe pas
}


void initializeBuses() {
    int i;
    int j;
    for (i = 0; i < MAX_BUSES; i++) {
        buses[i].bus_id = i + 1;
        strcpy(buses[i].departure, cities[i % 10]);
        strcpy(buses[i].destination, cities[(i + 1) % 10]);
        sprintf(buses[i].date, "2024-06-%02d", i + 10);
        for (j = 0; j < MAX_SEATS; j++) {
            buses[i].seats[j].seat_number = j + 1;
            buses[i].seats[j].is_reserved = 0;
            strcpy(buses[i].seats[j].passenger_name, "");
        }
    }
}

void displayBuses() {
    int i;

    printf("\n\t\t\t--------------------------------------------");
    printf("\n\t|   RESERVATION DES BUS A LUBUMBASHI | \n");
    printf(" \t\t\t\t\t--------------------------------------------");
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t----------------------------");
    printf("\n\t\t\t\t\t\t\t\t\t|  BUS DISPONIBLES      |\n");
    printf("\t\t\t\t\t\t\t\t\t----------------------------\n");
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t    ---------------------------------------------------------------------------------\n");
    printf("\t\t\t\t\t    |  Bus ID | DEPART                   |  DESTINATION           | DATE            |\n");
    printf("\t\t\t\t\t    ---------------------------------------------------------------------------------\n");
    for (i = 0; i < MAX_BUSES; i++) {
        printf("\t\t\t\t\t    |   %-6d|   %-23s|   %-21s| %-16s|\n",buses[i].bus_id, buses[i].departure, buses[i].destination, buses[i].date);
        printf("\t\t\t\t\t    ---------------------------------------------------------------------------------\n");
    }
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
}

int findBus(int bus_id) {
    int i;
    for (i = 0; i < MAX_BUSES; i++) {
        if (buses[i].bus_id == bus_id) {
            return i;
        }
    }
    return -1;
}

void generateReservationCode(char *code) {
    sprintf(code, "R%04d", rand() % 10000);
}

void reserveSeat() {
    int bus_id, seat_number, bus_index;
    char name[50], reservation_code[10],contact[50];
    const char *nom_fichier = "PAYMENT,FILE";
    const char *phrase_recherchee = seat_number;

    displayBuses();
    printf("\t\t\t\t\t-----------------------------------------\n");
    printf("\t\t\t\t\t|    ENTREZ L'ID DU BUS            |\n");
    printf("\t\t\t\t\t----------------------------------------- :");
    scanf("%d", &bus_id);
    system("CLS");

    bus_index = findBus(bus_id);
    if (bus_index == -1) {
        printf("\n\n\n\n\n\n");
        printf("\t-----------------------------------------\n");
        printf("\t|     L'ID DU BUS EST NON VALIDE        |\n");
        printf("\t-----------------------------------------");
        return;
    }
    printf("\n\t\t\t\t\t\t\t\t--------------------------------------------");
    printf("\n\t\t\t\t\t\t\t\t|   RESERVATION DES BUS A LUBUMBASHI | \n");
    printf(" \t\t\t\t\t\t\t\t--------------------------------------------");
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t---------------------------");
    printf("\n\t\t\t\t\t\t\t\t\t|   ENREGISTREMENT      |\n");
    printf("\t\t\t\t\t\t\t\t\t---------------------------\n");
    printf("\n\n\n\n");

    printf("\t\t\t--------------------------------\n");
    printf("\t\t\tEntrez le numero du siege (1-%d): ", MAX_SEATS);
    scanf("%d", &seat_number);
    while(seat_number >= 50){
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t Entrez le numero du siege (1-%d): ", MAX_SEATS);
        scanf("%d", &seat_number);
        if (seat_number > MAX_SEATS || seat_number == MAX_SEATS) {
            system("CLS");
            printf("\t\t\-----------------------------------------------\n");
            printf("\t\t\|  il y a plus de place cherche un autre Bus  |\n");
            printf("\t\t\-----------------------------------------------");
            return;
        }
    }

     if (seat_number > MAX_SEATS || seat_number == MAX_SEATS) {
            system("CLS");
            printf("\n\n");
            printf("\t\t\-----------------------------------------------\n");
            printf("\t\t\|  il y a plus de place cherche un autre Bus  |\n");
            printf("\t\t\-----------------------------------------------");
            printf("\n\n\n\n");
            return;
        }
    if (buses[bus_index].seats[seat_number - 1].is_reserved) {
            while(buses[bus_index].seats[seat_number - 1].is_reserved){
                printf("\t\t\t--------------------------------\n");
                printf("\t\t\t siege est deja reserve : ");
                scanf("%d", &seat_number);
            }
    }



    printf("\t\t\t--------------------------------\n");
    printf("\t\t\t Entrez Votre Nom : ");
    scanf("%s", name);

    printf("\t\t\t--------------------------------\n");
    printf("\t\t\t Votre numero de telephone : (+243) ");
    scanf("%s", contact);

    // Simulating mobile money payment
    printf("\t\t\t---------------------------------------------------------------------------\n");
    printf("\t\t\t Selectionne un Operateur Mobile (Argent) (1: Orange Money, 2: M-Pesa, 3: Airtel Money): ");
    printf("");
    int operator;
    scanf("%d", &operator);
    while((operator < 1 || operator > 3)){
        printf("\t\t\t---------------------------------------------------------------------------\n");
        printf("\t\t\t Selectionne un Operateur Mobile (Argent) (1: Orange Money, 2: M-Pesa, 3: Airtel Money): ");
        scanf("%d", &operator);
    }
    system("CLS");
    printf("\n\n\n");
    printf(" \t\t\ Processus de  PAIEMENT ..................................\n\n\n");
    printf("\t\t\-------------------------------\n");
    printf("\t\t\|    PAIEMENT AVEC SUCCES       |\n");
    printf("\t\t\-------------------------------\n\n\n");

    // Save payment proof
    FILE *payment_file = fopen(PAYMENT_FILE, "a");
    if (payment_file) {
        fprintf(payment_file, "Passenger: %s, Bus ID: %d, Seat: %d, Operator: %d\n", name, bus_id, seat_number, operator);
        fclose(payment_file);
    } else {
        printf("Failed to save payment proof\n");
    }

    // Generate reservation code
    generateReservationCode(reservation_code);

    // Reserve the seat
    buses[bus_index].seats[seat_number - 1].is_reserved = 1;
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_name, name);

    // Save passenger info
    strcpy(passengers[passenger_count].name, name);
    strcpy(passengers[passenger_count].contact, contact);
    passengers[passenger_count].bus_id = bus_id;
    passengers[passenger_count].seat_number = seat_number;
    strcpy(passengers[passenger_count].reservation_code, reservation_code);
    passenger_count++;

    // Save reservation to file
    FILE *file = fopen(RESERVATION_FILE, "a");
    if (file) {
        fprintf(file, "Name: %s, Contact: %s, Bus ID: %d, Seat: %d, Reservation Code: %s\n", name, contact, bus_id, seat_number, reservation_code);
        fclose(file);
        printf("\t\t\-------------------------------\n");
        printf("\t\t\|    RESERVATION REUSSIE     |\n");
        printf("\t\t\-------------------------------");
    } else {
        printf("Failed to save reservation\n");
    }
}

void viewReservations() {
    FILE *file = fopen(RESERVATION_FILE, "r");
    if (!file) {
        printf("No reservations found\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

void searchReservation() {
    char search_term[50];
    printf(" \n\n\n");
    printf("\t\t\t--------------------------------\n");
    printf("\t\t\t Entrez le nom du passsager: ");

    scanf("%s", search_term);

    FILE *file = fopen(RESERVATION_FILE, "r");
    if (!file) {
        printf("pas de reservation trouver\n");
        return;
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, search_term)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("Pas de resevation trouvee %s\n", search_term);
    }

    fclose(file);
}







void displayMenu() {
                    printf("\n\t\t\t\t\t\t\t\t--------------------------------------------");
                    printf("\n\t\t\t\t\t\t\t\t|         RESERVATION DES BUS A LUBUMBASHI | \n");
                    printf(" \t\t\t\t\t\t\t\t--------------------------------------------");
                    printf("\n");
                    printf("\n");
                    printf("\t\t\t\t\t\t\t\t\t----------------------------");
                    printf("\n\t\t\t\t\t\t\t\t\t|      MENU PRINCIPAL      |\n");
                    printf("\t\t\t\t\t\t\t\t\t----------------------------\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");

                    printf("                               |    1. Reserver un billet.\n");
                    printf("                               |    2. Afficher la liste de Reservations.\n");
                    printf("                               |    3. Rechercher des reservations.\n");
                    printf("                               |    4. Quitter.\n");
                    printf("\n");
                    printf("\n");
                    printf("                               votre  Choix : ");
}
