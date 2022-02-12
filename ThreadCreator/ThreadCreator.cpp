#include <iostream>
#include <windows.h>

typedef struct Array {
    int size;
    int* array;
    int max_index;
    int min_index;
    double average;
    /*~Array() {
        delete[] array;
        array = NULL;
        std::cout << "destructor worked";
    }*/
}*PARRAY;

DWORD WINAPI FindMinMaxAndPrint(LPVOID param) {
    PARRAY array = (PARRAY)param;
    int max = array->array[0];
    int min = array->array[0];

    for (size_t i = 0; i < array->size; i++)
    {
        if (max < array->array[i])
        {
            max = array->array[i];
            array->max_index = i;
            Sleep(7);
            continue;
        }

        if (min > array->array[i])
        {
            min = array->array[i];
            array->min_index = i;
        }

        Sleep(7);
    }

    std::cout << "Min: " << min << "\nMax: " << max << std::endl;
    return 0;
}

DWORD WINAPI FindAverageAndPrint(LPVOID param) {
    PARRAY array = (PARRAY)param;
    
    double sum = 0;

    for (size_t i = 0; i < array->size; i++)
    {
        sum += array->array[i];
        Sleep(12);
    }
    double average = sum / array->size;
    array->average = average;
    std::cout << "Average: " << average << std::endl;
    return 0;
}

int main()
{
    int numberAmount;
    std::cout << "Hello! Enter the amount of integers in array: ";
    std::cin >> numberAmount;
    std::cout << "Now enter array elements.\n";

    //will be deallocated in a struct
    int* array = new int[numberAmount];

    DWORD max_min_thread_id;
    
    for (size_t i = 0; i < numberAmount; i++)
    {
        std::cin >> array[i];
    }

    PARRAY parametr = (PARRAY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PARRAY));
    parametr->size = numberAmount;
    parametr->array = array;

    HANDLE min_maxThreadHandle = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        FindMinMaxAndPrint,       // thread function name
        (PARRAY)parametr,          // argument to thread function 
        0,                      // use default creation flags 
        &max_min_thread_id);   // returns the thread identifier 

    if (min_maxThreadHandle == NULL) {
        std::cerr << "Min max creation failed";
        return 1;
    }

    WaitForSingleObject(min_maxThreadHandle, INFINITE);

    CloseHandle(min_maxThreadHandle);

    

    DWORD average_thread_id;

    HANDLE averageThreadHandle = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        FindAverageAndPrint,       // thread function name
        (PARRAY)parametr,          // argument to thread function 
        0,                      // use default creation flags 
        &average_thread_id);   // returns the thread identifier 


    if (averageThreadHandle == NULL) {
        std::cerr << "Average Thread creation failed.\n";
        return 1;
    }

    WaitForSingleObject(averageThreadHandle, INFINITE);

    CloseHandle(averageThreadHandle);

    parametr->array[parametr->max_index] = parametr->average;
    parametr->array[parametr->max_index] = parametr->average;

    for (size_t i = 0; i < parametr->size; i++)
    {
        std::cout << parametr->array[i] << " ";
    }
    //HeapFree(GetProcessHeap(), 0, parametr);
    return 0;
}