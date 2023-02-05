/******************************************************************************/
#ifndef SYSTEM_COMMON_H
#define SYSTEM_COMMON_H 
/******************************************************************************/
#define PASTE3B_SIMPLE(a, b, c)                a b c
#define PASTE3_SIMPLE(a, b, c)                 PASTE3B_SIMPLE(a, b, c)

#define PASTE3B(a, b, c)                       a##b##c
#define PASTE3(a, b, c)                        PASTE3B(a, b, c)

#define PASTE2B(a, b)                          a##b
#define PASTE(a, b)                            PASTE2B(a, b)
/******************************************************************************/
#endif /* SYSTEM_COMMON_H */
/******************************************************************************/