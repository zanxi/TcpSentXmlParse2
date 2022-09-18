#include ".\stdafx.h"

/*
ZeroMemory (&dcb, sizeof (DCB));// �� ����� ��� ��� �����
*/
//������� ��������� ��� ����������� ������ �����
OVERLAPPED over;
//��� ��������� �������
DWORD dwSignal;
//������� ��������� ��� ������������ ��� �����
DCB dcb;
//���������� �����
HANDLE hPortDat = NULL;
//���������� ��� ������
char Data;
//==================================
//---------------------------------------------------------------------------
class MuClDataOutPort
{
private:

    //����� ������� ������������� �����
    bool InnitPort()
    {
        //��������� ���� ��� 1
        hPortDat = CreateFile(L"COM1", GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED, NULL);
        if (hPortDat == INVALID_HANDLE_VALUE)//���� ���� �� ������� �������
        {
            CloseHandle(hPortDat);
            return false;//������� �� ������� � �������
        }
        //����������� ��������� �����
        dcb.BaudRate = CBR_19200;//�������� ��������
        dcb.ByteSize = 8;//������ ��������
        dcb.StopBits = ONESTOPBIT;//���� �������� ���
        dcb.Parity = NULL;
        //��������� �� ������������ ���������

        if (!SetCommState(hPortDat, &dcb))  //���� �� ��������....
        {
            CloseHandle(hPortDat);
            return false;//������� �� ������� � �������
        }

        //���� ��� ����������� �� ���������� ������������� ���������
        return true;
    }
    //----------------------------------------------------
    //������� ������ ������ ����� ������
    BYTE ReadByteCOM()
    {
        if (InnitPort())
        {
            BYTE read = 0;
            DWORD dwByteRead = 0;
            do
            {//������ ���� �� �����
                if (!ReadFile(hPortDat, &read, sizeof(BYTE), &dwByteRead, NULL))
                {
                    return 0xFF;
                }
            } while (!dwByteRead);
            return read;//���������� ������
        }
        else
        {
            return 0xFF;
        }
    }
    //------------------------------------------------------
    //������� ������ ������ ����� ������
    bool WriteByteCOM(char bufer)
    {
        if (InnitPort())
        {
            DWORD dwByteWrite = 0;
            if (!WriteFile(hPortDat, &bufer, sizeof(char), &dwByteWrite, NULL))
            {
                return false;
            }
            return true;//���������� �������� � ����������
        }
        else
        {
            return false;
        }
    }
    //=============================================
    //������� ������ ������� ������
    //=============================================
    //=============================================
    //=============================================
public:
    //����� ������� ��� ������ � ������
    void GeneralCOMRead()
    {
        //��������� ������ � �����
        if (dwSignal & EV_DSR)//������ ������ ��� ������
        {
            //������ ���� �� �����
            Data = ReadByteCOM();
            //��������� ���� ����-����
        }
    }
    //---------------------------------------------------------
    void GeneralCOMWrite(char InData)
    {
        //��������� ������ � �����
        if (dwSignal & EV_CTS)//������ ������ ��� ������
        {
            //������� ���� �� ��� � ���������� ��� � ����
            WriteByteCOM(InData);
        }
    }
    //---------------------------------------------------------
    void CloseCOM()
    {
        if (over.hEvent)
        {
            CloseHandle(over.hEvent);//��������� ������ �������
        }
        if (hPortDat)
        {
            CloseHandle(hPortDat);
            hPortDat = NULL;
        }
    }
};
#pragma package(smart_init) 