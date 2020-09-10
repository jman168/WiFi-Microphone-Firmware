#include "imath.h"

int16_t table[32768];
bool table_filled = false;

void fill_table() {
    if(!table_filled) {
        for(int i = 0; i < 32768; i++) {   
            table[i] = sin((double)i * 0.000095874) * 32767.0;
        }
        table_filled = true;
    }
}

int16_t isin(int16_t theta) {
    fill_table();
    if(theta >= 0) {
        return table[theta];
    } else {
        return -table[abs(theta)];
    }
}