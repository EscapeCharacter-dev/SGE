#include "include/sge.h"

void update(uint64_t ticks) {

}

int main(void) {
    struct sge_init_struct initstrct;
    initstrct.fs = FALSE;
    initstrct.title = "SGE 0.0.1";
    initstrct.wW = initstrct.wH = initstrct.vW = initstrct.vH = 720;
    sge_init(initstrct);
    sge_start(&update);
    sge_end();
    return 0;
}