#include "myregex.h"

QRegExp creg::getRegularExpression(creg::RegularExpressions type)
{
  QRegExp expression;
  switch (type)
    {
    case REGEXP_PASSWORD: expression.setPattern("(?=.*[0-9])(?=.*[!@#$%^&*])(?=.*[a-z])(?=.*[A-Z])[0-9a-zA-Z!@#$%^&*]{6,}"); break;
    case REGEXP_LOGIN: expression.setPattern("^[A-z0-9_-]{1,20}$"); break;
    case REGEXP_NAME: expression.setPattern("([A-z]{1}[a-z]{1,20})$"); break;
    case REGEXP_EMAIL: expression.setPattern("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}\\b"); break;
    case REGEXP_CITIZENSHIP: expression.setPattern("([A-z]{1}[a-z]{1,20})$"); break;
    }

  return expression;
}

