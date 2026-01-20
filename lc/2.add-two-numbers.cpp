// @leet start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

class Solution {
public:
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		int m = 0;
		ListNode * n = nullptr;
		ListNode * _n;

		while (true) {
			bool e = true;
			if (nullptr != l1) {
				m += l1->val;
				l1 = l1->next;
				e = false;
			}
			if (nullptr != l2) {
				m += l2->val;
				l2 = l2->next;
				e = false;
			}
			if (0 != m) {
				e = false;
			}
			if (true == e) {
				break;
			}
			if (nullptr == n) {
				n = new ListNode (m % 10);
				_n = n;
			}
			else {
				n->next = new ListNode (m % 10);
				n = n->next;
			}

			m /= 10;
		}

		return _n;
	}
};
// @leet end
