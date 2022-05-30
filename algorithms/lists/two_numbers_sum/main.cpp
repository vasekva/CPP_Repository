/**
 * Суть задания вычислить сумму двух чисел, которые являются перевернутым списком значений,
 * после чего вернуть новый список, являющийся перевернутой суммой чисел.
 *
 *
 * Пример:
 * 2 -> 3 -> 5
 * 4 -> 5 -> 5
 *
 * return 6 -> 8 -> 0 -> 1
 *
 * */

/**
 * Definition for singly-linked list.
 * struct ListNode
 * {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution
{
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
        {
            ListNode *node = new ListNode();
            ListNode *curr = node;

            int carry = 0;
            while (l1 || l2)
            {
                int x = (l1 != nullptr) ? l1->val : 0;
                int y = (l2 != nullptr) ? l2->val : 0;
                int sum = carry + x + y;
                carry = sum / 10;
                curr->next = new ListNode(sum % 10);
                curr = curr->next;
                if (l1 != nullptr) l1 = l1->next;
                if (l2 != nullptr) l2 = l2->next;
            }
            if (carry > 0)
                curr->next = new ListNode(carry);

            return (node->next);
        }
};