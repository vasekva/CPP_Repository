/**
	Given a sorted array of distinct integers and a target value, return the index if the target is found.
 	If not, return the index where it would be if it were inserted in order.
	You must write an algorithm with O(log n) runtime complexity.

	Example 1:
	Input: nums = [1,3,5,6], target = 5
	Output: 2

	Example 2:
	Input: nums = [1,3,5,6], target = 2
	Output: 1

	Example 3:
	Input: nums = [1,3,5,6], target = 7
	Output: 4

*/

class Solution
{
	public:
		int search_insert(vector<int> &nums, int target)
		{
			int high = nums.size() - 1, low = 0, mid = (high + low) / 2;

			if (nums.size() == 0)
				return (0);

			while (low <= high)
			{
				if (nums[mid] == target)
					return (mid);
				else if (target > nums[mid])
					low = mid + 1;
				else
					high = mid - 1;

				mid = (high + low) / 2;
			}

			if (mid == 0 && nums[0] > target)
				return (0);
			else
				++mid;
			return (mid);
		}
};