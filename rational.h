#ifndef RATIONAL_H
#define RATIONAL_H
//#include <iostream>
//#include <sstream>
//#include <map>
//#include <set>
//#include <vector>
//#include <cctype>

class Rational
{
public:

    Rational(); // 0/1
    Rational(int numerator, int denominator); // n/d

    static void Test();

    int Numerator() const;  // getters
    int Denominator() const;

    friend int GetNok(const int &a, const int &b);
    friend int GetNod(int a, int b);

private:
    struct Ra {
        int s_numerator;
        int s_denominator;
    } Internal;
//    int GetNod(int a, int b);
//    int GetNok(const int &a, const int &b);

//    Rational operator*(const Rational &L, const Rational &R);
//    bool operator==(const Rational &L, const Rational &R);
//    istream &operator>>(istream &stream, Rational &R);
//    ostream &operator<<(ostream &stream, const Rational &R);
};

#endif // RATIONAL_H
