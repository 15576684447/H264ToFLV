//����  �����  20160615  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "flvfile.h"

#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

FILE *g_h264_file  = NULL;    //H264 ��Ƶ���ļ�
FILE *g_aac_file   = NULL;    //aac  ��Ƶ����Ϣ
FILE *g_flv_log    = NULL;    //��ӡ��ϸ��Ϣ
FILE *g_flv_out    = NULL;    //�����װ���flv��

extern char *flv_h264_name  = NULL;   //��Ƶ���ļ�           -v ���� �磺-v brazil-bq.264
extern char *flv_aac_name   = NULL;   //��Ƶ���ļ�           -a ���� �磺-a brazil-bq.aac 
extern char *flv_log_name   = NULL;   //��ӡ��ϸ��Ϣ         -t ���� �磺-t brazil-bq.log
extern char *flv_out_name   = NULL;   //���flv�ļ�          -o ���� �磺-v brazil-bq.flv 

int  g_errors        = 0;    //ͳ�ƴ������

//��flv����ļ�
void openFlvFiles()    
{
	if(flv_h264_name)
	{
		g_h264_file = fopen(flv_h264_name,"rb");
		if(!g_h264_file)
		{
			printf("���ļ�ʧ��: %s\n",flv_h264_name);
			system("pause");
			exit(0);
		}
	}
	if(flv_aac_name)
	{
		g_aac_file = fopen(flv_aac_name,"rb");
		if(!g_aac_file)
		{
			printf("���ļ�ʧ��: %s\n",flv_aac_name);
			system("pause");
			exit(0);
		}
	}
	if(flv_log_name)
	{
		g_flv_log = fopen(flv_log_name,"w");
		if(!g_flv_log)
		{
			printf("���ļ�ʧ��: %s\n",flv_log_name);
			system("pause");
			exit(0);
		}
	}

	if(flv_out_name)
	{
		g_flv_out = fopen(flv_out_name,"wb");
		if(!g_flv_out)
		{
			printf("���ļ�ʧ��: %s\n",flv_out_name);
			system("pause");
			exit(0);
		}
	}
}

//�ر�flv����ļ�
void closeFlvFiles()   
{
	if(g_h264_file)
		fclose(g_h264_file);
	if(g_aac_file)
		fclose(g_aac_file);
	if(g_flv_log)
		fclose(g_flv_log);
	if(g_flv_out)
		fclose(g_flv_out);
}