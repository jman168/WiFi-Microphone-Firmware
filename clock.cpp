#include <inttypes.h>
#include <math.h>
#include <iostream>

#define TARGET_FREQUENCY 24576000.0
#define XTAL 40000000.0

uint8_t best_sdm0, best_sdm1, best_sdm2, best_odiv;
double best_diff = 10000000000000000000000000.0;
double best_frequency = 0.0;

int main(int argc, char *argv[]) {
    for(uint16_t sdm0 = 0; sdm0 <= 255; sdm0++) {
        for(uint16_t sdm1 = 0; sdm1 <= 255; sdm1++) {
            for(uint8_t sdm2 = 0; sdm2 <= 63; sdm2++) {
                for(uint8_t odiv = 0; odiv <= 31; odiv++) {
                    double numerator = XTAL * ( (double)sdm2 + ((double)sdm1 / 256.0) + ((double)sdm0 / 65536.0) + 4.0 );
                    if(numerator < 500000000.0 && numerator > 350000000.0) {
                        double denomerator = 2.0 * ( (double)odiv + 2.0);
                        double frequency = numerator / denomerator;
                        if(abs(TARGET_FREQUENCY - frequency) < best_diff) {
                            best_diff = abs(TARGET_FREQUENCY - frequency);
                            best_sdm0 = sdm0;
                            best_sdm1 = sdm1;
                            best_sdm2 = sdm2;
                            best_odiv = odiv;
                            best_frequency = frequency;
                            printf("Found new best frequency: %f\n", frequency);
                        }
                    }
                }
            }
        }
    }

    printf("sdm0: %i, sdm1: %i, sdm2: %i, odiv: %i, rate: %f\n", best_sdm0, best_sdm1, best_sdm2, best_odiv, best_frequency);
}
