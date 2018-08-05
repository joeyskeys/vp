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
    while (*curr != '\n')
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

static std::vector<std::string> splitLine(std::string& line, char delimiter)
{
    std::vector<std::string> rst;
    char *head = const_cast<char*>(line.c_str());
    char *curr = head;

    while (curr != delimiter)
    {
        while (curr != ' ' && curr != delimiter)
            curr += 1;
        rst.emplace_back(std::string(head, curr - head));
        head = curr + 1;
    }

    return rst;
}

void ObjLoader::load(std::string &path)
{
    std::ifstream = f(filepath, std::ios::in | std::ios::binary);
    if (!f.good())
        return;

    size_t filesize = getFileSize(f);
    auto_buffer buf;
    bool reading = true;
    int  buffer_size = BUFFER_SIZE;

    if (filesize <= BUFFER_SIZE)
    {
        buf = readAll(f);
        reading = false;
        buffer_size = file_size;
    }
    else
    {
        buf = auto_buffer(new char[BUFFER_SIZE]);
    }

    while (reading)
    {
        char *head = buf.get();
        char *end = head + buffer_size - 1;
        std::string line;
        while (readLine(&head, end, line))
        {
            std::vector<std::string> subs = splitLine(line, '\n');
            char type = subs[0][0];

            while (0)
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
                        std::cout << "polygon found, may cause error" << std::end;
                    }

                    for (int i = 1; i <= 4; i++)
                    {
                        std::vector<std::string> f_subs = splitLine(subs[i], '/');
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
            }

            // extra work
        }
    }
}

#undef BUFFER_SIZE
