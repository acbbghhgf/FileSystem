#include "wwk_uchar.h"


wwk_uchar_t *wwk_uchar_new(size_t len)
{
    wwk_uchar_t *uc;
    uc = (wwk_uchar_t *)wwk_malloc(len + sizeof(*uc));
    uc->len = len;
    if(len > 0)
    {
        uc->data = (unsigned char *)uc +sizeof(*uc);
    }
    else
    {
        uc->data = 0;
    }
    return uc;
}

int wwk_uchar_delete(wwk_uchar_t *uc)
{
    wwk_free(uc);
    return 0;
}

wwk_uchar_t* wwk_uchar_dup_data(unsigned char* data, size_t len)
{
    wwk_uchar_t *uc;
    uc = wwk_uchar_new(len);
    if(uc && data)
    {
        memcpy(uc->data, data, len);
    }
    return uc;
}

wwk_uchar_t* wwk_uchar_dup_data_x(wwk_uchar_t *src)
{
    wwk_uchar_t *uc;
    uc = wwk_uchar_dup_data(src->data, src->len);
    return uc;
}
