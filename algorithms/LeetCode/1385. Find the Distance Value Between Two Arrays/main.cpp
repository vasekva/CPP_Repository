#include <vector>
using namespace std;

/*
 * Есть два массива интов arr1 и arr2 и целоцисленное значение d.
 * Необходимо вернуть значение расстояния между двумя массивами.
 *
 * Расстояние между массивами - кол-во чисел из arr1,
 * для которых модуль разницы со всеми числами из arr2 больше значения переменной d.
 *
    Принимаемые значения: arr1 = [4,5,8], arr2 = [10,9,1,8], d = 2
    Возвращаемое значение функции: 2

    Объяснение:
    Для arr1[0]=4 мы имеем:     Для arr1[1]=5 мы имеем:         Для arr1[2]=8 мы имеем:
    |4-10|=6 > d=2              |5-10|=5 > d=2                  |8-10|=2 <= d=2
    |4-9|=5 > d=2               |5-9|=4 > d=2                   |8-9|=1 <= d=2
    |4-1|=3 > d=2               |5-1|=4 > d=2                   |8-1|=7 > d=2
    |4-8|=4 > d=2               |5-8|=3 > d=2                   |8-8|=0 <= d=2

    res++;                      res++;                          есть результат, который <= d,
                                                                поэтому мы не делаем res++;
    return(2);
 */

class Solution
{
    public:
        int find_the_distance_value(vector<int> &arr1, vector<int> &arr2, int d)
        {
            sort(arr2.begin(), arr2.end());

            int res = 0;
            int tmp_dist = 0;
            for (int i = 0; i < arr1.size(); i++)
            {
                int low = 0, high = arr2.size() - 1, mid;
                int flag = 0;

                while (low <= high)
                {
                    mid = (high + low) / 2;

                    tmp_dist = abs(arr1[i] - arr2[mid]);

                    if (tmp_dist <= d)
                    {
                        flag = 1;
                        break;
                    }
                    else if (arr1[i] < arr2[mid])
                        high = mid - 1;
                    else
                        low = mid + 1;
                }
                if (!flag)
                    ++res;
            }
            return (res);
        }
};