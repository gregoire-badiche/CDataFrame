#include <stdlib.h>
#include <stdio.h>

int charcount(FILE *const f)
{
    char c;
    int count = 0;
    while (1)
    {
        c = fgetc(f);
        if (c == EOF || c == '\n')
            break;
        count++;
    }

    return count;
}

int scfind(FILE *const f)
{
    int c, count = 0;
    while (1)
    {
        c = fgetc(f);
        if (c == EOF || c == '\n')
        {
            break;
        }
        if (c == ';')
            count++;
    }
    return count;
}

int linecount(FILE *const f)
{
    int c, count = 0;
    while (1)
    {
        c = fgetc(f);
        if (c == EOF)
        {
            count++;
            break;
        }
        if (c == '\n')
            count++;
    }
    return count;
}

char ***load_csv(char *filename, int **widths, int *height)
{
    FILE *f = fopen(filename, "rt");
    if (f == NULL)
    {
        printf("Impossible to open file %d\n", filename);
        exit(0);
    }
    int const lines = linecount(f);
    fseek(f, 0, SEEK_SET);
    char ***data = (char ***)malloc(lines * sizeof(char **));
    int *sccounts = (int *)malloc(sizeof(int) * lines);
    int ccounts[lines];
    for (int i = 0; i < lines; i++)
    {
        sccounts[i] = scfind(f);
    }
    fseek(f, 0, SEEK_SET);
    for (int i = 0; i < lines; i++)
    {
        ccounts[i] = charcount(f);
    }
    fseek(f, 0, SEEK_SET);
    for (int i = 0; i < lines; i++)
    {
        int const chars = ccounts[i];
        char *str = (char *)malloc(sizeof(char) * (chars + 1));
        fgets(str, chars + 1, f);
        fgetc(f);
        int const sc = sccounts[i] + 1;
        data[i] = (char **)malloc(sizeof(char *) * sc);
        char *ptr = str;
        int k = 0;
        int j = 0;
        for (j = 0; j < chars; j++)
        {
            if (str[j] == ';')
            {
                if (j != 0)
                    if (str[j - 1] == '\\')
                        continue;
                data[i][k] = ptr;
                str[j] = '\0';
                ptr = &str[++j];
                k++;
            }
        }
        data[i][k] = ptr;
        str[j] = '\0';
    }
    for (int i = 0; i < lines; i++)
        sccounts[i]++;
    *widths = sccounts;
    *height = lines;
    fclose(f);
    return data;
}

void free_load_csv(char ****pos, int height)
{
    for (int i = 0; i < height; i++)
    {
        free((*pos)[i][0]);
        free((*pos)[i]);
    }
    free(*pos);
    *pos = NULL;
}

char **split(char *str, char delimiter, int *sections)
{
    char **res;
    *sections = 1;
    int i = 0;
    while(str[i] != '\0')
    {
        if(str[i] == delimiter)
            (*sections)++;
        i++;
    }
    res = (char **)malloc(sizeof(char *) * *sections);
    i = 0;
    int c = 1;
    int p = 0;
    res[0] = str;
    while (str[i] != '\0')
    {
        if(str[i] == delimiter)
        {
            if(i != 0)
                if(str[i - 1] == '\\')
                    continue;
            str[i] = '\0';
            res[c] = &str[i] + 1;
            c++;
        }
        i++;
    }
    return res;
}

void free_split(char ***pos)
{
    free(*pos);
    *pos = NULL;
}
