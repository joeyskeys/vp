#include <io/obj_loader.h>

int main()
{
    ObjLoader loader;
    loader.load("/mnt/media/workspace/repos/self/vp/src/asset/cube.obj");
    loader.printInfo();

    return 0;
}
