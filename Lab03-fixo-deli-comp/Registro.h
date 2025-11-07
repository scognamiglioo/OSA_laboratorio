#ifndef REGISTRO_H
#define REGISTRO_H

#include "Buffer.h"

enum Formato { FIXO, DELIMITADO, COMPRIMENTO };

class Buffer; 
class Registro {
public:
    virtual ~Registro() = default;

    virtual void pack(Buffer& buffer, Formato formato) const = 0;
    virtual void unpack(Buffer& buffer, Formato formato) = 0;
};

#endif
