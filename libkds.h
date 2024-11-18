#ifndef __LIBKDS_H_
#define __LIBKDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KDS_DYNLIB
#   ifdef __GNUC__ 
#       define      KDS_EXPORT     __attribute__((visiblity("default")))
#       define      KDS_IMPORT
#   elif defined(__MSC_VER)
#       define      KDS_EXPORT     __declspec(export)
#       define      KDS_IMPORT     __declspec(import)
#   endif
#else
#   define KDS_EXPORT
#   define KDS_IMPORT
#endif // KDS_DYNLIB


#define KDS_STR(x)              #x
#define KDS_MOD_NAME            KESTREL_DATA_SOURCE_MOD
#define KDS_MOD_NAME_STR        KDS_STR(KESTREL_DATA_SOURCE_MOD)

#ifdef __cplusplus
}
#endif

#endif // __LIBKDS_H_