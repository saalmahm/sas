#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_RESERVATIONS 100
#define MAX_NOM 30
#define MAX_PRENOM 30
#define MAX_TELEPHONE 15
#define MAX_STATUT 20

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

struct Reservation {
    int id;
    char nom[MAX_NOM];
    char prenom[MAX_PRENOM];
    char telephone[MAX_TELEPHONE];
    int age;
    char statut[MAX_STATUT];
    char date[11]; // format "YYYY-MM-DD"
};

struct Reservation reservations[MAX_RESERVATIONS];
int count = 0;
int id_counter = 1;

const char* statuts_disponibles[] = {"validé", "annulé", "traité", "reporté"};
const int nombre_statuts = sizeof(statuts_disponibles) / sizeof(statuts_disponibles[0]);

int is_valid_date(const char *date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;

    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) return 0;
        } else {
            if (day > 28) return 0;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return 0;
    }
    return 1;
}

int get_positive_integer() {
    int value;
    while (1) {
        if (scanf("%d", &value) != 1 || value < 0) {
            printf(RED "Entrée invalide. Veuillez entrer un entier positif: " RESET);
            while (getchar() != '\n'); // vider le tampon
        } else {
            while (getchar() != '\n'); // vider le tampon
            return value;
        }
    }
}

int is_valid_phone(const char* phone) {
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

void ajouter_reservation(char* nom, char* prenom, char* telephone, int age, char* statut, char* date) {
    if (count < MAX_RESERVATIONS) {
        reservations[count].id = id_counter++;
        strcpy(reservations[count].nom, nom);
        strcpy(reservations[count].prenom, prenom);
        strcpy(reservations[count].telephone, telephone);
        reservations[count].age = age;
        strcpy(reservations[count].statut, statut);
        strcpy(reservations[count].date, date);
        count++;
        printf(GREEN "Réservation ajoutée avec succès.\n" RESET);  // Message d'ajout
    } else {
        printf(RED "Capacité maximale atteinte.\n" RESET);
    }
}


void afficher_reservations() {
    if (count == 0) {
        printf(RED "Aucune réservation disponible.\n" RESET);
        return;
    }
    printf("+----+----------------+----------------+---------------+-----+------------+------------+\n");
    printf("| ID |       Nom      |     Prénom     |   Téléphone   | Âge |   Statut   |   Date     |\n");
    printf("+----+----------------+----------------+---------------+-----+------------+------------+\n");
    for (int i = 0; i < count; i++) {
        printf("| %2d | %-14s | %-14s | %-13s | %3d | %-10s | %s |\n",
               reservations[i].id, reservations[i].nom, reservations[i].prenom,
               reservations[i].telephone, reservations[i].age, reservations[i].statut,
               reservations[i].date);
    }
    printf("+----+----------------+----------------+---------------+-----+------------+------------+\n");
}

struct Reservation* rechercher_par_reference(int id) {
    for (int i = 0; i < count; i++) {
        if (reservations[i].id == id) {
            return &reservations[i];
        }
    }
    return NULL;
}

void modifier_reservation(int id) {
    struct Reservation* res = rechercher_par_reference(id);
    if (res) {
        printf("Modifier la réservation (ID: %d)\n", res->id);
        printf("Nouveau nom (actuel: %s): ", res->nom);
        scanf(" %[^\n]%*c", res->nom);

        printf("Nouveau prénom (actuel: %s): ", res->prenom);
        scanf(" %[^\n]%*c", res->prenom);

        char new_phone[MAX_TELEPHONE];
        do {
            printf("Nouveau téléphone (actuel: %s): ", res->telephone);
            scanf(" %[^\n]%*c", new_phone);
        } while (!is_valid_phone(new_phone));
        strcpy(res->telephone, new_phone);

        printf("Nouvel âge (actuel: %d): ", res->age);
        res->age = get_positive_integer();

        printf("Choisissez un statut:\n");
        for (int i = 0; i < nombre_statuts; i++) {
            printf("%d. %s\n", i + 1, statuts_disponibles[i]);
        }
        int choix_statut;
        do {
            printf("Entrez le numéro du statut choisi: ");
            choix_statut = get_positive_integer();
        } while (choix_statut < 1 || choix_statut > nombre_statuts);
        strcpy(res->statut, statuts_disponibles[choix_statut - 1]);

        printf("Nouvelle date (actuel: %s): ", res->date);
        char new_date[11];
        while (1) {
            scanf(" %10s", new_date);
            if (is_valid_date(new_date)) {
                strcpy(res->date, new_date);
                break;
            } else {
                printf(RED "Date invalide. Veuillez entrer une date valide (YYYY-MM-DD): " RESET);
            }
        }
    } else {
        printf(RED "Réservation non trouvée.\n" RESET);
    }
}

void supprimer_reservation(int id) {
    for (int i = 0; i < count; i++) {
        if (reservations[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                reservations[j] = reservations[j + 1];
            }
            count--;
            printf(GREEN "Réservation supprimée avec succès.\n" RESET);
            return;
        }
    }
    printf(RED "Réservation non trouvée.\n" RESET);
}

void afficher_statistiques(int choix) {
    int total_age = 0;
    int age_count[3] = {0}; // 0-18, 19-35, 36+
    int status_count[nombre_statuts];
    memset(status_count, 0, sizeof(status_count));

    for (int i = 0; i < count; i++) {
        total_age += reservations[i].age;
        if (reservations[i].age <= 18) age_count[0]++;
        else if (reservations[i].age <= 35) age_count[1]++;
        else age_count[2]++;

        for (int j = 0; j < nombre_statuts; j++) {
            if (strcmp(reservations[i].statut, statuts_disponibles[j]) == 0) {
                status_count[j]++;
            }
        }
    }

    switch (choix) {
        case 1:
            printf("Moyenne d'âge: %.2f\n", (count > 0 ? (float)total_age / count : 0));
            break;
        case 2:
            printf("Nombre de patients par tranche d'âge:\n");
            printf("0-18 ans: %d\n", age_count[0]);
            printf("19-35 ans: %d\n", age_count[1]);
            printf("36 ans et plus: %d\n", age_count[2]);
            break;
        case 3:
            printf("Nombre de réservations par statut:\n");
            for (int j = 0; j < nombre_statuts; j++) {
                printf("%s: %d\n", statuts_disponibles[j], status_count[j]);
            }
            break;
        default:
            printf(RED "Choix invalide.\n" RESET);
    }
}

void insertion_defaut() {
    ajouter_reservation("El Amrani", "Sofia", "0612345678", 25, "validé", "2024-10-01");
    ajouter_reservation("Bennani", "Youssef", "0687654321", 30, "annulé", "2024-10-02");
    ajouter_reservation("Benjelloun", "Salma", "0678901234", 40, "traité", "2024-10-03");
    ajouter_reservation("Mouhib", "Rachid", "0667890123", 50, "reporté", "2024-10-04");
    ajouter_reservation("Zouiten", "Nadia", "0656789012", 15, "validé", "2024-10-05");
    ajouter_reservation("El Ouardi", "Hamid", "0645678901", 20, "annulé", "2024-10-06");
    ajouter_reservation("Cherkaoui", "Meryem", "0634567890", 22, "traité", "2024-10-07");
    ajouter_reservation("Fassi", "Omar", "0623456789", 35, "reporté", "2024-10-08");
    ajouter_reservation("Ait Benhaddou", "Khadija", "0612340987", 28, "validé", "2024-10-09");
    ajouter_reservation("Brahimi", "Zineb", "0687654312", 33, "annulé", "2024-10-10");
}


int main() {
    insertion_defaut();
    printf(GREEN "Déjà %d réservations stockées.\n" RESET, count);
    int choix;
    do {
        printf("\nMenu :\n");
        printf(YELLOW "1. Ajouter une réservation\n" RESET);
        printf(YELLOW "2. Modifier une réservation\n" RESET);
        printf(YELLOW "3. Supprimer une réservation\n" RESET);
        printf(YELLOW "4. Afficher les réservations\n" RESET);
        printf(YELLOW "5. Statistiques\n" RESET);
        printf(YELLOW "6. Quitter\n" RESET);
        printf("Choisissez une option: ");
        choix = get_positive_integer();

        switch (choix) {
            case 1: {
                char nom[MAX_NOM], prenom[MAX_PRENOM], telephone[MAX_TELEPHONE], statut[MAX_STATUT], date[11];
                printf("Nom: "); scanf(" %[^\n]%*c", nom);
                printf("Prénom: "); scanf(" %[^\n]%*c", prenom);

                do {
                    printf("Téléphone (10 chiffres): ");
                    scanf(" %[^\n]%*c", telephone);
                } while (!is_valid_phone(telephone));

                printf("Âge: ");
                int age = get_positive_integer();

                printf("Choisissez un statut:\n");
                for (int i = 0; i < nombre_statuts; i++) {
                    printf("%d. %s\n", i + 1, statuts_disponibles[i]);
                }
                int choix_statut;
                do {
                    printf("Entrez le numéro du statut choisi: ");
                    choix_statut = get_positive_integer();
                } while (choix_statut < 1 || choix_statut > nombre_statuts);
                strcpy(statut, statuts_disponibles[choix_statut - 1]);

                printf("Date (YYYY-MM-DD): ");
                while (1) {
                    scanf(" %10s", date);
                    if (is_valid_date(date)) {
                        break;
                    } else {
                        printf(RED "Date invalide. Veuillez entrer une date valide (YYYY-MM-DD): " RESET);
                    }
                }
                ajouter_reservation(nom, prenom, telephone, age, statut, date);
                break;
            }
            case 2: {
                int id;
                printf("Entrer la référence unique (ID): ");
                id = get_positive_integer();
                modifier_reservation(id);
                break;
            }
            case 3: {
                int id;
                printf("Entrer la référence unique (ID) à supprimer: ");
                id = get_positive_integer();
                supprimer_reservation(id);
                break;
            }
            case 4:
                afficher_reservations();
                break;
            case 5: {
                printf("Choisissez le type de statistique à afficher:\n");
                printf("1. Moyenne d'âge\n");
                printf("2. Nombre de patients par tranche d'âge\n");
                printf("3. Nombre total de réservations par statut\n");
                int choix_statistique = get_positive_integer();
                afficher_statistiques(choix_statistique);
                break;
            }
            case 6:
                printf(GREEN "Au revoir!\n" RESET);
                break;
            default:
                printf(RED "Option invalide.\n" RESET);
        }
    } while (choix != 6);
    return 0;
}
