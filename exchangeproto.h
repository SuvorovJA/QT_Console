#ifndef EXCHANGEPROTO_H
#define EXCHANGEPROTO_H

#include <cstdint>

class ExchangeProto
{
public:
    ExchangeProto();

    enum TypesMessage { SET, GET, DATA, ACK  }; // до 8 типов
    enum Commands { ON, OFF, RUN, STOP, SIZE, VALUE, NAME, TIME }; // до 16 команд

#pragma pack(push, 1)
    struct ExchangePackMessageAlignmentOneByte {
        bool direction: 1 ; // 0 - request, 1 - answer
        TypesMessage typeOfMessage: 3; // тип сообщения сенсору/серверу
        Commands command: 4; // команда сенсору/серверу
        uint8_t idOfSensor; // номер опрашиваемого сенсора / номер сенсора отправляющего данные
        uint32_t simplecrc = 0; // простая контрольная сумма пакета при simplecrc=0
        uint8_t payloadLength; // размер нагрузки
        uint8_t payload[]; // полезные данные вариативной длинны
    };
#pragma pack(pop);

    struct ExchangePackMessageNonAlignment {
        bool direction: 1 ; // 0 - request, 1 - answer
        TypesMessage typeOfMessage: 3; // тип сообщения сенсору/серверу
        Commands command: 4; // команда сенсору/серверу
        uint8_t idOfSensor; // номер опрашиваемого сенсора / номер сенсора отправляющего данные
        uint32_t simplecrc; // простая контрольная сумма пакета
        uint8_t payloadLength; // размер нагрузки
        uint8_t payload[]; // полезные данные вариативной длинны
    };
};

#endif // EXCHANGEPROTO_H
