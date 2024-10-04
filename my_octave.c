#include <stdio.h>
#include <stdlib.h>
#define mod 10007

void matrix_array(int ****mat_array, int *count, int **rows, int **columns)
{
	int m, n, i, j;
	scanf("%d%d", &m, &n);
	(*count)++;
	// count-ul reprezinta numarul de matrici;
	(*rows) = (int *)realloc(*rows, (*count) * sizeof(int));
	if ((*rows) == NULL) {
		fprintf(stderr, "Realloc failed\n");
		return;
	}
	(*columns) = (int *)realloc(*columns, (*count) * sizeof(int));
	if ((*columns) == NULL) {
		fprintf(stderr, "Realloc failed\n");
		return;
	}
	(*rows)[(*count) - 1] = m;
	(*columns)[(*count) - 1] = n;
	// aloc memorie pentru noua matrice
	(*mat_array) = (int ***)realloc((*mat_array), (*count) * sizeof(int **));
	if ((*mat_array) == NULL) {
		// daca nu pot aloca opresc executia si afisez un mesaj;
		fprintf(stderr, "Realloc for matrix failed\n");
		return;
	}
	(*mat_array)[(*count) - 1] = (int **)malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
		(*mat_array)[(*count) - 1][i] = (int *)malloc((n) * sizeof(int));
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			scanf("%d", &(*mat_array)[(*count) - 1][i][j]);
}

void print_matrix(int n, int m, int **mat)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}
}

void multiply(int ****mat_array, int *count, int **rows, int **columns)
{
	int index1, index2;
	scanf("%d%d", &index1, &index2);

	if (index1 >= *count || index2 >= *count || index1 < 0 || index2 < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	if ((*columns)[index1] != (*rows)[index2]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	// Alocare memorie pentru matricea rezultat
	int result_rows = (*rows)[index1];
	int result_columns = (*columns)[index2];
	int **result_matrix = (int **)malloc(result_rows * sizeof(int *));
	for (int i = 0; i < result_rows; i++)
		result_matrix[i] = (int *)malloc(result_columns * sizeof(int));

	// Calculul produsului matricilor
	for (int i = 0; i < result_rows; i++) {
		for (int j = 0; j < result_columns; j++) {
			result_matrix[i][j] = 0;
				for (int k = 0; k < (*columns)[index1]; k++) {
					result_matrix[i][j] += (*mat_array)[index1][i][k] *
										(*mat_array)[index2][k][j] % mod;
					result_matrix[i][j] = ((result_matrix[i][j] % mod) + mod)
											% mod;
				}
		}
	}

	// Adăugare matrice rezultat în colecție
	(*count)++;
	*rows = (int *)realloc(*rows, (*count) * sizeof(int));
	*columns = (int *)realloc(*columns, (*count) * sizeof(int));
	(*rows)[(*count) - 1] = result_rows;
	(*columns)[(*count) - 1] = result_columns;
	*mat_array = (int ***)realloc(*mat_array, (*count) * sizeof(int **));
	(*mat_array)[(*count) - 1] = result_matrix;
}

void swap_matrix(int ***mat_1, int ***mat_2)
{
	int **aux = (*mat_1);
	(*mat_1) = (*mat_2);
	(*mat_2) = (aux);
}

void transposed(int n, int m, int ***mat)
{
	int **aux;
	int i, j;

	aux = (int **)malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
		aux[i] = (int *)malloc(n * sizeof(int));
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			aux[j][i] = (*mat)[i][j];
	// eliberez matricea originala
	for (i = 0; i < n; i++)
		free((*mat)[i]);
	free((*mat));

	(*mat) = (int **)malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
		(*mat)[i] = (int *)malloc(n * sizeof(int));

	// copiez matricea auxiliara in matricea originala
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			(*mat)[i][j] = aux[i][j];

	// eliberez matricea auxiliara
	for (i = 0; i < m; i++)
		free(aux[i]);
	free(aux);
}

int sum_elements(int ***mat, int n, int m)
{
	int i, j, sum = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++) {
			sum = (sum + (*mat)[i][j]) % mod;
			sum = ((sum % mod) + mod) % mod;
		}

	return sum;
}

void sorting(int ****mat, int *count, int **rows, int **columns)
{
	for (int i = 0; i < (*count) - 1; i++) {
		for (int j = i + 1; j < (*count); j++) {
			if (sum_elements(&(*mat)[i], (*rows)[i], (*columns)[i])
				> sum_elements(&(*mat)[j], (*rows)[j], (*columns)[j])) {
				swap_matrix(&(*mat)[i], &(*mat)[j]);
				int aux = (*rows)[i];
				(*rows)[i] = (*rows)[j];
				(*rows)[j] = aux;
				aux = (*columns)[i];
				(*columns)[i] = (*columns)[j];
				(*columns)[j] = aux;
			}
		}
	}
}

void size(int count, int *rows, int *columns)
{
	int index;
	scanf("%d", &index);
	if (index >= count || index <= -1)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", rows[index], columns[index]);
}

void resize(int ****mat, int **rows, int **columns, int *count)
{
	int index, newrows, newcolumns;
	scanf("%d", &index);

	scanf("%d", &newrows);
	int newroworder[newrows];
	for (int i = 0; i < newrows; i++)
		scanf("%d", &newroworder[i]);

	scanf("%d", &newcolumns);
	int newcolumnorder[newcolumns];
	for (int i = 0; i < newcolumns; i++)
		scanf("%d", &newcolumnorder[i]);

	if (index < 0 || index >= *count) {
		printf("No matrix with the given index\n");
		return;
	}

	int **matrix = (*mat)[index];
	int **resizedmatrix = (int **)malloc(newrows * sizeof(int *));
	for (int i = 0; i < newrows; i++) {
		resizedmatrix[i] = (int *)malloc(newcolumns * sizeof(int));
		for (int j = 0; j < newcolumns; j++)
			resizedmatrix[i][j] = matrix[newroworder[i]][newcolumnorder[j]];
	}

	// eliberez memoria matricei vechi
	for (int i = 0; i < (*rows)[index]; i++)
		free(matrix[i]);
	free(matrix);

	(*rows)[index] = newrows;
	(*columns)[index] = newcolumns;
	(*mat)[index] = resizedmatrix;
}

void free_mat(int ***mat_array, int *rows, int index, int *nr)
{
	int **matrix = mat_array[index];
	for (int i = 0; i < rows[index]; i++)
		free(matrix[i]);
	free(matrix);
	(*nr)--;
}

void release(int ****mat_array, int **rows, int **columns, int *count)
{
	// eliberam colectia
	int i, j;
	for (i = 0; i < (*count); i++) {
		for (j = 0; j < (*rows)[i]; j++)
			free((*mat_array)[i][j]);
		free((*mat_array)[i]);
	}
	free(*mat_array);
	// eliberam vectorii de dimensiuni
	free(*rows);
	free(*columns);
	(*count) = 0;
}

int main(void)
{
	char oper_letter;
	int m, n, nr = -1, index, index1, index2, count = 0;
	int *rows, *columns, ***mat_array;
	mat_array = (int ***)malloc(1 * sizeof(int **));
	rows = (int *)malloc(1 * sizeof(int));
	columns = (int *)malloc(1 * sizeof(int));
	while (1) {
		scanf(" %c", &oper_letter);
		if (oper_letter == 'L' || oper_letter == 'P' || oper_letter == 'D' ||
			oper_letter == 'Q' || oper_letter == 'T' || oper_letter == 'F' ||
			oper_letter == 'M' || oper_letter == 'O'	|| oper_letter == 'C') {
			if (oper_letter == 'L')
				matrix_array(&mat_array, &count, &rows, &columns);
			if (oper_letter == 'P') {
				scanf("%d", &index);
				if (index >= count || index <= -1)
					printf("No matrix with the given index\n");
				else
					print_matrix(rows[index], columns[index], mat_array[index]);
			}
			if (oper_letter == 'D')
				size(count, rows, columns);
			if (oper_letter == 'T') {
				int aux;
				scanf("%d", &index);
				if (index >= count || index <= -1)
					printf("No matrix with the given index\n");
				else
					transposed(rows[index], columns[index], &mat_array[index]);
				aux = rows[index];
				rows[index] = columns[index];
				columns[index] = aux;
			}
			if (oper_letter == 'O')
				sorting(&mat_array, &count, &rows, &columns);
			if (oper_letter == 'F') {
				scanf("%d", &index);
				if (index >= count || index < 0) {
					printf("No matrix with the given index\n");
				} else {
					// free_mat(mat_array[index], rows, columns, index,&nr);
					for (int i = index + 1; i < count; i++) {
						swap_matrix(&mat_array[i - 1], &mat_array[i]);
						int aux = rows[i];
						rows[i] = rows[i - 1];
						rows[i - 1] = aux;
						aux = columns[i];
						columns[i] = columns[i - 1];
						columns[i - 1] = aux;
					}
					free_mat(mat_array, rows, count - 1, &count);
					}
			}
			if (oper_letter == 'M')
				multiply(&mat_array, &count, &rows, &columns);
			if (oper_letter == 'C')
				resize(&mat_array, &rows, &columns, &count);
			if (oper_letter == 'Q') {
				release(&mat_array, &rows, &columns, &count);
				break;
			}
		} else {
			printf("Unrecognized command\n");
		}
	}
}
