#ifndef LOGOTYP_H
#define LOGOTYP_H


class LogoTyp
{
public:
    LogoTyp();
    void print();
private:
    char decode(int);
    int size;
    long unsigned int *data;
};

#endif // LOGOTYP_H
