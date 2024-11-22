#ifndef KDS_ERR_H_
#define KDS_ERR_H_

typedef enum  {
    KDS_OK                  = 0,
    KDS_NO_MEM              = (1 << 0),
    KDS_ACCESS_DENIED       = (1 << 1),
    KDS_INVALID_PARAM       = (1 << 2),
    KDS_INIT_FAIL           = (1 << 3),
    KDS_NO_VALUE            = (1 << 4)
} kds_error;

#endif // KDS_ERR_H_