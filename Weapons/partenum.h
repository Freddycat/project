#ifndef PART_ENUM_H
#define PART_ENUM_H

#include <iostream>
#include <glm/glm.hpp>
#include <vector>

#include "weaponparts.h"
#include "partnames.h"

using std::vector;

template <typename T>
struct PART_ENUM;
template <>
struct PART_ENUM<GunPart>
{
    static constexpr PART_NAME value = PART_NAME::GUN;
};
template <>
struct PART_ENUM<BeamPart>
{
    static constexpr PART_NAME value = PART_NAME::BEAM;
};
template <>
struct PART_ENUM<BlastPart>
{
    static constexpr PART_NAME value = PART_NAME::BLAST;
};
template <>
struct PART_ENUM<TestComponent>
{
    static constexpr PART_NAME value = PART_NAME::TEST;
};


template <typename T>
struct PART_MASK
{
    static constexpr uint8_t value = static_cast<uint8_t>(PART_ENUM<T>::value);
    // static inline const std::size_t value = next_id++;
};

#endif // WEAPON_EVENTS_H