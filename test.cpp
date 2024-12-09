#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <stdlib.h>
using namespace std;

// frac 有关的函数声明
int gcd (int a, int b);
void reduceFrac (int &a, int &b);

// 定义 frac 结构体
struct frac {
    int a, b;

    frac () = default; // default
    frac (int u, int d) {
        reduceFrac (u, d);
        a = u;
        b = d; 
    }

    // 声明运算符重载
    frac operator+ (frac );
	frac operator- (frac );
	frac operator* (frac );
	frac operator/ (frac );
    bool operator== (frac );
    friend ostream & operator << (ostream & os, frac );
    friend istream & operator >> (istream & os, frac & );
};

frac frac :: operator+(frac other) {
	return frac(a * other.b + other.a * b, b * other.b);
}
frac frac :: operator-(frac other) {
    return frac(a *other.b - b * other.a, a * b);
}
frac frac :: operator*(frac other) {
    return frac(a * other.a, b * other.b);
}
frac frac :: operator/(frac other) {
    return frac(a * other.b, b * other.a);
}
bool frac :: operator==(frac other) {
    if (a == other.a && b == other.b) {
        return true;
    } else {
        return false;
    }
}
ostream & operator << (ostream & os, frac x) {
	if (!x.b) {
        return os << "INF"; //当分母为 0 时，视为无穷大 
    } if(x.b == 1) {
        return os << x.a; //当分母为 1 时，视为整数  
    }
	return os << x.a << "/" << x.b;
}
istream & operator >> (istream & is, frac & x) {
    is >> x.a >> x.b;
    reduceFrac (x.a, x.b);
    return is;
}

int gcd (int a, int b) {
	return ! b ? a : gcd(b, a % b);
}
void reduceFrac (int &a, int &b) {
    if (b < 0) { // 令分母始终为正 
        a = - a;
        b = - b;
    } if (a == 0) {
        b = 1;
    } else {
        int g = gcd(abs(a), abs(b)); // 求最大公约数时，取绝对值 
        a /= g;
        b /= g;
    }
}

// mono 有关的的函数声明
void reduceMono (int &r, frac &coeff);
bool is_square (unsigned a);
bool is_prime (unsigned a);
void simple (unsigned &out, unsigned &in, unsigned &up);

// 定义 monomial 结构体
struct mono {
    int r;
    frac coeff;

    mono () = default;
    mono (int s, frac c) {
        reduceMono (s, c);
        r = s;
        coeff = c;
    }

    // 声明运算符重载
    mono operator+ (mono );
	mono operator- (mono );
	mono operator* (mono );
	mono operator/ (mono );
    bool operator== (mono );
    friend ostream & operator << (ostream & os, mono );
    friend istream & operator >> (istream & os, mono & );
};

mono mono :: operator+(mono other) {
    return {r, coeff + other.coeff};
}
mono mono :: operator-(mono other) {
    return {r, coeff - other.coeff};
}
mono mono :: operator*(mono other) {
    return mono(r * other.r, coeff * other.coeff);
}
mono mono :: operator/(mono other) {
    frac a = frac(1, other.r);
    return mono(r * other.r, coeff / other.coeff * a);
}
bool mono :: operator==(mono other) {
    if (coeff == other.coeff && r == other.r) {
        return true;
    } else {
        return false;
    }
}
ostream & operator << (ostream & os, mono x) {
    if (x.r == 1) {
        os << x.coeff;
    } else if (x.r == 0) {
        os << 0;
    } else {
        os << x.coeff << " sqrt(" << x.r << ")";
    }
    return os;
}
istream & operator >> (istream & is, mono & x) {
    is >> x.coeff >> x.r;
    reduceMono(x.r, x.coeff);
    return is;
}

void reduceMono (int &r, frac &coeff) {
    unsigned out, in, up;
    unsigned n;

    up = 2;
    out = 1;
    n = r;
    in = n;
    simple(out, in, up);
    
    coeff = frac(out, 1) * coeff;
    r = in;
}

bool is_square (unsigned a) {
	return (sqrt(a) - int(sqrt(a)) == 0);
}
bool is_prime (unsigned a) {
	unsigned i = 2;
	while(i < a) {
		if(a % i == 0) {
			break;
        }
		i++;
	}
	if (i == a) {
		return true;
    } else {
		return false;
    }
}
void simple (unsigned &out, unsigned &in, unsigned &up) {
	unsigned int k;
	unsigned in2 = in;
	if (is_square(in)) {
		if (in == 0) {
            out = 0;
        } else {
		    out *= sqrt(in);
		    in = 1; 				
		}
		return;
	}
	for (int i = 2; i < in; i++) {
		if (in <= 1 ) {
	    	return;
        }
		k = 0;
		if (!is_prime(i)) {
		    continue;
        }
 		in2 = in;
 		while (in % i == 0) {
 			in /= i;
 			++k;
	 	}
	 	in = in2;
	 	if (k >= up) {
	 		out *= pow(i, k / up);
	 		in /= pow(i, k - k % up);
		}
	}
}

// poly 有关的函数声明

// 定义 poly 结构体
struct poly {
    vector<mono> terms;

    poly () = default;
    
    // 声明运算符重载
    poly operator+ (poly );
	poly operator- (poly );
	poly operator* (poly );
    friend ostream & operator << (ostream & os, poly );
    friend istream & operator >> (istream & os, poly & );

    // 简化 polynomial
    void simplify() {
        terms.erase(remove_if(terms.begin(), terms.end(), [](mono& m) {
            return m.coeff.a == 0;
        }), terms.end());
    }

    // 输出 polynomial
    void print() {
        for (size_t i = 0; i < terms.size(); ++i) {
            if (i > 0) {
                cout << " + ";
            }
            cout << terms[i].coeff.a << "/" << terms[i].coeff.b << " " << "sqrt(" << terms[i].r << ")";
        }
    }
};

poly poly :: operator+(poly other) {
    poly result;
    result.terms.reserve(terms.size() + other.terms.size());
    auto it1 = terms.begin();
    auto it2 = other.terms.begin();

    while (it1 != terms.end() && it2 != other.terms.end()) {
        if (it1->r < it2->r) {
            result.terms.push_back(*it1++);
        } else if (it1->r > it2->r) {
            result.terms.push_back(*it2++);
        } else {
            mono sum = *it1 + *it2;
            if (sum.coeff.a != 0) { // 只添加非零项
                result.terms.push_back(sum);
            }
            ++it1;
            ++it2;
        }
    }

    result.terms.insert(result.terms.end(), it1, terms.end());
    result.terms.insert(result.terms.end(), it2, other.terms.end());

    // 移除零项并简化
    result.simplify();
    return result;
}
poly poly :: operator-(poly other) {
    poly result;
    result.terms.reserve(terms.size() + other.terms.size());
    auto it1 = terms.begin();
    auto it2 = other.terms.begin();

    while (it1 != terms.end() && it2 != other.terms.end()) {
        if (it1->r < it2->r) {
            result.terms.push_back(*it1++);
        } else if (it1->r > it2->r) {
            result.terms.push_back(*it2++);
        } else {
            mono sum = *it1 - *it2;
            if (sum.coeff.a != 0) { // 只添加非零项
                result.terms.push_back(sum);
            }
            ++it1;
            ++it2;
        }
    }

    result.terms.insert(result.terms.end(), it1, terms.end());
    result.terms.insert(result.terms.end(), it2, other.terms.end());

    // 移除零项并简化
    result.simplify();
    return result;
}
poly poly :: operator*(poly other) {
    poly result;
    for (auto& m1 : terms) {
        for (auto& m2 : other.terms) {
            result.terms.push_back(m1 * m2);
        }
    }
    return result;
}
ostream & operator << (ostream & os, poly x) {
    for (size_t i = 0; i < x.terms.size(); ++i) {
        if (i > 0) {
            cout << " + ";
        }
        cout << x.terms[i];
    }
}
istream & operator >> (istream & is, poly & x) {
    
}

int main () {
    mono a, b;
    
    cin >> a >> b;

    cout << sqrt(a.r) << endl;

    cout << (a + b) << endl;
    cout << (a - b) << endl;
    cout << (a * b) << endl;
    cout << (a / b) << endl;

    system("pause");

    return 0;
}