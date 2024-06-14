#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>

class Matrix {
    private:
        int columnas, renglones;
        float **datos;
        Matrix& asignar(Matrix m);

    public:
       Matrix(int numeroRenglones = 3,int numeroColumnas = 3, float valorIncial = 0.0f);
       Matrix(Matrix &matrizReferencia);

       ~Matrix();

       double radianes(int angulo) {
           return angulo * M_PI / 180.0;
       }
       void set(int renglon, int columna, float valor);
       float get(int renglon, int columna);
       int getNumeroRenglones();
       int getNumeroColumnas();

       Matrix &operator *=(Matrix &m);
       Matrix &operator*(const Matrix &m);
       Matrix operator*(float valorEscalar);
       Matrix &operator=(Matrix &m);
       Matrix &asignarMatrizIdentidad();
       Matrix &traslacion(Matrix figura, int tx, int ty);
       Matrix &escalacion(Matrix figura, int sx, int sy);
       Matrix &escalacionPuntoFijo(Matrix figura, int tx, int ty, float sx, float sy);
       Matrix &rotar(Matrix figura, float angulo);
       Matrix &rotarPuntoFijo(Matrix figura, double angulo, float, float);
       void mostrarMatriz();
};

#endif // MATRIX_H
