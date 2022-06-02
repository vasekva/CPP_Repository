class Solution
{
	public:
		int search(vector<int> &nums, int target)
		{
			int res_ind = -1;

			int low = 0, high = nums.size() - 1, mid = (low + high) / 2;

			while (low <= high)
			{
				if (target == nums[mid])
					return (mid);
				else if (target > nums[mid])
					low = mid + 1;
				else
					high = mid - 1;
				mid = (low + high) / 2;
			}

			return (res_ind);
		}
};