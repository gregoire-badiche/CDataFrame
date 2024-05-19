#include <stdio.h>
#include "column.h"
#include "cdataframe.h"
#include "sort.h"
#include "csv.h"
#include "list.h"
#include "sort.h"

typedef struct _person
{
    char *name;
    char *surname;
} PERSON;

typedef struct _address
{
    int number;
    char *street;
    int postalcode;
    char *city;
} ADDRESS;

typedef struct _date
{
    int day;
    int month;
    int year;
} DATE;

typedef struct _personnal_informations
{
    float money;
    char genre;
    PERSON name;
    ADDRESS address;
    DATE birthday;
} PERSONNAL_INFORMATIONS;

int main(int argc, char const *argv[])
{
    ENUM_TYPE t[11] =  {INT, CHAR, STRING, STRING, INT, STRING, INT, STRING, INT, INT, INT};
    CDATAFRAME *cdf = load_from_csv_fa("/home/gregoire/Development/CDataFrame/data.csv", t, 11);
    ADDRESS *johndoeaddress;
    johndoeaddress = (ADDRESS *) get_element(cdf, 3)->data->data[0];
    printf("John Doe lives in street %s, in city %s\n\n", johndoeaddress->street, johndoeaddress->city);

    printf("Before sorting :\n");

    display_cdataframe(cdf, -1, 20);
    

    int m = 15054;
    char g = 'f';
    PERSON n = {"Alicia", "Barry"};
    ADDRESS a = {91, "rue de Rivoli", 75001, "Paris"};
    DATE bd = {24, 11, 1976};
    sort(get_element(cdf, 0)->data, DESC);

    insert_value(get_element(cdf, 0)->data, &m);
    insert_value(get_element(cdf, 1)->data, &g);
    insert_value(get_element(cdf, 2)->data, &n);
    insert_value(get_element(cdf, 3)->data, &a);
    insert_value(get_element(cdf, 4)->data, &bd);

    sort(get_element(cdf, 0)->data, DESC);

    printf("\nAfter sorting and adding an element :\n");
    
    display_cdataframe_by_index(cdf, get_element(cdf, 0)->data->index, -1, 20);

    save_into_csv(cdf, "/home/gregoire/Development/CDataFrame/data2.csv");

    return 0;
}
