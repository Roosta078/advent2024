#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 2000

int get_data(char* input_file, int* list1, int* list2);
char* get_cli_input(int argc, char** argv);
int get_distance(int* list1, int* list2, int elements);
int get_similarity(int* list1, int* list2, int elements);

int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int main(int argc, char** argv){
    char* filename = get_cli_input(argc, argv);
    if (filename == NULL){
        return EXIT_FAILURE;
    }
    int* list1;
    int* list2;
    list1 = (int*)malloc(sizeof(int)*MAX_ELEMENTS);
    list2 = (int*)malloc(sizeof(int)*MAX_ELEMENTS);
    int elements = get_data(filename, list1, list2);
    if (elements <= 0){
        printf("no elements found\n");
        free(list1);
        free(list2);
        return 0;
    }

    qsort(list1, elements, sizeof(int), compare_ints);
    qsort(list2, elements, sizeof(int), compare_ints);
    printf("distance: %d\n", get_distance(list1, list2, elements));
    printf("similarity: %d\n", get_similarity(list1, list2, elements));
    free(list1);
    free(list2);
    return 0;
}


char* get_cli_input(int argc, char** argv){
    if (argc != 2){
        printf("incorrect input\n");
        return NULL;
    }
    return argv[1];
}

int get_data(char* input_file, int* list1, int* list2){
    FILE * fp;
    fp = fopen(input_file, "r");
    if (fp == NULL){
        return 0;
    }
    int n1, n2, elements;
    elements = 0;
    while(!feof(fp) && elements < MAX_ELEMENTS){
        fscanf(fp, "%d   %d", &n1, &n2);
        list1[elements] = n1;
        list2[elements] = n2;
        elements++;
    }
    if (elements == MAX_ELEMENTS){
        printf("WARNING: input has too many elements. Only reading first %d elements\n", MAX_ELEMENTS);
    }
    fclose(fp);
    return elements;

}

int get_distance(int* list1, int* list2, int elements){
    int distance = 0;
    for (int i = 0; i < elements; i++){
        distance += abs(list1[i]-list2[i]);
    }
    return distance;
}

int get_similarity(int* list1, int* list2, int elements){
    int l1_pos = 0;
    int l2_pos = 0;
    int similarity = 0;
    while ((l1_pos < elements) && (l2_pos < elements)){
        if (list1[l1_pos] > list2[l2_pos]){
            l2_pos++;
        } else if (list1[l1_pos] < list2[l2_pos]){
            l1_pos++;
        } else {
            int l1_cnt = 1;
            int l2_cnt = 1;
            int target = list1[l1_pos];
            l1_pos++;
            while(l1_pos < elements){
                if (list1[l1_pos] == target){
                    l1_cnt++;
                    l1_pos++;
                } else {
                    break;
                }
            }
            l2_pos++;
            while(l2_pos < elements){
                if (list2[l2_pos] == target){
                    l2_cnt++;
                    l2_pos++;
                } else {
                    break;
                }
            }
            similarity += l1_cnt*l2_cnt*target;
        }
    }
    return similarity;
}