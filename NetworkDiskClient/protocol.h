#ifndef PROTOCOL_H
#define PROTOCOL_H
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
typedef unsigned int uint;

#define REGIST_OK           "regist ok"
#define REGIST_FAILED       "regist failed:name existed"
#define LOING_OK            "login ok"
#define LOING_FAILED        "login failed"
#define SEARCH_USR_NO       "no such people"
#define SEARCH_USR_ONLINE   "online"
#define SEARCH_USR_OFFLINE  "offline"

#define UNKNOW_ERROR        "unknow error"
#define EXITSTED_FRIEND     "friend exist"
#define ADD_FRIEND_OFFLINE  "usr offline"
#define ADD_FRIEND_NOEXIST  "usr not exist"
#define DELETE_FRIEND_OK    "delete_friend_ok"
#define DELETE_FRIEND_FAILD "delete_friend_failed"
#define DIR_NO_EXIST        "dir not exist"
#define FILE_NAME_EXIST     "file name exist"
#define CREAT_DIR_OK        "create dir ok"
#define DEL_DIR_OK          "delete dir ok"
#define DEL_DIR_FAILURE     "delete dir failed :is regula file"
#define RENAMR_FILE_OK      "rename file ok"
#define RENAMR_FILE_FAILE   "rename file FAILE "
#define ENTER_DIR_FAILED    "enter  dir failed: is reguler file"
#define ENTER_DIR_OK        "enter dir ok"
enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_REGIST_REQUEST,       //注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,       //注册回复
    ENUM_MSG_TYPE_LOGIN_REQUEST,        //登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,        //登录回复
    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,   //获取在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,   //获取在线用户回复
    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,   //搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,   //搜索用户回复
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,   //添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,   //添加好友回复
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,     //同意添加好友回复
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,    //不同意添加好友回复
    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST, //刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND, //刷新好友回复
    ENUM_MSG_TYPE_DELETE_REQUEST,       //删除好友请求
    ENUM_MSG_TYPE_DELETE_RESPOND,       //删除好友回复
    ENUM_MSG_TYPE_PRIVATE_REQUEST,      //私聊好友请求
    ENUM_MSG_TYPE_PRIVATE_RESPOND,      //私聊好友回复
    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,   //群聊好友请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,   //群聊好友回复
    ENUM_MSG_TYPE_CREATE_PATH_REQUEST,  //创建路径请求
    ENUM_MSG_TYPE_CREATE_PATH_RESPOND,  //创建路径回复  DIR 路径
    ENUM_MSG_TYPE_CREATE_FILES_REQUEST, //创建文件夹请求
    ENUM_MSG_TYPE_CREATE_FILES_RESPOND, //创建文件夹回复
    ENUM_MSG_TYPE_FLUSH_FILES_REQUEST,  //刷新文件夹请求
    ENUM_MSG_TYPE_FLUSH_FILES_RESPOND,  //刷新文件夹回复
    ENUM_MSG_TYPE_DEL_DIR_REQUEST,      //删除文件夹(目录）请求
    ENUM_MSG_TYPE_DEL_DIR_RESPOND,      //删除文件夹（目录）回复
    ENUM_MSG_TYPE_RENAME_FILE_REQUEST,  //重命名文件请求
    ENUM_MSG_TYPE_RENAME_FILE_RESPOND,  //重命名文件回复
    ENUM_MSG_TYPE_ENTER_DIR_REQUEST,    //进入文件夹请求
    ENUM_MSG_TYPE_ENTER_DIR_RESPOND,    //进入文件夹回复
    //ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_MAX=0X00ffffff
};

struct FileInfo
{
    char caFileName[32];    //文件名字
    int iFileType;      //文件类型
};


struct PDU
{
    uint uiPDUlen;      //总的协议单元数据大小
    uint uiMsgType;     //消息类型
    char caData[64];    //文件其他信息(文件名等) char * 用户名，等临时要传输的短数据
    uint uiMsgLen;      //实际消息长度
    int caMsg[];        //实际消息    主要就是通过这个去访问消息数据
};

PDU *mkPDU(uint uiMsgLen);   //声明一个以PDU自定义类型的返回值的函数，uiMsgLen是可变的

#endif // PROTOCOL_H
