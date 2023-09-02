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
reg1->RootKey=HKEY_LOCAL_MACHINE;          //выбор раздела
reg1->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);     //открывает/создаёт ключ

	 if(reg1->ValueExists("ShowSecondsInSystemClock"))  //Если параметр в заданном ключе существует
	  {
		i1=reg1->ReadInteger("ShowSecondsInSystemClock");
			if (i1==1) {reg1->WriteInteger("ShowSecondsInSystemClock",0);  //запись данных
				CheckBox1->Caption="Выкл";
				CheckBox1->Checked=False;
			}
				else { reg1->WriteInteger("ShowSecondsInSystemClock",1);  //запись данных
					   CheckBox1->Caption="Вкл";}

	   }
	  else { //В противном случае
			reg1->WriteInteger("ShowSecondsInSystemClock",1);  //запись данных
			CheckBox1->Caption="Вкл";
			CheckBox1->Checked=True;
	  }
				 TextRestart->Caption="Перезайдите в систему!";
	 //ShowMessage("Перезайдите в систему!");
//
reg1->CloseKey();                          //Закрытие ключа
delete reg1;                               //удаляем объект, который нам больше не нужен
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox2Click(TObject *Sender)
{

//ShellExecuteA(0,0,"cmd","/c fsutil behavior set disabledeletenotify NTFS 1",0,SW_SHOW);
//ShellExecuteA(0,0,"cmd","/c fsutil behavior set disabledeletenotify ReFS 1",0,SW_SHOW);
  if(i2==0) {
		ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c fsutil behavior set disabledeletenotify NTFS 1 && fsutil behavior set disabledeletenotify ReFS 1").c_str(), 0, SW_SHOWNORMAL);
		i2=1;
		CheckBox2->Caption="Выключен";
		CheckBox2->Checked=true;
			} else {
					ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c fsutil behavior set disabledeletenotify NTFS 0 && fsutil behavior set disabledeletenotify ReFS 0").c_str(), 0, SW_SHOWNORMAL);
					i2=0;
					CheckBox2->Caption="Включен";
					CheckBox2->Checked=false;
					}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox3Click(TObject *Sender)
{
TRegistry *reg1=new TRegistry();
reg1->RootKey=HKEY_CURRENT_USER;          //выбор раздела
reg1->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",true);     //открывает/создаёт ключ

	 if(reg1->ValueExists("ConfirmFileDelete"))  //Если параметр в заданном ключе существует
	  {
		i1=reg1->ReadInteger("ConfirmFileDelete");
			if (i1==1) {reg1->WriteInteger("ConfirmFileDelete",0);  //запись данных
				CheckBox3->Caption="Вкл";
				CheckBox3->Checked=True;
				ShowMessage("Будьте осторожны! Файлы будут удаляться без предупреждения.");
			}
				else { reg1->WriteInteger("ConfirmFileDelete",1);  //запись данных
					   CheckBox3->Caption="Выкл";
					   CheckBox3->Checked=False;
					   ShowMessage("Теперь файлы будут удаляться с предупреждением.");
					   }

	   }
	  else { //В противном случае
			reg1->WriteInteger("ConfirmFileDelete",0);  //запись данных
			CheckBox3->Caption="Вкл";
			CheckBox3->Checked=True;
			ShowMessage("Будьте осторожны! Файлы будут удаляться без предупреждения.");
	  }

//
reg1->CloseKey();                          //Закрытие ключа
delete reg1;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::CheckBox4Click(TObject *Sender)
{
TRegistry *reg1=new TRegistry();
 reg1->RootKey=HKEY_CURRENT_USER;          //выбор раздела
reg1->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);     //открывает/создаёт ключ

	 if(reg1->ValueExists("Start_ShowClassicMode"))  //Если параметр в заданном ключе существует
	  {
		i1=reg1->ReadInteger("Start_ShowClassicMode");
			if (i1==0) {reg1->WriteInteger("Start_ShowClassicMode",1);  //запись данных
				CheckBox4->Caption="Вкл";
				CheckBox4->Checked=True;
			   //	ShowMessage("Будьте осторожны! Файлы будут удаляться без предупреждения.");
			}
				else { reg1->WriteInteger("Start_ShowClassicMode",0);  //запись данных
					   CheckBox4->Caption="Выкл";
					   CheckBox4->Checked=False;
				 //	   ShowMessage("Теперь файлы будут удаляться с предупреждением.");
					   }

	   }
	  else { //В противном случае
			reg1->WriteInteger("Start_ShowClassicMode",1);  //запись данных
			CheckBox4->Caption="Вкл";
			CheckBox4->Checked=True;
			//ShowMessage("Будьте осторожны! Файлы будут удаляться без предупреждения.");
	  }

//
reg1->CloseKey();
delete reg1;
}
//---------------------------------------------------------------------------

