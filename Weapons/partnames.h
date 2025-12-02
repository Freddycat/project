#ifndef PART_NAMES_H
#define PART_NAMES_H

#include <iostream>
#include <glm/glm.hpp>
#include <vector>

using std::vector;

enum class PART_NAME : uint8_t
{
    GUN = 1 << 0,
    BEAM = 1 << 1,
    BLAST = 1 << 2,
    TEST = 1 << 3,
};

// Setters must return PART_NAME

inline PART_NAME operator|(PART_NAME lhs, PART_NAME rhs)
{
  return static_cast<PART_NAME>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline PART_NAME &operator|=(PART_NAME &lhs, PART_NAME rhs)
{
  // This now works because lhs | rhs returns PART_NAME
  lhs = lhs | rhs; 
  return lhs;
}

// Checker must return the underlying integer type for implicit 'if' conversion

inline uint8_t operator&(PART_NAME lhs, PART_NAME rhs)
{
    // Return uint8_t (the underlying type) for use in 'if' statements
    return (static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline uint8_t operator&(uint8_t lhs, PART_NAME rhs)
{
    // Returns the result of the bitwise AND operation between the raw integer 
    // and the casted enum value.
    return (lhs & static_cast<uint8_t>(rhs));
}

/* 
// quick fixes to make my enums ints I guess
inline PART_NAME operator|(PART_NAME lhs, PART_NAME rhs)
{
    return static_cast<PART_NAME>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline PART_NAME &operator|=(PART_NAME &lhs, PART_NAME rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

inline PART_NAME operator&(PART_NAME lhs, PART_NAME rhs)
{
    return static_cast<PART_NAME>(static_cast<uint8_t>(lhs) &
                                  static_cast<uint8_t>(rhs));
}
                                   */

#endif // WEAPON_EVENTS_H