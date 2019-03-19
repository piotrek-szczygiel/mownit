#include <gsl/gsl_ieee_utils.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    float f = 1.0;
    for (unsigned short i = 0; i < 150; ++i) {
        f /= 2.0f;
        gsl_ieee_printf_float(&f);
        puts("\n");
    }

    return 0;
}
