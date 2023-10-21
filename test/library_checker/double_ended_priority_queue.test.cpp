#define PROBLEM "https://judge.yosupo.jp/problem/double_ended_priority_queue"

#include "all.hpp"
#include "ds/removable_queue.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    RemovableQueue<std::priority_queue<int, vi, std::greater<>>> q1(a);
    RemovableQueue<std::priority_queue<int>> q2(a);
    _for (q) {
        dR(char, t);
        if (t == '0') {
            dR(int, x);
            q1.push(x), q2.push(x);
        }
        else if (t == '1') {
            int x = q1.pop();
            q2.remove(x);
            writeln(x);
        }
        else {
            int x = q2.pop();
            q1.remove(x);
            writeln(x);
        }
    }
    return 0;
}