#include "Validaciones.h"
#define MI_TOUPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 32) : (c))
#define MI_TOLOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define ES_LETRA(c)   (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') ? 1 : 0)

int normalizar_nombre(const char* nombre_original, char* nombre_normalizado)
{
    char* lec = (char*)nombre_original;
    char* esc = nombre_normalizado;
    int palabrasEscritas = 0;


    while(*lec) // Mientras haya cadena para leer
    {
        while(*lec && !ES_LETRA(*lec)) // Busco la primera/proxima palabra, ignoro espacios y ','
        {
            lec++;
        }

        if(*lec) // Proceso Palabra
        {
            if(palabrasEscritas == 1) // Escribir separador ANTES de esta palabra
            {
                // Segunda palabra => viene el nombre: escribir ", "
                *esc = ',';
                esc++;
                *esc = ' ';
                esc++;
            }
            else if(palabrasEscritas > 1)
            {
                // Tercera palabra en adelante => espacio entre nombres
                *esc = ' ';
                esc++;
            }

            *esc = MI_TOUPPER(*lec); // Primer letra Mayuscula
            lec++;
            esc++;
            while(*lec && ES_LETRA(*lec)) // Resto de letras Minsucula
            {
                *esc = MI_TOLOWER(*lec);
                lec++;
                esc++;
            }

            palabrasEscritas++;
        }
    }
    *esc = '\0';
    return 1;
}
