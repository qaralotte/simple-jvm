#include <filesystem>
#include <queue>
#include <regex>

#include "include/finder.h"
#include "include/log.h"
#include "include/config.h"
#include "include/cmd.h"

#include "lib/zip_file.hpp"

using namespace classfinder;

// private:

// 读取IO文件
vector<uchar> ClassFinder::readByIO(const string &path) {
    vector<uchar> bytes;
    ifstream fs(path, ios::binary);
    if (fs.fail()) {
        ERROR("打开class文件[%s]出错", path.c_str());
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
        ERROR("class文件[%s]读取失败", path.c_str());
        fs.close();
        exit(0);
    }

    fs.close();
    return bytes;
}

// 处理classpath (xxxx;xxxx -> {xxxx, xxxx})
vector<string> ClassFinder::handleClasspath() {
    string classpath = Cmd::get("classpath");
    regex re(";");
    return vector<string> {
            sregex_token_iterator(classpath.begin(), classpath.end(), re, -1),
            sregex_token_iterator()
    };
}

string ClassFinder::mergePath(string ori_path, string file_path) {
    uint ori_length = ori_path.size();
    if (ori_path.back() != '/') ori_length += 1;
    return file_path.substr(ori_length, file_path.size() - 1);
}

// 遍历文件夹
vector<uchar> ClassFinder::findInDir(const string &oripath, bool traverse_next) {
    if (Cmd::get("classpath").empty()) return vector<uchar>();

    /* 广搜 */
    queue<filesystem::directory_iterator> filelistque;
    filelistque.push(filesystem::directory_iterator(oripath));
    while (!filelistque.empty()) {
        auto filelist = filelistque.front();
        filelistque.pop();
        for (auto file : filelist) {
            if (file.status().type() == filesystem::file_type::directory) {
                if (traverse_next) filelistque.push(filesystem::directory_iterator(file.path()));
            } else {
                string pkgpath = mergePath(oripath, file.path().string());
                if (pkgpath == classname) {
                    return readByIO(file.path());
                }
            }
        }
    }
    return vector<uchar>();
}

// 遍历jar/zip包
vector<uchar> ClassFinder::findInJar(const string &jarpath) {
    vector<uchar> data;
    miniz_cpp::zip_file zfile(jarpath);
    if (zfile.has_file(classname)) {
        auto oridata = zfile.read(classname);
        for (char ch : oridata) {
            data.push_back(ch);
        }
        return data;
    }
    return vector<uchar>();
}

// public:

/* 双亲委派模型 */
/* Application -> Extension -> User */

vector<uchar> ClassFinder::findClass() {

    // 提取文件后缀名
    auto getSuffix = [&](string path) -> string {
        string suffix;
        for (auto riter = path.rbegin(); riter != path.rend(); ++riter) {
            if (*riter == '.') break;
            suffix.insert(suffix.begin(), *riter);
        }
        return suffix;
    };

    // 遍历文件夹
    auto traverseDir = [&](string dirpath) -> vector<uchar> {
        filesystem::path path(dirpath);
        filesystem::directory_iterator filelists(path);
        for (auto file : filelists) {
            if (file.status().type() != filesystem::file_type::directory) {
                if (getSuffix(file.path().string()) == "class") {
                    if (mergePath(dirpath, file.path().string()) == classname) {
                        return readByIO(file.path().string());
                    }
                } else if (getSuffix(file.path().string()) == "jar" || getSuffix(file.path().string()) == "zip") {
                    return findInJar(file.path().string());
                }
            }
        }
        return vector<uchar>();
    };

    vector<uchar> result;

    // Application (<classpath>)
    if (!Cmd::get("classpath").empty()) {
        auto classpaths = handleClasspath();
        for (string classpath : classpaths) {
            if (classpath.back() == '*') {
                // xxx/* -> xxx
                result = findInDir(classpath.substr(0, classpath.size() - 2), true);
            } else {
                filesystem::path path(classpath);
                if (status(path).type() == filesystem::file_type::directory) {
                    result = findInDir(path, false);
                } else if (getSuffix(path) == "jar" || getSuffix(path) == "zip") {
                    result = findInJar(path);
                }
            }
        }
    }
    // Extension (<JAVA_HOME>/lib/ext)
    if (result.empty()) {
        result = traverseDir(JAVA_HOME + "/lib/ext");
    }
    // Bootstrap (<JAVA_HOME>/lib)
    if (result.empty()) {
        result = traverseDir(JAVA_HOME + "/lib");
    }
    return result;
}