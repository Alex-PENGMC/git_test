#include "pengmc.h"

int main(void)
{
    char message1[256] = {
        "alexalexalex " " %s----%s " " pengmcpengmc"
    };
    char message2[256];

    ARRAY2ZERO(message2, 256);

    char *string1 = "today is sunny.";
    char *string2 = "i am test the special character.";

    snprintf(message2, ARRAY_LENGTH_1D(message2), message1, string1, string2);

    DBG_PRINTF("message1:%s", message1);

    LINE_FEED;

    DBG_PRINTF("message2:%s", message2);

    LINE_FEED;

    return 0;
}