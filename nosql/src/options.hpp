//
// Created by 0x822a5b87 on 2025/1/16.
//

#ifndef NOSQL_OPTIONS_HPP
#define NOSQL_OPTIONS_HPP

const float DefaultMinFillFactor = 0.5;
const float DefaultMaxFillFactor = 0.95;

class Options
{
public:
    // minFillFactor indicates the minimum fill factor to combine two node.
    float minFillFactor;
    // maxFillFactor indicates the maximum fill factor to separate one node.
    float maxFillFactor;

    Options() : minFillFactor(DefaultMinFillFactor), maxFillFactor(DefaultMaxFillFactor)
    {}

    Options(float minFillFactor, float maxFillFactor) : minFillFactor(minFillFactor), maxFillFactor(maxFillFactor)
    {}

    ~Options() = default;
};


#endif //NOSQL_OPTIONS_HPP
