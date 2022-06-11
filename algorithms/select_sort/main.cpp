#include <iostream>
#include <vector>

using namespace std;

int32_t get_min_ind(vector<int32_t> &arr)
{
    int32_t min_elem = arr[0];
    size_t min_ind = 0;

    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] < min_elem)
        {
            min_elem = arr[i];
            min_ind = i;
        }
    }
    return (min_ind);
}

vector<int32_t> select_sort(vector<int32_t> &arr)
{
    if (arr.empty())
        return (arr);

    vector<int32_t> new_vector;
    size_t min_ind = 0;

    while (!arr.empty())
    {
        min_ind = get_min_ind(arr);
        new_vector.push_back(arr[min_ind]);
        arr.erase(arr.begin() + min_ind);
    }
    return (new_vector);
}

int main()
{
    vector<int32_t> arr = {1, 0, -20, 3, 13, 82, -13};

    vector<int32_t> new_vector = select_sort(arr);

    for (const auto &c : new_vector)
        std::cout << c << " ";
    std::cout << std::endl;

    return (0);
}