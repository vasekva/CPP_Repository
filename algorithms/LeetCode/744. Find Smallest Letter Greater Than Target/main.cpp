#include <vector>
using namespace std;

/**
    Given a characters array letters that is sorted in non-decreasing order and a character target,
    return the smallest character in the array that is larger than target.

    Note that the letters wrap around.
    For example, if target == 'z' and letters == ['a', 'b'], the answer is 'a'.

    Example 1:
    Input: letters = ["c","f","j"], target = "a"
    Output: "c"

    Example 2:
    Input: letters = ["c","f","j"], target = "c"
    Output: "f"

    Example 3:
    Input: letters = ["c","f","j"], target = "d"
    Output: "f"
*/

class Solution
{
    public:
        char next_greatest_letter(vector<char> &letters, char target)
        {
            int low = 0, high = letters.size() - 1, mid;

            while (low <= high)
            {
                mid = (low + high) / 2;
                if (target < letters[mid])
                {
                    // если мы в левом конце или значение левее не больше искомого
                    if (mid == 0 || (mid != 0 && letters[mid - 1] <= target))
                        break;
                    else
                        high = mid - 1; // иначе сдвигаемся на половину влево
                }
                else // иначе сдвигаемся на половину вправо
                    low = mid + 1;
            }
            if (mid == letters.size() - 1 && letters[mid] <= target) // если в итоге оказались в правом конце
                mid = 0;
            return (letters[mid]);
        }
};