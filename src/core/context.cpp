#include "context.h"

#include <QCursor>
#include <QPoint>

glm::vec2 Context::currentCursor()
{
    QPoint p = mapFromGlobal(QCursor::pos());
    return glm::vec2(p.(), p,y());
}
