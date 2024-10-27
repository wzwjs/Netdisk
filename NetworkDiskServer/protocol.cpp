#include "protocol.h"

PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;  //总的pdu的长度
    PDU * pdu=(PDU *)malloc(uiPDULen);   //分配地址
    if(pdu==NULL){
        exit(EXIT_FAILURE);
    }
    memset(pdu,0,uiPDULen);
    pdu->uiPDUlen=uiPDULen;
    pdu->uiMsgLen=uiMsgLen;
    return pdu;
}
