#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "projekti.h"

/* Simple C-progran where you can add countries to "Olympics", add medals for countries
and save the results in a file. Also supports reading the results from file.*/


/* Function to read an array from file */
int openTable(Olympics *olympcs, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        return 1;
    }

    unsigned int i = 0;
    olympcs->numCountries = 0;

    olympcs->countries = calloc(1, sizeof(Country));
    char buffer[100];
    
    /*read only certain format from file(name a b c), 
    where a,b,c are number of medals. If file has a different format, exit and print error.*/
    while(fgets(buffer, sizeof(buffer), file)!=NULL){
        if(sscanf(buffer, "%s %d %d %d", olympcs->countries[i].name, &olympcs->countries[i].gold,
                                        &olympcs->countries[i].silver, &olympcs->countries[i].bronze) != 4){    
            return 1;
        }
        i++;
        olympcs->numCountries += 1;
        olympcs->countries = realloc(olympcs->countries, (olympcs->numCountries + 2) * sizeof(Country));
        
    }
    fclose(file);
    return 0;
}

/* Function to write current contents of array to a file */
int writeTable(Olympics *olympcs, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
        return 1;

    if (olympcs->numCountries > 0){
        for (unsigned int i = 0; i < olympcs->numCountries; i++){
            int ret = fprintf(file,"%s %d %d %d", olympcs->countries[i].name, olympcs->countries[i].gold, 
                                                olympcs->countries[i].silver, olympcs->countries[i].bronze);
            fprintf(file, "\n");
            if (ret < 0){
                return 1;
            }
        }
    }
    else {
        printf("List is empty!\n");
        return 1;
    }
    fclose(file);
    return 0;
}



/* Compare function that compares countries based on medals. If all medals same, sort alphabetically. */
int compare(const void *c1, const void *c2)
{
    Country *country1 = (Country *) c1;
    Country *country2 = (Country *) c2;

    int goldDiff = country1->gold - country2->gold;
    if (goldDiff != 0){
        return -goldDiff;
    }
    else {
        int silverDiff = country1->silver -country2->silver;
        if (silverDiff != 0){
            return -silverDiff;
        }
        else {
            int bronzeDiff = country1->bronze - country2->bronze;
            if (bronzeDiff != 0){
                return -bronzeDiff;

            }
            else
                return strcmp(country1->name, country2->name);
        }
    }
}

/* Function to add medals to a certain country */
void addMedals(Olympics *olympcs, char *name, int gold, int silver, int bronze)
{
    unsigned int i = 0;
    while (strcmp(olympcs->countries[i].name, name) != 0){      //search the right country "name" from the list.
        i++;
    }
    olympcs->countries[i].gold += gold;
    olympcs->countries[i].silver += silver;
    olympcs->countries[i].bronze += bronze;

}

/* Function to sort and print current contents of array */
void printCountries(Olympics *olympcs)
{
    qsort(olympcs->countries, olympcs->numCountries, sizeof(Country), compare); //sort countries allways before printing
    unsigned int i = 0;
    printf("\n");
    while (i < olympcs->numCountries){
        printf("%s %d %d %d \n",olympcs->countries[i].name, olympcs->countries[i].gold,
                                olympcs->countries[i].silver, olympcs->countries[i].bronze);
        i++; 
    }
    printf("\n");

}

/* Function to add a country to the array */
int initCountry(Olympics *olympcs, char *name)
{
    olympcs->countries = realloc(olympcs->countries, (olympcs->numCountries + 2) * sizeof(Country));

    unsigned int i = 0;
    int inlist = 0;

    while (i < olympcs->numCountries){
        if (!strcmp(name, olympcs->countries[i].name)){     //Check if a country allready in the array
            inlist = 1;
        }
        i++; 
    }
    if (!inlist){
        strcpy(olympcs->countries[i].name, name);
        olympcs->countries[i].gold = 0;
        olympcs->countries[i].silver = 0;
        olympcs->countries[i].bronze = 0;

        olympcs->numCountries += 1;
        return 0;
    }
    else{
        
        return 1;
    }
}

/* Function to process user input and call the corresponding functions */
int doCommand(Olympics *olympcs, char *buf)
{
    char command;
    char name[100];
    int gold = 0;
    int silver = 0;
    int bronze = 0;
    unsigned int i = 0;
    int inlist = 0;

    sscanf(buf, "%c %s %d %d %d",&command, name, &gold, &silver, &bronze);
    if (buf[1] != ' ' && buf[2] != 0){
        printf("Invalid command format, separate command and parameters with a space\n");
        return 0;
    }
    
    switch(command){
        case 'A':           //Add a country to the list
            if (strlen(name) > 20){
                printf("Name too long, please enter a shorter name(<20)\n");
                return 0;
            }
            if (strlen(name) <= 1){
                printf("Please enter a valid country name, name must be longer than 1 character!\n");
                return 0; 
            }   
            if(!initCountry(olympcs, name))
                printf("\"%s\" added successfully to Olympics!\n", name);
            else
                printf("Country \"%s\" allready in Olympics, please enter a new country\n", name);
            
            break;

        case 'M':           //add medals to a country
            if (!olympcs->numCountries){
                printf("List empty! Please add a country first using \"A\"\n");
                return 0;
            }
            while (i < olympcs->numCountries){                  //check if user input "country" in countries
                if (strcmp(olympcs->countries[i].name, name) != 0){
                    i++;
                }
                else{
                    addMedals(olympcs, name, gold, silver, bronze);
                    inlist = 1;
                    break;
                }
            }
            if (inlist)
                printf("Medals updated!\n");
            if (!inlist)
                printf("%s not in Olympics, try a different name\n", name);
            
            break;
            

        case 'L':           //print array of countries and sort
            if (!olympcs->numCountries){
                printf("List empty! Please add a country first using \"A\"\n");
                return 0;
            }
            printCountries(olympcs);
            break;
        
        case 'W':           //save array to file
            if(!writeTable(olympcs, name)){
                printf("Writing succeeded!\n");
            }
            else
                printf("Error in writing!\n");
            
            break;

        case 'O':           //load array from file
            if(!openTable(olympcs, name)){
                printf("Succesfully read table from file!\n");
            }
            else
                printf("Error in reading file!\n");
            
            break;

        case 'Q':          //quit and FREE all
            free(olympcs->countries);
            printf("Exiting program...");
            return 1;
            break;

        default:
            printf("Invalid command, use A, M, L, W, O, Q instead\n");
            break;
    }
    return 0;


}


int main(void)
{
    char buffer[100] = {0};
    Olympics olympics;

    olympics.numCountries = 0;
    olympics.countries = NULL;
    
    int quit = 0;

    //main loop
    while(!quit){
        printf("Enter a command: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL){      
            printf("Enter a valid command!\n");
            continue;
        }
        quit = doCommand(&olympics, buffer);
    }
    return 0;
}