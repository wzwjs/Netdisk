#include "protocol.h"

PDU *mkPDU(uint uiMsgLen)    //传入的是实际消息的长度
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;  //总的pdu的长度
    PDU * pdu=(PDU *)malloc(uiPDULen);   //分配地址
                                        //自定义这个数据类型的目的接收通过这个结构体去访问的到对应的数据
    if(pdu==NULL){
        exit(EXIT_FAILURE);           //错误退出程序
    }
    memset(pdu,0,uiPDULen);      //清空该结构体的内容
    pdu->uiPDUlen=uiPDULen;     //将清空后面的总的协议数据单元的大小赋值，将实际消息长度消息赋值
    pdu->uiMsgLen=uiMsgLen;
    return pdu;
}
