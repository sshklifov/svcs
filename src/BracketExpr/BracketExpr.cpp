#include "BracketExpr.h"

#include <stdexcept>
#include <cassert>

const unsigned QUOTIENT_SHIFT = 6;
const unsigned REMAINDER_MASK = 63;

static void SetMaskBit (uint64_t mask[2], char c)
{
    unsigned quot = c >> QUOTIENT_SHIFT;
    unsigned rem = c & REMAINDER_MASK;
    mask[quot] |= uint64_t (1) << rem;
}

static uint64_t GenOnes (unsigned fromBit, unsigned toBit)
{
    assert (toBit <= 63);
    assert (toBit >= fromBit);

    unsigned range = toBit - fromBit;
    uint64_t res = (uint64_t (1) << range) - 1;
    res = (res<<1) | 1;
    res <<= fromBit;

    return res;
}

static void SetMaskBitRange (uint64_t mask[2], char from, char to)
{
    if (from > to)
    {
        throw std::runtime_error ("Scanset::Scanset: invalid range");
    }

    int scenario = ((from>=63) << 1) | (to>=63);
    switch (scenario)
    {
    case 0:
        mask[0] |= GenOnes (from, to);
        break;

    case 1:
        mask[0] |= GenOnes (from, 63);
        mask[1] |= GenOnes (0, to - 63);
        break;

    case 3:
        mask[1] |= GenOnes (from - 63, to - 63);
        break;

    default:
        assert ("unexpected expression result");
    };
}

static const char* SetMaskFmt (uint64_t mask[2], const char* fmt)
{
    if (*fmt != '[') throw std::runtime_error ("Scanset::Scanset: expected [");
    ++fmt;

    mask[0] = 0;
    mask[1] = 0;

    bool flip = (*fmt=='^');
    fmt += flip;

    if (*fmt == '-') throw std::runtime_error ("Scanset::Scanset: trailing - in begining");
    while (*fmt != ']' && *fmt)
    {
        while (*fmt != '-' && *fmt != ']' && *fmt)
        {
            SetMaskBit (mask, *fmt);
            ++fmt;
        }

        if (*fmt == '-')
        {
            if (*(fmt+1) == '\0' || (*fmt+1) == ']')
                throw std::runtime_error ("Scanset::Scanset: trailing - in end");
            SetMaskBitRange (mask, *(fmt-1), *(fmt+1));
            fmt += 2;
        }
    }
    if (*fmt != ']') throw std::runtime_error ("Scanset::Scanset: unmatched ]");

    uint64_t flipBits = 0;
    flipBits = ~flipBits * flip;
    mask[0] ^= flipBits;
    mask[1] ^= flipBits;

    return fmt + 1;
}

extern const char* BuildBracketExpr (BracketExpr& expr, const char* s)
{
    return SetMaskFmt (expr, s);
}

extern bool BracketExprContainsChar (const BracketExpr& expr, char c)
{
    unsigned quot = c >> QUOTIENT_SHIFT;
    unsigned rem = c & REMAINDER_MASK;
    return expr[quot] & uint64_t (1) << rem;
}

extern void CopyBracketExpr (BracketExpr& dst, const BracketExpr& src)
{
    dst[0] = src[0];
    dst[1] = src[1];
}
