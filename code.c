#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { // structure pour récupérer le temps 
    int hours;
    int minutes ;
    float seconds;
} Time;
double get_time_seconds(const char* time) { // récupère le temps du joueur dans le format demandé et convertit en secondes
    Time t;
    sscanf(time, "%dh%dm%fs", &t.hours, &t.minutes, &t.seconds);
    return t.hours * 3600 + t.minutes * 60 + t.seconds;
}

int is_valid_integer(const char *str) { // vérifie que ce qui est entrer par l'utilisateur est bien un chiffre (elle le fait chiffre par chiffre)
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int get_int_input() { // récupère le choix de l'utilisateur et vérifie que c'est un chiffre entier avant de convertir en entier si c'est bon
    char input[100];
    int number;
    while (1) {
        scanf("%s", input);
        if (is_valid_integer(input)) {
            number = atoi(input); // atoi sert à convertir une chaine de caractère en un entier (exemple : "123" -> 123)
            break;
        } else {
            printf("\nInvalid input. Please enter a valid number.\n");
        }
    }
    return number;
}
float get_float_input() { // récupère le choix de l'utilisateur et vérifie que c'est un chiffre réel avant de continuer  
    char input[100];
    float number;
    while (1) {
        scanf("%s", input);
        char *endptr;
        number = strtof(input, &endptr); // utilisée pour convertir une chaîne de caractères en un nombre à virgule flottante de type float.
        if (*endptr == '\0') {
            break;
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }
    return number;
}


Time Constr_time() { // fonction fonctionnant avec la structure Time : sert à récupérer le temps d'un joueur lorsque c'est nécessaire
    Time t;
    t.hours = 0;
    t.minutes = 0;
    t.seconds = 0;

    do {
        printf("hours? ");
        t.hours = get_int_input(); // vérification que c'est bien un entier avec get_int_input
        printf("minutes? ");
        t.minutes = get_int_input();
        printf("seconds? ");
        t.seconds = get_float_input(); // pareil, avec get_float_input

        if (t.hours < 0 || t.hours > 24 || t.minutes < 0 || t.minutes > 59 || t.seconds < 0 || t.seconds > 59) {
            printf("Invalid input. Please enter the time in the correct format.\n");
        }
    } while (t.hours < 0 || t.hours > 24 || t.minutes < 0 || t.minutes > 59 || t.seconds < 0 || t.seconds > 59);

    return t;
}

typedef struct{  // structure pour récupérer la date 
    int days ;
    int month ;
    int year;
}Date ;

typedef struct {  // structure pour récupérer les informations d’un entraînement 
    char date[50];
    char type[50];
    char time[50];
    int position;
} Training;

typedef struct { // structure pour le nom d'un athlète 
    char name[50];
} Athlete;

Date constr_Date() { // structure pour récupérer la date demandée à l'utilisateur
    Date x;
    int verifdays;
    do {
        printf("Which day? \n");
        x.days = get_int_input(); // vérification que c'est bien correct (pas un caractère)
        printf("Which month? (1-12)\n");
        x.month = get_int_input();
        printf("Which year? (0-2024)\n");
        x.year = get_int_input();

        switch (x.month) {// vérifie que c'est bien réaliste (12 mois max, 31/30/28/29 jours...)
            case 2:
                if ((x.year % 4 == 0 && x.year % 100 != 0) || x.year % 400 == 0) {
                    verifdays = 29;
                } else {
                    verifdays = 28;
                }
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                verifdays = 30;
                break;
            default:
                verifdays = 31;
                break;
        }

        if (x.days < 1 || x.days > verifdays || x.month < 1 || x.month > 12 || x.year < 0 || x.year > 2024) {
            printf("Invalid input. Please enter the date in the correct format.\n");
        }

    } while (x.days < 1 || x.days > verifdays || x.month < 1 || x.month > 12 || x.year < 0 || x.year > 2024);
    return x;
}

void write_training_data(const char* filename, Training* trainings, int num_trainings) {// permet d'écrire les informations de chaque athlète dans un fichier créé à leur nom
    FILE *file = fopen(filename, "w"); // ouverture du fichier pour écrire dans celui-ci
    if (file == NULL) {
        printf("Error while opening the file %s.\n", filename);
        return;
    }
    
    for (int i = 0; i < num_trainings; i++) {
         if(strcmp(trainings[i].type, "Relay4x400m")==0){
        fprintf(file, "%s ; %s ; %s ; %d\n", trainings[i].date, trainings[i].type, trainings[i].time, trainings[i].position);
         }
        else {fprintf(file, "%s ; %s ; %s\n", trainings[i].date, trainings[i].type, trainings[i].time);
    }}
    fclose(file);
}

void get_trainings(const char *filename) {  // permet d'afficher l'historique des entraînements de l'athlète demandé
    FILE *file;
    char line[256];
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("No training found in the file %s.\n", filename);
        return;
    }
    printf("History of the athlete practices :\n");
      while (fgets(line, sizeof(line), file)) {   // lecture du fichier ligne à ligne
       printf("%s", line);
    }
    fclose(file);
}


void add_training(const char *filename) { // fonction pour ajouter des entrainements à l'athlete choisi 
    FILE* file;
    Training new;
    Date date;
    Time time;
    char linedate[100];
    char linetime[100];
    char line[256];
    char date1[100];


    file = fopen(filename, "r"); // ouverture du fichier en mode lecture pour vérifier les entrées existantes
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    printf("What is the date ?\n");
    date = constr_Date();
    sprintf(linedate, "%04d-%02d-%02d", date.year, date.month, date.days); // on met la date dans le format que l'on veut et la stocke dans linedate pour pouvoir la copier directement après 

    int type_code;
    do {
        printf("What is the type of the practice ? Write:\n 1 for 100m\n 2 for 400m\n 3 for 5000m\n 4 for marathon\n 5 for Relay4x400m\n");
        type_code = get_int_input(); 
    } while (type_code > 5 || type_code < 1);

    if (type_code == 5) { // on vérifie la condition pour le relais : pas 2 relais le même jour !
        int found = 0;  // variable nous indiquant si une même date est trouvée
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%s ; %*s ; %*s ; %*d", date1); // on stocke la date de chaque ligne dans date1
            if (strcmp(date1, linedate) == 0) { // on vérifie si les deux sont les mêmes...
                printf("Error: you can’t add a second relay practice in the same day.\n");
                found = 1;
                break;  // on quitte la boucle si la même date est trouvée pour le relais

            }
        }
        fclose(file); 
        if (found) {
            return; // on quitte la fonction si la même date est trouvée
        }
    } else {
        fclose(file); 
    }

    file = fopen(filename, "a"); // ouverture en a nous permettra d'ajouter à la fin du fichier des nouveaux entrainements
    if (file == NULL) {
        printf("Error opening file for appending.\n");
        return;
    }

    switch (type_code) { // permet, en fonction du choix de l'utilisateur, de stocker dans new.type l'entraînement correspondant
        case 1:
            strcpy(new.type, "100m");
            break;
        case 2:
            strcpy(new.type, "400m");
            break;
        case 3:
            strcpy(new.type, "5000m");
            break;
        case 4:
            strcpy(new.type, "marathon");
            break;
        case 5:
            strcpy(new.type, "Relay4x400m");
            break;
        default:
            printf("Input error\n");
            fclose(file);
            return;
    }

    printf("What is the time of your athlete?\n");
    time = Constr_time(); // on récupère le temps grâce à cette fonction
    sprintf(linetime, "%dh%dm%.2fs", time.hours, time.minutes, time.seconds);
    if (type_code == 5) {
        do {
            printf("Enter the position of the athlete during the relay: ");
            scanf("%d", &new.position);
            if (new.position < 1 || new.position > 4) {
                printf("False input. Please try again, enter a number between 1 and 4 inclusive.\n");
            }
        } while (new.position < 1 || new.position > 4);
    }
    if (type_code==5){
        fprintf(file, "%s ; %s ; %s ; %d\n", linedate, new.type, linetime, new.position);
    }
    else{
        fprintf(file, "%s ; %s ; %s\n", linedate, new.type, linetime);} // enfin, copie toutes les informations récoltées sur une nouvelle ligne à la fin du fichier 
    fclose(file);
}
void athlete_summary(const char* filename, const char* type) {  // résumé des performances avec pire et meilleur temps d’un athlète pour un type d’entraînement, tel qu'enregistrer dans un fichier    
    FILE *file;
    Time t;
    char line[256];
    double best_time = -20;  // Initialisation avec une valeur arbitrairement basse destinée à être mise à jour     
    double worst_time = 9999999; // Initialisation avec une valeur arbitrairement haute destinée à être mise à jour
double total_time = 0;
    int num_trainings = 0; // compteur pour le nombre d’entraînements
    file = fopen(filename, "r");  // ouverture du fichier en mode lecture
    if (file == NULL) {
        printf("No training found in the file %s.\n", filename);
        return;
    }

    while (fgets(line, sizeof(line), file)) { // lecture du fichier ligne par ligne
        char temp_type[50];
        double time_seconds = 0.0;
        char recuptime[100];
        sscanf(line, "%*s ; %s ; %s", temp_type, recuptime); // extraction du type et du temps depuis la ligne, ignore la date 
        time_seconds = get_time_seconds(recuptime);  // conversion en secondes
        if (strcmp(temp_type, type) == 0) { // si le type dans le fichier correspond au type demandé 
            num_trainings++; // comptage du nombre d’entraînement pour le calcul du temps moyen
            if (time_seconds > best_time) { // mise à jour du meilleur temps si nécessaire
                best_time = time_seconds;
            }
            if (time_seconds < worst_time) {  // mise à jour du pire temps si nécessaire
                worst_time = time_seconds;
            }
            total_time += time_seconds;// accumulation du temps total pour calculer le temps moyen plus tard
        }
    } 
// }
    fclose(file);
    if (num_trainings == 0) {  // si aucun entraînement trouvé
        printf("\nNo type training %s found for this athlete.\n", type);
    } else {
        printf("Summary of the athlete performances for the %s :\n", type); // affichage du résumé
        printf("Best time : %.2lf seconds\n", best_time); // meilleur temps
        printf("Worst time : %.2lf seconds\n", worst_time);  // pire temps
        printf("Average time : %.2lf seconds\n", total_time/num_trainings); // temps moyen
    }
}

int partition(double arr[], char names[][100], int low, int high) {
// tri rapide, fait un tableau en deux parties autour d'un pivot tel que les éléments à gauche du pivot soient plus petits ou égaux au pivot, et ceux à droite soient plus grands
    double pivot = arr[high]; // le dernier élément du tableau est utilisé comme pivot
    char pivot_name[100]; 
    strcpy(pivot_name, names[high]);  // copie le nom du dernier élément dans le pivot
    int i = low - 1; // initialisation de i, index pour le placement des éléments plus petits que le pivot
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) { // compare les éléments avec le pivot 
            i++; // on déplace i pour un nouvel élément inférieur ou égal au pivot
           // Échanger arr[i] et arr[j]
            double temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;

            // Échanger names[i] et names[j]
            char temp_name[100];
            strcpy(temp_name, names[i]);
            strcpy(names[i], names[j]);
            strcpy(names[j], temp_name);
        }
    }
    // Échanger arr[i + 1] et arr[high] (ou le pivot), on place le pivot à la position correcte pour séparer les 2 parties du tableau
    double temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

 // Échanger names[i + 1] et names[high]
    strcpy(names[high], names[i + 1]);
    strcpy(names[i + 1], pivot_name);
    return i + 1;  // retourne la position du pivot
}

void quicksort(double arr[], char names[][100], int low, int high) { // sert à faire du tri rapide (utilisation d’un pivot (pi) + trie de manière récursive) 
    if (low < high) {
        int pi = partition(arr, names, low, high); // met les éléments inférieurs ou égaux au pivot à gauche ; les éléments supérieurs à droite 
        quicksort(arr, names, low, pi - 1);  // Trie à gauche du pivot (récursif)
        quicksort(arr, names, pi + 1, high); // Trie à droite du pivot (récursif)
    }
}
void top_athletes(Athlete *athletes, int num_ath, const char* type, int num_top) { // fonction pour classer les athlètes du meilleur au moins bon
   double times[100]; // stockera les temps moyens
    char names[100][100];
    int num_athletes = 0;
    for (int i = 0; i < num_ath; i++) { // boucle pour récupérer les performances de chaque athlète
        char filename[100];
        sprintf(filename, "%s.txt", athletes[i].name);  // création du nom de fichier
        FILE* file = fopen(filename, "r"); // ouverture en mode lecture
        if (file == NULL) {
            printf("\nImpossible to open the file %s.\n", filename);
            continue;
        }
        char line[256];
        int count = 0; // correspondra au nombre d’entraînements en fonction du type demandé
        double total_time = 0.0; // somme des temps pour calculer la moyenne
        if (strcmp(type, "Relay4x400m") == 0) { // si c’est un relais (car le relais a la position en plus)
            while (fgets(line, sizeof(line), file)) { // lecture ligne par ligne
                char temp_type[500]; 
                char temp_time[500];
                int temp_position;
                int items_parsed = sscanf(line, "%*s ; %s ; %s ; %*d", temp_type, temp_time); // on vérifie que sscanf lit bien deux éléments avant de continuer
                if (items_parsed == 2 && strcmp(temp_type, type) == 0) { 
                    total_time += get_time_seconds(temp_time);
                    count++; //on incrémenté pour ensuite l’utiliser pour le calcul de la moyenne 
                }
            }
        } else { // si ce n’est pas un relais (pas de position à prendre en compte) 
            while (fgets(line, sizeof(line), file)) { //lecture ligne par ligne 
                char temp_type[500]; 
                char temp_time[500];
                int items_parsed = sscanf(line, "%*s ; %s ; %s", temp_type, temp_time); 
                if (items_parsed == 2 && strcmp(temp_type, type) == 0) {
                    total_time += get_time_seconds(temp_time);
                    count++;
                }
            }
        }
        fclose(file);
        
        if (count > 0) {
            times[num_athletes] = total_time / count;  // calcul de la moyenne des temps
            strcpy(names[num_athletes], athletes[i].name);   // stockage du nom de l’athlète dans le tableau des noms 
            num_athletes++; // on incrémente pour pouvoir savoir combien d’athlètes participent à l’entraînement dont il est question 
        } else {
            printf("\nNo valid data for %s in file %s.\n", type, filename); 
        }
    }

    quicksort(times, names, 0, num_athletes - 1);   // tri des athlètes par temps moyen 
// affichage des meilleurs athlètes
    printf("\nFor the %s practice, the best athletes are :\n", type);
    for (int i = 0; i < num_top && i < num_athletes; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }

    if (num_top == 3 && num_athletes >= 3) { // message si l’utilisateur demande le top 3 uniquement 
        printf("\nYou should send them to the JO !!!\n");
    }
}

int compare_dates(Date date1, Date date2) { // fonction permettant de vérifier quelle date est avant l'autre et que la progression du joueur soit logique
    if (date1.year != date2.year) {
        return date1.year - date2.year;
    } else if (date1.month != date2.month) {
        return date1.month - date2.month;
    } else {
        return date1.days - date2.days;
    }
}

void athlete_progression(const char* filename, const char* type, Date date1, Date date2) { // calcule la différence entre 2 temps à 2 dates et pour un type donné 
    FILE *file;
    char line[256];  //tampon pour lire chaque ligne
    double time1 = 0.0, time2 = 0.0;
    int found1 = 0, found2 = 0; // indicateurs pour savoir si les temps ont été trouvés
    char firstdate[256]; 
    char secdate[256];
    sprintf(firstdate, "%04d-%02d-%02d", date1.year, date1.month, date1.days); // stocke la première date dans ‘firstdate’ sous la forme d’une ligne
    sprintf(secdate, "%04d-%02d-%02d", date2.year, date2.month, date2.days); //stocke la seconde date dans ‘secdate’ sous la forme d’une ligne

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("\nNo training found in file %s.\n", filename);
        return;
    }

     // Recherche du temps de l'athlète pour l'épreuve à chaque date

    while (fgets(line, sizeof(line), file)) { // lecture ligne par ligne
        char temp_type[50], temp_date[50], temp_time[50]; // on y stockera le type, date et temps de chaque ligne
        if (strcmp(type, "Relay4x400m") == 0) {
            sscanf(line, "%*s ; %s ; %s ; %*d", temp_type, temp_time); // extrait le type et temps, ignore la date (en respectant le même format)
        } else {
            sscanf(line, "%*s ; %s ; %s", temp_type, temp_time); // extrait le type et temps, ignore la date (en respectant le même format)
        }
        if (strcmp(temp_type, type) == 0) { // vérifie si les types correspondent 
            if (strcmp(type, "Relay4x400m") == 0) {
                sscanf(line, "%s ; %*s ; %s ; %*d", temp_date, temp_time);
            } else {
                sscanf(line, "%s ; %*s ; %s", temp_date, temp_time); 
            }
            if (strcmp(temp_date, firstdate) == 0) {
                time1 = get_time_seconds(temp_time);  // temps est converti en secondes et stocké dans time1
                found1 = 1; // indique que le temps est trouvé
            }
            if (strcmp(temp_date, secdate) == 0) {
                time2 = get_time_seconds(temp_time);
                found2 = 1;
            }
            if (found1 && found2) { //  si on a 2 temps, on continue : on peut faire les calculs 
                double difference = time2 - time1;
                if (compare_dates(date1, date2) > 0) { 
                    difference = time1 - time2;
                }
                if (difference < 0) { // 1er cas : il s’est amélioré entre les deux dates
                    printf("\nBetween %s and %s, the time of this athlete decreased by %.2f seconds\n", firstdate, secdate, -difference);
                    printf("YOUHOU! The athlete has progressed!\n");
                } else if (difference > 0) { // 2e cas : il a régressé entre les deux dates
                    printf("\nBetween %s and %s, the time of this athlete increased by %.2f seconds\n", secdate, firstdate, difference);
                    printf("Olala, the athlete has regressed! He should run faster! :(\n");
                } else {
                    printf("\nThe athlete's time has remained the same, that's already something.\n");
                }
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
    printf("\nNo matching data found for the given dates.\n");
}


int main() {
// création des athlètes
    Athlete athletes[] = {
        {"Hari"},
        {"Rayan"},
        {"Amine"},
        {"Dahmane"},
        {"Hicham"}
    };

   // données d'entraînement pour chaque athlète
    Training hari_trainings[] = {
        {"2024-05-01" , "100m" , "00h00m12.5s" },
        {"2024-05-05" , "400m" , "00h00m58.3s" },
        {"2024-04-02" , "100m" , "00h00m13.5s" },
        {"2024-11-18" , "100m" , "00h00m10.5s"},
        {"2024-05-10" , "5000m" , "00h21m45s" },
        {"2024-05-12" , "Relay4x400m" , "00h3m32s" , 3},
        {"2024-03-17" , "400m" , "00h00m60.3s"},
        {"2024-06-23" , "5000m" , "00h22m45s"},
        {"2024-05-24" , "Relay4x400m" , "00h3m50s" , 2},
        {"2024-06-24" , "Marathon" , "3h00m32s" },
    };
    Training rayan_trainings[] = {
        {"2024-05-02" , "100m" , "00h00m11.8s"},
        {"2024-05-06" , "Relay4x400m" , "00h3m52s" , 1 },
        {"2024-05-11" , "Marathon" , "3h30m01s"},
        {"2024-05-05" , "400m" , "00h00m58.3s"},
        {"2024-02-10" , "5000m" , "00h22m45s"},
       {"2024-05-12" , "100m" , "00h00m11.21s"},
        {"2024-03-25" , "Relay4x400m" , "00h3m24s", 3},
        {"2024-04-23" , "Marathon" , "3h25m00s"},
        {"2024-02-03" , "400m" , "00h00m58.28s"},
        {"2024-01-04" , "5000m" , "00h21m43s"}
    };
    Training amine_trainings[] = {
        {"2024-05-02" , "100m" , "00h00m11.8s"},
        {"2024-05-03" , "400m" , "00h51.2s"},
        {"2024-05-07" , "5000m" , "00h19m15s"},
        {"2024-05-12" , "Marathon" , "3h15m00s"},
        {"2024-02-23" , "Relay4x400m" , "00h3m36s", 2},
        {"2024-05-23" , "100m" , "00h00m12.2s"},
        {"2024-03-03" , "400m" , "00h50.3s"},
        {"2024-02-14" , "5000m" , "00h21m13s"},
        {"2024-03-13" , "Marathon" , "3h12m00s"},
        {"2024-08-12" , "Relay4x400m" , "00h3m40s", 2}
    };
    Training dahmane_trainings[] = {
        {"2024-05-04" , "100m" , "00h00m11.5s"},
        {"2024-05-08" , "400m" , "00h00m52.4s"},
        {"2024-05-13" , "5000m" , "00h19m20s"},
        {"2024-08-12" , "Relay4x400m" , "00h3m40s", 3},
        {"2024-05-12" , "Marathon" , "3h15m00s"},
        {"2024-05-03" , "100m" , "00h00m10.5s"},
        {"2024-05-17" , "400m" , "00h00m53.4s"},
        {"2024-05-12" , "5000m" , "00h19m18s"},
        {"2024-08-06" , "Relay4x400m" , "00h3m23s", 1},
        {"2024-03-02" , "Marathon" , "3h08m00s"}
    };
    Training hicham_trainings[] = {
        {"2024-05-09" , "100m" , "00h00m11.1s"},
        {"2024-05-14" , "400m" , "00h00m52.1s"},
        {"2024-05-19" , "5000m" , "00h15m45s"},
        {"2024-07-06" , "Relay4x400m" , "00h3m23s", 1},
        {"2024-03-02" , "Marathon" , "3h08m02s"},
        {"2024-05-09" , "100m" , "00h00m12.3s"},
        {"2024-05-14" , "400m" , "00h00m53.1s"},
        {"2024-05-19" , "5000m" , "00h15m22s"},
        {"2024-08-12" , "Relay4x400m" , "00h3m31s", 2},
        {"2024-03-02" , "Marathon" , "3h02m02s"}
    };

 // écriture des données d'entraînement dans des fichiers pour chaque athlète
    write_training_data("Hari.txt", hari_trainings, sizeof(hari_trainings) / sizeof(hari_trainings[0]));
    write_training_data("Rayan.txt", rayan_trainings, sizeof(rayan_trainings) / sizeof(rayan_trainings[0]));
    write_training_data("Amine.txt", amine_trainings, sizeof(amine_trainings) / sizeof(amine_trainings[0]));
    write_training_data("Dahmane.txt", dahmane_trainings, sizeof(dahmane_trainings) / sizeof(dahmane_trainings[0]));
    write_training_data("Hicham.txt", hicham_trainings, sizeof(hicham_trainings) / sizeof(hicham_trainings[0]));

 // affichage de la liste des athlètes avec des numéros associés
    printf("\nAthletes list :\n");
    for (int i = 0; i < sizeof(athletes) / sizeof(athletes[0]); i++) {
        printf("%d. %s\n", i + 1, athletes[i].name);
    }


    int user_choice;

    do {
        printf("\nDo you want the history of an athlete ? (1 for Yes, 2 for No) : \n");
        user_choice = get_int_input(); // on vérifie que ce que l’utilisateur entre est bon (comme à chaque fois)

        if (user_choice == 1) {
            int athlete_choice;
            do {
                printf("Choose the athlete that you want to check up (1 to %ld) : \n", sizeof(athletes) / sizeof(athletes[0])); // sizeof(athletes)/sizeof(athletes[0]) correspond au nombre d’athlètes au total (athlètes étant une structure..)
                athlete_choice = get_int_input(); // vérification que le choix de l’utilisateur est bon 
                if (athlete_choice < 1 || athlete_choice > sizeof(athletes) / sizeof(athletes[0])) { 
                    printf("\nInvalid choice.\n");
                }
            } while (athlete_choice < 1 || athlete_choice > sizeof(athletes) / sizeof(athletes[0]));

            char filename[100];
            sprintf(filename, "%s.txt", athletes[athlete_choice - 1].name); // on récupère grâce au choix de l’utilisateur le nom de l’athlète et ouvre le fichier correspondant
            get_trainings(filename);

            do {
                printf("\nDo you want to add practices to this athlete ? (1 for Yes, 2 for No) : \n");
                user_choice = get_int_input(); // vérification
                if (user_choice == 1) {
                    add_training(filename);
                } else if (user_choice != 2) {
                    printf("\nInvalid answer.\n");
                }
            } while (user_choice != 1 && user_choice != 2);
        } else if (user_choice != 2) {
            printf("\nInvalid answer.\n");
        }
    } while (user_choice != 2);



    do{
    printf("\nDo you want to see a top of the best athletes ? (1 for Yes, 2 for No.)\n");
    user_choice = get_int_input(); // vérification 
    } while(user_choice !=1 && user_choice!=2); 
    char choice2[100];
    int top;

    if (user_choice == 1) {
        do {
            printf("\nOf which practice ? : Write : \n 1 for 100m \n 2 for 400m \n 3 for 5000m \n 4 for Marathon \n 5 for Relay4x400m \n");
            scanf("%s", choice2);
        } while (strcmp(choice2, "5") > 0 || strcmp(choice2, "1") < 0); // on va poser la question jusqu’à ce que ça marche 

        if (strcmp(choice2, "1") == 0) { // on met en lien le choix de l’utilisateur et l'entraînement qui y correspond 
            strcpy(choice2, "100m");
        } else if (strcmp(choice2, "2") == 0) {
            strcpy(choice2, "400m");
        } else if (strcmp(choice2, "3") == 0) {
            strcpy(choice2, "5000m");
        } else if (strcmp(choice2, "4") == 0) {
            strcpy(choice2, "Marathon");
        } else if(strcmp(choice2, "5")==0){
            strcpy(choice2,"Relay4x400m");
        }

        printf("\nYou selected: %s\n", choice2);

        do {
            printf("\nWhat is the top that you want ? (Minimum=1 and Maximum=5)\n");
            top = get_int_input(); // vérification 
        } while (top < 1 || top > 5);

        top_athletes(athletes, 5, choice2, top);
    }

    int choice; 
    int athlete_choice; 
    do {
        printf("\nDo you want to see the summary of an athlete ? (1 for Yes, 2 for No) : \n");
        choice = get_int_input(); // vérification du choix 

        if (choice == 1) {
            char filename[100];
            do {
                printf("Choose an athlete (1: Hari, 2: Rayan, 3: Amine, 4: Dahmane, 5: Hicham) : ");
                athlete_choice = get_int_input(); // vérification 
            } while (athlete_choice < 1 || athlete_choice > 5);

            char word[100];
            int choice3;
            do {
                printf("\nOf which practice ? : Write : \n 1 for 100m \n 2 for 400m \n 3 for 5000m \n 4 for Marathon \n 5 for Relay4x400m \n");
                choice3 = get_int_input();
            } while (choice3 < 1 || choice3 > 5); // on va redemander jusqu’à ce que le choix est bon 

            switch (choice3) { // on met en lien le choix et l’entraînement qui correspond 
                case 1:
                    strcpy(word, "100m");
                    break;
                case 2:
                    strcpy(word, "400m");
                    break;
                case 3:
                    strcpy(word, "5000m");
                    break;
                case 4:
                    strcpy(word, "Marathon");
                    break;
                case 5:
                    strcpy(word, "Relay4x400m");
                    break;
                default:
                    printf("Input error\n");
                    break;
            }

            sprintf(filename, "%s.txt", athletes[athlete_choice - 1].name); // d’après le choix, on récupère l’athlète correspondant, lui ajoute txt pour ensuite pouvoir l’ouvrir dans la fonction athlete_summary 
            athlete_summary(filename, word);
        }
    } while (choice != 2);

    int choice_progress;// variable pour stocker le choix de l'utilisateur (1 ou 2)
    int athletechoice;  // variable pour stocker le choix de l'athlète (1 à 5)
    FILE* file;
    char filename[100];
    char typetraining[100];
    int choice4;

    do {
        printf("\nDo you want to see the athlete progression ? (1 for Yes, 2 for No) : \n");
        choice_progress = get_int_input(); // vérification 

        if (choice_progress == 1) {
            do {
                printf("\nChoose an athlete (1: Hari, 2: Rayan, 3: Amine, 4: Dahmane, 5: Hicham) : \n ");
                athletechoice = get_int_input(); // vérification 
            } while (athletechoice < 1 || athletechoice > 5);

            sprintf(filename, "%s.txt", athletes[athletechoice - 1].name); // on récupère le choix de l’athlète pour ensuite ouvrir le fichier correspondant 
            file = fopen(filename, "r");

            char line[100];
            get_trainings(filename); 

            do {
                printf("\nOf which practice ? : Write : \n 1 for 100m \n 2 for 400m \n 3 for 5000m \n 4 for Marathon \n 5 for Relay 4*400 m \n");
                choice4 = get_int_input(); // vérification 
            } while (choice4 < 1 || choice4 > 5);

            switch (choice4) { // on copie dans typetraining l’entraînement en fonction du choix de l’utilisateur (1, 2, 3, 4 ou 5)
                case 1:
                    strcpy(typetraining, "100m");
                    break;
                case 2:
                    strcpy(typetraining, "400m");
                    break;
                case 3:
                    strcpy(typetraining, "5000m");
                    break;
                case 4:
                    strcpy(typetraining, "Marathon");
                    break;
                case 5:
                    strcpy(typetraining, "Relay4x400m");
                    break;
                default:
                    printf("\nInput error\n");
                    break;
            }
            Date date1;
            Date date2;
            char linedate[256];
            printf("\nWhat is the first date ?\n"); 
            date1 = constr_Date(); // récupération de la date grâce à la fonction 
            sprintf(linedate, "%d-%d-%d", date1.year, date1.month, date1.days); // stocke la date dans ‘linedate’ sous forme d’une ligne de chaîne de caractères
            printf("\nWhat is the second date ?\n");
            date2 = constr_Date();
            sprintf(linedate, "%d-%d-%d", date2.year, date2.month, date2.days); // pareil ici 
            athlete_progression(filename, typetraining, date1, date2); // on renvoie vers la fonction pour calculer la progression 
        }
    } while (choice_progress != 2);
printf("\n\n\n\n\n\n\nGoodbye, see you soon ! :D\n");
    fclose(file);
    return 0;
}
