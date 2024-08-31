#include "JSONBool.h"

JSONBool::JSONBool(bool value) : value(value) {}

JSONValueType JSONBool::getType() const
{
    return JSONValueType::BOOL;
}

std::string JSONBool::toString() const
{
    return value ? "true" : "false";
}
