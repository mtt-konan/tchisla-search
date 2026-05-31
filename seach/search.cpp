/*
 *	Author:
 *		Indestinee
 *	Date:
 *		2016.08.08
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;
#define HASH
const int cnt_f = 12, m = 20;
const long long infd = 1e12, infn = 1e3, mod = 1e9+7;
char buff[256];
const int num_seed = 10000;
long long seeds[num_seed];
inline long long seed(const long long &x) {
	return x < num_seed ? seeds[x] : x;
}
inline long long gcd(long long x, long long y) {
	long long r;
	while (y) {
		r = x % y;
		x = y;
		y = r;
	}
	return x;
}
inline long long lcm(const long long &x, const long long &y) {
	return x / gcd(x, y) * y;
}
inline long long sqr(const long long &x) {
	return x * x;
}
inline bool is_sqr(const long long &x) {
	return sqr(sqrt(x + 0.5)) == x;
}
inline void reduce(long long &x, long long &y) {
	long long g = gcd(x, y);
	if (g)
		x /= g, y /= g;
}
inline string int_to_str(long long n) {
	if (n == 0)
		return "0";
	string ret;
	while(n) {
		ret = char(n % 10 + '0') + ret;
		n /= 10;
	}
	return ret;
}
class frac;
inline void show(const frac &a, bool tb);
class frac{	
private:
	long long d, n;
	inline void _reduce() {
		if (n == 0) {
			d = 0, n = 1;
			return;
		}
		if (d < 0)
			d = 0, n = 1;
		reduce(d, n);
	}
public:
	frac(const long long &dd = 0, const long long &nn = 1):d(dd), n(nn) {
		_reduce();
		if (d > infd || n > infn)
			d = 0, n = 1;
	}
	
	inline friend size_t _HASH(const frac &a) {
		return seed(a.d) * seed(a.n) % mod * seed(a.d % mod * (a.n % mod) % num_seed);
	} 
	inline friend frac operator + (const frac &a, const frac &b) {
		long long	uv = lcm(a.n, b.n),
					u = uv / a.n, v = uv / b.n;
        return frac(u * a.d + v * b.d, uv);
	}
	inline friend frac operator - (const frac &a, const frac &b) {
		long long	uv = lcm(a.n, b.n),
					u = uv / a.n, v = uv / b.n;
        return frac(abs(u * a.d - v * b.d), uv);
	}
	inline friend frac operator * (frac a, frac b) {
		reduce(a.d, b.n);
		reduce(b.d, a.n);
		return frac(a.d * b.d, a.n * b.n);
	}
	inline friend frac operator / (frac a, frac b) {
        reduce(a.d, b.d);
		reduce(a.n, b.n);
		return frac(a.d * b.n, a.n * b.d);
	}
	inline friend bool operator == (const frac &a, const long long &b) {
		if (b == 0)
			return a.d == 0;
		return a.d / a.n == b && a.d % a.n == 0;
	}
	inline friend bool operator == (const frac &a, const frac &b) {
		return a.d == b.d && a.n == b.n;
	}
	inline friend bool operator < (const frac &a, const long long &b) {
		return a.d / a.n < b;
	}
	inline friend bool operator < (const frac &a, const frac &b) {
		return (a.d == b.d) ? a.n < b.n : a.d < b.d;
		return (a - b) < 0;
	}
	inline friend frac quick_pow(frac a, long long b) {
        frac ret = frac(1);
        while(b) {
            if(b & 1)
                ret = ret * a;
            a = a * a;
            b >>= 1;
        }
        return ret;
	}
	inline friend frac quick_pow(const frac &a, const frac &b) {
		if (b.n != 1)
			return frac(0);
		return quick_pow(a, b.d);
	}
	inline friend frac reciprocal(const frac &a) {
		return frac(a.n, a.d);
	}
	inline friend frac sqrt(const frac &a) {
		return (is_sqr(a.n) && is_sqr(a.d)) ? frac(sqrt(a.d + 0.5), sqrt(a.n + 0.5)) : frac(0);
	}
	inline friend int to_int(const frac &a) {
		return (a.n == 1 && a.d < cnt_f) ? a.d : 0;
	}
	inline friend string to_str(const frac &a) {
		if (a.n == 1)
			sprintf(buff, "%I64d", a.d);
		else
			sprintf(buff, "(%I64d | %I64d)", a.d, a.n);
		return buff;
	}
	inline int in() {
		if (!gets(buff))
			return -1;
		d = 0; n = 1;
		long long *now = &d;
		for (char *c = buff; *c; c++) {
			if (*c >= '0' && *c <= '9')
				*now = *now * 10 + *c - '0';
			else if (*c == '/') 
				now = &n, *now = 0;
		}
		_reduce();
		return 1;
	}
}zero, a, b, f[cnt_f];
class info{
public:
	int step, method;
	frac bef1, bef2;
	info() {}
	info(const int &step, const int &method = 0, const frac &bef1 = zero, const frac &bef2 = zero):step(step), method(method), bef1(bef1), bef2(bef2) {}
	inline void set(int x) {
		step = x;
	}
}neg(-1, 0);
int base, steps;
#ifdef HASH
class cmp{
public:
	size_t operator () (const frac &a) const {
		return _HASH(a);
	}
};
unordered_map<frac, info, cmp>mp;
unordered_set<frac, cmp>s[m];
unordered_set<frac, cmp>::iterator it, it1, it2;
#else
map<frac, info>mp;
set<frac>s[m];
set<frac>::iterator it, it1, it2;
#endif
queue<frac> q;
char symbol[10][10] = {"+", "-", "*", "/", "^", "^-"};
inline void show(const frac &a, bool tb) {
	if (!mp.count(a)) {
		printf("Not find %s within %d steps..\n", to_str(a).c_str(), steps);
		return;
	}
	info infor = mp[a];
	if (infor.method < 0) {
		printf("%s is origin, cost = %d\n", to_str(a).c_str(), infor.step);
		return;
	}
	if (infor.method < 6) {
		printf("%s = %s %s %s, cost = %d\n", to_str(a).c_str(), to_str(infor.bef1).c_str(), symbol[infor.method], to_str(infor.bef2).c_str(),  infor.step);
		if (tb) {
			show(infor.bef1, 1);
			puts("\t----");
			show(infor.bef2, 1);
		}
	} else if (infor.method == 6) {
		printf("%s = %s!, cost = %d\n", to_str(a).c_str(), to_str(infor.bef1).c_str(), infor.step);
	} else if (infor.method == 7) {
		printf("%s = sqrt(%s), cost = %d\n", to_str(a).c_str(), to_str(infor.bef1).c_str(), infor.step);
	} else {
		cout << to_str(a) << endl;
		printf("%d\n", infor.method);
		exit(-1);
	}
}
inline frac factor(const frac &a) {
	return f[to_int(a)];
}
void update(const frac &now, const int &step, const int &method, const frac &a = zero, const frac &b = zero) {
	int x = (mp.count(now) > 0) ? mp[now].step : 0;
	if (x != 0 && x <= step)
		return;
	s[step].insert(now);
	mp[now] = info(step, method, a, b);
	q.push(now);
}
void init() {
	f[0] = frac(1);
	srand(time(0)+clock());
	for (int i = 1; i < cnt_f; i++)
		f[i] = f[i-1] * (long long)i;
	f[0] = zero;
	for (int i = 0; i < num_seed; i++)
		seeds[i] = (long long)rand() << 32 | (long long) rand(); 
}
void show(int id) {
	for (it = s[id].begin(); it != s[id].end(); it++)
		show(*it, 0);
}
int main() {
#ifdef HASH
	puts("\tUse HASH..\n");
#else
	puts("\tUse RBTree..\n");
#endif
	init();
	frac now = zero;
	printf("Please input base and steps, for prepare the progress: ");
	cin >> base >> steps;
	gets(buff);
	base = base < 0 ? -base : base;
	steps = steps < 0 ? -steps : steps;
	mp[zero] = neg;
	for (int i = 1; i <= steps; i++) {
		now = now * 10LL + base;
		update(now, i, -1);
		for (int j = 1; j < i; j++) {
			int k = i - j;
			if (j > k)
				break;
			for (it1 = s[j].begin(); it1 != s[j].end(); it1++) {
				a = *it1;
				for (it2 = s[k].begin(); it2 != s[k].end(); it2++) {
					b = *it2;
					update(a + b, i, 0, a, b);
					update(a - b, i, 1, a, b);
					update(a * b, i, 2, a, b);
					update(a / b, i, 3, a, b);
					update(b / a, i, 3, b, a);
					update(quick_pow(a, b), i, 4, a, b);
					update(quick_pow(b, a), i, 4, b, a);
					update(quick_pow(reciprocal(a),b), i, 5, a, b);
					update(quick_pow(reciprocal(b),a), i, 5, b, a);
				}
			}
		}
		while (!q.empty()) {
			a = q.front(); q.pop();
			update(sqrt(a), i, 7, a);
			update(factor(a), i, 6, a);
		}
		// show(i, 1);
		printf("Step[%d] finished..\n", i);
	}
	puts("Start..\n");
	while (~a.in()) {
		puts("**************");
		if (!(a == 0))
			show(a, 1);
		puts("**************\n");
	}
	return 0;
}