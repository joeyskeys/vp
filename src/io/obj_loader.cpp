#include "obj_loader.h"
#include <model/utils.h>

#include <fstream>
#include <iostream>
#include <string>

// define the buffer size, 4M
// if file size is smaller than this, read all in
// else use caching for memory efficiency
#define BUFFER_SIZE 4 * 1024 * 1024

ObjLoader::ObjLoader()
{

}

ObjLoader::~ObjLoader()
{

}

ObjLoader::ObjLoader(const ObjLoader& rhs):
	verts(rhs.verts),
	norms(rhs.norms),
	indices(rhs.indices),
	objects(rhs.objects)
{

}

ObjLoader& ObjLoader::operator=(const ObjLoader& rhs)
{
	verts = rhs.verts;
	norms = rhs.norms;
	indices = rhs.indices;
	objects = rhs.objects;

	return *this;
}

static bool readLine(char **buf_ptr, const char *end, std::string& line)
{
    char *curr = *buf_ptr;
    while (*curr == '\n' || *curr == '\0')
    {
        curr++;
        if (curr > end)
            return false;
    }
    while (*curr != '\n' && *curr != '\0')
    {
        if (curr < end)
        {
            curr += 1;
        }
        else
        {
            return false;
        }
    }

    line = std::string(*buf_ptr, curr - *buf_ptr);
    *buf_ptr = curr + 1;
    return true;
}

static std::vector<std::string> splitLine(std::string& line, std::string&& delimiter)
{
    std::vector<std::string> rst;
    char *head = const_cast<char*>(line.c_str());
    char *curr = head;

    while (*curr != '\0')
    {
        for (int i = 0; i < delimiter.size(); i++)
        {
            //curr += 1;
            if (*curr == delimiter[i])
            {
                rst.push_back(std::string(line, head - line.c_str(), curr - head));
                head = curr + 1;
                break;
            }
        }
        ++curr;
    }
    rst.push_back(std::string(line, head - line.c_str(), curr - head));

    return rst;
}

void ObjLoader::load(std::string &&path)
{
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f.good())
        return;

    size_t filesize = getFileSize(f);
    auto_buffer buf;
    bool reading = true;
    int  buffer_size = BUFFER_SIZE;
        
    if (filesize < BUFFER_SIZE)
    {
        std::cout << "read all" << std::endl;
        buf = readAll(f);
        reading = false;
        buffer_size = filesize;
        buf[filesize] = '\0';
    }
    else
    {
        std::cout << "read part" << std::endl;
        buf = auto_buffer(new char[BUFFER_SIZE]);
        f.read(buf.get(), BUFFER_SIZE);
    }
    
    char *buf_head = buf.get();
    char *buf_end = buf_head + buffer_size;

    do
    {
        char *head = buf_head;
        char *end = buf_end;
        std::string line;

        while (readLine(&head, end, line))
        {
            std::cout << "read line " << line << std::endl;
            std::vector<std::string> subs = splitLine(line, " ");
            char type = subs[0][0];

            do
            {
                if (type == '#')
                {
                    // comment
                    break;
                }

                if (type == 'v')
                {
                    if (subs[0].size() == 1)
                    {
                        // vertex
                        verts.push_back(std::stof(subs[1]));
                        verts.push_back(std::stof(subs[2]));
                        verts.push_back(std::stof(subs[3]));
                    }
                    else
                    {
                        char sub_type = subs[0][1];

                        if (sub_type == 'n')
                        {
                            // normal
                            norms.push_back(std::stof(subs[1]));
                            norms.push_back(std::stof(subs[2]));
                            norms.push_back(std::stof(subs[3]));
                            break;
                        }
                        
                        if (sub_type == 't')
                        {
                            // uv
                            break;
                        }

                    }
                    break;
                }

                if (type == 'f')
                {
                    // face indices
                    if (subs.size() > 4)
                    {
                        // don't support quad or polygon for now
                        std::cout << "polygon found, may cause error" << std::endl;
                    }

                    for (int i = 1; i < 4; i++)
                    {
                        std::vector<std::string> f_subs = splitLine(subs[i], "/");
                        int cnt = f_subs.size();
                        indices.push_back(std::stoi(f_subs[0]));

                        if (cnt == 2)
                        {
                            // index/uv
                            // don't handle uv for now
                        }
                        else if (cnt == 3)
                        {
                            // index/uv/normal
                            norm_indices.push_back(std::stoi(f_subs[2]));
                        }
                    }

                    break;
                }

                if (type == 'o')
                {
                    // ojbect
                    // each element indicate the start index of an object
                    // except the final one
                    objects.push_back(indices.size());

                    break;
                }
                
                if (type == 'g')
                {
                    // group
                    break;
                }

                if (type == 's')
                {
                    // smooth setting
                    break;
                }
                
                if (type == 'u') // usemtl
                {
                    // specify material
                    break;
                }

                if (type == 'm') //mtllib
                {
                    // material
                    break;
                }
            } while (0);
        }

        if (head < end)
        {
            int remain_data = end - head + 1;
            memcpy(buf.get(), head, remain_data);
            head = buf_head + remain_data;
            buffer_size = BUFFER_SIZE - remain_data;
        }
        else
        {
            if (f.eof())
                break;
        }

        if (f.gcount() < buffer_size)
        {
            buffer_size = f.gcount();
            end = head + buffer_size;
            *end = '\0';
        }
        f.read(head, buffer_size);
    } while (reading);
}

void ObjLoader::reset()
{
    verts.clear();
    norms.clear();
    indices.clear();
    norm_indices.clear();
    objects.clear();
}

void ObjLoader::fillMesh(Mesh *mesh)
{
    mesh->insertVerts(verts.data(), verts.size() / 3);
    mesh->insertNorms(norms.data(), norms.size() / 3);
    mesh->insertTriangles(indices.data(), indices.size() / 3);
}

void ObjLoader::loadIntoMesh(Mesh *mesh)
{

}

void ObjLoader::printInfo()
{
    std::cout << "verts :" << verts.size() << std::endl;
    for (int i = 0; i < verts.size() / 3; i++)
        std::cout << verts[i * 3] << " " << verts[i * 3 + 1] << " " << verts[i * 3 + 2] << std::endl;

    std::cout << "norms :" << norms.size() << std::endl;
    for (int i = 0; i < norms.size() / 3; i++)
        std::cout << norms[i * 3] << " " << norms[i * 3 + 1] << " " << norms[i * 3 + 2] << std::endl;
    
    std::cout << "indices :" << indices.size() << std::endl;
    for (int i = 0; i < indices.size() / 3; i++)
        std::cout << indices[i * 3] << " " << indices[i * 3 + 1] << " " << indices[i * 3 + 2] << std::endl;
}

#undef BUFFER_SIZE
