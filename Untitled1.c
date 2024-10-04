#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define GREEN "\033[1;32m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define ORANGE "\033[1;33m"
#define RESET "\033[0m"

#define resMax 100

struct Reservation {
    int id;
    char nom[20];
    char prenom[20];
    char telephone[15];
    int age;
    char statut[20];
    char date[12];
};

struct Reservation resTable[resMax];
int cpt = 0;
int idCounter = 1;

const char* disponibleStatu[] = {"valide", "annulée", "traitée", "reportée"};
const int nombre_statuts = sizeof(disponibleStatu) / sizeof(disponibleStatu[0]);

int chaeckDate(const char *date);
int checkInt();
int valideTele(const char* phone);
int checkUniquePhone(const char* phone);
void ajouter_reservation(char* nom, char* prenom, char* telephone, int age, char* statut, char* date);
void afficher_resTable();
struct Reservation* rechercher_par_reference(int id);
void modifier_reservation(int id);
void supprimer_reservation(int id);
void afficher_statistiques(int choix);
void insertion_defaut();
void afficher_details_reservation(int id);

void trier_reservations_par_nom(int ordre);
void trier_reservations_par_statut(int ordre);
void trier_reservations_par_date(int ordre);
void trier_reservations();


int chaeckDate(const char *date) {
    int a, m, j;
    if (sscanf(date, "%d-%d-%d", &a, &m, &j) != 3) return 0;
    if (m < 1 || m > 12) return 0;
    if (j < 1 || j > 31) return 0;

    if (m == 2) {
        if (j > 28) return 0;
    } else if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (j > 30) return 0;
    }
    return 1;
}


int checkInt() {
    int var;
    while (1) {
        if (scanf("%d", &var) != 1 || var < 0) {
            printf(MAGENTA "error: entier invalid: " RESET);
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            return var;
        }
    }
}


int valideTele(const char* phone) {
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}


int checkUniquePhone(const char* phone) {
    for (int i = 0; i < cpt; i++) {
        if (strcmp(resTable[i].telephone, phone) == 0) {
            return 0;
        }
    }
    return 1;
}


void ajouter_reservation(char* nom, char* prenom, char* telephone, int age, char* statut, char* date) {
    if (!checkUniquePhone(telephone)) {
        printf(MAGENTA "ce numéro %s est deja existe.\n" RESET, telephone);
        return;
    }

    if (cpt < resMax) {
        resTable[cpt].id = idCounter++;
        strcpy(resTable[cpt].nom, nom);
        strcpy(resTable[cpt].prenom, prenom);
        strcpy(resTable[cpt].telephone, telephone);
        resTable[cpt].age = age;
        strcpy(resTable[cpt].statut, statut);
        strcpy(resTable[cpt].date, date);
        cpt++;
    } else {
        printf(MAGENTA "Capacité plein.\n" RESET);
    }
}


void afficher_resTable() {
    if (cpt == 0) {
        printf(MAGENTA "aucune reservation pour ce moment.\n" RESET);
        return;
    }
    printf(CYAN "+----+----------------------+----------------------+---------------+-----+--------------+------------+\n" RESET);
    printf(CYAN "| id |         nom          |        prenom        |   telephone   | age |    statut    |    date    |\n" RESET);
    printf(CYAN "+----+----------------------+----------------------+---------------+-----+--------------+------------+\n" RESET);
    for (int i = 0; i < cpt; i++) {
        printf("| %2d | %-20s | %-20s | %-13s | %3d | %-12s | %s |\n",
               resTable[i].id, resTable[i].nom, resTable[i].prenom,
               resTable[i].telephone, resTable[i].age, resTable[i].statut,
               resTable[i].date);
    }
    printf(CYAN "+----+----------------------+----------------------+---------------+-----+--------------+------------+\n" RESET);
}


struct Reservation* rechercher_par_reference(int id) {
    for (int i = 0; i < cpt; i++) {
        if (resTable[i].id == id) {
            return &resTable[i];
        }
    }
    return NULL;
}


void modifier_reservation(int id) {
    struct Reservation* res = rechercher_par_reference(id);
    if (res) {
        printf(ORANGE "vous pouvez modifier cette reservation (id: %d)\n" RESET, res->id);
        printf("entrer le nouveau nom (actuel: %s): ", res->nom);
        scanf(" %[^\n]%*c", res->nom);

        printf("entrer le nouveau prenom (actuel: %s): ", res->prenom);
        scanf(" %[^\n]%*c", res->prenom);

        char new_phone[15];
        do {
            printf("entrer le nouveau numero de telephone (actuel: %s): ", res->telephone);
            scanf(" %[^\n]%*c", new_phone);
        } while (!valideTele(new_phone));
        strcpy(res->telephone, new_phone);

        printf("entrer le nouvel age (actuel: %d): ", res->age);
        res->age = checkInt();

        printf("veuillez choisi un statut:\n");
        for (int i = 0; i < nombre_statuts; i++) {
            printf("%d. %s\n", i + 1, disponibleStatu[i]);
        }
        int choix_statut;
        do {
            printf("entrez le numero du statut que tu veut choisi: ");
            choix_statut = checkInt();
        } while (choix_statut < 1 || choix_statut > nombre_statuts);
        strcpy(res->statut, disponibleStatu[choix_statut - 1]);

        printf("entrer la nouvelle date (actuel: %s): ", res->date);
        char new_date[11];
        while (1) {
            scanf(" %10s", new_date);
            if (chaeckDate(new_date)) {
                strcpy(res->date, new_date);
                break;
            } else {
                printf(MAGENTA "Date invalide. Veuillez entrer une date valide (YYYY-MM-DD): " RESET);
            }
        }
    } else {
        printf(MAGENTA "Réservation non trouvée.\n" RESET);
    }
}


void supprimer_reservation(int id) {
    for (int i = 0; i < cpt; i++) {
        if (resTable[i].id == id) {
            for (int j = i; j < cpt - 1; j++) {
                resTable[j] = resTable[j + 1];
            }
            cpt--;
            printf(GREEN "Réservation supprimée avec succès.\n" RESET);
            return;
        }
    }
    printf(MAGENTA "Réservation non trouvée.\n" RESET);
}


void afficher_statistiques(int choix) {
    int total_age = 0;
    int age_cpt[3] = {0}; // 0-18, 19-35, 36+
    int status_cpt[nombre_statuts];
    memset(status_cpt, 0, sizeof(status_cpt));

    for (int i = 0; i < cpt; i++) {
        total_age += resTable[i].age;
        if (resTable[i].age <= 18) age_cpt[0]++;
        else if (resTable[i].age <= 35) age_cpt[1]++;
        else age_cpt[2]++;

        for (int j = 0; j < nombre_statuts; j++) {
            if (strcmp(resTable[i].statut, disponibleStatu[j]) == 0) {
                status_cpt[j]++;
                break;
            }
        }
    }

    printf(ORANGE "Statistiques:\n" RESET);
    printf("Âge moyen: %.2f\n", cpt ? (double)total_age / cpt : 0);
    printf("Répartition par âge:\n");
    printf("0-18: %d\n19-35: %d\n36+: %d\n", age_cpt[0], age_cpt[1], age_cpt[2]);

    for (int j = 0; j < nombre_statuts; j++) {
        printf("Statut '%s': %d\n", disponibleStatu[j], status_cpt[j]);
    }
}


void insertion_defaut() {
    ajouter_reservation("salma", "hamdi", "0617815445", 20, "validé", "2024-10-01");
    ajouter_reservation("allaoui", "khalid", "0623547890", 50, "annulé", "2024-10-02");
    ajouter_reservation("magha", "rikia", "0634567890", 25, "traité", "2024-10-03");
    ajouter_reservation("montadir", "hiba", "0667092378", 40, "reporté", "2024-10-04");
    ajouter_reservation("hamili", "rose", "0678003355", 16, "validé", "2024-10-05");
    ajouter_reservation("el ghizlani", "soufia", "0706235688", 35, "annulé", "2024-10-06");
    ajouter_reservation("hamzaoui", "amin", "0611220098", 25, "traité", "2024-10-07");
    ajouter_reservation("alhafidi", "kenza", "0667992038", 40, "reporté", "2024-10-08");
    ajouter_reservation("El mouradi", "salima", "0714121617", 25, "validé", "2024-10-09");
    ajouter_reservation("ait hamou", "nouhaila", "0678999125", 32, "annulé", "2024-10-10");

}


void afficher_details_reservation(int id) {
    struct Reservation* res = rechercher_par_reference(id);
    if (res) {
        printf(ORANGE "Détails de la réservation (ID: %d)\n" RESET, res->id);
        printf("Nom: %s\n", res->nom);
        printf("Prénom: %s\n", res->prenom);
        printf("Téléphone: %s\n", res->telephone);
        printf("Âge: %d\n", res->age);
        printf("Statut: %s\n", res->statut);
        printf("Date: %s\n", res->date);
    } else {
        printf(MAGENTA "Réservation non trouvée.\n" RESET);
    }
}


void trier_reservations_par_nom(int ordre) {
    for (int i = 0; i < cpt - 1; i++) {
        for (int j = i + 1; j < cpt; j++) {
            int condition = (ordre == 1) ? strcmp(resTable[i].nom, resTable[j].nom) > 0 : strcmp(resTable[i].nom, resTable[j].nom) < 0;
            if (condition) {
                struct Reservation temp = resTable[i];
                resTable[i] = resTable[j];
                resTable[j] = temp;
            }
        }
    }
}

void trier_reservations_par_statut(int ordre) {
    for (int i = 0; i < cpt - 1; i++) {
        for (int j = i + 1; j < cpt; j++) {
            int condition = (ordre == 1) ? strcmp(resTable[i].statut, resTable[j].statut) > 0 : strcmp(resTable[i].statut, resTable[j].statut) < 0;
            if (condition) {
                struct Reservation temp = resTable[i];
                resTable[i] = resTable[j];
                resTable[j] = temp;
            }
        }
    }
}

void trier_reservations_par_date(int ordre) {
    for (int i = 0; i < cpt - 1; i++) {
        for (int j = i + 1; j < cpt; j++) {
            int condition = (ordre == 1) ? strcmp(resTable[i].date, resTable[j].date) > 0 : strcmp(resTable[i].date, resTable[j].date) < 0;
            if (condition) {
                struct Reservation temp = resTable[i];
                resTable[i] = resTable[j];
                resTable[j] = temp;
            }
        }
    }
}


void trier_reservations() {
    printf(ORANGE "Choisissez un critère de tri:\n" RESET);
    printf("1. Nom\n");
    printf("2. Statut\n");
    printf("3. Date\n");
    printf("Votre choix: ");
    int choix = checkInt();
    printf("Choisissez un ordre:\n");
    printf("1. Croissant\n");
    printf("2. Décroissant\n");
    printf("Votre choix: ");
    int ordre = checkInt();
    switch (choix) {
        case 1: trier_reservations_par_nom(ordre); break;
        case 2: trier_reservations_par_statut(ordre); break;
        case 3: trier_reservations_par_date(ordre); break;
        default: printf(MAGENTA "Choix invalide.\n" RESET); break;
    }
}

int main() {
    int choix;
    insertion_defaut();
    while (1) {
        printf(GREEN "\nMenu:\n" RESET);
        printf("1. Ajouter une réservation\n");
        printf("2. Afficher les réservations\n");
        printf("3. Modifier une réservation\n");
        printf("4. Supprimer une réservation\n");
        printf("5. Afficher des statistiques\n");
        printf("6. Trier les réservations\n");
        printf("7. Afficher les détails d'une réservation\n");
        printf("8. Quitter\n");
        printf("Votre choix: ");
        choix = checkInt();

        switch (choix) {
            case 1: {
                char nom[30], prenom[30], telephone[15], statut[20], date[11];
                printf("Nom: ");
                scanf(" %[^\n]%*c", nom);
                printf("Prénom: ");
                scanf(" %[^\n]%*c", prenom);
                do {
                    printf("Téléphone: ");
                    scanf(" %[^\n]%*c", telephone);
                } while (!valideTele(telephone));
                printf("Âge: ");
                int age = checkInt();
                printf("Statut: ");
                scanf(" %[^\n]%*c", statut);
                do {
                    printf("Date (YYYY-MM-DD): ");
                    scanf(" %10s", date);
                } while (!chaeckDate(date));
                ajouter_reservation(nom, prenom, telephone, age, statut, date);
                break;
            }
            case 2:
                afficher_resTable();
                break;
            case 3: {
                printf("Entrez l'ID de la réservation à modifier: ");
                int id = checkInt();
                modifier_reservation(id);
                break;
            }
            case 4: {
                printf("Entrez l'ID de la réservation à supprimer: ");
                int id = checkInt();
                supprimer_reservation(id);
                break;
            }
            case 5: {
                printf("Choisissez une option:\n");
                printf("1. Statistiques par âge\n");
                printf("2. Statistiques par statut\n");
                int option = checkInt();
                afficher_statistiques(option);
                break;
            }
            case 6:
                trier_reservations();
                break;
            case 7: {
                printf("Entrez l'ID de la réservation à afficher: ");
                int id = checkInt();
                afficher_details_reservation(id);
                break;
            }
            case 8:
                printf(GREEN "Au revoir!\n" RESET);
                exit(0);
            default:
                printf(MAGENTA "Choix invalide. Réessayez.\n" RESET);
        }
    }
    return 0;
}
