#ifndef JSON_NUMBER_H
#define JSON_NUMBER_H

#include <cmath>

#include "JSONValue.h"

class JSONNumber : public JSONValue
{
public:
    JSONNumber(double value);

    JSONValueType getType() const override;

    std::string toString() const override;

private:
    double value;
};

#endif