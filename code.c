#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 10  // Taille maximale de la file (nombre de clients dans la file)

// Structure représentant un client
typedef struct {
    int id;         // Identifiant unique du client
    char nom[20];  
    char srv[20];   // Service demandé par le client
} Client;

// Structure représentant la file de clients
typedef struct {
    Client clients[MAX_CLIENTS];  // Tableau qui contient les clients
    int taille;                   // Nombre actuel de clients dans la file
} File;

// Structure représentant un achat dans une liste chaînée
typedef struct Achat {
    char produit[50];       // Produit acheté
    struct Achat* suivant;  // Pointeur vers le prochain achat
} Achat;

// Fonction pour ajouter un client à la file
void ajouter(File* f, int id, char nom[], char srv[]) {
    if (f->taille < MAX_CLIENTS) {
        f->clients[f->taille].id = id;
        strcpy(f->clients[f->taille].nom, nom);
        strcpy(f->clients[f->taille].srv, srv);
        f->taille++;
        printf("Client ajouté avec succès.\n");
    } else {
        printf("La file est pleine.\n");
    }
}

// Fonction pour afficher les clients dans la file
void afficher(File* f) {
    if (f->taille == 0) {
        printf("La file est vide.\n");
    } else {
        for (int i = 0; i < f->taille; i++) {
            printf("Client ID: %d, Nom: %s, Service: %s\n", f->clients[i].id, f->clients[i].nom, f->clients[i].srv);
        }
    }
}

// Fonction pour supprimer un client de la file par son ID
void supprimer(File* f, int id) {
    int trouve = 0;

    for (int i = 0; i < f->taille; i++) {
        if (f->clients[i].id == id) {
            for (int j = i; j < f->taille - 1; j++) {
                f->clients[j] = f->clients[j + 1];
            }
            f->taille--;
            trouve = 1;
            printf("Client avec ID %d supprimé.\n", id);
            break;
        }
    }

    if (!trouve) {
        printf("Client avec ID %d non trouvé.\n", id);
    }
}

// Fonction pour afficher les achats d'un client (utilisant une liste chaînée)
void afficher_achats(Achat* achat) {
    if (achat == NULL) {
        printf("Aucun achat précédent.\n");
        return;
    }

    printf("Achats précédents :\n");
    while (achat != NULL) {
        printf("- %s\n", achat->produit);
        achat = achat->suivant;
    }
}

// Fonction pour ajouter un achat dans la liste chaînée
Achat* ajouter_achat(Achat* achat, char produit[]) {
    Achat* nouveauAchat = (Achat*)malloc(sizeof(Achat));
    if (nouveauAchat == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        return achat;
    }
    
    strcpy(nouveauAchat->produit, produit);
    nouveauAchat->suivant = NULL;

    if (achat == NULL) {
        return nouveauAchat;
    }

    Achat* temp = achat;
    while (temp->suivant != NULL) {
        temp = temp->suivant;
    }
    temp->suivant = nouveauAchat;
    return achat;
}

// Fonction pour sauvegarder les informations du client et ses achats dans un fichier
void sauvegarder_client_et_achats(Client* client, Achat* achats, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "a");  // Ouvrir le fichier en mode ajout (a)
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }

    // Sauvegarder les informations du client
    fprintf(fichier, "Client ID: %d, Nom: %s, Service: %s\n", client->id, client->nom, client->srv);

    // Sauvegarder les achats
    fprintf(fichier, "Achats:\n");
    while (achats != NULL) {
        fprintf(fichier, "- %s\n", achats->produit);
        achats = achats->suivant;
    }

    fclose(fichier);  
    printf("Informations du client et achats sauvegardés dans le fichier.\n");
}

int main() {
    // Initialisation de la file avec une taille de 0 (aucun client au départ)
    File file = { .taille = 0 };
    Achat* achats = NULL;  // Liste chaînée des achats du client
    int choix, id = 1;  // `id` commence à 1 pour les clients
    char nom[20], srv[20], produit[50];  // Variables pour stocker le nom, le service et le produit acheté
    int idSuppression;  // Variable pour stocker l'ID du client à supprimer

    do {
        // Affichage du menu principal
        printf("\n--- Menu --- \n");
        printf("1. Ajouter un client\n");
        printf("2. Afficher la file\n");
        printf("3. Supprimer un client\n");
        printf("4. Ajouter un achat\n");
        printf("5. Afficher les achats précédents\n");
        printf("6. Sauvegarder les informations du client et ses achats\n");
        printf("7. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);  // L'utilisateur entre son choix dans le menu

        switch (choix) {
            case 1:  // Si l'utilisateur choisit d'ajouter un client
                printf("Nom du client : ");
                scanf("%s", nom);  // L'utilisateur entre le nom du client
                printf("Service demandé : ");
                scanf("%s", srv);  // L'utilisateur entre le service demandé par le client
                ajouter(&file, id, nom, srv);  // Appel de la fonction pour ajouter un client à la file
                id++;  // On incrémente l'ID pour le prochain client
                break;

            case 2:  // Si l'utilisateur choisit d'afficher la file
                afficher(&file);  // Appel de la fonction pour afficher tous les clients dans la file
                break;

            case 3:  // Si l'utilisateur choisit de supprimer un client
                printf("Entrez l'ID du client à supprimer : ");
                scanf("%d", &idSuppression);  // L'utilisateur entre l'ID du client à supprimer
                supprimer(&file, idSuppression);  // Appel de la fonction pour supprimer le client
                break;

            case 4:  // Si l'utilisateur choisit d'ajouter un achat
                printf("Entrez le produit acheté : ");
                scanf("%s", produit);  // L'utilisateur entre le produit acheté
                achats = ajouter_achat(achats, produit);  // Ajout du produit à la liste chaînée des achats
                printf("Achat ajouté : %s\n", produit);
                break;

            case 5:  // Si l'utilisateur choisit d'afficher les achats précédents
                afficher_achats(achats);  // Appel de la fonction pour afficher les achats
                break;

            case 6:  // Si l'utilisateur choisit de sauvegarder les informations du client et ses achats
                if (file.taille > 0) {  // Vérifier qu'il y a des clients dans la file
                    int clientId;
                    printf("Entrez l'ID du client pour sauvegarder ses informations : ");
                    scanf("%d", &clientId);  // L'utilisateur entre l'ID du client à sauvegarder
                    if (clientId > 0 && clientId <= file.taille) {
                        // Appeler la fonction pour sauvegarder le client et ses achats dans un fichier
                        sauvegarder_client_et_achats(&file.clients[clientId - 1], achats, "clients.txt");
                    } else {
                        printf("Client introuvable.\n");  // Si l'ID est invalide
                    }
                } else {
                    printf("Aucun client dans la file.\n");  // Si la file est vide
                }
                break;

            case 7:  
                printf("Au revoir !\n");
                break;

            default:  // Si l'utilisateur entre un choix invalide
                printf("Choix invalide. Réessayez.\n");
        }
    } while (choix != 7);  

    return 0;  
}
