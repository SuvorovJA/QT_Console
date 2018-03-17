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
 *
 *
 * высвобождение памяти в деструкторе, метод release(GiveMeMem*) - освобождает память,
 *      обнуляет указатель, вызывает деструктор
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
 * public do(enum OperationType, returned*, int sizeOfReturn [, copyFrom* | const GiveMeMem&])
 * этот же метод используется в конструкторах
 *
 */


class GiveMeMem
{
public:

    GiveMeMem();
};

#endif // GIVEMEMEM_H