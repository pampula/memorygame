#include <QApplication>
#include "memorygame.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MemoryGame window;
    window.show();

    return app.exec();
}
