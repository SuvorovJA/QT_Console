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
 * высвобождение памяти в деструкторе, метод release(GiveMeMem*) - освобождает память,
 *                                  обнуляет указатель, вызывает деструктор
 *
 * - при создании всегда делается инициализация памяти 0
 * - всегда проверка границ
 *
 * конструкторы:
 *  - создать пустой, потом вызвать ресайз или копию
 *  - создать конкретный
 *  - создать конкретный используя копию
 *
 * params
 * 1) enum class OperationType - тип операции, (не особо нужен, но чтобы в коде было видно
 *              тип операции, можно в private method использовать в switch)
 * 2) void returned* - возврат. указатель на память (TODO лучше не void* а шаблонным образом
 *              передавать типизированный указатель)
 * 3) int sizeOfReturn - количество памяти размерности sizeof(парам2)
 * 4) void copyFrom* | const GiveMeMem& - источник для копии
 *
 * перегруженный метод операций
 * public bool do(enum OperationType, returned*, int sizeOfReturn [, copyFrom* | const GiveMeMem&])
 * returned isOk()
 * этот же метод используется в конструкторах
 *
 */


// TODO replace to member field with setter
#define GIVEMEMEM_DEBUG 1

#if GIVEMEMEM_DEBUG == 1
#include <string>
#include <sstream>
#endif



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
              *); // RELEASE (освобождается память по указателю, обязательно должна принадлежать объекту)
    bool doit(Operation); // RELEASE (освобождается память занятая объектом)
    //

    bool isOk();
    void *access();
    unsigned int getSize();
    ~GiveMeMem();

private:

    bool isSuccessful;
    void *memholder;
    unsigned int sizeOfHoldedMem;
    //
    bool getMem(const int &size); // alloc mem
    bool cleanMem(); // memholder clean
    bool copyMem(const void *, const unsigned int &); // copy size from void* to memholder
    //
#if GIVEMEMEM_DEBUG == 1
    void dbgMsg(const std::string &);
    std::string toStr(void *);
    std::string toStr(const void *&);
    std::string toStr(Operation);
    std::string toStr(int); // TODO template function use
#endif

};
#endif // GIVEMEMEM_H
