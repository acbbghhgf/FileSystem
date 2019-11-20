#ifndef __WWK_UCHAR_HH
#define __WWK_UCHAR_HH

#include "wwk_type.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wwk_uchar
{
    unsigned char *data;
    size_t len;
}wwk_uchar_t;

wwk_uchar_t *wwk_uchar_new(size_t len);
int wwk_uchar_delete(wwk_uchar_t *uc);
wwk_uchar_t* wwk_uchar_dup_data(unsigned char* data, size_t len);
wwk_uchar_t* wwk_uchar_dup_data_x(wwk_uchar_t *src);



#ifdef __cplusplus
};
#endif
#endif