#include <iostream>
#define ll long long
#define de(x) cerr << #x << " " << x << endl;
using namespace std;
const int N = 4e7 + 10;
ll a[N], tree[N], lazy[N];
void _p(int x, int y, int l, int r, int index, int k);

void build(int r, int l, int index);

ll find(int x, int y, int l, int r, int index);

int main() {
  int n, m;
  cin >> n >> m;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  build(1, n, 1);
  for (int i = 0; i < m; i++) {
    int op, x, y, k;
    cin >> op;
    if (op == 1) {
      cin >> x >> y >> k;
      _p(x, y, 1, n, 1, k);
    } else {
      cin >> x >> y;
      cout << find(x, y, 1, n, 1) << endl;
    }
  }
  return 0;
}
ll find(int x, int y, int l, int r, int index) {
  if (x <= l && r <= y) {
    return tree[index];
  }
  int m = l + ((r - l) >> 1);
  if (lazy[index]) {
    tree[index * 2] += (m - l + 1) * lazy[index];
    tree[index * 2 + 1] += (r - m) * lazy[index]; // 因为右边界是m+1的状态
    // 所以，那个应该是r-m  而非（r-m+1） 原式子应该是 r-（m + 1） + 1;《=》r-m;
    lazy[index * 2] += lazy[index];
    lazy[index * 2 + 1] += lazy[index];
    lazy[index] = 0;
  }
  ll sum = 0;
  if (x <= m) {
    // cout << "x";
    // de(index);
    sum += find(x, y, l, m, index * 2);
  }
  if (y > m) {
    // cout << "y";
    // de(index);
    sum += find(x, y, m + 1, r, index * 2 + 1);
  }
  // de(sum);
  return sum; //   是为了那些不再倒数第二层深处的地方，也有返回值
}

void _p(int x, int y, int l, int r, int index, int k) {
  if ((x <= l && r <= y)) {
    tree[index] += k * (r - l + 1);
    // lazy[index * 2] += k + lazy[index];
    // lazy[index * 2 + 1] += k + lazy[index];
    lazy[index] += k; // 只有区间修改的时候，才需要标记下放。？
    // 一旦识别到完全符合的区间就返回，不再进行往下寻找，
    // 除非最后需要寻找的区间是在lazy标记下面的区间里面
    // 而这一块的操作是放在了，后面区间查询里面
    return;
  }
  int m = l + ((r - l) >> 1);
  if (lazy[index]) { // 这里模板是给出了 s！=t的条件的，但是，其实好像没关系
    // 因为树长不会更改，他的标记在无法探测到的地方，
    // 但是可能会导致下标越界
    tree[index * 2] += lazy[index] * (m - l + 1);
    tree[index * 2 + 1] += lazy[index] * (r - m);
    lazy[index * 2] += lazy[index];
    lazy[index * 2 + 1] += lazy[index];
    lazy[index] = 0;
  }

  if (x <= m) { // 他是完全将区间修改的操作，放在了第一个if里面了
    // 而我重复 相加了。
    // tree[index * 2] += k * (m - x + 1);
    _p(x, y, l, m, index * 2, k);
  }
  if (m < y) {
    // tree[index * 2 + 1] += k * (y - m + 1);
    _p(x, y, m + 1, r, index * 2 + 1, k);
  }
  tree[index] = tree[index * 2 + 1] + tree[index * 2];
}

void build(int l, int r, int index) {
  if (r == l) {
    tree[index] = a[l];
    return;
  } else {
    int m = l + ((r - l) >> 1);
    build(l, m, index * 2);
    build(m + 1, r, index * 2 + 1);
    tree[index] = tree[index * 2] + tree[index * 2 + 1];
  }
}
