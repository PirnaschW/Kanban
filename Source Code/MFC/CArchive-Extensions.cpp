
#include "pch.h"

CArchive& operator << (CArchive& ar, const std::wstring& s) { ar << s.size(); for (const auto& c : s) ar << c;; return ar; }
CArchive& operator >> (CArchive& ar, std::wstring& s) { size_t l; ar >> l; s.resize(l); for (auto& c : s) ar >> c; return ar; }
