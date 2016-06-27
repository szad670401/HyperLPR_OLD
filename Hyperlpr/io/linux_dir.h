//
// Created by 庾金科 on 2/9/16.
//

#ifndef QUICK_LPR_DIR_HPP
#define QUICK_LPR_DIR_HPP

#endif //QUICK_LPR_DIR_HPP


#define MAX_LEN 1024 * 512

#include <iostream>
#include<fstream>
#include <sys/dir.h>
#include <sys/stat.h>
#include<vector>

using namespace std;


class Directory_mac
{

public:
    vector<std::string> GetListFiles( const std::string& path, const std::string & exten = "*", bool addPath = true );
    vector<std::string> GetListFilesR ( const std::string& path, const std::string & exten = "*", bool addPath = true );
    vector<std::string> GetListFolders( const std::string& path, const std::string & exten = "*", bool addPath = true );
private:
    vector<string> fileName;
    vector<string> folderName;




    //判断是否为文件夹]
    bool isDir(const char* path);

    //遍历文件夹的驱动函数
    void findFiles(const char *path);

    //遍历文件夹de递归函数
    void findFiles(const char *path, int recursive);

    //判断是否为目录





};







bool Directory_mac::isDir(const char* path)
{
    struct stat st;
    lstat(path, &st);
    return 0 != S_ISDIR(st.st_mode);
}


//遍历文件夹的驱动函数
void Directory_mac::findFiles(const char *path)
{
    unsigned long len;
    char temp[MAX_LEN];
    //去掉末尾的'/'
    len = strlen(path);
    strcpy(temp, path);
    if(temp[len - 1] == '/') temp[len -1] = '\0';

    if(isDir(temp))
    {
        //处理目录
        int recursive = 1;


        findFiles(temp, recursive);
    }
    else   //输出文件
    {
        printf("======%s\n", path);
    }
}



//遍历文件夹de递归函数
void Directory_mac::findFiles(const char *path, int recursive)
{
    DIR *pdir;
    struct dirent *pdirent;
    char temp[MAX_LEN];
    pdir = opendir(path);
    if(pdir)
    {
        while((pdirent = readdir(pdir)))
        {
            //跳过"."和".."
            if(strcmp(pdirent->d_name, ".") == 0
               || strcmp(pdirent->d_name, "..") == 0)
                continue;
            sprintf(temp, "%s/%s", path, pdirent->d_name);

            //当temp为目录并且recursive为1的时候递归处理子目录
            if(isDir(temp) && recursive)
            {
                findFiles(temp, recursive);
                // cout<<temp<<endl;

                folderName.push_back(string(temp));
                //this is a folder name


            }
            else
            {
                // printf("======%s\n", temp);
                fileName.push_back(string(temp));

            }
        }
    }
    else
    {
        printf("opendir error:%s\n", path);
    }
    closedir(pdir);
}

vector<std::string> Directory_mac::GetListFolders( const std::string& path, const std::string & exten , bool addPath  ){



    cout<<"debug message_  :"<<path<<endl;


    findFiles(path.c_str());

    return folderName;


}

vector<std::string> Directory_mac::GetListFiles( const std::string& path, const std::string & exten, bool addPath)
{



    findFiles(path.c_str());


    return fileName;



}



