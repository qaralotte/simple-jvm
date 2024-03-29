#include <filesystem>
#include <queue>
#include <regex>

#include "include/finder.h"
#include "include/config.h"
#include "include/cmd.h"

#include "include/libzip/zip.h"

using namespace classfinder;

map<string, zip *> ClassFinder::unz_jars;

// private:

// 处理classpath (xxxx;xxxx -> {xxxx, xxxx})
vector<string> ClassFinder::handleClasspath() {
    string classpath = Cmd::classpath;
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
ClassPath ClassFinder::findInDir(const string &oripath, bool traverse_next) {
    if (Cmd::classpath.empty()) return ClassPath(classpath::NOT_FOUND);

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
                if (pkgpath == filename) {
                    return ClassPath(classpath::DIR, file.path().string(), classname, filename);
                }
            }
        }
    }
    return ClassPath(classpath::NOT_FOUND);
}

// 遍历jar/zip包
ClassPath ClassFinder::findInJar(const string &jarpath) {
    if (ClassFinder::unz_jars.find(jarpath) == ClassFinder::unz_jars.end()) {
        ClassFinder::unz_jars[jarpath] = zip_open(jarpath.c_str(), 0, NULL);
    }

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(ClassFinder::unz_jars[jarpath], filename.c_str(), 0, &st);

    zip_file *f = zip_fopen(ClassFinder::unz_jars[jarpath], filename.c_str(), 0);
    if (f == NULL) return ClassPath(classpath::NOT_FOUND);
    zip_fclose(f);
    return ClassPath(classpath::JAR, jarpath, classname, filename);
}

// public:

/* 双亲委派模型 */
/* Application -> Extension -> User */

ClassPath ClassFinder::findClass() {

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
    auto traverseDir = [&](string dirpath) -> ClassPath {
        filesystem::path path(dirpath);
        filesystem::directory_iterator filelists(path);
        for (auto file : filelists) {
            if (file.status().type() != filesystem::file_type::directory) {
                if (getSuffix(file.path().string()) == "class") {
                    if (mergePath(dirpath, file.path().string()) == filename) {
                        return ClassPath(classpath::DIR, file.path().string(), classname, filename);
                    }
                } else if (getSuffix(file.path().string()) == "jar" || getSuffix(file.path().string()) == "zip") {
                    return findInJar(file.path().string());
                }
            }
        }
        return ClassPath(classpath::NOT_FOUND);
    };

    ClassPath result(classpath::NOT_FOUND);

    // Bootstrap (<JAVA_HOME>/lib)
    if (result.type == classpath::NOT_FOUND) {
        result = traverseDir(JAVA_HOME + "/lib");
    }

    // Extension (<JAVA_HOME>/lib/ext)
    if (result.type == classpath::NOT_FOUND) {
        result = traverseDir(JAVA_HOME + "/lib/ext");
    }

    // Application (<classpath>)
    if (result.type == classpath::NOT_FOUND) {
        if (!Cmd::classpath.empty()) {
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
    }


    return result;
}