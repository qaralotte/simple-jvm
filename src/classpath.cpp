#include "include/classpath.h"
#include "include/log.h"

#include "lib/zip_file.hpp"

using namespace classpath;

// private:

/* 读取文件二进制数据 */
vector<uchar> ClassPath::readInDir() {
    vector<uchar> bytes;
    ifstream fs(path, ios::binary);
    if (fs.fail()) {
        ERROR("打开class文件[%s.class]出错", path.c_str());
        fs.close();
        exit(0);
    }

    /* 获取文件大小 */
    fs.seekg(0, ios::end);
    uint fsize = fs.tellg();
    fs.seekg(0, ios::beg);

    /* 读取内容到bytes缓冲区 */
    bytes.resize(fsize);
    fs.read(reinterpret_cast<char *>(bytes.data()), fsize);
    if (bytes.empty()) {
        ERROR("%s 读取失败", path.c_str());
        fs.close();
        exit(0);
    }

    fs.close();
    return bytes;
}

vector<uchar> ClassPath::readInJar() {
    vector<uchar> data;
    miniz_cpp::zip_file zfile(path);
    auto oridata = zfile.read(class_name);
    for (char ch : oridata) {
        data.push_back(ch);
    }
    return data;
}

// public:
vector<uchar> ClassPath::read() {
    if (type == JAR) {
        return readInJar();
    } else if (type == DIR) {
        return readInDir();
    }
    return vector<uchar>();
}

