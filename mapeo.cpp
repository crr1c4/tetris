#include "mapeo.h"

void Mapeo::establecerVentana(float wx1, float wy1, float wx2, float wy2) {
    xwMin = wx1;
    ywMin = wy1;
    xwMax = wx2;
    ywMax = wy2;
}

void Mapeo::establecerPuertoVision(float pvx1, float pvy1, float pvx2, float pvy2) {
    xpvMin = pvx1;
    ypvMin = pvy1;
    xpvMax = pvx2;
    ypvMax = pvy2;
}

void Mapeo::mapear(int xW, int yW, int &xpv, int &ypv, int L, int M) {
    float sx = (xpvMax - xpvMin) / (xwMax - xwMin);
    float sy = (ypvMax - ypvMin) / (ywMax - ywMin);
    xpv = sx * (xW - xwMin) + xpvMin + L;
    ypv = sy * (ywMin - yW) + ypvMin + M;
}
