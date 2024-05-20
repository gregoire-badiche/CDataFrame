#include <stdio.h>
#include <stdlib.h>
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
    ENUM_TYPE t[11] =  {FLOAT, CHAR, STRING, STRING, INT, STRING, INT, STRING, INT, INT, INT};
    CDATAFRAME *cdf = load_from_csv_fa("/home/gregoire/Development/CDataFrame/data.csv", t, 11);
    ADDRESS *johndoeaddress;
    johndoeaddress = (ADDRESS *) get_element(cdf, 3)->data->data[0];
    printf("John Doe lives in street %s, in city %s\n\n", johndoeaddress->street, johndoeaddress->city);

    printf("Before sorting :\n");

    display_cdataframe(cdf, -1, 20);
    
    sort(get_element(cdf, 0)->data, DESC);

    float m = 15054.15;
    char g = 'f';
    PERSON n = {"Alicia", "Barry"};
    ADDRESS a = {91, "rue de Rivoli", 75001, "Paris"};
    DATE bd = {24, 11, 1976};

    void **data = malloc(sizeof(void **) * 5);
    data[0] = &m;
    data[1] = &g;
    data[2] = &n;
    data[3] = &a;
    data[4] = &bd;

    add_row(cdf, data);

    free(data);

    sort(get_element(cdf, 0)->data, ASC);

    printf("\nAfter sorting and adding an element :\n");
    
    display_cdataframe_by_index(cdf, get_element(cdf, 0)->data->index, -1, 20);

    delete_column(cdf, 3);
    delete_row(cdf, 1);
    printf("\nThe dataframe after deleting the address column, and the 2nd row (Alicia Barry)\n");
    display_cdataframe_by_index(cdf, get_element(cdf, 0)->data->index, -1, 20);

    char f = 'f';

    printf("\nIn the dataframe, there is %d women over %d persons\n", are_occurences_of(get_element(cdf, 1)->data, &f), get_cdataframe_cols_size(cdf));

    PERSON p2 = {"Emily", "Burgh"};
    PERSON p3 = {"Eric", "Zing"};

    sort(get_element(cdf, 2)->data, DESC);

    printf("Emily Burgh is indexed at %d, and", search_value_in_column(get_element(cdf, 2)->data, &p2));
    printf(" Eric Zing is indexed at %d.\n", search_value_in_column(get_element(cdf, 2)->data, &p3));

    save_into_csv(cdf, "/home/gregoire/Development/CDataFrame/data2.csv");

    return 0;
}
