#ifndef MYREGEX_H
#define MYREGEX_H

#include <QRegExp>

namespace creg
{
  enum RegularExpressions
  {
    REGEXP_LOGIN,
    REGEXP_PASSWORD,
    REGEXP_NAME,
    REGEXP_CITIZENSHIP,
    REGEXP_EMAIL
  };

  QRegExp getRegularExpression(RegularExpressions type);
}

#endif // MYREGEX_H
