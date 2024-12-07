#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
using namespace std;

// 定义 frac 结构体
struct frac {
    int a, b;

    frac (int a, int b): a(a), b(b) {
        int g = gcd(a, b);
        a /= g;
        b /= g;
    }

    frac operator+(const frac& other) const {
        return frac(a * other.b + b * other.a, a * b);
    }
    frac operator-(const frac& other) const {
        return frac(a *other.b - b * other.a, a * b);
    }
    frac operator*(const frac& other) const {
        return frac(a * other.a, b * other.b);
    }
    frac operator/(const frac& other) const {
        return frac(a * other.b, b * other.a);
    }
};

std::ostream & operator << (std::ostream && os, const frac & x) {
    os << x.a << '/' << x.b;
}

// 定义 monomial 结构体
struct mono {
    int r;
    frac coeff;

    mono (int r, frac coeff): r(r), coeff(coeff) {
        int sqrt_r = static_cast<int>(sqrt(r));
        for (int i = 2; i < sqrt_r; i++) {
            if (r % (i * i) == 0) {
                r /= (i * i);
                frac a = frac(i, 1);
                coeff = coeff * a;
            }
        }
    }

    mono operator*(const mono& other) const {
        return mono(r * other.r, coeff * other.coeff);
    }
    mono operator/(const mono& other) const {
        frac a = frac(1, other.r);
        return mono(r * other.r, coeff / other.coeff * a);
    }
    mono operator+(const mono& other) const {
        assert(r != other.r);
        return {r, coeff + other.coeff};
    }
};

// 定义 polynomial 结构体
struct poly {
    vector<mono> terms;

    poly operator+(const poly& other) const {
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

    poly operator*(const poly& other) const {
        poly result;
        for (const auto& m1 : terms) {
            for (const auto& m2 : other.terms) {
                result.terms.push_back(m1 * m2);
            }
        }
        return result;
    }

    // 简化 polynomial
    void simplify() {
        terms.erase(remove_if(terms.begin(), terms.end(), [](const mono& m) {
            return m.coeff.a == 0;
        }), terms.end());
    }

    // 输出 polynomial
    void print() const {
        for (size_t i = 0; i < terms.size(); ++i) {
            if (i > 0) {
                cout << " + ";
            }
            cout << terms[i].coeff << "sqrt(" << terms[i].r << ")";
        }
        cout << endl;
    }
};

// 定义 matrix 结构体
struct matrix {
    int rows, cols;
    vector<vector<poly>> data;

    matrix(int r, int c): rows(r), cols(c), data(r, vector<poly>(c, poly)) {}

    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                data[i][j].print();
                cout << " ";
            }
            cout << endl;
        }
    }
};

// 定义矩阵乘法
matrix matrixMultiply (const matrix& a, const matrix& b) {
    if (a.cols != b.rows) {
        cout << "Error." << endl;
    }
    matrix result (a.rows, b.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            for (int k = 0; k < a.cols; k++) {
                result.data[i][j] = result.data[i][j] + a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}

void test () {
    assert()
}

int main () {

}