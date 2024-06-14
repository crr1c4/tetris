#ifndef MAPEO_H
#define MAPEO_H


class Mapeo
{
public:
    public:
        void establecerVentana(float wx1, float wy1, float wx2, float wy2);
        void establecerPuertoVision(float pvx1, float pvy1, float pvx2, float pvy2);
        void mapear(int, int, int &xpv, int &ypv, int L, int M);

    private:
        float xwMin, xwMax, ywMin, ywMax;
        float xpvMin, ypvMin, xpvMax, ypvMax;

};

#endif // MAPEO_H
