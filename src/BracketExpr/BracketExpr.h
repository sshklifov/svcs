#ifndef BRACKET_EXPR_INCLUDED
#define BRACKET_EXPR_INCLUDED

#include <cstdint>

using BracketExpr = uint64_t[2];

extern const char* BuildBracketExpr (BracketExpr& expr, const char* s);
extern bool BracketExprContainsChar (const BracketExpr& expr, char c);
extern void CopyBracketExpr (BracketExpr& dst, const BracketExpr& src);

#endif /* BRACKET_EXPR_INCLUDED */
