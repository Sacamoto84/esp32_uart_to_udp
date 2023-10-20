#ifndef _model_h
#define _model_h

#include <mString.h>

#define MAX_S 27
#define MAX_L 12

class ListUI
{
public:
    mString<MAX_S> list[MAX_L];

    uint16_t line;       // Номер текущей линии
    uint32_t size;       // Номер текущей линии
    uint16_t pointInStr; // Номер позиции символа в строке

    // Поместить символ в массив
    void put(char value)
    {

        if (value != '\n')
        {
            list[line].add(value);
            pointInStr++;
        }
        else
            pointInStr = MAX_S;

        // Новая строка
        if (pointInStr >= MAX_S)
        {
            line++;
            pointInStr = 0;
            size++;

            if (line >= MAX_L)
                line = 0;

            list[line].clear();
        }
    }


};



#endif