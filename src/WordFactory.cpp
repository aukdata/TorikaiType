//
//  WordFactory.cpp
//  TorikaiType
//

#include "WordFactory.hpp"

WordType operator&(const WordType l, const WordType r) { return static_cast<WordType>(static_cast<uint8>(l) & static_cast<uint8>(r)); }
