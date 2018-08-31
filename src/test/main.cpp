#include <io/obj_loader.h>
#include <model/uniform_table.h>

#include <iostream>

int main()
{
    //ObjLoader loader;
    //loader.load("/mnt/media/workspace/repos/self/vp/src/asset/cube.obj");
    //loader.printInfo();

    //UniformTable tbl;
    //tbl.loadDescription("/home/chenmiwei/Work/source/vp/src/shaders/global_uniforms.json");
    //tbl.loadDescription("/home/chenmiwei/Work/source/vp/src/shaders/basic.json");
    
    Uniform u(UniformType::INT);
    int a = 2;
    u.setValue(&a);

    std::cout << *(int*)(u.buf.get()) << std::endl;

    return 0;
}
