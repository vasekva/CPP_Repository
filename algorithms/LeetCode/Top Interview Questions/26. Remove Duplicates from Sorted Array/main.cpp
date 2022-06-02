/**
	Example 2:

	Input: nums = [0,0,1,1,1,2,2,3,3,4]
	Output: 5, nums = [0,1,2,3,4,_,_,_,_,_]
	Explanation: Your function should return k = 5,
 	with the first five elements of nums being 0, 1, 2, 3, and 4 respectively.
	It does not matter what you leave beyond the returned k (hence they are underscores).

 */

class Solution
{
	public:
		int remove_duplicates(vector<int> &nums)
		{
			auto delimeter = std::unique(begin(nums), end(nums)); // переместить повторяющиеся элементы в конец

			nums.erase(delimeter, nums.end()); // удалить все элементы начиная с разделителя !(не уменьшает занимаемый capacity())
			nums.shrink_to_fit(); // уменьшить capacity() вектора до его size()

			return (nums.size());
		}
};

/**
 * First solution

class Solution
{
	public:
		int remove_duplicates(vector<int> &nums)
		{
			vector<int>::iterator bgn = nums.begin();

			bool cnt_flag = false;
			vector<int>::iterator fst;
			while (bgn != nums.end())
			{
				if (bgn + 1 != nums.end() && *bgn == *(bgn + 1))
				{
					fst = bgn + 1;
					while (fst != nums.end() && *fst == *bgn)
						fst++;
					nums.erase(bgn + 1, fst);
				}
				bgn++;
			}
			return (nums.size());
		}
};
*/