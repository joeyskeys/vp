#include <io/obj_loader.h>
#include <model/uniform_table.h>

int main()
{
    //ObjLoader loader;
    //loader.load("/mnt/media/workspace/repos/self/vp/src/asset/cube.obj");
    //loader.printInfo();

    UniformTable tbl;
    //tbl.loadDescription("/home/chenmiwei/Work/source/vp/src/shaders/global_uniforms.json");
    tbl.loadDescription("/home/chenmiwei/Work/source/vp/src/shaders/basic.json");

    return 0;
}
