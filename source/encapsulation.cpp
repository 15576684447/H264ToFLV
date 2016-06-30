//����  �����  20160617  xiaoc@pku.edu.cn
#include <string.h>
#include <stdlib.h>
#include "encapsulation.h"

EncaFlv::EncaFlv()
{
	m_param          = NULL;
}

void EncaFlv::create(flv_param *param)/* �����ڴ� */
{
	bool ok = true;
	m_param = param;
}
void EncaFlv::destory()/* �ͷ��ڴ� */
{

}

//����FLVͷ��Ϣ
void EncaFlv::setFlvHeader()
{
	m_flvHeader.signature[0] = 'F';
	m_flvHeader.signature[1] = 'L';
	m_flvHeader.signature[2] = 'V';

	m_flvHeader.version      = 0x01;
	m_flvHeader.present      = 0;

	if(m_param->b_audio)
	{
		m_flvHeader.present |= 0x04;
	}
	if(m_param->b_video)
	{
		m_flvHeader.present |= 0x01;
	}
	m_flvHeader.size[0] = 0x00;
	m_flvHeader.size[1] = 0x00;
	m_flvHeader.size[2] = 0x00;
	m_flvHeader.size[3] = 0x09;
}
//дFLVͷ��Ϣ
void EncaFlv::writeFlvHeader()
{
	setFlvHeader();
	fwrite(m_flvHeader.data,9,1,g_flv_out);
}

/* д��һ��Tag Size */
void EncaFlv::writeFirstTagSize()
{
	unsigned char data[4] = {0x00,0x00,0x00,0x00};
	fwrite(data,4,1,g_flv_out);
}
/* дǰһ��Tag Size */
void EncaFlv::writePreTagSize()
{
	unsigned char data[4] = {0x00,0x00,0x00,0x00};
	unsigned int  length  = m_tagHeader.getSize();
	data[3] = (unsigned char)(length&0xFF); length = length>>8;
	data[2] = (unsigned char)(length&0xFF); length = length>>8;
	data[1] = (unsigned char)(length&0xFF); length = length>>8;
	data[0] = (unsigned char)(length&0xFF); length = length>>8;
	fwrite(data,4,1,g_flv_out);
}


/*����Tagͷ��Ϣ*/
void EncaFlv::setTagHeader(unsigned char tagType, unsigned int timestamp)
{
	memset(m_tagHeader.data,0,11);
	m_tagHeader.tagType = tagType;
	m_tagHeader.timestamp[2] = (unsigned char)(timestamp&0xFF); timestamp = timestamp>>8;
	m_tagHeader.timestamp[1] = (unsigned char)(timestamp&0xFF); timestamp = timestamp>>8;
	m_tagHeader.timestamp[0] = (unsigned char)(timestamp&0xFF); timestamp = timestamp>>8;
	m_tagHeader.timestamp[3] = (unsigned char)(timestamp&0xFF); timestamp = timestamp>>8;	
}

/*����Tag���ݳ���*/
void EncaFlv::setTagHeaderDataSize(unsigned int datasize)
{
	m_tagHeader.datasize[2] = (unsigned char)(datasize&0xFF); datasize = datasize>>8;
	m_tagHeader.datasize[1] = (unsigned char)(datasize&0xFF); datasize = datasize>>8;
	m_tagHeader.datasize[0] = (unsigned char)(datasize&0xFF); datasize = datasize>>8;
}
//дTagͷ��Ϣ
void EncaFlv::writeTagHeader()
{
	fwrite(m_tagHeader.data,11,1,g_flv_out);
}
//дMetaData��Ϣ
void EncaFlv::writeMetaData()
{
	setTagHeader(TAGTYPE_META,0);
	unsigned char data[8] = {0x02,0x00,0x05,0x58,0x49,0x41,0x4F,0x43};
	setTagHeaderDataSize(8);
	writeTagHeader();
	fwrite(data,8,1,g_flv_out);
	/* дǰһ��Tag Size */
	writePreTagSize();
}

//���NAL��ʼ�� ��������ʼ������
int EncaFlv::checkStartecode(unsigned char data[4],unsigned char pos)
{
	if(data[pos]!=0x01) return 0;
	for(int i = 0; i<2; i++)
	{
		unsigned char pos2 = (pos + 4 - 1)%4;
		if(data[pos2]!=0x00) return 0;
		pos--;
	}
	unsigned char pos3 = (pos + 4 - 1)%4;
	if(data[pos3] == 0x00)
	{
		return 4;
	}
	else
	{
		return 3;
	}
	
}
//��ȡNAL��Ԫ
bool EncaFlv::readNal()
{
	unsigned char start[4] = {0xFF,0xFF,0xFF,0xFF};
	unsigned char data[4]  = {0xFF,0xFF,0xFF,0xFF};
	unsigned char pos      = 0;
	unsigned char d;
	//��ȡ��ʼ��
	fread(start,1,4,g_h264_file);
	
	if(start[0] == 0x00 && start[1] == 0x00)
	{
		if(start[2]== 0x01)
		{
			m_nal.startcodesize = 3;
		}
		else if(start[2] == 0x00)
		{
			if(start[3] == 0x01)
			{
				m_nal.startcodesize = 4;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	
	memcpy(m_nal.payload,start,4); m_nal.size = 4; //copy��nal��Ԫ��
	while(!feof(g_h264_file))
	{
		fread(&d,1,1,g_h264_file);
		data[pos]=d;
		m_nal.payload[m_nal.size++] = d;		
		
		//�����ʼ��
		int startsize = checkStartecode(data,pos);
		if(startsize)
		{
			m_nal.size -= startsize;
			fseek(g_h264_file,-startsize,SEEK_CUR);
			break;
		}
		pos++;
		pos = pos%4;

	}
	return true;
}
/* ��װ SPS PPS */
void EncaFlv::encapsulateSPSPPS()
{
	unsigned char *sps     = (unsigned char*)malloc(m_nal.size);
	unsigned int   spsSize =  m_nal.size;
	memcpy(sps,m_nal.payload,m_nal.size);
	
	if(!readNal())
	{
		free(sps);
		printf("SPS after must be PPS\n");
		return;
	}
	m_nal.nalType = (m_nal.payload[m_nal.startcodesize]&0x1F);//��ȡNAL����
	if(m_nal.nalType != NAL_PPS)
	{
		printf("SPS after must be PPS\n");
	}
	setTagHeaderDataSize(spsSize - m_nal.startcodesize + m_nal.size - m_nal.startcodesize + 5 + 11);//ע��11��AVconfiguration���ݳ���
	writeTagHeader(); 
	unsigned char data[6] = {0x17,0x00,0x00,0x00,0x00,0x01};//�ؼ�֡ H264 AVC sequence header  Composition time offset = 00 00 00 configurationVersion 0x01
	fwrite(data,6,1,g_flv_out);
	fwrite(sps + m_nal.startcodesize + 1,3,1,g_flv_out);//д��SPS������ǰ�����ֽ�
	unsigned char reserveBit0 = 0xFC;//111111
	unsigned char lengthSizeMinusOne = m_nal.startcodesize - 1;
	unsigned char d = reserveBit0 + lengthSizeMinusOne;
	fwrite(&d,1,1,g_flv_out);//NAL��ʼ���ֽ���Ŀ
	unsigned char numOfSequenceParameterSets = 0xE1; 
	fwrite(&numOfSequenceParameterSets,1,1,g_flv_out);//SPS����1��
	unsigned char sequenceParameterSetLength[2];

	unsigned int s = spsSize - m_nal.startcodesize;

	sequenceParameterSetLength[1] = (unsigned char)(s&0xFF); s = s>>8;
	sequenceParameterSetLength[0] = (unsigned char)(s&0xFF); s = s>>8;
	fwrite(sequenceParameterSetLength,2,1,g_flv_out);//дSPS����
	fwrite(sps+m_nal.startcodesize,spsSize-m_nal.startcodesize,1,g_flv_out);//дSPS
	free(sps);//�ͷ��ڴ�

	unsigned char numOfPictureParameterSets = 0x01; //PPS����
	fwrite(&numOfPictureParameterSets,1,1,g_flv_out);//дPPS����
	unsigned char pictureParameterSetLength[2];
	s = m_nal.size - m_nal.startcodesize;
	pictureParameterSetLength[1] = (unsigned char)(s&0xFF); s = s>>8;
	pictureParameterSetLength[0] = (unsigned char)(s&0xFF); s = s>>8;
	fwrite(pictureParameterSetLength,2,1,g_flv_out);//дPPS����
	fwrite(m_nal.payload + m_nal.startcodesize,m_nal.size-m_nal.startcodesize,1,g_flv_out);//дPPS
}
/* ��װ NAL*/
void EncaFlv::encapsulateNAL()
{
	setTagHeaderDataSize(m_nal.size - m_nal.startcodesize + 5 + 4);//5Ϊvideoͷ 4ΪNAL����
	writeTagHeader(); 
	unsigned char data[5] = {0x17,0x01,0x00,0x00,0x00};//�ؼ�֡ H264 AVC sequence header  Composition time offset = 00 00 00 
	fwrite(data,5,1,g_flv_out);

	unsigned char length[4];
	unsigned int s = m_nal.size - m_nal.startcodesize;

	length[3] = (unsigned char)(s&0xFF); s = s>>8;
	length[2] = (unsigned char)(s&0xFF); s = s>>8;
	length[1] = (unsigned char)(s&0xFF); s = s>>8;
	length[0] = (unsigned char)(s&0xFF); s = s>>8;
	fwrite(length,4,1,g_flv_out);//дNAL����
	fwrite(m_nal.payload + m_nal.startcodesize,m_nal.size-m_nal.startcodesize,1,g_flv_out);//дNAL
}

/* ��װ SEI*/
void EncaFlv::encapsulateSEI()
{
	unsigned char  *sei     = (unsigned char*)malloc(m_nal.size);
	unsigned int   seiSize =  m_nal.size;
	memcpy(sei,m_nal.payload,m_nal.size);

	if(!readNal())
	{
		free(sei);
		printf("SEI after must be IDR\n");
		return;
	}
	m_nal.nalType = (m_nal.payload[m_nal.startcodesize]&0x1F);//��ȡNAL����
	if(m_nal.nalType != NAL_SLICE_IDR)
	{
		printf("SEI after must be IDR\n");;
	}
	setTagHeaderDataSize(seiSize - m_nal.startcodesize + m_nal.size - m_nal.startcodesize + 8 + 5);//ע��8������NAL�ĳ���
	writeTagHeader(); 

	unsigned char data[5] = {0x17,0x01,0x00,0x00,0x00};//�ؼ�֡ H264 AVC sequence header  Composition time offset = 00 00 00 
	fwrite(data,5,1,g_flv_out);

	unsigned char length[4];
	unsigned int s = seiSize - m_nal.startcodesize;

	length[3] = (unsigned char)(s&0xFF); s = s>>8;
	length[2] = (unsigned char)(s&0xFF); s = s>>8;
	length[1] = (unsigned char)(s&0xFF); s = s>>8;
	length[0] = (unsigned char)(s&0xFF); s = s>>8;
	fwrite(length,4,1,g_flv_out);//дNAL����
	fwrite(sei + m_nal.startcodesize,seiSize-m_nal.startcodesize,1,g_flv_out);//дNAL

	free(sei);

    s = m_nal.size - m_nal.startcodesize;

	length[3] = (unsigned char)(s&0xFF); s = s>>8;
	length[2] = (unsigned char)(s&0xFF); s = s>>8;
	length[1] = (unsigned char)(s&0xFF); s = s>>8;
	length[0] = (unsigned char)(s&0xFF); s = s>>8;
	fwrite(length,4,1,g_flv_out);//дNAL����
	fwrite(m_nal.payload + m_nal.startcodesize,m_nal.size-m_nal.startcodesize,1,g_flv_out);//дNAL
}
/* ��װflv */
void EncaFlv::encapsulate()
{
	//дFLVͷ��Ϣ
	writeFlvHeader();
	/* д��һ��Tag Size */
	writeFirstTagSize();
	//дMetaData��Ϣ
	writeMetaData();

	m_tagConter = 1;

	m_DTS = 0.0;

	while(1)
	{
		if(!readNal())
			break;
		m_nal.nalType = (m_nal.payload[m_nal.startcodesize]&0x1F);//��ȡNAL����

		setTagHeader(TAGTYPE_VIDEO,(unsigned int)m_DTS);

		if(m_nal.nalType == NAL_SPS)
		{
			encapsulateSPSPPS();
			/* дǰһ��Tag Size */
			writePreTagSize();
		}
		else if(m_nal.nalType == NAL_SLICE_IDR)
		{
			encapsulateNAL();
			/* дǰһ��Tag Size */
			writePreTagSize();

			m_DTS += 1000.0/m_param->i_fps;
		}
		else if(m_nal.nalType == NAL_SLICE)
		{
			setTagHeaderDataSize(m_nal.size - m_nal.startcodesize + 5 + 4);//5Ϊvideoͷ 4ΪNAL����
			writeTagHeader(); 
			unsigned char data[5] = {0x27,0x01,0x00,0x00,0x00};//�ؼ�֡ H264 AVC sequence header  Composition time offset = 00 00 00 
			fwrite(data,5,1,g_flv_out);

			unsigned char length[4];
			unsigned int s = m_nal.size - m_nal.startcodesize;

			length[3] = (unsigned char)(s&0xFF); s = s>>8;
			length[2] = (unsigned char)(s&0xFF); s = s>>8;
			length[1] = (unsigned char)(s&0xFF); s = s>>8;
			length[0] = (unsigned char)(s&0xFF); s = s>>8;
			fwrite(length,4,1,g_flv_out);//дNAL����
			fwrite(m_nal.payload + m_nal.startcodesize,m_nal.size-m_nal.startcodesize,1,g_flv_out);//дNAL

			/* дǰһ��Tag Size */
			writePreTagSize();

			m_DTS += 1000.0/m_param->i_fps;
		}
		else if(m_nal.nalType == NAL_SEI)
		{
			encapsulateSEI();
			/* дǰһ��Tag Size */
			writePreTagSize();
			m_DTS += 1000.0/m_param->i_fps;
		}
		
		m_tagConter++;
	}
	printf("m_tagConter:    %d\n",m_tagConter);
}