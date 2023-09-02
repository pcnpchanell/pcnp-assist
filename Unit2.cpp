//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Registry.hpp"
#include "Windows.h"
#include "ShellAPI.h"
#include <shellapi.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
int i1,i2;


//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
i2=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox1Click(TObject *Sender)
{
TRegistry *reg1=new TRegistry();
reg1->RootKey=HKEY_LOCAL_MACHINE;          //����� �������
reg1->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);     //���������/������ ����

	 if(reg1->ValueExists("ShowSecondsInSystemClock"))  //���� �������� � �������� ����� ����������
	  {
		i1=reg1->ReadInteger("ShowSecondsInSystemClock");
			if (i1==1) {reg1->WriteInteger("ShowSecondsInSystemClock",0);  //������ ������
				CheckBox1->Caption="����";
				CheckBox1->Checked=False;
			}
				else { reg1->WriteInteger("ShowSecondsInSystemClock",1);  //������ ������
					   CheckBox1->Caption="���";}

	   }
	  else { //� ��������� ������
			reg1->WriteInteger("ShowSecondsInSystemClock",1);  //������ ������
			CheckBox1->Caption="���";
			CheckBox1->Checked=True;
	  }
				 TextRestart->Caption="����������� � �������!";
	 //ShowMessage("����������� � �������!");
//
reg1->CloseKey();                          //�������� �����
delete reg1;                               //������� ������, ������� ��� ������ �� �����
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox2Click(TObject *Sender)
{

//ShellExecuteA(0,0,"cmd","/c fsutil behavior set disabledeletenotify NTFS 1",0,SW_SHOW);
//ShellExecuteA(0,0,"cmd","/c fsutil behavior set disabledeletenotify ReFS 1",0,SW_SHOW);
  if(i2==0) {
		ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c fsutil behavior set disabledeletenotify NTFS 1 && fsutil behavior set disabledeletenotify ReFS 1").c_str(), 0, SW_SHOWNORMAL);
		i2=1;
		CheckBox2->Caption="��������";
		CheckBox2->Checked=true;
			} else {
					ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c fsutil behavior set disabledeletenotify NTFS 0 && fsutil behavior set disabledeletenotify ReFS 0").c_str(), 0, SW_SHOWNORMAL);
					i2=0;
					CheckBox2->Caption="�������";
					CheckBox2->Checked=false;
					}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox3Click(TObject *Sender)
{
TRegistry *reg1=new TRegistry();
reg1->RootKey=HKEY_CURRENT_USER;          //����� �������
reg1->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",true);     //���������/������ ����

	 if(reg1->ValueExists("ConfirmFileDelete"))  //���� �������� � �������� ����� ����������
	  {
		i1=reg1->ReadInteger("ConfirmFileDelete");
			if (i1==1) {reg1->WriteInteger("ConfirmFileDelete",0);  //������ ������
				CheckBox3->Caption="���";
				CheckBox3->Checked=True;
				ShowMessage("������ ���������! ����� ����� ��������� ��� ��������������.");
			}
				else { reg1->WriteInteger("ConfirmFileDelete",1);  //������ ������
					   CheckBox3->Caption="����";
					   CheckBox3->Checked=False;
					   ShowMessage("������ ����� ����� ��������� � ���������������.");
					   }

	   }
	  else { //� ��������� ������
			reg1->WriteInteger("ConfirmFileDelete",0);  //������ ������
			CheckBox3->Caption="���";
			CheckBox3->Checked=True;
			ShowMessage("������ ���������! ����� ����� ��������� ��� ��������������.");
	  }

//
reg1->CloseKey();                          //�������� �����
delete reg1;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::CheckBox4Click(TObject *Sender)
{
TRegistry *reg1=new TRegistry();
 reg1->RootKey=HKEY_CURRENT_USER;          //����� �������
reg1->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);     //���������/������ ����

	 if(reg1->ValueExists("Start_ShowClassicMode"))  //���� �������� � �������� ����� ����������
	  {
		i1=reg1->ReadInteger("Start_ShowClassicMode");
			if (i1==0) {reg1->WriteInteger("Start_ShowClassicMode",1);  //������ ������
				CheckBox4->Caption="���";
				CheckBox4->Checked=True;
			   //	ShowMessage("������ ���������! ����� ����� ��������� ��� ��������������.");
			}
				else { reg1->WriteInteger("Start_ShowClassicMode",0);  //������ ������
					   CheckBox4->Caption="����";
					   CheckBox4->Checked=False;
				 //	   ShowMessage("������ ����� ����� ��������� � ���������������.");
					   }

	   }
	  else { //� ��������� ������
			reg1->WriteInteger("Start_ShowClassicMode",1);  //������ ������
			CheckBox4->Caption="���";
			CheckBox4->Checked=True;
			//ShowMessage("������ ���������! ����� ����� ��������� ��� ��������������.");
	  }

//
reg1->CloseKey();
delete reg1;
}
//---------------------------------------------------------------------------

