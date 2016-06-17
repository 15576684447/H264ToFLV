#ifndef XIAOC_FLV_FILE_H
#define  XIAOC_FLV_FILE_H
//����  �����  20160615  xiaoc@pku.edu.cn

#include <stdio.h>
#include "h264toflv.h"


extern FILE *g_h264_file;  //H264 ��Ƶ���ļ�
extern FILE *g_aac_file;   //aac  ��Ƶ����Ϣ
extern FILE *g_flv_log;    //��ӡ��ϸ��Ϣ
extern FILE *g_flv_out;    //�����װ���flv��

extern char *flv_h264_name;  //��Ƶ���ļ�           -v ���� �磺-v brazil-bq.264
extern char *flv_aac_name;   //��Ƶ���ļ�           -a ���� �磺-a brazil-bq.aac 
extern char *flv_log_name;   //��ӡ��ϸ��Ϣ         -t ���� �磺-t brazil-bq.log
extern char *flv_out_name;   //���flv�ļ�          -o ���� �磺-v brazil-bq.flv 

extern int  g_errors;        //ͳ�ƴ������

void openFlvFiles();    //��flv����ļ�
void closeFlvFiles();   //�ر�flv����ļ�
#endif