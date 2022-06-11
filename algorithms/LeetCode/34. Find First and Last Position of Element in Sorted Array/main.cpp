#include <vector>
using namespace std;

/**
    Given an array of integers nums sorted in non-decreasing order,
    find the starting and ending position of a given target value.

    If target is not found in the array, return [-1, -1].
    You must write an algorithm with O(log n) runtime complexity.

    Example 1:
    Input: nums = [5,7,7,8,8,10], target = 8
    Output: [3,4]

    Example 2:
    Input: nums = [5,7,7,8,8,10], target = 6
    Output: [-1,-1]

    Example 3:
    Input: nums = [], target = 0
    Output: [-1,-1]
 * */

class Solution
{
    public:
        vector<int> searchRange(vector<int> &nums, int target)
        {
            vector<int> res = {-1, -1};

            if (nums.empty())
                return (res);

            int low = 0, high = nums.size() - 1, mid;
            /** поиск левой грани */
            while (low <= high)
            {
                mid = (low + high) / 2;
                if (nums[mid] == target)
                {
                    // если мы в самом начале или предыдущее число не равно искомому
                    if (mid == 0 || nums[mid - 1] != target)
                        break;
                    else // иначе ставим правую грань на текущую позицию
                        high = mid;
                }
                else if (target < nums[mid]) // деление вдвое в левую сторону
                    high = mid - 1;
                else  // деление вдвое в правую сторону
                    low = mid + 1;
            }
            if (low > high) // число не найдено
                return (res);

            res[0] = mid;
            low = mid, high = nums.size() - 1;

            /** поиск правой грани */
            while (low <= high)
            {
                mid = (low + high) / 2;
                if (nums[mid] == target)
                {
                    // если мы в самом конце или следующее число не равно искомому
                    if (mid == nums.size() - 1 || nums[mid + 1] != target)
                    {
                        break;
                    }
                    else
                        low = mid + 1; // деление вдвое в правую сторону
                }
                else
                    high = mid - 1; // деление вдвое в левую сторону
            }

            res[1] = mid;
            return (res);
        }
};