#include "Const.h"
#include <windows.h>

double Const::curTime()  {
    return GetTickCount() / 1000.0;
}