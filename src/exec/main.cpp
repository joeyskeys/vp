#include <QApplication>
#include <QSurfaceFormat>

#include <ui/viewport.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    QtViewport vp;
    vp.show();

    return app.exec();
}
