#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

#define resMax 100

struct Reservation {
    int id;
    char nom[30];
    char prenom[30];
    char telephone[15];
    int age;
    char statut[20];
    char date[11];
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

// Vérifie la validité de la date
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

// Vérifie si l'entrée est un entier positif
int checkInt() {
    int var;
    while (1) {
        if (scanf("%d", &var) != 1 || var < 0) {
            printf(RED "Entrée invalide. Veuillez entrer un entier positif: " RESET);
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            return var;
        }
    }
}

// Vérifie la validité du numéro de téléphone
int valideTele(const char* phone) {
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Vérifie si le numéro de téléphone est unique
int checkUniquePhone(const char* phone) {
    for (int i = 0; i < cpt; i++) {
        if (strcmp(resTable[i].telephone, phone) == 0) {
            return 0; // Numéro de téléphone déjà utilisé
        }
    }
    return 1; // Numéro de téléphone unique
}

// Ajoute une nouvelle réservation
void ajouter_reservation(char* nom, char* prenom, char* telephone, int age, char* statut, char* date) {
    if (!checkUniquePhone(telephone)) {
        printf(RED "Le numéro de téléphone %s est déjà utilisé.\n" RESET, telephone);
        return; // Sortir en cas de numéro déjà existant
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
        printf(RED "Capacité maximale atteinte.\n" RESET);
    }
}

// Affiche le tableau des réservations
void afficher_resTable() {
    if (cpt == 0) {
        printf(RED "Pas de réservation pour ce moment.\n" RESET);
        return;
    }
    printf(CYAN "+----+----------------+----------------+---------------+-----+------------+------------+\n" RESET);
    printf(CYAN "| ID |       Nom      |     Prénom     |   Téléphone   | Âge |   Statut   |   Date     |\n" RESET);
    printf(CYAN "+----+----------------+----------------+---------------+-----+------------+------------+\n" RESET);
    for (int i = 0; i < cpt; i++) {
        printf("| %2d | %-14s | %-14s | %-13s | %3d | %-10s | %s |\n",
               resTable[i].id, resTable[i].nom, resTable[i].prenom,
               resTable[i].telephone, resTable[i].age, resTable[i].statut,
               resTable[i].date);
    }
    printf(CYAN "+----+----------------+----------------+---------------+-----+------------+------------+\n" RESET);
}

// Recherche une réservation par ID
struct Reservation* rechercher_par_reference(int id) {
    for (int i = 0; i < cpt; i++) {
        if (resTable[i].id == id) {
            return &resTable[i];
        }
    }
    return NULL; // Retourne NULL si aucune réservation n'est trouvée
}

// Modifie une réservation existante
void modifier_reservation(int id) {
    struct Reservation* res = rechercher_par_reference(id);
    if (res) {
        printf(YELLOW "Modifier la réservation (ID: %d)\n" RESET, res->id);
        printf("Nouveau nom (actuel: %s): ", res->nom);
        scanf(" %[^\n]%*c", res->nom);

        printf("Nouveau prénom (actuel: %s): ", res->prenom);
        scanf(" %[^\n]%*c", res->prenom);

        char new_phone[15];
        do {
            printf("Nouveau téléphone (actuel: %s): ", res->telephone);
            scanf(" %[^\n]%*c", new_phone);
        } while (!valideTele(new_phone));
        strcpy(res->telephone, new_phone);

        printf("Nouvel âge (actuel: %d): ", res->age);
        res->age = checkInt();

        printf("Choisissez un statut:\n");
        for (int i = 0; i < nombre_statuts; i++) {
            printf("%d. %s\n", i + 1, disponibleStatu[i]);
        }
        int choix_statut;
        do {
            printf("Entrez le numéro du statut choisi: ");
            choix_statut = checkInt();
        } while (choix_statut < 1 || choix_statut > nombre_statuts);
        strcpy(res->statut, disponibleStatu[choix_statut - 1]);

        printf("Nouvelle date (actuel: %s): ", res->date);
        char new_date[11];
        while (1) {
            scanf(" %10s", new_date);
            if (chaeckDate(new_date)) {
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

// Supprime une réservation par ID
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
    printf(RED "Réservation non trouvée.\n" RESET);
}

// Affiche les statistiques des réservations
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
            }
        }
    }

    switch (choix) {
        case 1:
            printf("Moyenne d'âge: %.2f\n", (cpt > 0 ? (float)total_age / cpt : 0));
            break;
        case 2:
            printf("Nombre de patients par tranche d'âge:\n");
            printf("0-18 ans: %d\n", age_cpt[0]);
            printf("19-35 ans: %d\n", age_cpt[1]);
            printf("36 ans et plus: %d\n", age_cpt[2]);
            break;
        case 3:
            printf("Nombre total de réservations par statut:\n");
            for (int j = 0; j < nombre_statuts; j++) {
                printf("%s: %d\n", disponibleStatu[j], status_cpt[j]);
            }
            break;
        default:
            printf(RED "Choix invalide.\n" RESET);
    }
}

// Insère des réservations par défaut
void insertion_defaut() {
    ajouter_reservation("Hamdi", "Salma", "0612345678", 19, "valide", "2024-01-01");
    ajouter_reservation("Bennani", "Youssef", "0687654321", 30, "annulée", "2024-10-02");
    ajouter_reservation("Benjelloun", "Salma", "0678901234", 40, "traitée", "2024-07-03");
    ajouter_reservation("Mouhib", "Rachid", "0667890123", 50, "reportée", "2024-10-04");
    ajouter_reservation("Zouiten", "Nadia", "0656789012", 15, "valide", "2024-10-05");
    ajouter_reservation("El Ouardi", "Hamid", "0645678901", 20, "annulée", "2024-10-13");
    ajouter_reservation("Cherkaoui", "Meryem", "0634567890", 22, "traitée", "2024-10-07");
    ajouter_reservation("Fassi", "Omar", "0623456789", 35, "reportée", "2024-10-08");
    ajouter_reservation("Ait Benhaddou", "Khadija", "0612340987", 28, "valide", "2024-12-09");
    ajouter_reservation("Brahimi", "Zineb", "0687654312", 33, "annulée", "2024-02-10");
}

// Affiche les détails d'une réservation
void afficher_details_reservation(int id) {
    struct Reservation* res = rechercher_par_reference(id);
    if (res) {
        printf(CYAN "+----+----------------+----------------+---------------+-----+------------+------------+\n" RESET);
        printf("| ID |       Nom      |     Prénom     |   Téléphone   | Âge |   Statut   |   Date     |\n");
        printf("+----+----------------+----------------+---------------+-----+------------+------------+\n");
        printf("| %2d | %-14s | %-14s | %-13s | %3d | %-10s | %s |\n",
               res->id, res->nom, res->prenom,
               res->telephone, res->age, res->statut,
               res->date);
        printf("+----+----------------+----------------+---------------+-----+------------+------------+\n" RESET);
    } else {
        printf(RED "Réservation non trouvée.\n" RESET);
    }
}

// Trie les réservations par nom
void trier_reservations_par_nom(int ordre) {
    for (int i = 0; i < cpt - 1; i++) {
        for (int j = 0; j < cpt - i - 1; j++) {
            int comparison = strcmp(resTable[j].nom, resTable[j + 1].nom);
            if ((ordre == 1 && comparison > 0) || (ordre == 2 && comparison < 0)) {
                struct Reservation temp = resTable[j];
                resTable[j] = resTable[j + 1];
                resTable[j + 1] = temp;
            }
        }
    }
}

// Trie les réservations par statut
void trier_reservations_par_statut(int ordre) {
    for (int i = 0; i < cpt - 1; i++) {
        for (int j = 0; j < cpt - i - 1; j++) {
            int comparison = strcmp(resTable[j].statut, resTable[j + 1].statut);
            if ((ordre == 1 && comparison > 0) || (ordre == 2 && comparison < 0)) {
                struct Reservation temp = resTable[j];
                resTable[j] = resTable[j + 1];
                resTable[j + 1] = temp;
            }
        }
    }
}

// Trie les réservations par date
void trier_reservations_par_date(int ordre) {
    for (int i = 0; i < cpt - 1; i++) {
        for (int j = 0; j < cpt - i - 1; j++) {
            int comparison = strcmp(resTable[j].date, resTable[j + 1].date);
            if ((ordre == 1 && comparison > 0) || (ordre == 2 && comparison < 0)) {
                struct Reservation temp = resTable[j];
                resTable[j] = resTable[j + 1];
                resTable[j + 1] = temp;
            }
        }
    }
}

void rechercher_par_id() {
    int id;
    printf(GREEN "Entrer la référence unique (ID) à rechercher: " RESET);
    id = checkInt();
    struct Reservation* res = rechercher_par_reference(id);
    if (res) {
        afficher_details_reservation(res->id);
    } else {
        printf(RED "Réservation non trouvée.\n" RESET);
    }
}

void rechercher_par_nom() {
    char nom[30];
    printf(GREEN "Entrer le nom à rechercher: " RESET);
    scanf(" %[^\n]%*c", nom);

    int found = 0;
    for (int i = 0; i < cpt; i++) {
        if (strcmp(resTable[i].nom, nom) == 0) {
            afficher_details_reservation(resTable[i].id);
            found = 1;
        }
    }
    if (!found) {
        printf(RED "Aucune réservation trouvée pour le nom %s.\n" RESET, nom);
    }
}

void rechercher_par_date() {
    char date[11];
    printf(GREEN "Entrer la date à rechercher (YYYY-MM-DD): " RESET);
    while (1) {
        scanf(" %10s", date);
        if (chaeckDate(date)) {
            break;
        } else {
            printf(RED "Date invalide. Veuillez entrer une date valide (YYYY-MM-DD): " RESET);
        }
    }

    int found = 0;
    for (int i = 0; i < cpt; i++) {
        if (strcmp(resTable[i].date, date) == 0) {
            afficher_details_reservation(resTable[i].id);
            found = 1;
        }
    }
    if (!found) {
        printf(RED "Aucune réservation trouvée pour la date %s.\n" RESET, date);
    }
}

// Fonction principale
int main() {
    insertion_defaut();
    printf(GREEN "Déjà %d réservations stockées.\n" RESET, cpt);

    int choix;
    do {
        printf("\n" BLUE "==================== Menu ====================\n" RESET);
        printf(YELLOW "1. Ajouter une réservation\n" RESET);
        printf(YELLOW "2. Modifier une réservation\n" RESET);
        printf(YELLOW "3. Supprimer une réservation\n" RESET);
        printf(YELLOW "4. Afficher les réservations\n" RESET);
        printf(YELLOW "5. Consulter les détails d'une réservation\n" RESET);
        printf(YELLOW "6. Statistiques\n" RESET);
        printf(YELLOW "7. Trier les réservations\n" RESET);
        printf(YELLOW "9. Rechercher une réservation\n" RESET);
        printf(YELLOW "8. Quitter\n" RESET);
        printf(BLUE "==============================================\n" RESET);
        printf("Choisissez une option: ");
        choix = checkInt();

        switch (choix) {
            case 1: {
                char nom[30], prenom[30], telephone[15], statut[20], date[11];
                printf(GREEN "Nom: " RESET); scanf(" %[^\n]%*c", nom);
                printf(GREEN "Prénom: " RESET); scanf(" %[^\n]%*c", prenom);

                do {
                    printf(GREEN "Téléphone (10 chiffres): " RESET);
                    scanf(" %[^\n]%*c", telephone);
                } while (!valideTele(telephone));

                printf(GREEN "Âge: " RESET);
                int age = checkInt();

                printf(GREEN "Choisissez un statut:\n" RESET);
                for (int i = 0; i < nombre_statuts; i++) {
                    printf("%d. %s\n", i + 1, disponibleStatu[i]);
                }
                int choix_statut;
                do {
                    printf(GREEN "Entrez le numéro du statut choisi: " RESET);
                    choix_statut = checkInt();
                } while (choix_statut < 1 || choix_statut > nombre_statuts);
                strcpy(statut, disponibleStatu[choix_statut - 1]);

                printf(GREEN "Date (YYYY-MM-DD): " RESET);
                while (1) {
                    scanf(" %10s", date);
                    if (chaeckDate(date)) {
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
                printf(GREEN "Entrer la référence unique (ID): " RESET);
                id = checkInt();
                modifier_reservation(id);
                break;
            }
            case 3: {
                int id;
                printf(GREEN "Entrer la référence unique (ID) à supprimer: " RESET);
                id = checkInt();
                supprimer_reservation(id);
                break;
            }
            case 4:
                afficher_resTable();
                break;

            case 5: {
                int id;
                printf(GREEN "Entrer la référence unique (ID) pour consulter les détails: " RESET);
                id = checkInt();
                afficher_details_reservation(id);
                break;
            }

            case 6: {
                printf(GREEN "Choisissez le type de statistique à afficher:\n" RESET);
                printf("1. Moyenne d'âge\n");
                printf("2. Nombre de patients par tranche d'âge\n");
                printf("3. Nombre total de réservations par statut\n");
                int choix_statistique = checkInt();
                afficher_statistiques(choix_statistique);
                break;
            }

            case 7: {
                printf(GREEN "Choisissez un critère de tri:\n" RESET);
                printf("1. Par nom\n");
                printf("2. Par statut\n");
                printf("3. Par date\n");
                int critere = checkInt();

                printf(GREEN "Choisissez un ordre de tri:\n" RESET);
                printf("1. Croissante\n");
                printf("2. Decroissante\n");
                int ordre = checkInt();

                switch (critere) {
                    case 1:
                        trier_reservations_par_nom(ordre);
                        printf(GREEN "Réservations triées par nom.\n" RESET);
                        break;
                    case 2:
                        trier_reservations_par_statut(ordre);
                        printf(GREEN "Réservations triées par statut.\n" RESET);
                        break;
                    case 3:
                        trier_reservations_par_date(ordre);
                        printf(GREEN "Réservations triées par date.\n" RESET);
                        break;
                    default:
                        printf(RED "Critère de tri invalide.\n" RESET);
                }
                break;
            }

            case 9: {
    printf(GREEN "Choisissez le type de recherche:\n" RESET);
    printf("1. Par ID\n");
    printf("2. Par nom\n");
    printf("3. Par date (optionnelle)\n");
    int choix_recherche = checkInt();

    switch (choix_recherche) {
        case 1:
            rechercher_par_id();
            break;
        case 2:
            rechercher_par_nom();
            break;
        case 3:
            rechercher_par_date();
            break;
        default:
            printf(RED "Choix de recherche invalide.\n" RESET);
    }
    break;
}

            case 8:
                printf(GREEN "Au revoir!\n" RESET);
                break;

            default:
                printf(RED "Choix invalide.\n" RESET);
        }
    } while (choix != 8);

    return 0;
}
