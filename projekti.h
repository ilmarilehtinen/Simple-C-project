#ifndef PROJEKTI_H
#define PROJEKTI_H


// struct for each country
typedef struct {
    char name[21];
    unsigned int gold;
    unsigned int silver;
    unsigned int bronze;
} Country;

//struct of olympics
typedef struct {
    Country *countries;             //dynamic array of countries in olympics
    unsigned int numCountries;

} Olympics;

#endif