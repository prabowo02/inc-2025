class Node:
    def __init__(self, val):
        self.val = val

    def __mul__(self, other):
        return Node([other.val[x] for x in self.val])

class SegTree:
    def __init__(self, a):
        self.a = a
        self.nodes = [None for _ in range(len(a) * 2)]
        self.build(0, 0, len(a))

    def _leaf(self, op):
        k = op[1]
        if op[0] == '+':
            return Node(list(range(k, 16)) + list(range(k)))
        return Node([x ^ k for x in range(16)])

    def build(self, idx, l, r):
        if l + 1 == r:
            self.nodes[idx] = self._leaf(self.a[l])
            return

        mid = (l + r) // 2
        lc, rc = idx + 1, idx + 2 * (mid - l)
        self.build(lc, l, mid)
        self.build(rc, mid, r)
        self.nodes[idx] = self.nodes[lc] * self.nodes[rc]

    def _update(self, idx, l, r, x):
        if l > x or r <= x:
            return
        if l + 1 == r:
            self.nodes[idx] = self._leaf(self.a[x])
            return
        mid = (l + r) // 2
        lc, rc = idx + 1, idx + 2 * (mid - l)
        self._update(lc, l, mid, x)
        self._update(rc, mid, r, x)
        self.nodes[idx] = self.nodes[lc] * self.nodes[rc]

    def update(self, x, op):
        self.a[x] = op
        self._update(0, 0, len(self.a), x)


def solve():
    N, Q = [int(s) for s in input().split()]

    modules = []
    for _ in range(N):
        modules.append(input().split())
        modules[-1][1] = int(modules[-1][1])

    tree = SegTree(modules)

    for _ in range(Q):
        x, op1, op2 = input().split()
        tree.update(int(x) - 1, [op1, int(op2)])
        print(tree.nodes[0].val[0])


if __name__ == '__main__':
    solve()
