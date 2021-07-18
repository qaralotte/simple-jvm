#include <fstream>

#include "include/classpath.h"
#include "include/log.h"

#include "include/minizip-ng/unzip.h"

using namespace classpath;

// private:

/* 读取文件二进制数据 */
vector<uchar> ClassPath::readInDir() {
    vector<uchar> bytes;
    ifstream fs(path, ios::binary);
    if (fs.fail()) {
        ERROR("打开class文件: %s出错", path.c_str());
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

    int status = UNZ_OK;
    unz_file_info file_info;
    char filename_inzip[256];

    if (unzGetCurrentFileInfo(unz_file, &file_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0) != MZ_OK) {
        ERROR("读取%s的%s文件结构时出错", path.c_str(), file_name.c_str());
        exit(0);
    }
    const int BUFFER = 1242880;

    auto buf = (void *) malloc(BUFFER);
    if (unzOpenCurrentFilePassword(unz_file, nullptr) != UNZ_OK) {
        ERROR("打开%s的%s文件时出错", path.c_str(), file_name.c_str());
        exit(0);
    }

    do {
        status = unzReadCurrentFile(unz_file, buf, BUFFER);
        if (status < 0) {
            ERROR("读取%s的%s文件时出错", path.c_str(), file_name.c_str());
            exit(0);
        }
        // copy the buffer to a string
        if (status > 0) for (int i = 0; i < (int) status; i++) {
            data.push_back(*(((char *) buf) + i));
        }
    } while (status > 0);

    if (unzCloseCurrentFile(unz_file) != UNZ_OK) {
        ERROR("关闭%s的%s文件时出错", path.c_str(), file_name.c_str());
        exit(0);
    }
    free(buf);
    unzClose(unz_file);

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

