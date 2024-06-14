#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QIcon>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QMessageBox>

#include "mapeo.h"
#include "matrix.h"
#include "figura.h"

// Declaraciones del espacio de nombres para la interfaz de usuario.
QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

// Definición de la clase Dialog, heredando de QDialog.
class Dialog : public QDialog {
    Q_OBJECT

public:
    // Constructor y destructor.
    Dialog(QWidget *parent = nullptr); // Constructor con un parámetro opcional para el padre.
    ~Dialog(); // Destructor.

private:
    // Puntero a la interfaz de usuario generada por Qt Designer.
    Ui::Dialog *ui;
    // Puntero al objeto QPainter para realizar dibujos personalizados en el widget.
    QPainter* canvas;
    // Objeto Mapeo para manejar la lógica de mapeo del juego.
    Mapeo mapeo;
    // Puntero al objeto QTimer para manejar eventos temporizados.
    QTimer* timer;
    // Variables para la velocidad, ancho y alto del juego.
    int velocidad;
    int ancho, alto;
    // Variable para llevar la puntuación del juego.
    int puntuacion = 0;

    // Punteros a las figuras actual y siguiente.
    Figura* figuraActual;
    Figura* figuraSiguiente;

    // Array para almacenar las figuras colocadas y su cantidad.
    Figura* figurasColocadas[100];
    int cantidadFigurasColocadas;

    // Métodos privados para la lógica del juego.
    void cargarFigura(); // Cargar la figura actual.
    void generarFiguraSiguiente(); // Generar la siguiente figura.
    void cargarFiguraSiguiente(TipoFigura tipo); // Cargar la siguiente figura de un tipo específico.
    bool detectarColision(); // Detectar colisiones en el juego.
    // Objetos QMediaPlayer para reproducir sonidos del juego.
    QMediaPlayer* reproductorFiguraColocada;
    QMediaPlayer* reproductorFinJuego;
    // Método para pausar el juego.
    void pausar();

    // Método para reiniciar el juego.
    void reiniciarJuego();

private slots:
    // Slots privados para manejar eventos.
    void bajarFigura(); // Bajar la figura actual.
    void on_btnReiniciar_clicked(); // Manejar el clic en el botón de reiniciar.

protected:
    // Métodos protegidos para manejar eventos de pintura y teclado.
    void paintEvent(QPaintEvent* event) override; // Evento de pintura.
    void keyPressEvent(QKeyEvent* event) override; // Evento de presionar una tecla.
};

#endif // DIALOG_H
