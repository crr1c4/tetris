#include "dialog.h"
#include "ui_dialog.h"

/**
 * @author Christian Venegas González.
 * @date 9/6/2024
*/

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this); // Configura la interfaz de usuario.
    setWindowIcon(QIcon(":img/icono.png")); // Establece el ícono de la ventana.

    // Inicialización del reproductor de sonidos para la figura colocada.
    reproductorFiguraColocada = new QMediaPlayer();
    reproductorFiguraColocada->setMedia(QUrl("qrc:/sonidos/colocado.mp3")); // Establece el archivo de sonido.
    reproductorFiguraColocada->setVolume(20); // Ajusta el volumen del sonido.

    // Inicialización del reproductor de sonidos para el fin del juego.
    reproductorFinJuego = new QMediaPlayer();
    reproductorFinJuego->setMedia(QUrl("qrc:/sonidos/fin.mp3")); // Establece el archivo de sonido.
    reproductorFinJuego->setVolume(20); // Ajusta el volumen del sonido.

    // Inicialización del temporizador.
    timer = new QTimer();
    velocidad = 750; // Establece la velocidad de caída de las figuras en milisegundos.

    cargarFigura(); // Carga la figura actual.
    generarFiguraSiguiente(); // Genera la siguiente figura.

    cantidadFigurasColocadas = 0; // Inicializa la cantidad de figuras colocadas.
    puntuacion = 0; // Inicializa la puntuación.

    // Ajusta el ancho y alto del área de juego según el tamaño del widget y del menú.
    ancho = size().width() - ui->menu->size().width();
    alto = size().height();

    // Conecta el temporizador con la función bajarFigura para que se llame cada vez que el temporizador se active.
    connect(timer, SIGNAL(timeout()), this, SLOT(bajarFigura()));
    timer->start(velocidad); // Inicia el temporizador con el intervalo especificado.
}

Dialog::~Dialog() {
    delete ui; // Libera la memoria asignada a la interfaz de usuario.
    delete canvas; // Libera la memoria asignada al objeto QPainter.
    delete timer; // Libera la memoria asignada al temporizador.
    delete figuraActual; // Libera la memoria asignada a la figura actual.
    delete figuraSiguiente; // Libera la memoria asignada a la figura siguiente.

    // Libera la memoria asignada a todas las figuras colocadas.
    for (int i = 0; i < cantidadFigurasColocadas; i++)
        delete figurasColocadas[i];

    // Libera la memoria asignada a los reproductores de sonido.
    delete reproductorFiguraColocada;
    delete reproductorFinJuego;
}

void Dialog::cargarFigura() {
    // Crea una nueva figura de un tipo aleatorio.
    figuraActual = new Figura(static_cast<TipoFigura>(QRandomGenerator::global()->bounded(5)));
    // Mueve la figura a una posición inicial específica.
    figuraActual->mover(10, 20);
}

void Dialog::generarFiguraSiguiente() {
    // Crea una nueva figura siguiente de un tipo aleatorio.
    figuraSiguiente = new Figura(static_cast<TipoFigura>(QRandomGenerator::global()->bounded(5)));
    // Ajusta la posición inicial de la figura siguiente basada en su tipo.
    if (figuraSiguiente->obtenerTipo() != TipoFigura::I)
        figuraSiguiente->mover(22, 12);
    else
        figuraSiguiente->mover(23, 11);

    // Si la figura actual y la figura siguiente son del mismo tipo, generar otra figura siguiente.
    if (figuraActual->obtenerTipo() == figuraSiguiente->obtenerTipo())
        generarFiguraSiguiente();
}

void Dialog::cargarFiguraSiguiente(TipoFigura tipo) {
    // Crea una nueva figura del tipo especificado.
    figuraActual = new Figura(tipo);
    // Mueve la figura a una posición inicial específica.
    figuraActual->mover(10, 20);
    // Genera la siguiente figura.
    generarFiguraSiguiente();
}

bool Dialog::detectarColision() {
    // Verifica si la figura actual colisiona con los bordes del área de juego.
    if (figuraActual->detectarColision(20))
        return true;

    // Verifica si la figura actual colisiona con alguna de las figuras ya colocadas.
    for (int i = 0; i < cantidadFigurasColocadas; i++)
        if (figuraActual->detectarColision(figurasColocadas[i]))
            return true;

    // Si no hay colisión, devuelve falso.
    return false;
}

void Dialog::bajarFigura() {
    // Mueve la figura actual hacia abajo.
    figuraActual->mover(0, -1);

    // Verifica si la figura actual ha llegado al fondo o ha colisionado con otra figura.
    if (detectarColision()) {
        // Si la figura colisiona, vuelve a la posición anterior.
        figuraActual->mover(0, 1);

        // Verifica si la figura actual ha llegado al tope del área de juego.
        if (figuraActual->verificarArea(20)) {
            // Reproduce el sonido de fin de juego.
            reproductorFinJuego->play();
            // Detiene el temporizador.
            timer->stop();
            // Muestra un mensaje de fin de juego con la puntuación.
            QMessageBox msg;
            msg.setWindowIcon(QIcon(":img/icono.png"));
            msg.setWindowTitle(":(");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Fin del juego, su puntuación fue de " + QString::number(puntuacion));
            msg.exec();
            // Reinicia el juego.
            reiniciarJuego();
            return;
        }

        // Agrega la figura actual al arreglo de figuras colocadas.
        figurasColocadas[cantidadFigurasColocadas++] = figuraActual;
        // Carga la siguiente figura.
        cargarFiguraSiguiente(figuraSiguiente->obtenerTipo());

        // Reproduce el sonido de la figura colocada.
        reproductorFiguraColocada->play();

        // Actualiza la puntuación.
        puntuacion += 4;

        // Ajusta la velocidad del juego según la puntuación.
        if (puntuacion > 30) {
            timer->stop();
            velocidad = 500;
            timer->start(velocidad);
        };

        if (puntuacion > 60) {
            timer->stop();
            velocidad = 250;
            timer->start(velocidad);
        };

        if (puntuacion > 100) {
            timer->stop();
            velocidad = 100;
            timer->start(velocidad);
        };

        // Actualiza el marcador en la interfaz de usuario.
        ui->puntuacion->setText(QString::number(puntuacion));
    }

    // Vuelve a pintar la ventana del juego.
    repaint();
}

void Dialog::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event) // Evita el warning de compilación sobre el parámetro no utilizado.

    // Inicializa el objeto QPainter para dibujar en el widget.
    canvas = new QPainter(this);

    int L = 0, M = alto; // Inicializa las coordenadas para el mapeo.

    // Establece el mapeo entre las coordenadas del mundo real y las coordenadas de la pantalla.
    mapeo.establecerPuertoVision(0, 0, ancho, alto); // Coordenadas de los píxeles en la pantalla.
    mapeo.establecerVentana(0, 0, 20, 20); // Coordenadas del mundo real.

    // Dibuja la figura actual y la siguiente en el canvas.
    figuraActual->dibujar(canvas, &mapeo, L, M);
    figuraSiguiente->dibujar(canvas, &mapeo, L, M);

    // Dibuja todas las figuras colocadas en el canvas.
    for (int i = 0; i < cantidadFigurasColocadas; i++)
        figurasColocadas[i]->dibujar(canvas, &mapeo, L, M);

    canvas->end(); // Finaliza el dibujo.
}

void Dialog::keyPressEvent(QKeyEvent *event) {
    // Maneja los eventos de teclado.
    if (event->key() == Qt::Key_W) {
        // Rota la figura actual 90 grados en sentido horario.
        figuraActual->rotar(90);
        // Si la rotación causa una colisión, deshace la rotación.
        if (detectarColision()) figuraActual->rotar(-90);
    }
    if (event->key() == Qt::Key_A) {
        // Mueve la figura actual hacia la izquierda.
        figuraActual->mover(-1, 0);
        // Si el movimiento causa una colisión, deshace el movimiento.
        if (detectarColision()) figuraActual->mover(1, 0);
    }
    if (event->key() == Qt::Key_S) {
        // Mueve la figura actual hacia abajo.
        figuraActual->mover(0, -1);
        // Si el movimiento causa una colisión, deshace el movimiento.
        if (detectarColision()) figuraActual->mover(0, 1);
    }
    if (event->key() == Qt::Key_D) {
        // Mueve la figura actual hacia la derecha.
        figuraActual->mover(1, 0);
        // Si el movimiento causa una colisión, deshace el movimiento.
        if (detectarColision()) figuraActual->mover(-1, 0);
    }
    if (event->key() == Qt::Key_Escape)
        pausar(); // Pausa el juego si se presiona la tecla Escape.

    repaint(); // Vuelve a pintar el widget para mostrar los cambios.
}

void Dialog::pausar() {
    static bool pausado = false; // Variable estática para controlar el estado de pausa.

    // Si el juego está pausado, reanuda el temporizador con la velocidad actual.
    if (pausado)
        timer->start(velocidad);
    else
        timer->stop(); // Si no está pausado, detiene el temporizador.

    pausado = !pausado; // Cambia el estado de pausa.
}

void Dialog::reiniciarJuego() {
    velocidad = 750; // Restaura la velocidad del juego.

    // Carga la figura actual y genera la siguiente.
    cargarFigura();
    generarFiguraSiguiente();

    cantidadFigurasColocadas = 0; // Reinicia el contador de figuras colocadas.
    puntuacion = 0; // Reinicia la puntuación.
    ui->puntuacion->setText(QString::number(puntuacion)); // Actualiza la etiqueta de la puntuación.

    timer->start(velocidad); // Inicia el temporizador con la nueva velocidad.
}

void Dialog::on_btnReiniciar_clicked() {
    reiniciarJuego(); // Llama a la función para reiniciar el juego cuando se hace clic en el botón de reinicio.
}


