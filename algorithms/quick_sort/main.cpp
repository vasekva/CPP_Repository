using namespace std;
#include <algorithm>
#include <iostream>

int get_pivot_point(int arr[], int bgn, int end)
{
    int pivot = arr[bgn];
    int count = 0;

    for (int i = bgn + 1; i <= end; i++)
    {
        if (arr[i] <= pivot)
            count++;
    }

    int pivot_point = bgn + count;
    swap(arr[bgn], arr[pivot_point]);

    int i = bgn, j = end;
    while (i < pivot_point && j > pivot_point)
    {
        while (arr[i] <= pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i < pivot_point && j > pivot_point)
            swap(arr[i++], arr[j--]);
    }
    return (pivot_point);
}

void quick_sort(int arr[], int bgn, int end)
{
    if (bgn >= end)
        return;

    int delimeter = get_pivot_point(arr, bgn, end);
    quick_sort(arr, bgn, delimeter - 1);
    quick_sort(arr, delimeter + 1, end);
}

int main()
{
    int arr[] = {1, 0, -20, 3, 13, 82, -13};
    int size = 7;

    quick_sort(arr, 0, size - 1);

    for (auto c : arr)
        std::cout << c << " ";

    return 0;
}