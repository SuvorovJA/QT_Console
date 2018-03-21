#include "rational.h"

#include <iostream>
#include <sstream>
//#include <cctype>

using namespace std;


// BEGIN друзья класса
int GetNod(int a, int b)   // наибольший общий делитель
{
    a = abs(a);
    b = abs(b);
    while (a > 0 && b > 0) {
        if (a > b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    return a + b;
}
int GetNok(const int &a, const int &b)
{
    return a * b / GetNod(a, b);
}
// EDN друзья класса



Rational::Rational()
{
    // default is 0/1
    Internal.s_numerator = 0;
    Internal.s_denominator = 1;
}

Rational::Rational(int numerator, int denominator)
{
    if (numerator == 0) {
        Internal.s_numerator = 0;
        Internal.s_denominator = 1; // mandatory reset to 1
    } else {
        int nod = GetNod(numerator, denominator);
        if (nod == 0) nod = 1;
        Internal.s_numerator = numerator / nod; // norma 10/20 to 1/2
        Internal.s_denominator = denominator / nod;
        if (Internal.s_numerator < 0 && Internal.s_denominator < 0) { // -n/-d reset to n/d
            Internal.s_denominator = abs(Internal.s_denominator);
            Internal.s_numerator = abs(Internal.s_numerator);
        } else if (Internal.s_numerator > 0 && Internal.s_denominator < 0) { // n/-d reset to -n/d
            Internal.s_denominator = Internal.s_denominator * (-1);
            Internal.s_numerator = Internal.s_numerator * (-1);
        }
    }
}


int Rational::Numerator() const
{
    return Internal.s_numerator;
}
int Rational::Denominator() const
{
    return Internal.s_denominator;
}

// операторы напрямую к классу не относятся

Rational operator*(const Rational &L, const Rational &R)
{
    return Rational { (L.Numerator() * R.Numerator()),
                      (R.Denominator() * L.Denominator()) };
}

Rational operator/(const Rational &L, const Rational &R)
{
    return Rational { (L.Numerator() * R.Denominator()), (L.Denominator()
                      * R.Numerator()) };
}
Rational operator+(const Rational &L, const Rational &R)
{
    if (L.Denominator() == R.Denominator()) {
        return Rational { (L.Numerator() + R.Numerator()), L.Denominator() };
    } else {
        int nok = GetNok(L.Denominator(), R.Denominator());
        return Rational { (L.Numerator() * (nok / L.Denominator()))
                          + (R.Numerator() * (nok / R.Denominator())), nok };
    }
}

Rational operator-(const Rational &L, const Rational &R)
{
    if (L.Denominator() == R.Denominator()) {
        return Rational { (L.Numerator() - R.Numerator()), L.Denominator() };
    } else {
        int nok = GetNok(L.Denominator(), R.Denominator());
        return Rational { (L.Numerator() * (nok / L.Denominator()))
                          - (R.Numerator() * (nok / R.Denominator())), nok };
    }
}

istream &operator>>(istream &stream, Rational &R)
{
    int n = 0;
    int d = 0;
    int c = 0;
    stream >> ws;       // eat up any leading white spaces
    c = stream.peek();  // peek character 1
    if (c == EOF)       return stream;
    if (isdigit(c) || c == '-')       stream >> n;
    c = stream.peek();  // peek character 2
    if (c == EOF)       return stream;
    if (!isdigit(c) && c == '/')        stream.ignore(1);
    c = stream.peek();  // peek character 3
    if (c == EOF)       return stream;
    if (isdigit(c) || c == '-')       stream >> d;
    if (d == 0)     return stream;
    R = {n, d};
    return stream;
}

ostream &operator<<(ostream &stream, const Rational &R)
{
    stream << R.Numerator() << "/" << R.Denominator();
    return stream;
}

bool operator<(const Rational &L, const Rational &R)
{
    if (L.Denominator() == R.Denominator()) {
        return (L.Numerator() < R.Numerator());
    } else if (L.Numerator() == R.Numerator()) {
        return (L.Denominator() > R.Denominator());
    } else {
        int nok = GetNok(L.Denominator(), R.Denominator());
        return ((L.Numerator() * (nok / L.Denominator()))
                < (R.Numerator() * (nok / R.Denominator())));
    }
}

bool operator==(const Rational &L, const Rational &R)
{
    return (L.Numerator() == R.Numerator() && L.Denominator() == R.Denominator());
}

// =============== TESTING =================


void Rational::Test() // static similar as main()
{
    cout << "overkill solution:" << endl;
    cout << "задача с дробями решена классом Rational с переопределением операторов <<,>>,+-/*,==,>"
         << endl;
    cout << "в таком варианте struct не особо нужен, но применен в качестве внутреннего формата хранения"
         << endl << endl;
    {
        ostringstream output;
        output << Rational(-6, 8);
        cout << "Rational(-6, 8) должен стать \"-3/4\" : " << output.str() << endl << endl;
        {
            cout << "из потока введено \"5/7\", должно пройти сравнение с Rational(5, 7)" << endl;
            istringstream input("5/7");
            Rational r;
            input >> r;
            if (r == Rational(5, 7)) {
                cout << "5/7 считано верно: " << r << endl;
            } else {
                cout << "5/7 считано неверно: " << r << endl;
            }
            cout << endl;
        }

        cout << "из потока введено \"5/7\" и \"10/8\" , должно считаться \"5/7\" и \"5/4\"" << endl;
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        if ( r1 == Rational(5, 7) && r2 == Rational(5, 4) ) {
            cout << "считано верно: " << r1 << " " << r2 << endl;
        } else {
            cout << "считано неверно: " << r1 << " " <<  r2 << endl;
        }
        cout << endl;

        cout << "ввод из пустого потока не должен переписывать ранее считанные \"5/7\" и \"5/4\"" <<
             endl;
        input >> r1;
        input >> r2;
        if (r1 == Rational(5, 7) && r2 == Rational(5, 4)) {
            cout << "ок: " << r1 << " " <<  r2 << endl;
        } else {
            cout << "не ок: " << r1 << " " <<  r2 << endl;
        }
        cout << endl;


        cout << "арифметика с \"5/7\" и \"5/4\" (с десятичной проверкой)" << endl;
        float a = 5. / 7.;
        float b = 5. / 4.;
        cout << "\"5/7\"+\"5/4\"= " << r1 + r2 << "\t (" << a << "+" << b << "=" << a + b << ")\t\t  55/28=" << 55. / 28. << endl;
        cout << "\"5/7\"-\"5/4\"= " << r1 - r2 << "\t (" << a << "-" << b << "=" << a - b << ")\t -15/28=" << -15. / 28. << endl;
        cout << "\"5/7\"*\"5/4\"= " << r1 *r2 << "\t (" << a << "*" << b << "=" << a *b << ")\t\t  25/28=" << 25. / 28. << endl;
        cout << "\"5/7\" / \"5/4\" =  " << r1 / r2 << "\t  (" << a << "/" << b << "=" << a / b << ")\t  4/7=" << 4. / 7. << endl;

        cout << endl << "ok" << endl;
    }
}
