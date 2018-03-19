#ifndef GIVEMEMEM_H
#define GIVEMEMEM_H

/*
 *
 * class-wrapper работы с памятью:
 * создать, удалить, перераспределить, очистить, скопировать, используя
 * malloc, calloc, realloc, memset, memcpy.
 *
 * хранится длинна удержанной памяти - поле класса. метод size() возвращает.
 *
 * хранится поле удачи, тип bool, удачна ли последняя операция или нет. метод isOk()
 * при isOk() = false -- возвращаемый указатель null
 *
 * метод access() - возвращает указатель на выделенную память
 *
 * метод release() - освобождает память, высвобождение памяти в деструкторе,
 *                                  обнуляет указатель, вызывает деструктор
 *
 * - при создании всегда делается инициализация памяти 0
 * - всегда проверка границ  // TODO?
 *
 * конструкторы:
 *  - создать пустой, потом вызвать реаллокейт или копию
 *  - создать конкретный
 *  - создать конкретный используя копию
 *
 * params
 * 1) enum class OperationType - тип операции, (не особо нужен, но чтобы в коде было видно
 *              тип операции, можно в private method использовать в switch)
 * 2) int sizeOfReturn - количество памяти размерности sizeof(парам2)
 * 3) void copyFrom* | const GiveMeMem& - источник для копии
 *  ps: возврата указателя на выделенную память нет, есть access()
 *
 * перегруженный метод операций
 * public bool do(enum OperationType, int sizeOfReturn [, copyFrom* ])
 * returned isOk()
 * этот же метод используется в конструкторах
 *
 * static флаг debugEnabled - вкл/выкл диагностических сообщений, метод setDebug(bool);
 *
 */

#include <string>
#include <sstream>

class GiveMeMem
{
public:

    enum class Operation { ALLOCATE, REALLOCATE, COPYEXT, COPYMEMBER, RELEASE };
    GiveMeMem();
    GiveMeMem(Operation, const int &); // ALLOCATE, REALLOCATE
    GiveMeMem(Operation, const int &, void *); // COPYEXT, COPYMEMBER
    //
    bool doit(Operation, const int &); // ALLOCATE, REALLOCATE
    bool doit(Operation, const int &, void *); // COPYEXT, COPYMEMBER
    //
    bool doit(Operation, void
              *); // RELEASE (освобождается память по указателю, должна принадлежать объекту)
    bool doit(Operation); // RELEASE (освобождается память занятая объектом)
    //

    bool isOk();
    void *access();
    unsigned int getSize();
    ~GiveMeMem();
    static void setDebug(bool);

private:

    bool isSuccessful;
    void *memholder;
    unsigned int sizeOfHoldedMem;
    //
    static bool debugEnabled;
    //
    bool getMem(const int &size); // alloc mem to memholder
    bool cleanMem(); // memholder clean
    bool copyMem(const void *, const unsigned int &); // copy size from void* to memholder
    //
    void dbgMsg(const std::string &);
    std::string toStr(void *);          // TODO template function use
    std::string toStr(const void *&);
    std::string toStr(Operation);
    std::string toStr(int);
};
#endif // GIVEMEMEM_H
