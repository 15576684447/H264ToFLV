#ifndef XIAOC_ENCAPSULATION_H
#define XIAOC_ENCAPSULATION_H
//����  �����  20160615  xiaoc@pku.edu.cn
#include "h264toflv.h"
#include "flvfile.h"
#include "nal.h"

#define  TAGTYPE_AUDIO  8
#define  TAGTYPE_VIDEO  9
#define  TAGTYPE_META   18

#define M16E6 16777216 //16^6
#define M16E4 65536    //16^4
#define M16E2 256      //16^4
struct FlvHeader
{
	union
	{
		struct
		{
			unsigned char signature[3]; //�ֱ���ASCII���ʾ�� F L V
			unsigned char version;      //�汾��һ���ֽڱ�ʾ��һ��Ϊ0x01
			unsigned char present;      //4��ʾֻ����Ƶ 1��ʾֻ����Ƶ 5 ��ʾ������ƵҲ����Ƶ ����ֵ�Ƿ�
			unsigned char size[4];      //ΪFLV Header�ĳ��ȣ�Ϊ�̶�ֵ0x00000009  �ڱ�׼�й涨���汾1һ��Ϊ9����δ���汾�п��ܻ�����
		};
		unsigned char data[9];          //�洢����FLVͷ������ ���������ݹ����ڴ�
	};
};

struct TagHeader
{
	union
	{
		struct
		{
			unsigned char tagType;      //TAG����: 8 ��ʾ��Ƶtag 9��ʾ��Ƶtag 18��ʾ�ű����� 40 ��ʾ�����˲�����Ƶ 41��ʾ�����˲�����Ƶ 50��ʾ�����˲��Ľű����� ����ֵ����
			unsigned char datasize[3];  //ռ24λ ��ʾ��ǰtag�ĺ������ȵ��ڵ�ǰ����tag���ȼ�ȥ11��tagͷ��Ϣ��
			unsigned char timestamp[4]; //ռ24λΪ��Ե�һ��Tag��ʱ�������˵�һ��Tag��ʱ���Ϊ0��Ҳ���Խ�����Tag��ʱ���ȫ����Ϊ0�����������Զ����� ÿ��tag�ĵ�����ֽڿ�ʼ
			unsigned char streamID;     //һֱΪ0
		};
		unsigned char data[11];
	};
	unsigned int getSize()//��ȡsize
	{
		int s = datasize[0]*M16E4 + datasize[1]*M16E2 + datasize[2];
		return s + 11;
	}
};

struct Video
{
    /*
    1 �ؼ�֡ ��Ӧ����AVC��
    2 �ǹؼ�֡��Ӧ����AVC��
    3 h263�ķǹؼ�֡
    4 ���������ɹؼ�֡
    5 ��Ƶ��Ϣ������֡   **/
    int          m_frameType;//֡����

    /*
    2 H.263
    3 ��Ļ��Ƶ
    4 VP6
    5 VP6 ������alphaͨ��
    6 ��Ļ��Ƶ�汾2
    7 AVC��H.264��    **/
    int          m_codecID; //����������

    /*
    ����AVCʱ�д��ֶ�
    0  AVC sequence header (SPS��PPS��Ϣ��)
    1 AVC NALU
    2 AVC end of sequence (lower level NALU sequence ender is
    not required or supported)**/
    int          m_AVCPacketType; //AVC������

    /*
    ���ʱ��� ���AVCPacketType=0x01�� Ϊ���ʱ�����
    ��������Ϊ0��  
    ��ֵ��ʾ��ǰ֡����ʾʱ��  tag��ʱ��Ϊ����ʱ��    
    ��ʾʱ����� ����ʱ�� + CompositionTime    **/
    int          m_CompositionTime; //��ʾʱ��ƫ��
};
class EncaFlv
{
public:
	flv_param* m_param;         //���ò�����Ϣ

	FlvHeader    m_flvHeader;     //flvͷ��Ϣ
	TagHeader    m_tagHeader;     //tagͷ��Ϣ
	unsigned int m_tagConter;     //����tag����
	NAL          m_nal;           //�洢NAL��Ԫ
	Video        m_video;         //�洢video��Ϣ

	double       m_DTS;           //����ʱ��

	EncaFlv();
	void create(flv_param *param);/* �����ڴ� */
	void destory();/* �ͷ��ڴ� */

	//����FLVͷ��Ϣ
	void setFlvHeader();
	//дFLVͷ��Ϣ
	void writeFlvHeader();

	/* д��һ��Tag Size */
	void writeFirstTagSize();

	/* дǰһ��Tag Size */
	void writePreTagSize();

	/*����Tagͷ��Ϣ*/
	void setTagHeader(unsigned char tagType, unsigned int timestamp);
	/*����Tag���ݳ���*/
	void setTagHeaderDataSize(unsigned int datasize);
	//дTagͷ��Ϣ
	void writeTagHeader();

	//дMetaData��Ϣ
	void writeMetaData();

	//���NAL��ʼ�� ��������ʼ������
	int checkStartecode(unsigned char data[4],unsigned char pos);

	//��ȡNAL��Ԫ
	bool readNal();

	/* ��װ SPS PPS */
	void encapsulateSPSPPS();

	/* ��װ NAL*/
	void encapsulateNAL();

	/* ��װ SEI*/
	void encapsulateSEI();

	/* ��װflv */
	void encapsulate();

};

#endif