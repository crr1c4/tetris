#include "figura.h"

/**
 * @author Christian Venegas González.
 * @date 9/6/2024
 */

/**
 * @brief Figura::Figura El constructor recomendado para la figura.
 * @param tipo: Tipo de figura que se manejara.
 */
Figura::Figura(TipoFigura tipo) {
    this->tipo = tipo;
    asignarVertices();
    asignarColor();
    asignarEsqueleto();
    orientacion = N;
}

/**
 * Constructor por defecto
 * @brief Figura::Figura
 * @deprecated
 */
Figura::Figura() {}

/**
 * @brief Figura::dibujar Dibuja la figura en el canvas.
 * @param canvas: El puntero del lienzo donde se va a dibujar la figura.
 * @param mapeo: El puntero donde se mapeara la figura.
 * @param L: La base x para el mapeo.
 * @param M: La base y para el mapeo.
 */
void Figura::dibujar(QPainter *canvas, Mapeo *mapeo, int L, int M) {
    int x, y;
    QPolygon poligono; // Objeto poligono

    canvas->setPen(QPen(color, 2)); // Color del poligono.
    for (int i = 0; i < numeroVertices; i++) {
        // Los puntos se mapean y se asignan al poligono.
        mapeo->mapear(puntos[i][0], puntos[i][1], x, y, L, M);
        poligono << QPoint(x, y);
    }

    // Se manda a dibujar al poligono en el canvas.
    canvas->drawPolygon(poligono);
}
/**
 *
 * @brief Figura::mover Método para trasladar tanto la figura como el esqueleto.
 * @param tx La traslación en x.
 * @param ty La traslacíon en y.
 */
void Figura::mover(int tx, int ty) {
    // Declaracion de las metrices auxiliares.
    Matrix figura(numeroVertices, 3);
    Matrix resultado(numeroVertices, 3);

    // Los puntos se pasan a la matriz figura, se realiza la traslación, y regresan.
    for (int r = 0; r < numeroVertices; r++) for (int c = 0; c < 3; c++) figura.set(r, c, puntos[r][c]);
    resultado.traslacion(figura, tx, ty); // Se aplica la traslación.
    for (int r = 0; r < numeroVertices; r++) for (int c = 0; c < 3; c++) puntos[r][c] = resultado.get(r, c);

    // Se aplica el mismo procedimiento para el esqueleto.
    Matrix esqueleto(numeroPuntosEsqueleto, 3);
    Matrix resultadoEsqueleto(numeroPuntosEsqueleto, 3);

    for (int r = 0; r < numeroPuntosEsqueleto; r++) for (int c = 0; c < 3; c++) esqueleto.set(r, c, puntosEsqueleto[r][c]);
    resultadoEsqueleto.traslacion(esqueleto, tx, ty);
    for (int r = 0; r < numeroPuntosEsqueleto; r++) for (int c = 0; c < 3; c++) puntosEsqueleto[r][c] = resultadoEsqueleto.get(r, c);
}

/**
 * @brief Figura::rotar Método para rotar tanto la figura como el esqueleto.
 * @param angulo El angulo usado para rotar (en grados).
 */
void Figura::rotar(int angulo) {
    // Declaracion de las metrices auxiliares.
    Matrix figura(numeroVertices, 3);
    Matrix resultado(numeroVertices, 3);

    // Los puntos se pasan a la matriz figura, se realiza la ratación, y regresan.
    for (int r = 0; r < numeroVertices; r++) for (int c = 0; c < 3; c++) figura.set(r, c, puntos[r][c]);
    resultado.rotarPuntoFijo(figura, angulo, puntos[0][0], puntos[0][1]);
    for (int r = 0; r < numeroVertices; r++) for (int c = 0; c < 3; c++) puntos[r][c] = resultado.get(r, c);

    // Se aplica el mismo procedimiento para el esqueleto.
    Matrix esqueleto(numeroPuntosEsqueleto, 3);
    Matrix resultadoEsqueleto(numeroPuntosEsqueleto, 3);

    for (int r = 0; r < numeroPuntosEsqueleto; r++) for (int c = 0; c < 3; c++) esqueleto.set(r, c, puntosEsqueleto[r][c]);
    resultadoEsqueleto.rotarPuntoFijo(esqueleto, angulo, puntosEsqueleto[0][0], puntosEsqueleto[0][1]);

    // Debido a que los puntos del esqueleto se mueven al rotarlos, se debe manejar su orientacion y trasladar dependiendo de hacia a donde esten mirando.
    if (orientacion == Orientacion::N) orientacion = W;
    else if (orientacion == Orientacion::W) orientacion = S;
    else if (orientacion == Orientacion::S) orientacion = E;
    else if (orientacion == Orientacion::E) orientacion = N;

    if (orientacion == W) resultadoEsqueleto.traslacion(resultadoEsqueleto, -1, 0);
    if (orientacion == S) resultadoEsqueleto.traslacion(resultadoEsqueleto, 0, -1);
    if (orientacion == E) resultadoEsqueleto.traslacion(resultadoEsqueleto, 1, 0);
    if (orientacion == N) resultadoEsqueleto.traslacion(resultadoEsqueleto, 0, 1);

    for (int r = 0; r < numeroPuntosEsqueleto; r++) for (int c = 0; c < 3; c++) puntosEsqueleto[r][c] = resultadoEsqueleto.get(r, c);
}

/**
 * @brief Figura::escalar Método para escalar la figura.
 * @param sx El factor de escalación en x.
 * @param sy El factor de escalación en y.
 * @param ppx El punto pivote en x.
 * @param ppy El punto pivote en y.
 */
void Figura::escalar(float sx, float sy, int ppx, int ppy) {
    // Declaracion de las metrices auxiliares.
    Matrix figura(numeroVertices, 3);
    Matrix resultado(numeroVertices, 3);

    // Los puntos se pasan a la matriz figura, se realiza la escalación, y regresan.
    for (int r = 0; r < numeroVertices; r++) for (int c = 0; c < 3; c++) figura.set(r, c, puntos[r][c]);
    resultado.escalacionPuntoFijo(figura, ppx, ppy, sx, sy);
    for (int r = 0; r < numeroVertices; r++) for (int c = 0; c < 3; c++) puntos[r][c] = resultado.get(r, c);
}

/**
 * @brief Figura::detectarColision Método que indica si la figura salio del plano de juego
 * @param anchoJuego el ancho del plano de juego.
 * @return true si salio del campo del juego, falso en caso contrario.
 */
bool Figura::detectarColision(int anchoJuego) {
    for (int i = 0; i < numeroVertices; i++)
        if (puntos[i][0] < 0 || puntos[i][0] > anchoJuego || puntos[i][1] < 0) return true;
    return false;
}

/**
 * @brief Figura::detectarColision Metodo que verifica si existe una colision con otra figura.
 * @param figuraReferencia El puntero a la otra figura con la que se va a verificar la colisión.
 * @return true si hay colisión, false en caso contrario.
 */
bool Figura::detectarColision(Figura *figuraReferencia) {
    // Declaracion del poligono auxiliar.
    QPolygonF poligono;

    // Conversión de la figura en poligono.
    for (int i = 0; i < figuraReferencia->numeroVertices; i++)
        poligono << QPointF(figuraReferencia->puntos[i][0], figuraReferencia->puntos[i][1]);

    //Verificar si algún punto de la figura this está dentro del otra figura, usando el esqueleto.
    for (int i = 0; i < numeroPuntosEsqueleto; i++)
         if (poligono.containsPoint(QPointF(puntosEsqueleto[i][0] + 0.5, puntosEsqueleto[i][1] + 0.5) , Qt::OddEvenFill))
             return true;

    return false;
}

/**
 * @brief Figura::verificarArea Método para verificar si la figura esta arriba del plano de juego, esto sirve para verificar el fin del juego.
 * @param altura la altura del plano de juego.
 * @return true si esta arriba del plano, falso en caso cantrario.
 */
bool Figura::verificarArea(int altura) {
    for (int i = 0; i < numeroVertices; i++)
        if (puntos[i][1] > altura) return true;
    return false;
}

/**
 * @brief Figura::obtenerTipo Obtiene el tipo de figura.
 * @return TipoFigura
 */
TipoFigura Figura::obtenerTipo() {
    return tipo;
}

/**
 * @brief Figura::asignarPuntos Metodo para elegir los vertices dependiendo del tipo de figura.
 */
void Figura::asignarVertices() {
    // Cada figura tiene un numero distintos de vertices.
    if (tipo == C) numeroVertices = 5;
    if (tipo == I) numeroVertices = 5;
    if (tipo == L) numeroVertices = 7;
    if (tipo == T) numeroVertices = 9;
    if (tipo == Z) numeroVertices = 9;

    // Asignacion de la ultima columna de la matriz.
    for (int i = 0; i < numeroVertices; i++) puntos[i][2] = 1;

    // Coordenadas de la figura del cubo.
    if (tipo == C) {
        puntos[0][0] = 0;	puntos[0][1] = 0;
        puntos[1][0] = 2;	puntos[1][1] = 0;
        puntos[2][0] = 2;	puntos[2][1] = 2;
        puntos[3][0] = 0;	puntos[3][1] = 2;
        puntos[4][0] = 0;	puntos[4][1] = 0;
    }

    // Coordenadas de la figura de la linea.
    if (tipo == I) {
        puntos[0][0] = 0;	puntos[0][1] = 0;
        puntos[1][0] = 1;	puntos[1][1] = 0;
        puntos[2][0] = 1;	puntos[2][1] = 4;
        puntos[3][0] = 0;	puntos[3][1] = 4;
        puntos[4][0] = 0;	puntos[4][1] = 0;
    }

    // Coordenadas de la figura L.
    if (tipo == L) {
        puntos[0][0] = 0;	puntos[0][1] = 0;
        puntos[1][0] = 2;	puntos[1][1] = 0;
        puntos[2][0] = 2;	puntos[2][1] = 3;
        puntos[3][0] = 1;	puntos[3][1] = 3;
        puntos[4][0] = 1;	puntos[4][1] = 1;
        puntos[5][0] = 0;	puntos[5][1] = 1;
        puntos[6][0] = 0;	puntos[6][1] = 0;
    }

    // Coordenadas de la figura T.
    if (tipo == T) {
        puntos[0][0] = 1;	puntos[0][1] = 0;
        puntos[1][0] = 2;	puntos[1][1] = 0;
        puntos[2][0] = 2;	puntos[2][1] = 3;
        puntos[3][0] = 1;	puntos[3][1] = 3;
        puntos[4][0] = 1;	puntos[4][1] = 2;
        puntos[5][0] = 0;	puntos[5][1] = 2;
        puntos[6][0] = 0;	puntos[6][1] = 1;
        puntos[7][0] = 1;	puntos[7][1] = 1;
        puntos[8][0] = 1;	puntos[8][1] = 0;
    }

    // Coordenadas de la figura Z.
    if (tipo == Z) {
        puntos[0][0] = 0;	puntos[0][1] = 0;
        puntos[1][0] = 1;	puntos[1][1] = 0;
        puntos[2][0] = 1;	puntos[2][1] = 1;
        puntos[3][0] = 2;	puntos[3][1] = 1;
        puntos[4][0] = 2;	puntos[4][1] = 3;
        puntos[5][0] = 1;	puntos[5][1] = 3;
        puntos[6][0] = 1;	puntos[6][1] = 2;
        puntos[7][0] = 0;	puntos[7][1] = 2;
        puntos[8][0] = 0;	puntos[8][1] = 0;
    }
}

/**
 * @brief Figura::asignarEsqueleto Asignacion del esqueleto de la figura, el esqueleto nos sirve para checar colisiones.
 */
void Figura::asignarEsqueleto() {
    // Todas las figuras tienen 4 cuadros, por lo que el numero de puntos es 4
    numeroPuntosEsqueleto = 4;

    // Asignacion de la ultima columna de la matriz.
    for (int i = 0; i < numeroPuntosEsqueleto; i++) puntosEsqueleto[i][2] = 1;

    // Coordenadas de la figura del cubo.
    if (tipo == C) {
        puntosEsqueleto[0][0] = 0;    puntosEsqueleto[0][1] = 0;
        puntosEsqueleto[1][0] = 1;	  puntosEsqueleto[1][1] = 0;
        puntosEsqueleto[2][0] = 1;    puntosEsqueleto[2][1] = 1;
        puntosEsqueleto[3][0] = 0;    puntosEsqueleto[3][1] = 1;
    }

    // Coordenadas de la figura de la linea.
    if (tipo == I) {
        puntosEsqueleto[0][0] = 0;	  puntosEsqueleto[0][1] = 0;
        puntosEsqueleto[1][0] = 0;	  puntosEsqueleto[1][1] = 1;
        puntosEsqueleto[2][0] = 0;    puntosEsqueleto[2][1] = 2;
        puntosEsqueleto[3][0] = 0;    puntosEsqueleto[3][1] = 3;
    }

    // Coordenadas de la figura L.
    if (tipo == L) {
        puntosEsqueleto[0][0] = 0;	  puntosEsqueleto[0][1] = 0;
        puntosEsqueleto[1][0] = 1;	  puntosEsqueleto[1][1] = 0;
        puntosEsqueleto[2][0] = 1;    puntosEsqueleto[2][1] = 1;
        puntosEsqueleto[3][0] = 1;    puntosEsqueleto[3][1] = 2;
    }

    // Coordenadas de la figura T.
    if (tipo == T) {
        puntosEsqueleto[0][0] = 1;	  puntosEsqueleto[0][1] = 0;
        puntosEsqueleto[1][0] = 1;	  puntosEsqueleto[1][1] = 1;
        puntosEsqueleto[2][0] = 0;    puntosEsqueleto[2][1] = 1;
        puntosEsqueleto[3][0] = 1;	  puntosEsqueleto[3][1] = 2;
    }

    // Coordenadas de la figura Z o serpiente.
    if (tipo == Z) {
        puntosEsqueleto[0][0] = 0;	  puntosEsqueleto[0][1] = 0;
        puntosEsqueleto[1][0] = 0;	  puntosEsqueleto[1][1] = 1;
        puntosEsqueleto[2][0] = 1;    puntosEsqueleto[2][1] = 1;
        puntosEsqueleto[3][0] = 1;    puntosEsqueleto[3][1] = 2;
    }
}

/**
 * @brief Figura::asignarColor Método que asigna el color de la figura dependiendo de su tipo.
 */
void Figura::asignarColor() {
    if (tipo == C) color = QColor(253, 245, 30);
    if (tipo == I) color = QColor(125, 35, 181);
    if (tipo == L) color = QColor(0, 156, 255);
    if (tipo == T) color = QColor(213, 7, 7);
    if (tipo == Z) color = QColor(62, 254, 27);
}

