#include "matrix.h"

#include <iostream>
#include <math.h>
#include <assert.h>

using namespace std;

Matrix::Matrix(int numeroRenglones,int numeroColumnas, float valorInicial) {
    renglones = numeroRenglones;
    columnas = numeroColumnas;

    // Se definen el número de renglones en el arreglo de punteros.
    datos = new float*[renglones];
    assert(datos);

    // Cada renglon apunta a un arreglo de longitud columnas.
    for(int r = 0; r < renglones; r++) {
        datos[r] = new float[columnas];
        assert(datos[r]);
    }

    for(int r = 0; r < renglones; r++)
        for(int c = 0; c < columnas; c++)
            datos[r][c] = valorInicial;
}

Matrix::Matrix(Matrix &matrizReferencia) {
    renglones = matrizReferencia.renglones;
    columnas = matrizReferencia.columnas;

    datos = new float*[renglones];
    assert(datos);

    for(int r = 0; r < renglones; r++) {
        datos[r] = new float[columnas];
        assert(datos[r]);
    }

    for(int i=0; i<renglones; i++)
        for( int j=0; j<columnas; j++)
            datos[i][j]=matrizReferencia.get(i,j);
}


inline void Matrix::set(int renglon, int columna, float valor) {
    datos[renglon][columna] = valor;
}

float Matrix::get(int renglon, int columna) {
    return datos[renglon][columna];
}

int Matrix::getNumeroColumnas() {
    return columnas;
}

int Matrix::getNumeroRenglones() {
    return renglones;
}

Matrix::~Matrix() {
    for(int i = 0; i < renglones; i++)
        delete [] datos[i];
    delete []datos;
}

Matrix &Matrix::operator*=(Matrix &m) {
   Matrix auxiliar(renglones, m.columnas);

   for(int r = 0; r < renglones; r++) {
        for(int c = 0; c < m.columnas; c++) {
            float suma = 0;

            for(int k = 0; k < columnas; k++) {
                //suma += datos[r][k] * m.get(k,c);
                suma += floor(datos[r][k] * m.get(k,c) + 0.5);
                auxiliar.set(r,c,suma);
            }
      }
   }

   asignar(auxiliar);
   //*this = auxiliar;
   return *this;
}

Matrix Matrix::operator*(float valorEscalar) {
    Matrix auxiliar(renglones, columnas);

    for(int r = 0; r < renglones; r++)
        for(int c = 0; c < columnas; c++)
            auxiliar.set(r, c, datos[r][c] * valorEscalar);

    return auxiliar;
}


Matrix &Matrix::operator*(const Matrix &m) {
    Matrix * result = new Matrix(renglones, columnas, 0);
    assert(result);

    for(int r=0; r < renglones; r++)
        for(int c = 0;  c < columnas; c++)
            for(int k = 0; k < columnas; k++)
                result->datos[r][c] += datos[r][k] * m.datos[k][c];

    return *result;
}

Matrix &Matrix::operator=(Matrix &m) {
    for(int r = 0; r < renglones; r++)
        delete [] datos[r];
    delete []datos;

    renglones= m.renglones;
    columnas = m.columnas;
    datos = new float*[renglones];

    assert(datos);

    for(int r = 0; r < renglones; r++) {
        datos[r] = new float[columnas];
        assert(datos[r]);
    }

    for( int r =0; r < renglones; r++)
        for(int c = 0; c < columnas; c++)
            datos[r][c] = m.datos[r][c];

    return *this;
}


Matrix &Matrix::asignar(Matrix m) {
    for(int r = 0; r < m.renglones; r++)
        for(int c = 0;c < m.columnas; c++)
            datos[r][c] = m.get(r,c);

    return *this;
}

Matrix &Matrix::asignarMatrizIdentidad() {
    for(int r = 0; r < renglones; r++)
        for(int c = 0; c < columnas; c++)
            if(r == c) datos[r][c]= 1;
            else datos[r][c]= 0;

  return *this;
}

Matrix &Matrix::traslacion(Matrix figura, int tx, int ty) {
    Matrix identidad(3, 3);
    identidad.asignarMatrizIdentidad();

    identidad.set(2, 0, tx);
    identidad.set(2, 1, ty);

    figura *= identidad;

    asignar(figura);
    return *this;
}

Matrix &Matrix::escalacion(Matrix figura, int sx, int sy) {
    Matrix identidad(3, 3);
    identidad.asignarMatrizIdentidad();

    identidad.set(0, 0, sx);
    identidad.set(1, 1, sy);

    figura *= identidad;

    asignar(figura);
    return *this;
}

Matrix &Matrix::escalacionPuntoFijo(Matrix figura, int pfx, int pfy, float sx, float sy) {
    Matrix identidad(3, 3);
    identidad.asignarMatrizIdentidad();

    float escalacionX = pfx * (1 - sx);
    float escalacionY = pfy * (1 - sy);

    identidad.set(0, 0, sx);
    identidad.set(1, 1, sy);
    identidad.set(2, 0, escalacionX);
    identidad.set(2, 1, escalacionY);

    figura *= identidad;

    asignar(figura);
    return *this;

}

Matrix &Matrix::rotar(Matrix figura, float angulo) {
    Matrix identidad(3, 3);
    identidad.asignarMatrizIdentidad();

    float rads = radianes(angulo);
    identidad.set(0, 0, cos(rads));
    identidad.set(0, 1, sin(rads));
    identidad.set(1, 0, -sin(rads));
    identidad.set(1, 1, cos(rads));

    figura*= identidad;

    asignar(figura);
    return *this;
}

Matrix &Matrix::rotarPuntoFijo(Matrix figura, double angulo, float ppx, float ppy) {
    Matrix identidad(3, 3);
    identidad.asignarMatrizIdentidad();

    float rads = radianes(angulo);
    float renglon2_0 = ppx * (1 - cos(rads)) + ppy * sin(rads);
    float renglon2_1 = ppy * (1 - cos(rads)) - ppx * sin(rads);

    identidad.set(0, 0, cos(rads));
    identidad.set(0, 1, sin(rads));
    identidad.set(1, 0, -sin(rads));
    identidad.set(1, 1, cos(rads));
    identidad.set(2, 0, renglon2_0);
    identidad.set(2, 1, renglon2_1);

    figura *= identidad;

    asignar(figura);
    return *this;
}

void Matrix::mostrarMatriz() {
    for(int r = 0; r < renglones; r++) {
        for(int c = 0; c < columnas; c++)
            cout << datos[r][c] << "\t";
        cout << endl;
    }

    cout << "-----------------------------------------------------------" << endl;
}

