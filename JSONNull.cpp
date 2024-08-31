#include "JSONNull.h"

JSONValueType JSONNull::getType() const
{
    return JSONValueType::NILL;
}

std::string JSONNull::toString() const
{
    return "null";
}
