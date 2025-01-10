#pragma once
#include <stdint.h>

// https://gist.github.com/ruby0x1/81308642d0325fd386237cfa3b44785c
// https://notes.underscorediscovery.com/constexpr-fnv1a/

namespace fnv1a
{
    constexpr uint64_t val_64_const = 0xcbf29ce484222325;
    constexpr uint64_t prime_64_const = 0x100000001b3;

    inline const uint64_t Hash(const char* key)
    {
        uint64_t len = strlen(key);

        uint64_t hash = val_64_const;
        uint64_t prime = prime_64_const;

        for (int i = 0; i < len; ++i) {
            uint8_t value = key[i];
            hash = hash ^ value;
            hash *= prime;
        }

        return hash;
    }

    inline constexpr uint64_t HashConst(const char* const str, const uint64_t value = val_64_const) noexcept
    {
        return (str[0] == '\0') ? value : HashConst(&str[1], (value ^ uint64_t((uint8_t)str[0])) * prime_64_const);
    }
}

#define HASH(str) fnv1a::HashConst(str)