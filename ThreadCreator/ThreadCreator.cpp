#include <iostream>
#include <windows.h>
#include <mutex>

typedef struct Array {
    int size = 0;
    int* array = NULL;
    int max_index = 0;
    int min_index = 0;
    double average = 0;
};

std::mutex mutex;

DWORD WINAPI FindMinMaxAndPrint(LPVOID param) {
    Array* array = (Array*)(param);
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

    mutex.lock();
    std::cout << "Min: " << min << "\nMax: " << max << std::endl;
    mutex.unlock();

    return 0;
}

DWORD WINAPI FindAverageAndPrint(LPVOID param) {
    Array* array = (Array*)param;
    
    double sum = 0;

    for (size_t i = 0; i < array->size; i++)
    {
        sum += array->array[i];
        Sleep(12);
    }
    double average = sum / array->size;
    array->average = average;

    mutex.lock();
    std::cout << "Average: " << average << std::endl;
    mutex.unlock();

    return 0;
}

int main()
{
    int numberAmount;
    std::cout << "Hello! Enter the amount of integers in array: ";
    std::cin >> numberAmount;
    std::cout << "Now enter array elements.\n";

    int* array = new int[numberAmount];

    DWORD max_min_thread_id;
    
    for (size_t i = 0; i < numberAmount; i++)
    {
        std::cin >> array[i];
    }

    Array* parametr = new Array;
    parametr->size = numberAmount;
    parametr->array = array;

    HANDLE min_maxThreadHandle = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        FindMinMaxAndPrint,       // thread function name
        (Array*)parametr,          // argument to thread function 
        0,                      // use default creation flags 
        &max_min_thread_id);   // returns the thread identifier 

    if (min_maxThreadHandle == NULL) {
        std::cerr << "Min max creation failed";
        return 1;
    } 

    DWORD average_thread_id;

    HANDLE averageThreadHandle = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        FindAverageAndPrint,       // thread function name
        (Array*)parametr,          // argument to thread function 
        0,                      // use default creation flags 
        &average_thread_id);   // returns the thread identifier 


    if (averageThreadHandle == NULL) {
        std::cerr << "Average Thread creation failed.\n";
        return 1;
    }

    WaitForSingleObject(averageThreadHandle, INFINITE);
    WaitForSingleObject(min_maxThreadHandle, INFINITE);

    CloseHandle(averageThreadHandle);
    CloseHandle(min_maxThreadHandle);

    parametr->array[parametr->max_index] = parametr->average;
    parametr->array[parametr->min_index] = parametr->average;

    for (size_t i = 0; i < parametr->size; i++)
    {
        std::cout << parametr->array[i] << " ";
    }

    delete[] array;
    delete parametr;
    return 0;
}