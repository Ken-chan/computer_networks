/* C program for Merge Sort */
#include<stdlib.h>
#include<stdio.h>
#include<ctpl_stl.h>
#include <time.h>
ctpl::thread_pool* p;

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void parMergeSort(int id, void** args)
{
    int* arr = *((int**)args[0]);
    int l = *((int*)args[1]);
    int r = *((int*)args[2]);
    delete[] args;
    if (l < r)
    {
        int m = l+(r-l)/2;

        int _l1 = l;
        int _l2 = m + 1;
        int _r1 = m;
        int _r2 = r;
        void** args1 = new void*[3];
        void** args2 = new void*[3];
        args1[0] = &arr;
        args2[0] = &arr;
        args1[1] = &_l1;
        args2[1] = &_l2;
        args1[2] = &_r1;
        args2[2] = &_r2;
        if (p->n_idle() > 1)
        {
            std::future<void> fut1 = p->push(parMergeSort, args1);
            std::future<void> fut2 = p->push(parMergeSort, args2);
            fut1.wait();
            fut2.wait();

            merge(arr, l, m, r);
        } else
        {

            parMergeSort(-1, args1);
            parMergeSort(-1, args2);

            merge(arr, l, m, r);
        }
    }
}

void mergeSort(int arr[], int l, int r)
{
    p = new ctpl::thread_pool(8);
    if (l < r)
    {
        int m = l+(r-l)/2;

        int _l1 = l;
        int _l2 = m + 1;
        int _r1 = m;
        int _r2 = r;
        void** args1 = new void*[3];
        void** args2 = new void*[3];
        args1[0] = &arr;
        args2[0] = &arr;
        args1[1] = &_l1;
        args2[1] = &_l2;
        args1[2] = &_r1;
        args2[2] = &_r2;
        std::future<void> fut1 = p->push(parMergeSort, args1);
        std::future<void> fut2 = p->push(parMergeSort, args2);
        fut1.wait();
        fut2.wait();

        merge(arr, l, m, r);
    }
    delete p;
}

void printArray(int A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

int main()
{


    int arr[2000000];
    for(int i = 0; i < 2000000; i++)
        arr[i] = std::rand()%100;
    int arr_size = sizeof(arr)/sizeof(arr[0]);

    //printf("Given array is \n");
    //printArray(arr, arr_size);
    const clock_t begin_time = clock();
    mergeSort(arr, 0, arr_size - 1);


    //printf("\nSorted array is \n");
    //printArray(arr, arr_size);
    printf("%f",float( clock () - begin_time ) /  CLOCKS_PER_SEC);
    return 0;
}
