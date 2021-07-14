#ifndef __CONTACTO_H__
#define __CONTACTO_H__
struct _Contacto {
    char *nombre;
    char *apellido;
    unsigned edad;
    char *telefono;
};
typedef struct _Contacto *Contacto;
#endif /* __CONTACTO_H__ */
