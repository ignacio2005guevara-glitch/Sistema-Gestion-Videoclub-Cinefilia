#include "Validaciones.h"
#define MI_TOUPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 32) : (c))
#define MI_TOLOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define ES_LETRA(c)   (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') ? 1 : 0)

int normalizar_nombre(const char* nombre_original, char* nombre_normalizado)
{
    char* lec = (char*)nombre_original;
    char* esc = nombre_normalizado;
    bool primeraLetra = true;
    bool apellidoEscrito = false;


    while(*lec)
    {
        while(*lec && !ES_LETRA(*lec))
        {
            lec++;
        }
        // Proceso Palabra
        if(*lec)
        {
            primeraLetra = true;
            while(*lec && ES_LETRA(*lec))
            {
                *esc = primeraLetra == true ? MI_TOUPPER(*lec) : MI_TOLOWER(*lec);
                primeraLetra = false;
                lec++;
                esc++;
            }
            if(apellidoEscrito == false)
            {
                *esc = ',';
                esc++;
                apellidoEscrito = true;
            }
            *esc = ' ';
            esc++;
        }
    }
    *(esc-1) = '\0';
    return 1;
}
