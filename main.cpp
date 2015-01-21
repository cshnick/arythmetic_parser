#include <QApplication>

#include "booleanwindow.h"
#include <functional>

struct add_x {
  add_x(int x) : x(x) {}
  int operator()(int y, int z) { return z + y; }

private:
  int x;
};

template<typename Type1, typename Type2, typename Operation>
void forString(const QString &str, Type1 t1, Type2 t2) {
    return Operation(t1, t2);
}

int main(int argc, char *argv[])
{


    QApplication app(argc, argv);
    BooleanWindow window;
    window.setGeometry(0, 0, 480, 240);
    window.show();
    return app.exec();
}
