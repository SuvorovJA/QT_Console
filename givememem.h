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
    GiveMeMem(Operation, void *, int *); // ALLOCATE, REALLOCATE
    GiveMeMem(Operation, void *, int *, void *); // COPYEXT, COPYMEMBER
    bool doit(Operation, void *, int *); // ALLOCATE, REALLOCATE
    bool doit(Operation, void *, int *, void *); // COPYEXT, COPYMEMBER
    bool doit(Operation, void *); // RELEASE
    bool doit(Operation); // RELEASE
    bool isOk();
    unsigned int size();
    ~GiveMeMem();

private:

    bool isSuccessful;
    void *memholder;
    unsigned int sizeOfHoldedMem;
#if GIVEMEMEM_DEBUG == 1
    void dbgMsg(const std::string &);
    std::string toStr(void *);
    std::string toStr(Operation);
    std::string toStr(int); // TODO template function use
#endif

};
#endif // GIVEMEMEM_H
