#include "conn/Connection.h"
#include "GlobalUtil.h"
int main() {
    conn::init("../server.json");
    conn::waitConns();
    GlobalUtil::init();
    conn::mainloop();
    return 0;
}
