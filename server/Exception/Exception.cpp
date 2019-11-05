#include "Exception.h"
#include "../const.h"
#include "../scheduler/Logic.h"

Json Exception::dump() {
    return Logic::toErrMessage(message, errCode);
}

void Exception::setMessage(Json message) {
    this->message = message;
}
