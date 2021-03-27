#include "MFC-Win.h"

namespace MFC
{

  void AfxMessageBox(std::string_view s)
  {
    ::AfxMessageBox((LPCTSTR) s.data());
  }

};
