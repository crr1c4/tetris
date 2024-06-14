#ifndef FIGURA_H
#define FIGURA_H

#include <QPainter>
#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <limits>

#include "mapeo.h"
#include "matrix.h"

// Enumeración para los tipos de figuras.
enum TipoFigura {
    C, // Cuadrado
    L, // Tipo L
    T, // Tipo T
    I, // Tipo Línea
    Z, // Tipo Z
};

// Enumeración para las orientaciones posibles de una figura.
enum Orientacion {
    N, // Norte
    S, // Sur
    E, // Este
    W  // Oeste
};

class Figura {

    public:
        // Constructor que inicializa la figura con un tipo específico.
        Figura(TipoFigura tipo);

        // Constructor por defecto.
        Figura();

        // Dibuja la figura en el canvas.
        void dibujar(QPainter* canvas, Mapeo* mapeo, int L, int M);

        // Movimientos de la figura: trasladar, rotar y escalar.
        void mover(int tx, int ty);
        void rotar(int angulo);
        void escalar(float sx, float sy, int ppx, int ppy);

        // Detecta si la figura colisiona con las orillas del canvas.
        bool detectarColision(int anchoJuego);

        // Detecta si la figura colisiona con otra figura.
        bool detectarColision(Figura *figuraReferencia);

        // Método para verificar si la figura está fuera del área de juego.
        bool verificarArea(int altura);

        // Devuelve el tipo de la figura.
        TipoFigura obtenerTipo();

    private:
        // Tipo de la figura (cuadrado, línea, etc.).
        TipoFigura tipo;

        // Orientación de la figura.
        Orientacion orientacion;

        // Arreglo que contiene los puntos de la figura.
        int puntos[9][3];

        // Arreglo para puntos del esqueleto de la figura.
        float puntosEsqueleto[5][3];

        // Asigna los puntos del esqueleto.
        void asignarEsqueleto();

        // Color de la figura.
        QColor color;

        // Número de vertices que tiene la figura.
        int numeroVertices;

        // Número de puntos en el esqueleto.
        int numeroPuntosEsqueleto;

        // Asignación de los puntos y colores a la figura.
        void asignarVertices();
        void asignarColor();
};

#endif // FIGURA_H
