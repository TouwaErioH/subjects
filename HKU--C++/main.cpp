#include "ListNode.h"

/* You are not allowed to change the code in this file */

extern ListNode * SolveB(int n);

int main() {
    int n;
    cin >> n;
    // get the head node of rearranged list
    ListNode *ls = SolveB(n);
    for (int i = 0; i < n; i++) {
        cout << ls->val <<" ";
        ls = ls->next;
    }
    return 0;
}