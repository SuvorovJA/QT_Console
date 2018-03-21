#ifndef EXCHANGEPROTO_H
#define EXCHANGEPROTO_H

#include <cstdint>

class ExchangeProto
{
public:
    ExchangeProto();

    enum TypesMessage { SET = 1, GET, DATA, ACK  }; // до 8 типов
    enum Commands { ON, OFF, RUN, STOP, SIZE, VALUE, NAME, TIME }; // до 16 команд

#pragma pack(push, 1)
    struct ExchangePackMessageAlignmentOneByte { // 1+1+2+1+8 = 13 bytes
        bool direction: 1 ; // 0 - request, 1 - answer
        TypesMessage typeOfMessage: 3; // тип сообщения сенсору/серверу
        Commands command: 4; // команда сенсору/серверу
        uint8_t idOfSensor; // номер опрашиваемого сенсора / номер сенсора отправляющего данные
        uint16_t simplecrc = 0; // простая контрольная сумма пакета при simplecrc=0
        uint8_t payloadLength; // размер нагрузки
        uint8_t *payload;   // здесь видимо только указателем
        /* uint8_t payload[1]; // полезные данные вариативной длинны, нет, =1!
         * flexible array member ‘ExchangeProto::ExchangePackMessageAlignmentOneByte::payload’
         * not at end of ‘class ExchangeProto’ uint8_t payload[]; */
    };
#pragma pack(pop)

    struct ExchangePackMessageNonAlignment { // 1+1+2+1+3? = 8 bytes?
        bool direction: 1 ; // 0 - request, 1 - answer
        TypesMessage typeOfMessage: 3; // тип сообщения сенсору/серверу
        Commands command: 4; // команда сенсору/серверу
        uint8_t idOfSensor; // номер опрашиваемого сенсора / номер сенсора отправляющего данные
        uint16_t simplecrc; // простая контрольная сумма пакета
        uint8_t payloadLength; // размер нагрузки
        uint8_t payload[]; // полезные данные вариативной длинны
    };

    int dummy = 0xAA; // placeholder for mem debug

    typedef   ExchangePackMessageAlignmentOneByte EPMa;
    typedef   ExchangePackMessageNonAlignment EPMna;

    EPMa InternalEPMa; // fields
    EPMna InternalEPMna;

    //шаблон т.к. у нас два типа
    template<typename T> void PrintEPM(const T &);
    template<typename T> int mySizeof(const T &);
};

#endif // EXCHANGEPROTO_H

/*
 * It is one of those things which are different between C and C++.
 * A flexible array member is valid in C but not C++.
 * That said, many modern compilers compile C as a subset of C++,
 * taking care to care only when you torque up the compiler error diagnostics.
 * C++ does not support flexible array members.(This feature might be provided
 * as an extension by some C++ compilers, but would probably be valid only
 * for POD structure types.)
 * So yes, this is undefined behavior.
 * The correct way (in both C and C++) to write such a thing is to give it a
 * non-zero dimension:
 *
 * template <typename T>
 * struct Vector {
 *     int length;
 *     T ts[1];
 * };
 *
 * You have another issue: you must allocate memory for said object.
 * Simply specifying an initializer is not enough.
 * As far as every access to such a thing exists, the compiler only
 * ever thinks it is its minimal size.
 *
 * you cannot apply the sizeof operator to a flexible array.
 */
