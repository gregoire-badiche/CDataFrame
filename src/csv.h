#ifndef CSV_H
#define CSV_H

char ***load_csv(char *filename, int **widths, int *height);

/**
 * @brief Free the memory allocated by `load_csv()`
 * @param pos A pointer to the result of `load_csv()`
 * @param height The height given by `load_csv()`
*/
void free_load_csv(char ****pos, int height);

char **split(char *str, char delimiter, int *sections);

/**
 * @brief Free the memory allocated by `split()`
 * @param pos A pointer to the result of the `split()` function
*/
void free_split(char ***pos);

#endif