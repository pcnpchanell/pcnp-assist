//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Registry.hpp"
#include "sysutils.hpp"
#include <winapifamily.h>
#include <shellapi.h>
#include "ShellAPI.h"
#include "Windows.h"
#include "winreg.h"
#include "sddl.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <process.h>
#include "IniFiles.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
String Link1="https://www.youtube.com/@PCbezproblem";
String Link2="https://ulap.top";
String Language="RUS";

String s,s1,wv,we,proc,procv,procm,biosy,winvibor,winnum,theme;
int version=8;
int x,y,po=0,po1=0,trim,safemode=0;
char c1,c2;
int i,len,memkol,UpdErr,con=0,FirstStart=1,sel=1,openserv=1;
bool analis=0,upd=0;
//TSystemInfo SI;
TMemoryStatus MS;
TRegistry *reg2 = new TRegistry(KEY_ALL_ACCESS);
TRegistry *reg1 = new TRegistry(KEY_ALL_ACCESS);
TStringList *list=new TStringList;
TStringList *fs=new TStringList;
TStringList *uninstall=new TStringList;
TStringList *appsnames=new TStringList;
TStringList *cleansizes=new TStringList;
TStringList *tempnames=new TStringList;
//---------------------------------------------------------------------------
int EditRegistryRight(String TextForCmd)
{

try{
int FileHandle;//////////////////
String Path = ExtractFilePath(Application->ExeName);
String FileName = "code.txt";
FileHandle=FileCreate(Path + FileName);//////////////////
FileClose(FileHandle);//////////////////
TStringList *l=new TStringList;
l->LoadFromFile(Path + FileName);
l->Add(TextForCmd);
l->SaveToFile(Path + FileName);
//ShowMessage("CNJG");
ShellExecute(Form1->Handle, L"open", L"cmd.exe",String().sprintf(L"/c regini code.txt").c_str(), 0, SW_SHOWNORMAL);
Sleep(500);//WinExec("cmd /c regini code.txt", SW_SHOWNORMAL);
DeleteFile("code.txt"); delete l;
return 1;
} catch(...) {return 0;}
}
//--------------------

//------------------------------------------------------------------------
int CheckRR(HKEY key,String Adres)
{
int check=0;
TRegistry *reg3 = new TRegistry(KEY_ALL_ACCESS);
try{
  reg3->RootKey=key;
  reg3->OpenKey(Adres,true);
  reg3->WriteInteger("Test",1);
  reg3->DeleteValue("Test");
  check=1;
  reg3->CloseKey();
} catch(...) {
  check=0;
}
delete reg3;
return check;
}
//----------------------------------------------------------------------------
void FolderSize(String Folder, long long &Size, int &fCount, int &folCount)
{         // определение размера папки. Size - в байтах.
	TSearchRec SR;
	if (Folder[Folder.Length()] == '\\')
		Folder.SetLength(Folder.Length() - 1);
	if (FindFirst(Folder + "\\*.*", faAnyFile, SR) == 0)
		do
		{
			if (SR.Name != "." && SR.Name != "..")
			{
				if ((SR.Attr & faDirectory) != 0)
				{
			   //	ShowMessage(Folder + "\\" + SR.Name);
					FolderSize(Folder + "\\" + SR.Name, Size, fCount, folCount);
					folCount++;
					if(fCount==555) {
					//Sleep(1);
					 Application->ProcessMessages();
					 if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;
					}
				}
				else
				{
					//ShowMessage(Folder + "\\" + SR.Name+" , "+SR.Size);
					if(SR.Size>1) Size = Size + SR.Size;
					fCount++;
						}
			}
		} while (FindNext(SR) == 0);
		FindClose(SR);
}
//--------------------------------------------------------------------------
long long GetFileSize(const String FileName)
{
  TSearchRec SearchRec;
  long long returnValue=0;

  if(FindFirst(ExpandFileName(FileName), faAnyFile, SearchRec)==0)
	 returnValue=SearchRec.Size;
	 FindClose(SearchRec);
	 //ShowMessage(returnValue);

  return returnValue;

}
//---------------------------------------------------------------------------

void __fastcall ScanPCTemp(TObject *Sender)
{

String pathdir=GetEnvironmentVariable("Temp");
long long TotalSize=0;
long long Size; int fCount; int folCount;
FolderSize(pathdir, Size, fCount, folCount);
	  //	Sleep(1);
	  //Application->ProcessMessages();

TotalSize+=Size;
Size=0;
pathdir="C:\\Windows\\SoftwareDistribution\\Download";
FolderSize(pathdir, Size, fCount, folCount);
TotalSize=TotalSize+Size;
Size=0;
pathdir="C:\\Windows\\Temp";
FolderSize(pathdir, Size, fCount, folCount);
TotalSize+=Size;
	   //	Sleep(1);
	  //Application->ProcessMessages();

Size=0;
pathdir="C:\\Windows\\WinSxS";
//FolderSize(pathdir, Size, fCount, folCount);
//if(Size>7000000000) TotalSize=TotalSize+Size-7000000000;
Size=0;
if (FileExists("C:\\hiberfil.sys")) TotalSize=TotalSize+GetFileSize("C:\\hiberfil.sys");

double s(0.1);
s=TotalSize/1024/1024;
 Form1->Label133->Caption=String(s)+" Mб+";

}
//------------------------------
void BrowserCache(String Folder, long long &Size)
{         // определение размера папки. Size - в байтах.
	TSearchRec SR;
	long long s; int a1,a2,stop=0; String p;  int l;
	if (Folder[Folder.Length()] == '\\')
		Folder.SetLength(Folder.Length() - 1);
	if (FindFirst(Folder + "\\*.*", faAnyFile, SR) == 0)
		do
		{
			if (SR.Name != "." && SR.Name != "..")
			{
				if ((SR.Attr & faDirectory) != 0)
				{
				stop=0;
					for (int i = 1; i < SR.Name.Length(); i++) {
						if(SR.Name.SubString(i,5)=="Cache" or SR.Name.SubString(i,5)=="cache") {
							stop=1;
							FolderSize(Folder + "\\" + SR.Name,s,a1,a2);
							if(s>2049000) {
							Size+=s;
							s=s/1024/1024;
							p=Folder + "\\" + SR.Name;

							tempnames->Add(p);

							l=p.Length();
							if(l>70) p=p.SubString(1,20)+"..."+p.SubString(l-45,46);
							Form1->CleanBox->Items->Add(p+" ("+String(s)+" Мб)");

							s=0;
							Sleep(1);
							  Application->ProcessMessages();
							  if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
								else  Form1->ProgressBar1->Position=0;
							}
						}
					}
			   //	ShowMessage(Folder + "\\" + SR.Name);
					if(stop==0) BrowserCache(Folder + "\\" + SR.Name, Size);

				}
				else
				{
					for (int i = 1; i < SR.Name.Length(); i++) {
						if(SR.Name.SubString(i,8)=="-journal") {
							s=GetFileSize(Folder + "\\" + SR.Name);
							if(s>2049000) {
							Size+=s;
							s=s/1024/1024;

							p=Folder + "\\" + SR.Name;

							tempnames->Add(p);

							l=p.Length();
							if(l>70) p=p.SubString(1,20)+"..."+p.SubString(l-45,46);
							Form1->CleanBox->Items->Add(p+" ("+String(s)+" Мб)");
							s=0;
								  Sleep(1);
								  Application->ProcessMessages();
								  if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;
							}
						}
					}
					//ShowMessage(Folder + "\\" + SR.Name+" , "+SR.Size);
					//if(SR.Size>1) Size = Size + SR.Size;

						}
			}
		} while (FindNext(SR) == 0);
		FindClose(SR);
}
//-----------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

reg2->CloseKey();
//Проверка на файлы:
if (!FileExists("AppDesc.ini",true) || !FileExists("ServDesc.ini",true) || !FileExists("SetACL.exe",true) || !FileExists("ServDef.psl",true)) {
	 ShowMessage("App files were corrupted. Reinstall the APP.");
	 Close();
	 }


Form1->Left=Screen->Width/2.0-Form1->Width/2.0;
Form1->Top=Screen->Height/2.0-Form1->Height/2.0;

BitBtn12->Visible=false;
PageControl1->Visible=true;
Panel3->Visible=false;
PageControl1->ActivePage=TabSheet1;
CardPanel2->ActiveCard=Card6;
CardPanel3->Visible=false;

FILE *f;

//fclose(f);
GlobalMemoryStatus(&MS);
memkol=MS.dwTotalPhys/1024/1024; //(RAM в МБ)
Label6->Caption="Оперативной памяти: "+String(memkol)+" Мб";
 //определение версии Виндовс
 TRegistry *reg=new TRegistry(KEY_ALL_ACCESS);
try {
 reg->RootKey=HKEY_LOCAL_MACHINE;          //выбор раздела
reg->OpenKey("Software\\Microsoft\\Windows NT\\CurrentVersion",true);     //открывает/создаёт ключ
wv=reg->ReadString("CurrentBuild");
s1=wv.SubString1(0,2);
if (StrToInt(s1)>=22) wv="Windows 11";
		  else wv="Windows 10";
s1=reg->ReadString("ProductName");
we=s1.SubString1(12,3);
Label1->Caption="Текущая ОС: "+wv+" "+we+" "+reg->ReadString("DisplayVersion")+
	" build "+reg->ReadString("CurrentBuild");
reg->CloseKey();

//определение версии процессора и озу
reg->RootKey=HKEY_LOCAL_MACHINE;          //выбор раздела
reg->OpenKey("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",true);
proc=reg->ReadString("ProcessorNameString");
len=proc.Length();
for (i = 0; i < len; i++) {
   if (proc.SubString(i,5)=="Intel") procv="Intel";
   if (proc.SubString(i,3)=="AMD") procv="AMD";
}
reg->CloseKey();

reg->RootKey=HKEY_LOCAL_MACHINE;          //BIOS YEAR
reg->OpenKey("HARDWARE\\DESCRIPTION\\System\\BIOS",true);
biosy=reg->ReadString("BIOSReleaseDate").SubString(7,4);

Label5->Caption="Процессор: "+proc;
Label7->Caption="Год выпуска BIOS: "+biosy;

 if(memkol<3800)     ComboBox1->ItemIndex=2;
 else {
	 if(memkol>5000 & StrToInt(biosy)>=2017) ComboBox1->ItemIndex=4;
	 else ComboBox1->ItemIndex=3;
 }

} catch (...) { ShowMessage("Ваша версия системы не поддерживается");
}
reg->CloseKey();
delete reg;

//------------------РЕКЛАМА-----------------------------------
//int rek=random(2);
TFileStream* FU1 = new TFileStream("rek1.jpg", fmCreate | fmOpenWrite);
TFileStream* FU2 = new TFileStream("rek2.jpg", fmCreate | fmOpenWrite);
TFileStream* FU3 = new TFileStream("li1.txt", fmCreate | fmOpenWrite);
TFileStream* FU4 = new TFileStream("li2.txt", fmCreate | fmOpenWrite);

 try
 {
 IdHTTP1->Get("http://ulap.top/pcnp_prog/adv1.jpg", FU1);
 IdHTTP1->Get("http://ulap.top/pcnp_prog/adv2.jpg", FU2);
 IdHTTP1->Get("http://ulap.top/pcnp_prog/link1.txt", FU3);
 IdHTTP1->Get("http://ulap.top/pcnp_prog/link2.txt", FU4);

 Timer1->Enabled = true;
 delete(FU1);
 delete(FU2);
 delete(FU3);
 delete(FU4);

 con=1;
 }
 catch (...)
 {
 con=0;
 delete(FU1);
 delete(FU2);
 delete(FU3);
 delete(FU4);

  //ShowMessage("Не получилось подключиться к сайту программы");
 }

 if(con==1) {  //Если есть интернет соединение:

 // ПРОВЕРКА ОБНОВЛЕНИЙ ПРИ СТАРТЕ.
UpdErr=0;
  TFileStream* FU = new TFileStream("version.txt", fmCreate | fmOpenWrite);
 try
 {
 IdHTTP1->Get("http://ulap.top/pcnp_prog/version.txt", FU);
 }
 catch (...){ }
 delete FU;
  if (UpdErr==0)
  {
	  f=fopen("version.txt","r");
	  char t[10];
	  String newvers;
	  fgets (t, sizeof(t), f);
		   for(int it=0;it<4;it++)
			  {
			   newvers+=t[it];  }
					  if (StrToInt(newvers)>version)
					  {
					  if(MessageBox(NULL, L"Желаете ли вы обновиться?", L"Доступна новая версия",  MB_YESNO) == IDYES)
{
					  //ShowMessage("Доступна новая версия");
					  ShellExecute(0,L"open",L"https://ulap.top/pcnp-soft", NULL, NULL, SW_NORMAL);
 }
					  }


  fclose(f);
  }
  //Загрузка данных об установке на сервер
try {
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	if(reg2->ValueExists("NotFirstStart")) {
	  //программа запускается не в первый раз
		   if(reg2->ValueExists("ThemeStyle")) theme=reg2->ReadString("ThemeStyle");

				if(theme=="Light")
				{
				 Form1->StyleName="Windows10 Clear Day";
				 N5->Caption="Темная тема";
				}  else {
						  Form1->StyleName="Material Oxford Blue SE";
						  N5->Caption="Светлая тема";
						}
		   if(reg2->ValueExists("Language")) Language=reg2->ReadString("Language");

	  FirstStart=0;
	} else {
	  Timer3->Enabled=true;
	  reg2->WriteInteger("NotFirstStart",1);
	  reg2->WriteInteger("BootMode",0); //0-normal mode
	   reg2->CloseKey();
	  //ОПРЕДЕЛЯЕМ ЯЗЫК
	reg2->RootKey=HKEY_LOCAL_MACHINE;
	reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Nls\\Language",true);
	String l=reg2->ReadString("Default");
	if(l=="0419" || l=="1000" || l=="0819" || l=="0022" || l=="0422") Language="RUS";
	   else Language="EN";
	reg2->CloseKey();

	if(Language=="EN")  ShellExecute(0,L"open",L"http://easywin.info/pcnp_prog/thanks.php", NULL, NULL, SW_NORMAL);
	if(Language=="RUS") ShellExecute(0,L"open",L"https://ulap.top/pcnp_prog/thanks.php", NULL, NULL, SW_NORMAL);
	}
	reg2->CloseKey();

} catch (...) {
}
   reg2->CloseKey();
  DeleteFile("version.txt");

  }
  else {
  //Загрузка данных об установке на сервер
try {
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	if(reg2->ValueExists("NotFirstStart")) {
	  //программа запускается не в первый раз
		   if(reg2->ValueExists("ThemeStyle")) theme=reg2->ReadString("ThemeStyle");

				if(theme=="Light")
				{
				 Form1->StyleName="Windows10 Clear Day";
				 N5->Caption="Темная тема";
				}  else {
						  Form1->StyleName="Material Oxford Blue SE";
						  N5->Caption="Светлая тема";
						}
		   if(reg2->ValueExists("Language")) Language=reg2->ReadString("Language");

	  FirstStart=0;
	} else {
		Timer3->Enabled=true;
	  reg2->WriteInteger("NotFirstStart",1);
	  reg2->WriteInteger("BootMode",0); //0-normal mode
	  ShellExecute(0,L"open",L"http://ulap.top/pcnp_prog/thanks.php", NULL, NULL, SW_NORMAL);
	   reg2->CloseKey();
	  //ОПРЕДЕЛЯЕМ ЯЗЫК
	reg2->RootKey=HKEY_LOCAL_MACHINE;
	reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Nls\\Language",true);
	String l=reg2->ReadString("Default");
	if(l=="0419" || l=="1000" || l=="0819" || l=="0022" || l=="0422") Language="RUS";
	   else Language="EN";
	reg2->CloseKey();

	}
} catch(...) {}
   }
   reg2->CloseKey();
   reg2->RootKey=HKEY_CURRENT_USER;
   reg2->OpenKey("Software\\PCNP",true);
  if(!reg2->ValueExists("BootMode")) reg2->WriteInteger("BootMode",0);
	if(reg2->ValueExists("BootMode")) {
		if(reg2->ReadInteger("BootMode")==0) {N6->Caption="Войти в безопасный режим"; safemode=0;    }
		if(reg2->ReadInteger("BootMode")==1) {N6->Caption="Выйти из безопасного режима"; safemode=1;  }
		}
	reg2->CloseKey();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn10Click(TObject *Sender)
{
PageControl1->Visible=false;
PageControl2->Visible=false;
Panel3->Visible=false;
BitBtn12->Visible=false;
CardPanel3->Visible=true;
CardPanel3->ActiveCard=Card10;

tempnames->Clear();
CleanBox->Clear();
long long TotalSize=0;
long long Size; int fCount; int folCount; int MB;
fCount=555;

Application->ProcessMessages();
					 if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;

String pathdir=GetEnvironmentVariable("TEMP");
FolderSize(pathdir, Size, fCount, folCount);
TotalSize+=Size;
MB=Size/1024/1024;
int l=pathdir.Length();
CleanBox->Items->Add(pathdir+" (-"+String(MB)+" Мб)");
tempnames->Add(pathdir);

Application->ProcessMessages();
					 if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;
Size=0;
pathdir="C:\\Windows\\Temp";
FolderSize(pathdir, Size, fCount, folCount);
TotalSize+=Size;
MB=Size/1024/1024;
l=pathdir.Length();
CleanBox->Items->Add(pathdir+" (-"+String(MB)+" Мб)");
tempnames->Add(pathdir);

Application->ProcessMessages();
					 if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;
Size=0;
pathdir="C:\\Windows\\SoftwareDistribution\\Download";
FolderSize(pathdir, Size, fCount, folCount);
TotalSize=TotalSize+Size;
MB=Size/1024/1024;
l=pathdir.Length();
CleanBox->Items->Add(pathdir+" (-"+String(MB)+" Мб)");
tempnames->Add(pathdir);

Application->ProcessMessages();
					 if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;
Size=0;
pathdir="C:\\Windows\\Prefetch";
FolderSize(pathdir, Size, fCount, folCount);
TotalSize=TotalSize+Size;
MB=Size/1024/1024;
l=pathdir.Length();
CleanBox->Items->Add(pathdir+" (-"+String(MB)+" Мб)");
tempnames->Add(pathdir);

Application->ProcessMessages();
					 if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;
Size=0;
pathdir="C:\\Windows\\WinSxS";
FolderSize(pathdir, Size, fCount, folCount);
if(Size>7000000000)
	{
	Size-=7000000000;
	TotalSize=TotalSize+Size;
	MB=Size/1024/1024;
	l=pathdir.Length();
	CleanBox->Items->Add(pathdir+" (-"+String(MB)+" Мб)");
	tempnames->Add(pathdir);
	}

    Application->ProcessMessages();
					 if(Form1->ProgressBar1->Position<100) Form1->ProgressBar1->Position++;
									else  Form1->ProgressBar1->Position=0;
Size=0;
pathdir="C:\\hiberfil.sys";
	if (FileExists("C:\\hiberfil.sys")) {
	Size=GetFileSize("C:\\hiberfil.sys");
	TotalSize=TotalSize+Size;
	MB=Size/1024/1024;
	l=pathdir.Length();
	if(l>35) pathdir=pathdir.SubString(1,25)+pathdir.SubString(l-20,21);
	CleanBox->Items->Add(pathdir+" - Отключить гибернацию (-"+String(MB)+" Мб)");
	tempnames->Add(pathdir);
	}
fCount=0;
CleanBox->Items->Add("Удалить историю просмотров в проводнике");
tempnames->Add("");

CleanBox->Items->Add("Очистить кэш магазина Майкрософт");
tempnames->Add("");

//Браузеры
pathdir=GetEnvironmentVariable("USERPROFILE")+"\\AppData";
Size=0;
BrowserCache(pathdir,Size);
TotalSize=TotalSize+Size;

TotalSize=TotalSize/1024/1024;
Label115->Caption="Можно освободить "+String(TotalSize)+" Мб на диске С:";

ProgressBar1->Visible=false;
CleanBox->Visible=true;
Label113->Visible=true;
Label114->Visible=true;
Label115->Visible=true;
Button9->Visible=true;
Button10->Visible=true;
Button11->Visible=true;
Button12->Visible=true;
Button13->Visible=true;
SpeedButton2->Visible=true;
VirtualImage97->Visible=true;

}
//-------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c taskkill /f /im explorer.exe && start explorer.exe").c_str(), 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
  //EdgeBrowser1->Refresh();
 //<object width=""640"" height=""390"">  <param name=""movie"" value=""http://www.youtube.com/v/L7NWdxFAHdY&hl=en_US&feature=player_embedded&version=3"">   </param><param name=""allowFullScreen"" value=""true"">    </param><param name=""allowScriptAccess"" value=""always"">    </param><embed src=""http://www.youtube.com/v/L7NWdxFAHdY&hl=en_US&feature=player_embedded&version=3"" type=""application/x-shockwave-flash"" allowfullscreen=""true"" allowScriptAccess=""always"" width=""240"" height=""190"">    </embed></object>      </body>     </html>  ');
 //EdgeBrowser1->Navigate(1);
  //EdgeBrowser1->NavigateToString("<html><body><object width=""640"" height=""390"">  <param name=""movie"" value=""http://www.youtube.com/v/k-A8ju9KJxs&hl=en_US&feature=player_embedded&version=3"">   </param><param name=""allowFullScreen"" value=""true"">    </param><param name=""allowScriptAccess"" value=""always"">    </param><embed src=""http://www.youtube.com/v/k-A8ju9KJxs&hl=en_US&feature=player_embedded&version=3"" type=""application/x-shockwave-flash"" allowfullscreen=""true"" allowScriptAccess=""always"" width=""240"" height=""190"">    </embed></object></body> </html>");

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
BitBtn12->Visible=false;
BitBtn13->OnClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{

ShellExecute(0,L"open",L"https://www.youtube.com/playlist?list=PL5Ps-y0uPjuOdYilgi7MIdfbuGso_8pH2", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{//   Вторая страница - скачивание виндовс и подготовка к установке
Panel1->Visible=false;
Panel2->Visible=true;
  if(ComboBox1->ItemIndex==4) {
	winvibor="11";
	Label9->Caption="Вы выбрали Windows 11 Home. Давайте приступим к установке:";
	Label10->Caption="1. Скачайте программу для записи установщика Windows на флешку";
	Label11->Caption="Скачать Windows 11 (последняя версия)";
	//EdgeBrowser1->Refresh();
	//EdgeBrowser1->NavigateToString("<html><body><h1>hello</h1><object width=""337"" height=""178"">  <param name=""movie"" value=""https://www.youtube.com/watch?v=eRD2mPngBqE"">   </param><param name=""allowFullScreen"" value=""true"">    </param><param name=""allowScriptAccess"" value=""always"">    </param><embed src=""https://www.youtube.com/watch?v=eRD2mPngBqE"" type=""application/x-shockwave-flash"" allowfullscreen=""true"" allowScriptAccess=""always"" width=""337"" height=""178"">    </embed></object></body> </html>");
	//EdgeBrowser1->NavigateToString("<html><body><iframe width=""337"" height=""178"" src=""https://www.youtube.com//embed//eRD2mPngBqE"" title=""YouTube video player"" frameborder=""0"" allow=""accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"" allowfullscreen><//iframe><//body> <//html>");
   //	 EdgeBrowser1->Refresh();
 //<object width=""640"" height=""390"">  <param name=""movie"" value=""http://www.youtube.com/v/L7NWdxFAHdY&hl=en_US&feature=player_embedded&version=3"">   </param><param name=""allowFullScreen"" value=""true"">    </param><param name=""allowScriptAccess"" value=""always"">    </param><embed src=""http://www.youtube.com/v/L7NWdxFAHdY&hl=en_US&feature=player_embedded&version=3"" type=""application/x-shockwave-flash"" allowfullscreen=""true"" allowScriptAccess=""always"" width=""240"" height=""190"">    </embed></object>      </body>     </html>  ');
	//EdgeBrowser1->Navigate("http://ulap.top/pcnp_prog/win11inst.html");
	 VirtualImage89->ImageIndex=29;
	 Label14->Caption="Что, если не загружается с флешки?";
	 Label15->Caption="5 ошибок при установке Windows";
	 Label16->Visible=false;
	}
   if(ComboBox1->ItemIndex==3) {
	winvibor="10";
	Label9->Caption="Вы выбрали Windows 10 Home. Давайте приступим к установке:";
	Label10->Caption="1. Скачайте программу для записи установщика Windows на флешку";
	Label11->Caption="Скачать Windows 10 (скачайте средство для установки)";
	//EdgeBrowser1->Navigate("http://ulap.top/pcnp_prog/win10inst.html");
	 VirtualImage89->ImageIndex=30;
	 Label14->Caption="Что, если не загружается с флешки?";
	 Label15->Caption="5 ошибок при установке Windows";
	 Label16->Visible=false;
  }
   if(ComboBox1->ItemIndex==2) {
	winvibor="8e";
	Label9->Caption="Вы выбрали Windows 8.1 Embedded. Давайте приступим к установке:";
	Label10->Caption="1. Скачайте образ ISO для записи на флешку";
	Label11->Caption="Официальная ссылка уже не доступна. Получите вместе с лицензией";
	//EdgeBrowser1->Navigate("http://ulap.top/pcnp_prog/win10inst.html");
	 VirtualImage89->ImageIndex=30;
	 Label14->Caption="Что, если не загружается с флешки?";
	 Label15->Caption="5 ошибок при установке Windows";
	 Label16->Visible=false;
  }
  if(ComboBox1->ItemIndex==1) {
	winvibor="8h";
	Label9->Caption="Вы выбрали Windows 8.1 Home. Давайте приступим к установке:";
	Label10->Caption="1. Скачайте образ ISO для записи на флешку";
		Label11->Caption="Официальная ссылка уже не доступна. Получите вместе с лицензией";
//	EdgeBrowser1->Navigate("http://ulap.top/pcnp_prog/win10inst.html");
	 VirtualImage89->ImageIndex=30;
	 Label14->Caption="Что, если не загружается с флешки?";
	 Label15->Caption="5 ошибок при установке Windows";
	 Label16->Visible=false;
  }
  if(ComboBox1->ItemIndex==0) {
	winvibor="7";
	Label9->Caption="Вы выбрали Windows 7 Home. Давайте приступим к установке:";
	Label10->Caption="1. Скачайте образ ISO для записи на флешку";
	Label11->Caption="Официальная ссылка уже не доступна. Получите вместе с лицензией";
   //	EdgeBrowser1->Navigate("http://ulap.top/pcnp_prog/win7inst.html");
	VirtualImage89->ImageIndex=31;
	 Label14->Caption="Что, если не загружается с флешки?";
	 Label15->Caption="5 ошибок при установке Windows";
	 Label16->Visible=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn5Click(TObject *Sender)
{
 Panel2->Visible=false;
Panel1->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3Click(TObject *Sender)
{
ShellExecute(0,L"open",L"http://ulap.top/pcnp-soft", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
FILE *f;
UpdErr=0;
  TFileStream* FU = new TFileStream("version.txt", fmCreate | fmOpenWrite);
 try
 {
 IdHTTP1->Get("http://ulap.top/pcnp_prog/version.txt", FU);
 }
 catch (...)
 {
 UpdErr=1;
 ShowMessage("Обновления недоступны на данный момент");
 }
 delete FU;
  if (UpdErr==0)
  {
	  f=fopen("version.txt","r");
	  char t[10];
	  String newvers;
	  fgets (t, sizeof(t), f);
		   for(int it=0;it<4;it++)
			  {
			   newvers+=t[it];   }
			   if (StrToInt(newvers)<=version)
					 {
					 ShowMessage("Установлена последняя версия");
					 }
					  if (StrToInt(newvers)>version)
					  {
					  ShowMessage("Доступна новая версия");
					  ShellExecute(0,L"open",L"http://ulap.top/pcnp-soft", NULL, NULL, SW_NORMAL);

					  }


  fclose(f);
  }
  DeleteFile("version.txt");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{
//Form1->Show();

CardPanel3->Visible=false;
BitBtn12->Visible=false;
PageControl2->Visible=false;
Panel3->Visible=false;
PageControl1->Visible=true;

}
//---------------------------------------------------------------------------



void __fastcall TForm1::Label11Click(TObject *Sender)
{
if(winvibor=="11") ShellExecute(0,L"open",L"https://www.microsoft.com/ru-ru/software-download/windows11", NULL, NULL, SW_NORMAL);
if(winvibor=="10") ShellExecute(0,L"open",L"https://www.microsoft.com/ru-ru/software-download/windows10", NULL, NULL, SW_NORMAL);
//if(winvibor=="8e") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
//if(winvibor=="8h") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
//if(winvibor=="7") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Label11MouseLeave(TObject *Sender)
{
Label11->Font->Style = TFontStyles() << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label11MouseEnter(TObject *Sender)
{
Label11->Font->Style = TFontStyles() << fsUnderline << fsItalic;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Label14Click(TObject *Sender)
{                      ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=SArCVCFVzI8", NULL, NULL, SW_NORMAL);
/* if(winvibor=="11") ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=SArCVCFVzI8", NULL, NULL, SW_NORMAL);
if(winvibor=="10") ShellExecute(0,L"open",L"https://www.microsoft.com/ru-ru/software-download/windows10", NULL, NULL, SW_NORMAL);
if(winvibor=="8e") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
if(winvibor=="8h") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
if(winvibor=="7") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
*/
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label15Click(TObject *Sender)
{   ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=acHvQpMn1dw", NULL, NULL, SW_NORMAL);
/*if(winvibor=="11") ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=acHvQpMn1dw", NULL, NULL, SW_NORMAL);
if(winvibor=="10") ShellExecute(0,L"open",L"https://www.microsoft.com/ru-ru/software-download/windows10", NULL, NULL, SW_NORMAL);
if(winvibor=="8e") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
if(winvibor=="8h") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
if(winvibor=="7") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
*/
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label16Click(TObject *Sender)
{
//if(winvibor=="11") ;
if(winvibor=="10") ShellExecute(0,L"open",L"https://www.microsoft.com/ru-ru/software-download/windows10", NULL, NULL, SW_NORMAL);
if(winvibor=="8e") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
if(winvibor=="8h") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
if(winvibor=="7") ShellExecute(0,L"open",L"https://www.microsoft.com/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PageControl1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
Label16->Caption=String(x)+"  "+String(y);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label14MouseEnter(TObject *Sender)
{
Label14->Font->Style = TFontStyles() << fsUnderline << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label14MouseLeave(TObject *Sender)
{
Label14->Font->Style = TFontStyles() << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label15MouseEnter(TObject *Sender)
{
Label15->Font->Style = TFontStyles() << fsUnderline << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label15MouseLeave(TObject *Sender)
{
Label15->Font->Style = TFontStyles() << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label16MouseEnter(TObject *Sender)
{
Label16->Font->Style = TFontStyles() << fsUnderline << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label16MouseLeave(TObject *Sender)
{
Label16->Font->Style = TFontStyles() << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn6Click(TObject *Sender)
{
//EdgeBrowser1->Refresh();
PageControl1->ActivePage=TabSheet2;
BitBtn12->Visible=true;
CardPanel1->ActiveCard=Card1;
}
//---------------------------------------------------------------------------





void __fastcall TForm1::ComboBoxEx1Change(TObject *Sender)
{
if(ComboBoxEx1->Text=="Программы для учебы и работы") CardPanel1->ActiveCard=Card1;
if(ComboBoxEx1->Text=="Мультимедиа и развлечения") CardPanel1->ActiveCard=Card2;
if(ComboBoxEx1->Text=="Интернет - серфинг и работа") CardPanel1->ActiveCard=Card3;
if(ComboBoxEx1->Text=="Обработка фото, аудио и видео") CardPanel1->ActiveCard=Card4;
if(ComboBoxEx1->Text=="Мессенджеры на ПК") CardPanel1->ActiveCard=Card5;
OnCardPanel1->Top=0;
//OnCardPanel2->Top=0;
//OnCardPanel3->Top=0;
//OnCardPanel4->Top=0;
//OnCardPanel5->Top=0;
Memo2->Visible=false;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::VirtualImage16Click(TObject *Sender)
{ ShellExecute(0,L"open",L"https://www.videolan.org/vlc/download-windows.html", NULL, NULL, SW_NORMAL);


}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage14Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.videolan.org/vlc/index.ru.html", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage20Click(TObject *Sender)
{ ShellExecute(0,L"open",L"https://www.faststone.org/FSIVDownload.htm", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage18Click(TObject *Sender)
{
  ShellExecute(0,L"open",L"https://www.faststone.org/", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage21Click(TObject *Sender)
{ ShellExecute(0,L"open",L"https://codecguide.com/download_kl.htm", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage28Click(TObject *Sender)
{
  ShellExecute(0,L"open",L"https://www.irfanview.net/lang/irfanview_lang_russian.exe", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage26Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.irfanview.com/", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn7Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"http://ulap.top/litsenzii/office/614-office-2021-pro-plus", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn8Click(TObject *Sender)
{
ShellExecute(0,L"open",L"http://ulap.top/litsenzii/soft/308-gdrive", NULL, NULL, SW_NORMAL);


}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage30Click(TObject *Sender)
{ ShellExecute(0,L"open",L"https://www.google.com/intl/ru_uA/drive/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage73Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.tracker-software.com/product/pdf-xchange-viewer", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage75Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.tracker-software.com/product/pdf-xchange-viewer", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ScrollBar1Change(TObject *Sender)
{
//po1=ScrollBar1->Position;
if(CardPanel1->ActiveCard==Card1) {
		if(po1>po) OnCardPanel1->Top-=5;
			else   OnCardPanel1->Top+=5;
}
/*if(CardPanel1->ActiveCard==Card2) {
		if(po1>po) OnCardPanel2->Top-=5;
			else   OnCardPanel2->Top+=5;
}
if(CardPanel1->ActiveCard==Card3) {
		if(po1>po) OnCardPanel3->Top-=5;
			else   OnCardPanel3->Top+=5;
}
if(CardPanel1->ActiveCard==Card4) {
		if(po1>po) OnCardPanel4->Top-=5;
			else   OnCardPanel4->Top+=5;
}
if(CardPanel1->ActiveCard==Card5) {
		if(po1>po) OnCardPanel5->Top-=5;
			else   OnCardPanel5->Top+=5;
} */
po=po1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage3Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=zqjMiXb2EhQ", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage2Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.microsoft.com/ru-ru/microsoft-365/explore-microsoft-365-for-home", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage74Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=Uwpx7cR2YfQ", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage31Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=o1DtF_mHCn0", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage12Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://zoom-us-zoom.ru.uptodown.com/windows/download", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage10Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.zoom.us/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage8Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://libreoffice.ru.uptodown.com/windows", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage37Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.google.com/intl/ru/chrome/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage42Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.opera.com/ru", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage45Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://filezilla-project.org/download.php?type=client", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage44Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=VQdyphkSLCw&list=PL5Ps-y0uPjuO9qpIMEbG-eUk8RUrK06rY&index=3", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage46Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://notepad.ru.uptodown.com/windows", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage48Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://notepad-plus-plus.org/downloads/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage78Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.win-rar.com/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage49Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://aflink.ru/g/69m495sz3jb46ad4edae729e2d2c2b/?i=3", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage50Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=gEcuMrIHhpM", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage54Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=JH76NFlHsA8", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage53Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://ru.itopvpn.com/screen-recorder", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage55Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://drive.google.com/file/d/1cTKJizFox_36XLtJcoJWymhheW-0toFe/view", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage58Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=HO58b2K-3XY", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage57Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.audacityteam.org/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage59Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.audacityteam.org/download/windows/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage62Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=VAwi1-DLzu4", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage63Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://download.gimp.org/gimp/help/windows/2.10/gimp-help-2.10.0-ru-setup.exe", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage61Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.gimp.org/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage67Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.videosoftdev.com/ru/free-video-converter/download", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::PageControl1Change(TObject *Sender)
{
BitBtn12->Visible=false;
if(PageControl1->ActivePage==TabSheet2)
	{
		CardPanel1->ActiveCard=Card1;
		Memo2->Visible=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage72Click(TObject *Sender)
{   ShellExecute(0,L"open",L"https://www.viber.com/ru/download/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage71Click(TObject *Sender)
{     ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=l4dAeNsckrc", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage70Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://desktop.telegram.org/?setln=ru", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BitBtn13Click(TObject *Sender)
{

CardPanel3->Visible=false;
PageControl1->Visible=false;
PageControl2->Visible=false;
Panel3->Visible=true;
BitBtn12->Visible=false;

if(wv!="Windows 11") ShowMessage("Некоторые твики могут не работать на вашей системе, если у вас нет последних обновлений");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn15Click(TObject *Sender)
{ //Analis of reg. settings
//TRegistry *reg1 = new TRegistry(KEY_ALL_ACCESS);
ToggleSwitch1->Enabled=true;
ToggleSwitch2->Enabled=true;
ToggleSwitch3->Enabled=true;
ToggleSwitch4->Enabled=true;
ToggleSwitch5->Enabled=true;
ToggleSwitch6->Enabled=true;
ToggleSwitch7->Enabled=true;
ToggleSwitch8->Enabled=true;
ToggleSwitch9->Enabled=true;
ToggleSwitch10->Enabled=true;
ToggleSwitch11->Enabled=true;
ToggleSwitch12->Enabled=true;
ToggleSwitch13->Enabled=true;
ToggleSwitch14->Enabled=true;
ToggleSwitch15->Enabled=true;
ToggleSwitch16->Enabled=true;
ToggleSwitch17->Enabled=true;
ToggleSwitch18->Enabled=true;
ToggleSwitch19->Enabled=true;
ToggleSwitch20->Enabled=true;
ToggleSwitch21->Enabled=true;
ToggleSwitch22->Enabled=true;
ToggleSwitch23->Enabled=true;
ToggleSwitch24->Enabled=true;
ToggleSwitch25->Enabled=true;
ToggleSwitch26->Enabled=true;
//ToggleSwitch27->Enabled=true;
ToggleSwitch28->Enabled=true;
//ToggleSwitch29->Enabled=true;
//ToggleSwitch30->Enabled=true;
ToggleSwitch31->Enabled=true;
ToggleSwitch32->Enabled=true;
ToggleSwitch33->Enabled=true;
try {
reg2->RootKey=HKEY_CURRENT_USER;          //отображение превьешек на панели задач
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);
if(reg2->ValueExists("ExtendedUIHoverTime"))
	{
	  if(reg2->ReadInteger("ExtendedUIHoverTime")!=1) ToggleSwitch1->State=tssOff;
		else ToggleSwitch1->State=tssOn;
	}
	else ToggleSwitch1->State=tssOff;
reg2->CloseKey();

   //MAX CACHE ICONS
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",true);
if(reg2->ValueExists("Max Cached Icons")) s=reg2->ReadString("Max Cached Icons");
	else s="0";
int ri=StrToInt(s);
if(ri>1000) ToggleSwitch2->State=tssOn;
	else ToggleSwitch2->State=tssOff;

 reg2->CloseKey();
// Быстрее обозначать процесс как зависший
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Control Panel\\Desktop",true);
if(reg2->ValueExists("HungAppTimeout")) s=reg2->ReadString("HungAppTimeout");
	else s="10000";
i=StrToInt(s);
if(i<3500) ToggleSwitch3->State=tssOn;
	else ToggleSwitch3->State=tssOff;
 reg2->CloseKey();
// Автоматически закрывать зависшее приложение
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Control Panel\\Desktop",true);
if(reg2->ValueExists("AutoEndTasks")) s=reg2->ReadString("AutoEndTasks"); else s="7";
i=StrToInt(s);
if(i==1) ToggleSwitch4->State=tssOn;
	else ToggleSwitch4->State=tssOff;
 reg2->CloseKey();
// Отключить функцию очистки файла подкачки Виндоус, для ускорения завершения работы компьютера

if(CheckRR(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management")==0) {
//ShowMessage("Это полуавтоматическая версия программы. Вручную поменяйте владельца следующего ключа в редакторе реестра на Администратора и дайте ему права на запись: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management. Затем повторите попытку настройки");
//ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
//Sleep(500);
//EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management [1 5 8 17]");
}
if(CheckRR(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management")==1) {

	try {
	reg2->RootKey=HKEY_LOCAL_MACHINE;
	reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management",true);
	i=reg2->ReadInteger("ClearPageFileAtShutdown");
	if(i==0) ToggleSwitch5->State=tssOn;
		else ToggleSwitch5->State=tssOff;
	 reg2->CloseKey();
	} catch (...) {  reg2->CloseKey();  ShowMessage("Ошибка чтения 1105");
	};
} else {ShowMessage("Не удалось изменить права доступа к HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management");}

// Пониженный приоритет для запуска автозагрузки         SUBINACL /keyreg "HKEY_LOCAL_MACHINE\SOFTWARE\Mozilla" /setowner=Администраторы /grant=Администраторы=f
//if(CheckRR(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps")==0) {
//ShowMessage("Это полуавтоматическая версия программы. Вручную поменяйте владельца следующего ключа в редакторе реестра на Администратора и дайте ему права на запись: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps. Затем повторите попытку настройки");

//ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
//Sleep(500);
//EditRegistryRight("\\Registry\\machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps [1 5 8 17]");
//}
/*if(CheckRR(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps")==1) {
	try {
	reg2->RootKey=HKEY_LOCAL_MACHINE;
	reg2->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps",true);
	i=reg2->ReadInteger("Delay_Sec");
	if(i!=0) ToggleSwitch6->State=tssOn;
		else ToggleSwitch6->State=tssOff;
	 reg2->CloseKey();
	}  catch (...) {  reg2->CloseKey(); ShowMessage("Ошибка чтения 1106");   }
} else  {ShowMessage("Не удалось изменить права доступа к HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps");}
  */
 // Перезагрузка откл. при экране смерти
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\CrashControl",true);
i=reg2->ReadInteger("AutoReboot");
if(i!=0) ToggleSwitch7->State=tssOff;
	else ToggleSwitch7->State=tssOn;
 reg2->CloseKey();

 // Отключить изменения в реестре с удаленного компьютера
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\RemoteRegistry",true);
i=reg2->ReadInteger("Start");
if(i==4) ToggleSwitch8->State=tssOn;
	else ToggleSwitch8->State=tssOff;
 reg2->CloseKey();
 // Дополнительный процесс проводника
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);
 if(reg2->ValueExists("SeparateProcess")) {
i=reg2->ReadInteger("SeparateProcess");
if(i==1) ToggleSwitch9->State=tssOn;
	else ToggleSwitch9->State=tssOff;
 } else ToggleSwitch9->Enabled=false;
 reg2->CloseKey();
// Отключить отладчик
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug",true);
 if(reg2->ValueExists("Auto")) {
s=reg2->ReadString("Auto");
if(s=="0") ToggleSwitch10->State=tssOn;
	else ToggleSwitch10->State=tssOff;
 } else ToggleSwitch10->Enabled=false;
 reg2->CloseKey();
 // Отключать службы через 1 сек при выкл
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control",true);
 if(reg2->ValueExists("WaitToKillServiceTimeout")) {
s=reg2->ReadString("WaitToKillServiceTimeout");
if(s<1001) ToggleSwitch32->State=tssOn;
	else ToggleSwitch32->State=tssOff;
 } else ToggleSwitch32->State=tssOff;
 reg2->CloseKey();

 //---------------------------------------------------

 //------ СТРАНИЦА ОПТИМИЗАЦИЯ СЕТЕЙ -----------------
try {
 //---------авто обновление------------------------------------------
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate\\AU",true);
 if(reg2->ValueExists("NoAutoUpdate")) {
i=reg2->ReadInteger("NoAutoUpdate");
if(i==1) ToggleSwitch11->State=tssOn;
	else ToggleSwitch11->State=tssOff;
 }
 reg2->CloseKey();
//-----------   TTL оптимизация ------
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters",true);
 if(reg2->ValueExists("MaxCacheTtl")) {
i=reg2->ReadInteger("MaxCacheTtl");
if(i>10000) ToggleSwitch12->State=tssOn;
	else ToggleSwitch12->State=tssOff;
 }
 reg2->CloseKey();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters",true);
 if(reg2->ValueExists("maxcacheentryttllimit")) {
i=reg2->ReadInteger("maxcacheentryttllimit");
if(i<400) ToggleSwitch12->State=tssOn;
	else ToggleSwitch12->State=tssOff;
 }
 reg2->CloseKey();

//----------------  NEGATIVE
 reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters",true);
 if(reg2->ValueExists("MaxNegativeCacheTtl")) {
i=reg2->ReadInteger("MaxNegativeCacheTtl");
if(i==0) ToggleSwitch13->State=tssOn;
	else ToggleSwitch13->State=tssOff;
 }
 reg2->CloseKey();

 // ---------------   ХЄШ ТАБЛИЦА
  reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters",true);
 if(reg2->ValueExists("CacheHashTableBucketSize")) {
i=reg2->ReadInteger("CacheHashTableBucketSize");
if(i==384) ToggleSwitch15->State=tssOn;
	else ToggleSwitch15->State=tssOff;
 }
 reg2->CloseKey();

//--------------   MTU DETECT
  reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",true);
 if(reg2->ValueExists("EnablePMTUBHDetect")) {
i=reg2->ReadInteger("EnablePMTUBHDetect");
if(i==1) ToggleSwitch16->State=tssOn;
	else ToggleSwitch16->State=tssOff;
 }
 reg2->CloseKey();

//-------- SSD
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Power",true);
 if(reg2->ValueExists("HibernateEnabled"))
i=reg2->ReadInteger("HibernateEnabled"); else i=7777;
if(i==0) ToggleSwitch17->State=tssOn;
	else ToggleSwitch17->State=tssOff;

 reg2->CloseKey();

 reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\\PrefetchParameters",true);
 if(reg2->ValueExists("EnablePrefetcher"))
i=reg2->ReadInteger("EnablePrefetcher"); else i=7777;
if(i==0) ToggleSwitch18->State=tssOn;
	else ToggleSwitch18->State=tssOff;
 reg2->CloseKey();
//------------------------------
//--------  ВНЕШНИЙ ВИД --------------

//----ВАНДРАЙВ
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("CLSID\\{018D5C66-4533-4307-9B53-224DE2ED1FE6}",true);
 if(reg2->ValueExists("System.IsPinnedToNameSpaceTree"))
i=reg2->ReadInteger("System.IsPinnedToNameSpaceTree"); else i=7777;
if(i==0) ToggleSwitch24->State=tssOn;
	else ToggleSwitch24->State=tssOff;
 reg2->CloseKey();
//----Удалить корзину
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\ClassicStartMenu",true);
 if(reg2->ValueExists("{645FF040-5081-101B-9F08-00AA002F954E}"))
 { i=reg2->ReadInteger("{645FF040-5081-101B-9F08-00AA002F954E}");
   if (i==0) {
	ToggleSwitch25->State=tssOff;
	} else ToggleSwitch25->State=tssOn;
 } else ToggleSwitch25->State=tssOff;
 reg2->CloseKey();
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel",true);
 if(reg2->ValueExists("{645FF040-5081-101B-9F08-00AA002F954E}"))
 { i=reg2->ReadInteger("{645FF040-5081-101B-9F08-00AA002F954E}");
   if (i==0) {
	ToggleSwitch25->State=tssOff;
	} else ToggleSwitch25->State=tssOn;
 } else ToggleSwitch25->State=tssOff;
 reg2->CloseKey();

 //----корзина в проводнике
try {
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}",true);
 if(reg2->ValueExists("System.IsPinnedToNameSpaceTree"))
i=reg2->ReadInteger("System.IsPinnedToNameSpaceTree"); else i=7777;
if(i==1) ToggleSwitch33->State=tssOn;
	else ToggleSwitch33->State=tssOff;
 reg2->CloseKey();

} catch (...) {       // \\Registry\\machine\\software\\classes
try{
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}",true);
 if(reg2->ValueExists("System.IsPinnedToNameSpaceTree"))
i=reg2->ReadInteger("System.IsPinnedToNameSpaceTree"); else i=7777;
if(i==1) ToggleSwitch33->State=tssOn;
	else ToggleSwitch33->State=tssOff;
 reg2->CloseKey();
} catch(...) {
	 EditRegistryRight("\\Registry\\machine\\software\\classes\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E} [1 5 8 17]");
	 EditRegistryRight("HKEY_CLASSES_ROOT\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E} [1 5 8 17]");
   try{
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}",true);
 if(reg2->ValueExists("System.IsPinnedToNameSpaceTree"))
i=reg2->ReadInteger("System.IsPinnedToNameSpaceTree"); else i=7777;
if(i==1) ToggleSwitch33->State=tssOn;
	else ToggleSwitch33->State=tssOff;
 reg2->CloseKey();
} catch(...) { ShowMessage("Ошибка чтения 1133");}
}
}
//----Добавить этот компьютер
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\ClassicStartMenu",true);
 if(reg2->ValueExists("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"))
 { i=reg2->ReadInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}");
   if (i==1) {
	ToggleSwitch26->State=tssOff;
	} else ToggleSwitch26->State=tssOn;
 } else ToggleSwitch26->State=tssOff;
 reg2->CloseKey();
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel",true);
 if(reg2->ValueExists("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"))
 { i=reg2->ReadInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}");
   if (i==1) {
	ToggleSwitch26->State=tssOff;
	} else ToggleSwitch26->State=tssOn;
 } else ToggleSwitch26->State=tssOff;
 reg2->CloseKey();

} catch (...) {ShowMessage("Ошибка считывания состояния оптимизации сетей и ssd");
}
//Центр уведомлений
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Policies\\Microsoft\\Windows\\Explorer",true);
if(reg2->ValueExists("DisableNotificationCenter"))
i=reg2->ReadInteger("DisableNotificationCenter"); else i=7777;
if(i==0 or i==7777) ToggleSwitch28->State=tssOff;
	else ToggleSwitch28->State=tssOn;
 reg2->CloseKey();
//Блокировка экрана
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows",true);
if(reg2->ValueExists("NoLockScreen"))
i=reg2->ReadInteger("NoLockScreen"); else i=7777;
if(i==0 or i==7777) ToggleSwitch31->State=tssOff;
	else ToggleSwitch31->State=tssOn;
 reg2->CloseKey();

//МИНИ панели задач
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);
if(reg2->ValueExists("TaskbarSi"))
i=reg2->ReadInteger("TaskbarSi"); else i=7777;
if(i!=0) ToggleSwitch20->State=tssOff;
	else ToggleSwitch20->State=tssOn;
 reg2->CloseKey();
// OLD context menu
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}",true);
if(reg2->KeyExists("InprocServer32"))  ToggleSwitch21->State=tssOn;
else ToggleSwitch21->State=tssOff;
reg2->CloseKey();
 // Копировать в
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("AllFilesystemObjects\\shellex\\ContextMenuHandlers",true);
if(reg2->KeyExists("{C2FBB630-2971-11D1-A18C-00C04FD75D13}"))  ToggleSwitch22->State=tssOn;
else ToggleSwitch22->State=tssOff;
reg2->CloseKey();
 // Переместить в
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("AllFilesystemObjects\\shellex\\ContextMenuHandlers",true);
if(reg2->KeyExists("{C2FBB631-2971-11D1-A18C-00C04FD75D13}"))  ToggleSwitch23->State=tssOn;
else ToggleSwitch23->State=tssOff;
reg2->CloseKey();

// =====================================================================
// РЕДАКТОР КОНТЕКСТНОГО МЕНЮ - считываем
//----------------------------------------------------------------------
// Computer\HKEY_CLASSES_ROOT\Directory\Background\shell - name - command
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("Directory\\Background\\shell",true);
reg2->GetKeyNames(ListBox1->Items);
reg2->CloseKey();

} catch (...) { ShowMessage("Во время анализа произошли ошибки");}


// корректировка кнопок после анализа реестра
Label85->Visible=true;
BitBtn14->Visible=false;
Button1->Visible=true;

analis=1; //анализ выполнен
}
//------------------------------------------------------------
//-----------------------------------------------------------
// ------------       КНОПКИ ------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void __fastcall TForm1::ToggleSwitch2Click(TObject *Sender)
{
if(analis==1){
  reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",true);
if(reg2->ValueExists("Max Cached Icons")) s=reg2->ReadString("Max Cached Icons");
	else s="0";
i=StrToInt(s);
if(i<1000)
	{//УВЕЛИЧЕНИЕ КЭША ЗНАЧКОВ
	reg2->WriteString("Max Cached Icons","4096");
	ToggleSwitch2->State=tssOn;
	}
else {
 reg2->WriteString("Max Cached Icons","512");
 ToggleSwitch2->State=tssOff;
	 }
reg2->CloseKey();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch1Click(TObject *Sender)
{
if(analis==1){
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);
if(reg2->ValueExists("ExtendedUIHoverTime")) i=reg2->ReadInteger("ExtendedUIHoverTime");
	else i=100;

if(i!=1)
	{//отображение превьешек на панели задач
	reg2->WriteInteger("ExtendedUIHoverTime",1);
	ToggleSwitch1->State=tssOn;
	}
else {
 reg2->WriteInteger("ExtendedUIHoverTime",0);
 ToggleSwitch1->State=tssOff;
	 }
reg2->CloseKey();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch3Click(TObject *Sender)
{
if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Control Panel\\Desktop",true);
if(reg2->ValueExists("HungAppTimeout")) s=reg2->ReadString("HungAppTimeout");
	else s="10000";
i=StrToInt(s);
if(i>3000)
	{//Уменьшение времени ожидания ответа приложений, при его зависании
	reg2->WriteString("HungAppTimeout","3000");
	reg2->WriteString("WaitToKillAppTimeout","3000");
	ToggleSwitch3->State=tssOn;
	}
else {
 reg2->WriteString("HungAppTimeout","10000");
 reg2->WriteString("WaitToKillAppTimeout","15000");
 ToggleSwitch3->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1003");  }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch4Click(TObject *Sender)
{
if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Control Panel\\Desktop",true);
if(reg2->ValueExists("AutoEndTasks")) s=reg2->ReadString("AutoEndTasks");
	else s="1";
i=StrToInt(s);
if(i==2)
	{//Автоматически (без спроса) закрывать зависший процесс , 1-автомат, 2-спрашивать
	reg2->WriteString("AutoEndTasks","1");
	ToggleSwitch4->State=tssOn;
	}
else {
 reg2->WriteString("AutoEndTasks","2");
 ToggleSwitch4->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1004");  }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch5Click(TObject *Sender)
{
if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management",true);
i=reg2->ReadInteger("ClearPageFileAtShutdown");
if(i!=0)
	{//Отключить функцию очистки файла подкачки Виндоус, для ускорения завершения работы компьютера
	reg2->WriteInteger("ClearPageFileAtShutdown",0);
	ToggleSwitch5->State=tssOn;
	}
else {
 reg2->WriteInteger("ClearPageFileAtShutdown",1);
 ToggleSwitch5->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1005");  } reg2->CloseKey();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch6Click(TObject *Sender)
{
//if(analis==1){ try {
/*reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\DelayedApps",true);
if(reg2->ValueExists("Delay_Sec")) i=reg2->ReadInteger("Delay_Sec");
	else i=0;
if(i==0)
	{
	reg2->WriteInteger("Delay_Sec",120);
	ToggleSwitch6->State=tssOn;
	}
else {
 reg2->WriteInteger("Delay_Sec",0);
 ToggleSwitch6->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1006");  }  reg2->CloseKey();
} */
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch7Click(TObject *Sender)
{
if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\CrashControl",true);
if (reg2->ValueExists("AutoReboot")) i=reg2->ReadInteger("AutoReboot"); else i=7777;
if(i!=0)
	{//авто перезагрузка при синем экране
	reg2->WriteInteger("AutoReboot",0);
	ToggleSwitch7->State=tssOn;
	}
else {
 reg2->WriteInteger("AutoReboot",1);
 ToggleSwitch7->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1007");  } reg2->CloseKey();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch8Click(TObject *Sender)
{
if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\RemoteRegistry",true);
i=reg2->ReadInteger("Start");
if(i!=4)
	{//Отключить удаленное изменение реестра
	reg2->WriteInteger("Start",4);
	ToggleSwitch8->State=tssOn;
	}
else {
 reg2->WriteInteger("Start",3);
 ToggleSwitch8->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1008");  } reg2->CloseKey();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch9Click(TObject *Sender)
{
if(ToggleSwitch9->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);
i=reg2->ReadInteger("SeparateProcess");
if(i==0)
	{//Разделить процессы проводника на 2
	reg2->WriteInteger("SeparateProcess",1);
	ToggleSwitch9->State=tssOn;
	}
else {
 reg2->WriteInteger("SeparateProcess",0);
 ToggleSwitch9->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1009");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch10Click(TObject *Sender)
{
if(ToggleSwitch10->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug",true);
s=reg2->ReadString("Auto");
if(s!="0")
	{//Отключить отладчик ошибок процессов
	reg2->WriteString("Auto","0");
	ToggleSwitch10->State=tssOn;
	}
else {
 reg2->WriteString("Auto","1");
 ToggleSwitch10->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1010");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxEx2Change(TObject *Sender)
{
if(ComboBoxEx2->ItemIndex==0) CardPanel2->ActiveCard=Card6;
if(ComboBoxEx2->ItemIndex==1) CardPanel2->ActiveCard=Card7;
if(ComboBoxEx2->ItemIndex==2) CardPanel2->ActiveCard=Card8;
if(ComboBoxEx2->ItemIndex==3) CardPanel2->ActiveCard=Card9;


}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch11Click(TObject *Sender)
{
if(ToggleSwitch11->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate\\AU",true);
if (reg2->ValueExists("NoAutoUpdate")) i=reg2->ReadInteger("NoAutoUpdate");
if(i!=1)
	{//Отключить автообновления
	reg2->WriteInteger("NoAutoUpdate",1);
	ToggleSwitch11->State=tssOn;
	}
else {
 reg2->WriteInteger("NoAutoUpdate",0);
 ToggleSwitch11->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1011");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch12Click(TObject *Sender)
{
if(ToggleSwitch12->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters",true);
if (reg2->ValueExists("MaxCacheTtl")) i=reg2->ReadInteger("MaxCacheTtl"); else i=7777;
if(i<10000 or i==7777)
	{//Расширить кєш ттл
	reg2->WriteInteger("MaxCacheTtl",14400);
	reg2->WriteInteger("MaxCacheEntryTtlLimit",301);
	reg2->WriteInteger("MaxSOACacheEntryTtlLimit",300);
	ToggleSwitch12->State=tssOn;
	}
else {
 reg2->WriteInteger("MaxCacheTtl",6000);
 reg2->WriteInteger("MaxCacheEntryTtlLimit",100);
 ToggleSwitch12->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1012");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch13Click(TObject *Sender)
{
if(ToggleSwitch13->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters",true);
if (reg2->ValueExists("MaxNegativeCacheTtl")) i=reg2->ReadInteger("MaxNegativeCacheTtl"); else i=7777;
if(i!=0)
	{//не хранить кєш ошибок
	reg2->WriteInteger("MaxNegativeCacheTtl",0);
	reg2->WriteInteger("NegativeSOACacheTime",0);
	reg2->WriteInteger("NetFailureCacheTime",0);
	ToggleSwitch13->State=tssOn;
	}
else {
 reg2->WriteInteger("MaxNegativeCacheTtl",900);
	reg2->WriteInteger("NegativeSOACacheTime",900);
	reg2->WriteInteger("NetFailureCacheTime",900);
 ToggleSwitch13->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1013");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch14Click(TObject *Sender)
{
ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c ipconfig /flushdns").c_str(), 0, SW_SHOWNORMAL);
ToggleSwitch14->State=tssOn;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch15Click(TObject *Sender)
{
if(ToggleSwitch15->Enabled==true) {
  if(analis==1){
  if(CheckRR(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters")==0)
   {
 // ShowMessage("Это полуавтоматическая версия программы. Вручную поменяйте владельца следующего ключа в редакторе реестра на Администратора и дайте ему права на запись: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters. Затем повторите попытку настройки");
   ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
Sleep(500);
EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters [1 5 8 17]");
   }
   else{

  try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
i=0;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Parameters",true);
if (reg2->ValueExists("CacheHashTableBucketSize")) i=reg2->ReadInteger("CacheHashTableBucketSize"); else i=7777;
if(i!=384)
	{//хеш таблиц
	reg2->WriteInteger("CacheHashTableBucketSize",384);
	reg2->WriteInteger("CacheHashTableSize",6400);
	ToggleSwitch15->State=tssOn;
	}
else {
 if (reg2->ValueExists("CacheHashTableBucketSize")) reg2->DeleteValue("CacheHashTableBucketSize");
 if (reg2->ValueExists("CacheHashTableSize")) reg2->DeleteValue("CacheHashTableSize");
 ToggleSwitch15->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1015");  }
}
  }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch16Click(TObject *Sender)
{
if(ToggleSwitch16->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\tcpip\\parameters",true);
if (reg2->ValueExists("EnablePMTUBHDetect"))
i=reg2->ReadInteger("EnablePMTUBHDetect"); else i=7777;
if(i!=1)
	{//черные дыры
	reg2->WriteInteger("EnablePMTUBHDetect",1);
	ToggleSwitch16->State=tssOn;
	}
else {
  reg2->WriteInteger("EnablePMTUBHDetect",0);
 ToggleSwitch16->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1016");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch17Click(TObject *Sender)
{
if(ToggleSwitch17->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Power",true);
if (reg2->ValueExists("HibernateEnabled"))
i=reg2->ReadInteger("HibernateEnabled"); else i=7777;
if(i!=0)
	{//откл гибернацию
	reg2->WriteInteger("HibernateEnabled",0);
	ToggleSwitch17->State=tssOn;
	}
else {
  reg2->WriteInteger("HibernateEnabled",1);
 ToggleSwitch17->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1017");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch18Click(TObject *Sender)
{
if(ToggleSwitch18->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\\PrefetchParameters",true);
if (reg2->ValueExists("EnablePrefetcher"))
i=reg2->ReadInteger("EnablePrefetcher"); else i=7777;
if(i!=0)
	{//superfetch
	reg2->WriteInteger("EnablePrefetcher",0);
	ToggleSwitch18->State=tssOn;
	}
else {
  reg2->WriteInteger("EnablePrefetcher",1);
 ToggleSwitch18->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1018");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch19Click(TObject *Sender)
{
if(ToggleSwitch19->Enabled==true) {
  if(analis==1){ try {

if(trim==0)
	{//включить трим
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c fsutil behavior set disabledeletenotify NTFS 0 && fsutil behavior set disabledeletenotify ReFS 0").c_str(), 0, SW_SHOWNORMAL);
	trim=1;
	ToggleSwitch19->State=tssOn;
	ShowMessage("Команда TRIM включена. Чтобы отключить, нажмите еще раз.");
	}
else {
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c fsutil behavior set disabledeletenotify NTFS 1 && fsutil behavior set disabledeletenotify ReFS 1").c_str(), 0, SW_SHOWNORMAL);
	trim=0;
	ToggleSwitch19->State=tssOff;
	ShowMessage("Команда TRIM выключена. Чтобы включить, нажмите еще раз.");
	 }
} catch (...) { ShowMessage("Ошибка при работе с реестром №1019");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch32Click(TObject *Sender)
{
 if(ToggleSwitch32->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Control",true);
if (reg2->ValueExists("WaitToKillServiceTimeout"))
s=reg2->ReadString("WaitToKillServiceTimeout"); else s="7777";
i=StrToInt(s);
if(i>1000)
	{//при выключении гасить зависшие службы
	reg2->WriteString("WaitToKillServiceTimeout","1000");
	ToggleSwitch32->State=tssOn;
	}
else {
  reg2->WriteString("WaitToKillServiceTimeout","5000");
 ToggleSwitch32->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) {	 ShowMessage("Ошибка при работе с реестром №1032"); }
}
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToggleSwitch24Click(TObject *Sender)
{
 if(ToggleSwitch24->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("CLSID\\{018D5C66-4533-4307-9B53-224DE2ED1FE6}",true);
if (reg2->ValueExists("System.IsPinnedToNameSpaceTree"))
i=reg2->ReadInteger("System.IsPinnedToNameSpaceTree"); else i=7777;
if(i!=0)
	{//не показывать ВанДрайв в проводнике
	reg2->WriteInteger("System.IsPinnedToNameSpaceTree",0);
	ToggleSwitch24->State=tssOn;
	}
else {
  reg2->WriteInteger("System.IsPinnedToNameSpaceTree",1);
 ToggleSwitch24->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) {	 ShowMessage("Ошибка при работе с реестром №1024"); }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch25Click(TObject *Sender)
{
if(ToggleSwitch25->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\ClassicStartMenu",true);
if (reg2->ValueExists("{645FF040-5081-101B-9F08-00AA002F954E}"))
	{//удалить корзину с рабочего стола
	i=reg2->ReadInteger("{645FF040-5081-101B-9F08-00AA002F954E}"); } else i=7777;
 if(i==0 or i==7777)
	{
	reg2->WriteInteger("{645FF040-5081-101B-9F08-00AA002F954E}",1);
	ToggleSwitch25->State=tssOn;
	}
else {
	reg2->WriteInteger("{645FF040-5081-101B-9F08-00AA002F954E}",0);
	ToggleSwitch25->State=tssOff;
	 }
reg2->CloseKey();
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel",true);
if (reg2->ValueExists("{645FF040-5081-101B-9F08-00AA002F954E}"))
	{//удалить корзину с рабочего стола
	i=reg2->ReadInteger("{645FF040-5081-101B-9F08-00AA002F954E}"); } else i=7777;
 if(i==0 or i==7777)
	{
	reg2->WriteInteger("{645FF040-5081-101B-9F08-00AA002F954E}",1);
	}
else {
	reg2->WriteInteger("{645FF040-5081-101B-9F08-00AA002F954E}",0);
	 }
reg2->CloseKey();
} catch (...) {	 ShowMessage("Ошибка при работе с реестром №1025"); }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch33Click(TObject *Sender)
{
if(ToggleSwitch33->Enabled==true) {
  if(analis==1){

  if(CheckRR(HKEY_CLASSES_ROOT,"CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}")==0)
   {
  // ShowMessage("Это полуавтоматическая версия программы. Вручную поменяйте владельца следующего ключа в редакторе реестра на Администратора и дайте ему права на запись: HKEY_CLASSES_ROOT\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}. Затем повторите попытку настройки");

   ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_CLASSES_ROOT\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
Sleep(500);
EditRegistryRight("\\Registry\\machine\\software\\classes\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E} [1 5 8 17]");
   }
  else {
  try{
  reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}",true);
if (reg2->ValueExists("System.IsPinnedToNameSpaceTree"))
i=reg2->ReadInteger("System.IsPinnedToNameSpaceTree"); else i=7777;
if(i==0 or i==7777)
	{//показывать корзину в проводнике
	reg2->WriteInteger("System.IsPinnedToNameSpaceTree",1);
	ToggleSwitch33->State=tssOn;
	}
else {
  reg2->WriteInteger("System.IsPinnedToNameSpaceTree",0);
 ToggleSwitch33->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) {	 ShowMessage("Ошибка при работе с реестром №1033"); }
}
  }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch26Click(TObject *Sender)
{
 if(ToggleSwitch26->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\ClassicStartMenu",true);
if (reg2->ValueExists("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"))
	{//єтот компьютер на рабочего стола
	i=reg2->ReadInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"); } else i=7777;
 if(i!=0)
	{
	reg2->WriteInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}",0);
	ToggleSwitch26->State=tssOn;
	}
else {
	reg2->WriteInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}",1);
	ToggleSwitch26->State=tssOff;
	 }
reg2->CloseKey();
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel",true);
if (reg2->ValueExists("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"))
	{
	i=reg2->ReadInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"); } else i=7777;
 if(i!=0)
	{
	reg2->WriteInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}",0);
	}
else {
	reg2->WriteInteger("{20D04FE0-3AEA-1069-A2D8-08002B30309D}",1);
	 }
reg2->CloseKey();
} catch (...) {	 ShowMessage("Ошибка при работе с реестром №1026"); }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch28Click(TObject *Sender)
{
if(ToggleSwitch28->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Policies\\Microsoft\\Windows\\Explorer",true);
if(reg2->ValueExists("DisableNotificationCenter"))
i=reg2->ReadInteger("DisableNotificationCenter"); else i=7777;
if(i==0 or i==7777)
	{//Отключить центр уведомлений
	reg2->WriteInteger("DisableNotificationCenter",1);
	ToggleSwitch28->State=tssOn;
	}
else {
 reg2->WriteInteger("DisableNotificationCenter",0);
 ToggleSwitch28->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1028");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch31Click(TObject *Sender)
{
if(ToggleSwitch31->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows",true);
if(reg2->ValueExists("NoLockScreen"))
i=reg2->ReadInteger("NoLockScreen"); else i=7777;
if(i==0 or i==7777)
	{//Отключить блокировку экрана
	reg2->WriteInteger("NoLockScreen",1);
	ToggleSwitch31->State=tssOn;
	}
else {
 reg2->WriteInteger("NoLockScreen",0);
 ToggleSwitch31->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1031");  }
}
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToggleSwitch20Click(TObject *Sender)
{
 if(ToggleSwitch20->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",true);
if(reg2->ValueExists("TaskbarSi"))
i=reg2->ReadInteger("TaskbarSi"); else i=7777;
if(i!=0)
	{//Мини панель задач
	reg2->WriteInteger("TaskbarSi",0);
	ToggleSwitch20->State=tssOn;
	}
else {
 reg2->WriteInteger("TaskbarSi",1);
 ToggleSwitch20->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1020");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch21Click(TObject *Sender)
{
if(ToggleSwitch21->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}",true);
if(reg2->KeyExists("InprocServer32")==false)
	{//старое меню
	reg2->CreateKey("InprocServer32");
	reg2->OpenKey("InprocServer32",true);
	//reg2->WriteString("(По умолчанию)","");
	//reg2->WriteString("(Default)","");
	reg2->WriteString("","");
	ToggleSwitch21->State=tssOn;
	}
else {
 reg2->DeleteKey("InprocServer32");
 ToggleSwitch21->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1021");  }
}
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToggleSwitch22Click(TObject *Sender)
{
 if(ToggleSwitch22->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("AllFilesystemObjects\\shellex\\ContextMenuHandlers",true);
if(reg2->KeyExists("{C2FBB630-2971-11D1-A18C-00C04FD75D13}")==false)
	{//копировать в
	reg2->CreateKey("{C2FBB630-2971-11D1-A18C-00C04FD75D13}");
	ToggleSwitch22->State=tssOn;
	}
else {
 reg2->DeleteKey("{C2FBB630-2971-11D1-A18C-00C04FD75D13}");
 ToggleSwitch22->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1022");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch23Click(TObject *Sender)
{
  if(ToggleSwitch23->Enabled==true) {
  if(analis==1){ try {
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("AllFilesystemObjects\\shellex\\ContextMenuHandlers",true);
if(reg2->KeyExists("{C2FBB631-2971-11D1-A18C-00C04FD75D13}")==false)
	{//переместить в
	reg2->CreateKey("{C2FBB631-2971-11D1-A18C-00C04FD75D13}");
	ToggleSwitch23->State=tssOn;
	}
else {
 reg2->DeleteKey("{C2FBB631-2971-11D1-A18C-00C04FD75D13}");
 ToggleSwitch23->State=tssOff;
	 }
reg2->CloseKey();
} catch (...) { ShowMessage("Ошибка при работе с реестром №1023");  }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{     // Computer\HKEY_CLASSES_ROOT\Directory\Background\shell - name - command  OpenDialog1->FileName
String name;
if (OpenDialog1->Execute())
{
name = InputBox(L"Ответьте на вопрос", L"Название для меню", L"");
try{
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("Directory\\Background\\shell",true);
reg2->CreateKey(name);
reg2->OpenKey(name,true);
reg2->CreateKey("command");
reg2->OpenKey("command",true);
reg2->WriteString("",""""+OpenDialog1->FileName+"""");
reg2->CloseKey();
ListBox1->Items->Add(name);
} catch(...) { ShowMessage("Не удалось добавить в меню"); }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
String name=ListBox1->Items->Strings[ListBox1->ItemIndex];
try{
if(name!="") {
reg2->RootKey=HKEY_CLASSES_ROOT;
reg2->OpenKey("Directory\\Background\\shell",true);
reg2->DeleteKey(name);
reg2->CloseKey();
ListBox1->Items->Delete(ListBox1->ItemIndex);
}
} catch(...) {ShowMessage("Не удалось удалить");};

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{           // ТЕЛЕМЕТРИЯ
int errcode=0;
if(Button4->Caption=="Заблокировать" || Button4->Caption=="Disable" ) {
   if(CheckRR(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\DiagTrack")==0) {
	//ShowMessage("Это полуавтоматическая версия программы. Вручную поменяйте владельца следующего ключа в редакторе реестра на Администратора и дайте ему права на запись: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\DiagTrack. Затем запретите системе права на запись и повторите попытку настройки");

	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\DiagTrack\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(1000);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\DiagTrack [1 8 19]");
	} else {
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\DiagTrack",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();
		} catch (...) {   ShowMessage("Ошибка! Не удалось отключить службу телеметрии"); errcode=1;}
	try{
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection",true);
		reg2->WriteInteger("AllowTelemetry",0);
		reg2->CloseKey();
		} catch(...) {ShowMessage("Ошибка записи в реестр Allow Telemetry"); errcode=1;};
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c sc config DiagTrack start= disabled").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c sc config dmwappushservice start= disabled").c_str(), 0, SW_SHOWNORMAL);

	if(Language=="EN") ShowMessage("So, I disabled the DiagTrack service, disabled telemetry in the registry, and disabled the dmwappushservice service. But you still need to manually go to OPTIONS -> Privacy & Security -> Windows Permissions subheading. Disable all monitoring and diagnostic functions in these settings. Then restart your PC.");
	else ShowMessage("Итак, я отключил службу DiagTrack, внес запрет на телеметрию в реестр и отключил службу dmwappushservice. Но тебе еще нужно вручную зайти в ПАРАМЕТРЫ -> Конфиденциальность и защита -> Подзаголовок Разрешения Windows. Поотключайте все функции наблюдения и диагностики в этих настройках. Затем перезагрузите ПК.");

	if(errcode==0) {
		if(Language=="EN") Label103->Caption="Disabled"; else Label103->Caption="Заблокировано";
		Label103->Font->Color=clRed;
		if(Language=="EN") Button4->Caption="Enable"; else Button4->Caption="Включить";

		}
	}
} else {

   //	ShowMessage("Это полуавтоматическая версия программы. Вручную верните разрешения на запись для системы в следующем ключе реестра: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\DiagTrack. И повторите попытку настройки");

	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\DiagTrack\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(1000);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\DiagTrack [1 5 8 17]");
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\DiagTrack",true);
		reg2->WriteInteger("Start",2);
		reg2->CloseKey();

		} catch (...) {   ShowMessage("Ошибка! Не удалось включить службу телеметрии"); errcode=1;}
	 try{
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection",true);
		reg2->DeleteValue("AllowTelemetry");
		reg2->CloseKey();
		} catch(...) {ShowMessage("Ошибка удаления Allow Telemetry"); errcode=1;};
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c sc config DiagTrack start= enabled").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c sc config dmwappushservice start= enabled").c_str(), 0, SW_SHOWNORMAL);
	if(Language=="EN") ShowMessage("OK, I've started the DiagTrack service and dmwappushservice. But you still need to manually go to SETTINGS -> Privacy and Security -> Windows Permissions subheading. Turn on the monitoring and diagnostic functions in these settings that you need. Then restart your PC.");
	else ShowMessage("ОК, я запустил службу DiagTrack и dmwappushservice. Но тебе еще нужно вручную зайти в ПАРАМЕТРЫ -> Конфиденциальность и защита -> Подзаголовок Разрешения Windows. Включите функции наблюдения и диагностики в этих настройках, которые вам нужны. Затем перезагрузите ПК.");
	if(errcode==0) {
		if(Language=="EN") Label103->Caption="Enabled"; else Label103->Caption="Работает";
		Label103->Font->Color=clLime;
		if(Language=="EN") Button4->Caption="Disable"; else Button4->Caption="Заблокировать";}
}	}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
if(Button5->Caption=="Начать блокировку" || Button5->Caption=="Start blocking") {
	 //Button5->Caption=="Продолжить блокировку";
	 N6Click(Sender);
	 }
 if(Button5->Caption=="Продолжить блокировку" || Button5->Caption=="Continue blocking") {
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WinDefend\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WinDefend [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Sense\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\Sense [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdNisSvc\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdNisSvc [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdNisDrv\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdNisDrv [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wscsvc\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\wscsvc [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdBoot\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdBoot [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdFilter\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdFilter [1 8 19]");
	Sleep(500);

	try {
	   /*	reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows Defender",true);
		reg2->WriteInteger("DisableAntiSpyware",1);
		reg2->WriteInteger("AllowFastServiceStartup",0);
		reg2->WriteInteger("ServiceKeepAlive",0);
		reg2->OpenKey("Real-Time Protection",true);
		reg2->WriteString("","");
		reg2->WriteInteger("DisableIOAVProtection",1);
		reg2->WriteInteger("DisableRealtimeMonitoring",1);
		reg2->CloseKey();
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet",true);
		reg2->WriteInteger("DisableBlockAtFirstSeen",1);
		reg2->WriteInteger("LocalSettingOverrideSpynetReporting",0);
		reg2->WriteInteger("SubmitSamplesConsent",2);
		reg2->CloseKey();
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Systray",true);
		reg2->WriteInteger("HideSystray",1);
		reg2->CloseKey();
		*/

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WinDefend",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Sense",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdNisSvc",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdNisDrv",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\wscsvc",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdBoot",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdFilter",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",true);
		reg2->DeleteValue("SecurityHealth");
		reg2->CloseKey();

		Label94->Caption="Заблокировано";
		Label94->Font->Color=clRed;
		N6Click(Sender);


		//Button5->Caption="Включить";
		//Form3->Show();
/*Sense — 3
WdBoot — 0
WdFilter — 0
WdNisDrv — 3
WdNisSvc — 3
WinDefend — 2
  */
	} catch (...) {   ShowMessage("Ошибка! Не удалось отключить антивирус");}


}


if(Button5->Caption=="Включить" || Button5->Caption=="Enable") {
if(safemode==1) {
	try {
	   /*	reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows Defender",true);
		reg2->WriteInteger("DisableAntiSpyware",0);
		reg2->WriteInteger("AllowFastServiceStartup",1);
		reg2->WriteInteger("ServiceKeepAlive",1);
		reg2->OpenKey("Real-Time Protection",true);
		reg2->WriteString("","");
		reg2->WriteInteger("DisableIOAVProtection",0);
		reg2->WriteInteger("DisableRealtimeMonitoring",0);
		reg2->CloseKey();
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet",true);
		reg2->WriteInteger("DisableBlockAtFirstSeen",0);
		reg2->WriteInteger("LocalSettingOverrideSpynetReporting",1);
		reg2->WriteInteger("SubmitSamplesConsent",0);
		reg2->CloseKey();
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Systray",true);
		reg2->WriteInteger("HideSystray",0);
		reg2->CloseKey();
		*/
		try{
		ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WinDefend\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WinDefend [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Sense\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\Sense [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdNisSvc\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdNisSvc [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdNisDrv\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdNisDrv [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wscsvc\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\wscsvc [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdBoot\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdBoot [1 8 19]");
	Sleep(500);
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\WdFilter\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(500);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\WdFilter [1 8 19]");
	Sleep(500);

			reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WinDefend",true);
		reg2->WriteInteger("Start",2);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\Sense",true);
		reg2->WriteInteger("Start",3);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdNisSvc",true);
		reg2->WriteInteger("Start",3);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdNisDrv",true);
		reg2->WriteInteger("Start",3);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\wscsvc",true);
		reg2->WriteInteger("Start",3);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdBoot",true);
		reg2->WriteInteger("Start",0);
		reg2->CloseKey();

		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WdFilter",true);
		reg2->WriteInteger("Start",0);
		reg2->CloseKey();


		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",true);
		reg2->WriteString("SecurityHealth","%windir%\\system32\\SecurityHealthSystray.exe");
		reg2->CloseKey();


		}   catch(...){
		if(Language=="EN") ShowMessage("Error. Try to switch to safe mode via top menu"); else
		ShowMessage("Ошибка включения антивируса/ Перейдите в безопасный режим");
		return;};
		if(Language=="EN") Label94->Caption="Enabled"; else Label94->Caption="Включено";
		Label94->Font->Color=clLime;

		N6Click(Sender);
		//Button5->Caption="Начать блокировку";
		//ShowMessage("Перезагрузите ПК");


	} catch (...) {   ShowMessage("Ошибка! Не удалось отключить антивирус");}
} else N6Click(Sender);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
//ToggleSwitch27Click(Sender);
//ToggleSwitch29Click(Sender);
//ToggleSwitch30Click(Sender);
//ToggleSwitch34Click(Sender);
//ToggleSwitch35Click(Sender);
//	if(Language=="EN") Label144->Caption="Disabled"; else 	Label144->Caption="Выключено";
//		Label144->Font->Color=clRed;

String s;
TIniFile *ServDef = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDef.psl");
int i1;
String svcname=ServGrid->Cells[0][sel];
try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\"+svcname,true);
if(reg2->ValueExists("Start")) {
i1=ServDef->ReadInteger("Def",svcname,2);
reg2->WriteInteger("Start",i1);
reg2->CloseKey();
} }catch (...) { ShowMessage("Error btn6");}
//if (Language=="EN")  ShowMessage("Restart the PC"); else ShowMessage("Перезагрузите ПК");

delete ServDef;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image3Click(TObject *Sender)
{
ShellExecute(0,L"open",Link1.w_str(), NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
if(con==1) {
try{
Image3->Picture->LoadFromFile("rek1.jpg");
Image4->Picture->LoadFromFile("rek2.jpg");
TStringList *l1=new TStringList;
l1->LoadFromFile("li1.txt");
Link1=l1->Strings[0];
l1->Clear();
l1->LoadFromFile("li2.txt");
Link2=l1->Strings[0];
delete(l1);
  } catch(...) {}
}   try{
  DeleteFile("rek1.jpg");
  DeleteFile("rek2.jpg");
  DeleteFile("li1.txt");
  DeleteFile("li2.txt");
} catch(...) {};


   Timer2->Enabled=true;
  Timer1->Enabled=false;

  }
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn9Click(TObject *Sender)
{
CardPanel3->Visible=false;
BitBtn12->Visible=false;
PageControl2->Visible=true;
PageControl2->ActivePage=DefTab;
Panel3->Visible=false;
PageControl1->Visible=false;
//==========ПРОВЕРКА АНТИВИРУСА========================================
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WinDefend",true);
try {
try{
i=reg2->ReadInteger("Start");

} catch(...) {i=4;}
reg2->CloseKey();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Policies\\Microsoft\\Windows Defender",true);
if(i==4)
{   //антивир отключен
	if(Language=="EN") Label94->Caption="Blocked"; else Label94->Caption="Заблокирован";
	Label94->Font->Color=clRed;
	if(Language=="EN") Button5->Caption="Turn on"; else Button5->Caption="Включить";
}
	else {
		if(Language=="EN") Label94->Caption="Active"; else Label94->Caption="Работает";
		Label94->Font->Color=clLime;
		if(safemode==0) { if(Language=="EN") Button5->Caption="Run the blocking"; else Button5->Caption="Начать блокировку"; }
			else { if(Language=="EN") Button5->Caption="Continue the blocking"; else Button5->Caption="Продолжить блокировку"; }
	}
} catch (...) {
	if(Language=="EN") Label94->Caption="Active"; Label94->Caption="Работает";
	Label94->Font->Color=clLime;
	if(safemode==0) { if(Language=="EN") Button5->Caption="Run the blocking"; else Button5->Caption="Начать блокировку"; }
			else { if(Language=="EN") Button5->Caption="Continue the blocking"; else Button5->Caption="Продолжить блокировку"; }
}
reg2->CloseKey();
 //==========ПРОВЕРКА обновлений========================================  wuauserv
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\wuauserv",true);
try {
if(reg2->ReadInteger("Start")==4)
{   //обновления работают
	if(Language=="EN") Label144->Caption="Disabled"; else 	Label144->Caption="Выключено";
	Label144->Font->Color=clRed;
	ToggleSwitch27->State=tssOff;
}
	else {
		if(Language=="EN") Label144->Caption="Enabled"; else 	Label144->Caption="Включено";
		Label144->Font->Color=clLime;
		ToggleSwitch27->State=tssOn;
	}
} catch (...) {
	if(Language=="EN") Label144->Caption="Enabled"; else 	Label144->Caption="Работает";
	Label144->Font->Color=clLime;
	ToggleSwitch27->State=tssOn;
}
reg2->CloseKey();
try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\DoSvc",true);
if(reg2->ReadInteger("Start")==4)  ToggleSwitch29->State=tssOff;
	else    ToggleSwitch29->State=tssOn;
reg2->CloseKey();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WaaSMedicSvc",true);
if(reg2->ReadInteger("Start")==4)  ToggleSwitch30->State=tssOff;
	else    ToggleSwitch30->State=tssOn;
reg2->CloseKey();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\UsoSvc",true);
if(reg2->ReadInteger("Start")==4)  ToggleSwitch34->State=tssOff;
	else    ToggleSwitch34->State=tssOn;
reg2->CloseKey();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\BITS",true);
if(reg2->ReadInteger("Start")==4)  ToggleSwitch35->State=tssOff;
	else    ToggleSwitch35->State=tssOn;
reg2->CloseKey();
	 upd=1;
	}   catch (...) {};

//==========ПРОВЕРКА телеметрии========================================
try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\DiagTrack",true);
		if(reg2->ReadInteger("Start")==4){
		if(Language=="EN") Label103->Caption="Disabled"; else Label103->Caption="Выключено";
		Label103->Font->Color=clRed;
		if(Language=="EN") Button4->Caption="Enable"; else Button4->Caption="Включить";
		} else {
		if(Language=="EN") Label103->Caption="Enabled"; else Label103->Caption="Работает";
		Label103->Font->Color=clLime;
		if(Language=="EN") Button4->Caption="Disable"; else Button4->Caption="Заблокировать";
		}
} catch (...) {   ShowMessage("Ошибка чтения параметров службы телеметрии");}
		reg2->CloseKey();
//==========ПРОВЕРКА гибернации========================================
if(FileExists("c:\\hiberfil.sys"))
{
if(Language=="EN") Label106->Caption="Enabled"; else Label106->Caption="Работает";
Label106->Font->Color=clLime;
if(Language=="EN") Button7->Caption="Disable"; else Button7->Caption="Заблокировать";
}
else {
	if(Language=="EN") Label106->Caption="Disabled"; else Label106->Caption="Выключено";
	Label106->Font->Color=clRed;
		if(Language=="EN") Button7->Caption="Enable"; else Button7->Caption="Включить";
}
 //==========ПРОВЕРКА SYSMAIN========================================  wuauserv
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\SysMain",true);
try {
if(reg2->ReadInteger("Start")==4)
{   //
	if(Language=="EN") Label110->Caption="Disabled"; else Label110->Caption="Выключено";
	Label110->Font->Color=clRed;
	if(Language=="EN") Button8->Caption="Enable"; else Button8->Caption="Включить";
}
	else {
		if(Language=="EN") Label110->Caption="Enabled"; else Label110->Caption="Работает";
		Label110->Font->Color=clLime;
		if(Language=="EN") Button8->Caption="Disable"; else Button8->Caption="Заблокировать";
	}
} catch (...) {
		if(Language=="EN") Label110->Caption="Enabled"; else Label110->Caption="Работает";
		Label110->Font->Color=clLime;
		if(Language=="EN") Button8->Caption="Disable"; else Button8->Caption="Заблокировать";
}
reg2->CloseKey();
 //==========ПРОВЕРКА WSEARCH========================================
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WSearch",true);
try {
if(reg2->ReadInteger("Start")==4)
{   //
	if(Language=="EN") Label126->Caption="Disabled"; else Label126->Caption="Выключено";
	Label126->Font->Color=clRed;
	if(Language=="EN") Button28->Caption="Enable"; else Button28->Caption="Включить";
}
	else {
		if(Language=="EN") Label126->Caption="Enabled"; else Label126->Caption="Работает";
		Label126->Font->Color=clLime;
		if(Language=="EN") Button28->Caption="Disable"; else Button28->Caption="Заблокировать";
	}
} catch (...) {
		if(Language=="EN") Label126->Caption="Enabled"; else Label126->Caption="Работает";
		Label126->Font->Color=clLime;
		if(Language=="EN") Button28->Caption="Disable"; else Button28->Caption="Заблокировать";
}
reg2->CloseKey();

// CHECKING EDGE

if(!DirectoryExists("C:\\Program Files (x86)\\Microsoft\\Edge",true))
{   //
	if(Language=="EN") Label141->Caption="Removed"; else Label141->Caption="Удален";
	Label141->Font->Color=clRed;
	if(Language=="EN") Button30->Caption="Install"; else Button30->Caption="Установить";
}
	else {
		if(Language=="EN") Label141->Caption="Working"; else Label141->Caption="Работает";
		Label141->Font->Color=clLime;
		if(Language=="EN") Button30->Caption="Remove"; else Button30->Caption="Удалить";
	}


}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
 if(Image3->Visible==true) {
	Image4->Visible=true;
	Image3->Visible=false;
	}
	else {
	Image3->Visible=true;
	Image4->Visible=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image4Click(TObject *Sender)
{
 ShellExecute(0,L"open",Link2.w_str(), NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
if(Button7->Caption=="Заблокировать" || Button7->Caption=="Disable") {
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c powercfg -h off").c_str(), 0, SW_SHOWNORMAL);
		if(Language=="EN") Label106->Caption="Disabled"; else Label106->Caption="Выключено";
		Label106->Font->Color=clRed;
		if(Language=="EN") Button7->Caption="Enable"; else Button7->Caption="Включить";
} else {
	ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c powercfg -h on").c_str(), 0, SW_SHOWNORMAL);
		if(Language=="EN") Label106->Caption="Enabled"; else Label106->Caption="Работает";
		Label106->Font->Color=clLime;
		if(Language=="EN") Button7->Caption="Disable"; else Button7->Caption="Заблокировать";
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualImage89Click(TObject *Sender)
{
if(winvibor=="11") {
	ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=eRD2mPngBqE", NULL, NULL, SW_NORMAL);
}   else {
	if(winvibor=="7") ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=LiBsj7nZ7qw", NULL, NULL, SW_NORMAL);
	else ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=igXSXthFn0A", NULL, NULL, SW_NORMAL);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{      // Блокировка службы SysMain
if(Button8->Caption=="Заблокировать" || Button8->Caption=="Disable") {
	//ShowMessage("Это полуавтоматическая версия программы. Вручную поменяйте владельца следующего ключа в редакторе реестра на Администратора и дайте ему права на запись: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wuauserv. Затем запретите системе права на запись и повторите попытку настройки");

	//ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wuauserv\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	//Sleep(1000);       // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\wuauserv
	//EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\wuauserv [1 8 19]");
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\SysMain",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();
		ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net stop SysMain").c_str(), 0, SW_SHOWNORMAL);
		if(Language=="EN") Label110->Caption="Disabled"; else  Label110->Caption="Заблокировано";
		Label110->Font->Color=clRed;
		if(Language=="EN") Button8->Caption="Enable"; else Button8->Caption="Включить";
		//ShowMessage("Перезагрузите ПК");
		} catch (...) {   ShowMessage("Ошибка! Не удалось отключить службу");}
} else {
	//ShowMessage("Это полуавтоматическая версия программы. Вручную поменяйте разрешения для системы в следующем ключе реестра и дайте ей права на запись: SYSTEM\\CurrentControlSet\\Services\\wuauserv. Затем повторите попытку настройки");
	//ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wuauserv\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	//Sleep(1000);       // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\wuauserv
	//EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\wuauserv [1 5 8 17]");
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\SysMain",true);
		reg2->WriteInteger("Start",2);
		reg2->CloseKey();
		if(Language=="EN") Label110->Caption="Enabled"; else  Label110->Caption="Работает";
		Label110->Font->Color=clLime;
		if(Language=="EN") Button8->Caption="Disable"; else Button8->Caption="Заблокировать";
		//ShowMessage("Перезагрузите ПК");
		} catch (...) {   ShowMessage("Ошибка! Не удалось включить службу");}
}
}
//---------------------------------------------------------------------------
void CleanFolder(String Folder, int &fCount)
{         // определение размера папки. Size - в байтах.
//ShowMessage(Folder + "\\*.*");
	TSearchRec SR;
	for (int i=1; i < Folder.Length(); i++) {
	   if (Folder.SubString(i,8)=="-journal") {DeleteFile(Folder); return;
       }
	}

	if (Folder[Folder.Length()] == '\\')
		Folder.SetLength(Folder.Length() - 1);
	if (FindFirst(Folder + "\\*.*", faAnyFile, SR) == 0)
		do
		{
			if (SR.Name != "." && SR.Name != "..")
			{
				if ((SR.Attr & faDirectory) != 0)
				{
				//ShowMessage(Folder + "\\" + SR.Name);
					CleanFolder(Folder + "\\" + SR.Name,fCount);
				}
				else
				{
				   //	ShowMessage(Folder + "\\" + SR.Name);
					DeleteFile(Folder + "\\" + SR.Name);
					fCount++;
				}

			}
		} while (FindNext(SR) == 0);
		FindClose(SR);
}
//--------------------------------------------------------
void __fastcall TForm1::Button9Click(TObject *Sender)
{ //Чистка выделенными командами
if(MessageBox(NULL, L"Вы уверены, что хотите удалить выбранные объекты?", L"Удаление кэша",  MB_YESNO) == IDYES)
{
int fCount=0;
list->Clear();
try{DeleteFile("comand.bat");} catch(...){};

int stopi;
int delnum=0;
String command="";
String elem;
for(int i=CleanBox->Items->Count-1;i>=0;i--)
{
	if (CleanBox->Checked[i])
	{
		elem=CleanBox->Items->Strings[i];
				for(int i=1;i<elem.Length();i++) {
					if(elem.SubString(i,2)==" (") stopi=i;
				}
			elem=elem.SubString(1,stopi-1);
			stopi=0;

	//ShowMessage(CleanBox->Items->Strings[i].SubString(1,17));
		if(elem.SubString(1,17)=="C:\\Windows\\WinSxS") {
		   list->Add("Dism.exe /online /cleanup-image /AnalyzeComponentStore");
			list->Add("Dism.exe /online /cleanup-image /StartComponentCleanup");
		} else
		if(CleanBox->Items->Strings[i]=="Удалить историю просмотров в проводнике") {
		   list->Add("Del /F /Q "+GetEnvironmentVariable("APPDATA")+"\\Microsoft\\Windows\\Recent\\*");
			//list->Add("Del /F /Q "+GetEnvironmentVariable("APPDATA")+"\\Microsoft\\Windows\\Recent\\AutomaticDestinations\\*";
			//list->Add("Del /F /Q "+GetEnvironmentVariable("APPDATA")+"\\Microsoft\\Windows\\Recent\\CustomDestinations\\*";
			list->Add("REG Delete HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU /VA /F");
			list->Add("REG Delete HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths /VA /F");
		 } else

		if(CleanBox->Items->Strings[i]=="Очистить кэш магазина Майкрософт") {
		//ShowMessage(CleanBox->Items->Strings[i]);
			 list->Add("wsreset");
		} else
		 if(elem.SubString(1,15)=="C:\\hiberfil.sys") {
		 //ShowMessage(CleanBox->Items->Strings[i]);
			 list->Add("powercfg -h off");

		} else {


			CleanFolder(tempnames->Strings[i],fCount);
				 //ShowMessage(elem);
				 //RemoveDir("C:/Users/coola/AppData/Local/Apps");
				 //command="Remove-Item -Recurse -Force -Path '"+elem+"'";
				 //ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_HIDE);
				 //Sleep(200);
		}





	CleanBox->Items->Delete(i);
	tempnames->Delete(i);

	delnum++;
	}
}
list->SaveToFile("comand.bat");
ShellExecute(Handle, L"open", L"comand.bat",0, 0, SW_HIDE);

Label115->Caption="Удаление проведено";
}

VirtualImage99->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage91Click(TObject *Sender)
{
if(Language=="EN") ShellExecute(0,L"open",L"https://www.youtube.com/@PCNPC", NULL, NULL, SW_NORMAL);
else ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=g9I0In7_Cc4", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage92Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=NqrCRrSfuTw", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage93Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/shorts/jnjwvVh9hPI", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage94Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=JiFQ5By2UWQ", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage95Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=Tg5WxOxM2qg", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
ShellExecute(Handle, L"open", L"cleanmgr.exe",String().sprintf(L"/d C:").c_str(), 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender)
{
String pathdir=GetEnvironmentVariable("Temp");
ShellExecute(Handle, L"open", pathdir.w_str(),0, 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
CleanBox->CheckAll(cbUnchecked, false, true);
// ShellExecute(Handle, L"open", L"temp",0, 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button12Click(TObject *Sender)
{
CleanBox->CheckAll(cbChecked, false, true);
// ShellExecute(Handle, L"open", L"C:\\Windows\\SoftwareDistribution\\Download",0, 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SearchBig(String spath)
{
TSearchRec SR;
int size;
if (spath[spath.Length()] == '\\')
		spath.SetLength(spath.Length() - 1);
if (FindFirst(spath + "\\*.*", faAnyFile, SR) == 0)
		do
		{
			if (SR.Name != "." && SR.Name != "..")
			{
				if ((SR.Attr & faDirectory) != 0)
				{
					  SearchBig(spath+ "\\" + SR.Name);
				}
				else
				{
				   if (SR.Size>500000000)
					  {
						StringGrid1->RowCount = StringGrid1->RowCount + 1;
						StringGrid1->Cells[0][StringGrid1->RowCount-1] = spath;
						StringGrid1->Cells[1][StringGrid1->RowCount-1] = SR.Name;
						if(SR.Size<1073741824) {
							size=SR.Size/1024/1024;
							StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Мб";
							} else {
							size=SR.Size/1024/1024/1024;
							StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Гб";
							}
						StringGrid1->Cells[3][StringGrid1->RowCount-1] = spath+"\\"+SR.Name;
						//ShowMessage(StringGrid1->Cells[3][StringGrid1->RowCount-1]);
						}

				}
			}
		} while (FindNext(SR) == 0);
		FindClose(SR);
}

//--------------------------------------------------------------
void __fastcall TForm1::Button14Click(TObject *Sender)
{
String spath="d:",s;

 if(SelectDirectory("Укажите путь к папке","",spath))
 {
 s=spath;
 //InputBox(L"Ответьте на вопрос", L"Название для меню", L"");
StringGrid1->RowCount=0;
  /*iAttributes |= faReadOnly * CheckBox1->Checked;
  iAttributes |= faHidden * CheckBox2->Checked;
  iAttributes |= faSysFile * CheckBox3->Checked;
  iAttributes |= faVolumeID * CheckBox4->Checked;
  iAttributes |= faDirectory * CheckBox5->Checked;
  iAttributes |= faArchive * CheckBox6->Checked;     */
  //iAttributes = faAnyFile;
  //поиск по загрузкам

  SearchBig(spath);

  Label124->Caption="Вот, какие большие файлы я нашел в "+s;
  Label124->Width=680;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button15Click(TObject *Sender)
{
 //РУЧНОЙ SERVICE
 String svcname=ServGrid->Cells[0][sel];
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\"+svcname,true);
		reg2->WriteInteger("Start",3);
		reg2->CloseKey();
		ServGrid->Cells[2][sel]="Вручную";
	} catch (...) {   ShowMessage("Ошибка переключения службы в ручной режим");}


}
//---------------------------------------------------------------------------


void __fastcall TForm1::VirtualImage97Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=uEYbSECiwtE", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage98Click(TObject *Sender)
{
 ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=bH8vgwLDJdo", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
 ShellExecute(Handle, L"open", L"systempropertiesprotection",0, 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
 ShellExecute(Handle, L"open", L"systempropertiesprotection",0, 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Label121MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
BitBtn9->Font->Style = TFontStyles() << fsBold << fsUnderline;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Label121MouseLeave(TObject *Sender)
{
BitBtn9->Font->Style = TFontStyles() << fsBold ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
if(FirstStart==1) {  Timer3->Enabled=true;
//предлагаем точку восстановления через 2 сек. после запуска
}
ScanPCTemp(Sender);
Label133->Left=Panel9->Width/2.0-Label133->Width/2.0;
if(FirstStart!=1) {
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	Form1->Top=reg2->ReadInteger("Top");
	reg2->CloseKey();
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	Form1->Left=reg2->ReadInteger("Left");
	reg2->CloseKey();
				   }
//СКАНИРОВАНИЕ ЯЗЫКА И ПЕРЕВОД ТЕКСТОВ
if(Language=="EN") {
BitBtn4->Caption="Home";
BitBtn13->Caption="Optimization";
BitBtn9->Caption="Services and Soft";
BitBtn10->Caption="Disk Cleaner";
BitBtn1->Caption="Autorun";

N2->Caption="Settings";
N3->Caption="Support us";
N1->Caption="Check updates";
N4->Caption="Create restore point";
N6->Caption="Safe mode";
if (N5->Caption=="Светлая тема") N5->Caption="Light theme";
if (N5->Caption=="Темная тема") N5->Caption="Dark theme";

N9->Caption="Language";

//службы
  DefTab->Caption="Antivirus  ";
 Memo5->Clear();
 Memo5->Lines->Add("Disabling the antivirus will free up about 700 MB of RAM and reduce the load on the processor.");
 Memo5->Lines->Add("It's in in any case will increase the performance of the PC. But at the same time, it will not have protection against");
 Memo5->Lines->Add("viruses.");
 Memo5->Lines->Add("");
 Memo5->Lines->Add("Should you disable Windows Defender Antivirus?");
 Memo5->Lines->Add(" - Yes, if you are an advanced user and your PC needs more resources and performance.");
 Memo5->Lines->Add(" - No, if you have a powerful enough PC or you are a novice user.");
 Memo5->Lines->Add("");
 Memo5->Lines->Add("  How to disable built-in protection?");
 Memo5->Lines->Add("I will add a few tweaks to the registry and block services that are related to the antivirus.");
 Memo5->Lines->Add("But I can’t do everything right away, I don’t have such rights, and neither do you. So in the process I");
 Memo5->Lines->Add("I will restart the PC in safe mode. And I'll ask you there to run me again and press the button.");
 Memo5->Lines->Add("");
 Memo5->Lines->Add("To turn on the antivirus again, it will be enough to press the button.");
 Memo5->Lines->Add("");
 Memo5->Lines->Add("To manually restart your PC into or out of Safe Mode, use SETTINGS -> Exit safe mode");
 Memo5->Lines->Add("");
  Label84->Caption="ANTIVIRUS WINDOWS DEFENDER";
 Label122->Caption="Current state:";
 VirtualImage91->Hint="Watch video";
 Button5->Caption="Start blocking";
 //UPDATES
 TabUpdate->Caption="Update Center";
 Label142->Caption="Windows Update Center";
 Label142->Left=TabUpdate->Width/2.0-Label142->Width/2.0;
 Label143->Caption="Current state:";
 Label143->Left=TabUpdate->Width/2.0-Label143->Width;
 Label145->Caption="I can disable or enable your Windows updates and all the services associated with it";
 Label145->Left=TabUpdate->Width/2.0-Label145->Width/2.0;
 VirtualImage110->Hint="Watch video";
 Button31->Caption="Disable updates";
 Button32->Caption="Enable updates";
 Button33->Caption="Clean updates cache";
 Button34->Caption="Open updates catalog";
 //Popular
 TabSheet4->Caption="Popular";
 Label138->Caption="This page contains services and features that users most often want to disable. I do not recommend turning everything off just like that. Think about what you don't need - and turn it off.";
 Label101->Caption="TELEMETRY";
 Label102->Caption="These services monitor your activities and relay them to Microsoft servers and others.";
 Label104->Caption="HIBERNATION";
 Label105->Caption="Disabling will free up space on the C drive and prolong the life of the SSD drive. If you have a system on an SSD, it is recommended!";
 Label109->Caption="Responsible for compression, merging RAM pages. To disable prefetching (one part of the service), find Superfetch in ""Optimization"" section";
 Label125->Caption="Search and indexing service for files and search histories in the system. You can turn it off if you don't need it. If you only want to disable indexing, do so in the disk settings";
 Label139->Caption="Edge Browser";
 Label140->Caption="The built-in Edge browser is blocked from removal. You can remove it here. But please note that the computer must have a different browser.";
 VirtualImage93->Hint="Watch video";
 VirtualImage94->Hint="Watch video";
 VirtualImage95->Hint="Watch video";
 VirtualImage106->Hint="Watch video";
 VirtualImage108->Hint="Watch video";
 //SERVICES
 ServTab->Caption="All Services";
 Label119->Caption="Here you can disable services that you do not need. So you free up computer resources for more important tasks. And I will help you figure out what to delete and what not.";
 ComboBox2->Items->Clear();
 ComboBox2->Text="Filter";
 ComboBox2->Items->Add("Remove filter");
 ComboBox2->Items->Add("Show what can be disabled");
 ComboBox2->Items->Add("Show only drivers");
 ComboBox2->Items->Add("Hide drivers");
 ComboBox2->Items->Add("Show Spy Services");
 ComboBox2->Items->Add("Show store related");
 ComboBox2->Items->Add("Show antivirus related");
 ComboBox2->Items->Add("Show related to Xbox");
 ComboBox3->Items->Clear();
 ComboBox3->Text="Leave only";
 ComboBox3->Items->Add("Automatic");
 ComboBox3->Items->Add("Manual");
 ComboBox3->Items->Add("Disabled");
 ComboBox3->Items->Add("All");
 Button27->Caption="Scan";
 Button36->Caption="Stop";
 Button15->Caption="Manual";
 Button24->Caption="Disabled";
 Button25->Caption="Auto";
 Button26->Caption="Block";
 Button6->Caption="Restore";
 VirtualImage86->Hint="Revert all services to their original state";
 VirtualImage104->Hint="Important warnings for this section";

 }



ScanPCTemp(Owner);

Label133->Left=Panel9->Width/2.0-Label133->Width/2.0;
Label131->Left=Panel9->Width/2.0-Label131->Width/2.0;
Label134->Left=Panel9->Width/2.0-Label134->Width/2.0;
Label135->Left=Panel9->Width/2.0-Label135->Width/2.0;
Label107->Width=578;
Label18->Width=478;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
Timer3->Enabled=false;
if(MessageBox(NULL, L"Желаете ли вы создать точку восстановления в системе? (с ее помощью вы всегда сможете вернуть настройки Windows обратно в момент создания этой точки)", L"Точка восстановления для обеспечения безопасности",  MB_YESNO) == IDYES)
{
  ShellExecute(Handle, L"open", L"systempropertiesprotection",0, 0, SW_SHOWNORMAL);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{   try{
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	reg2->WriteString("Language",Language);
	reg2->CloseKey();

	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	reg2->WriteInteger("Top",Form1->Top);
	reg2->CloseKey();
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	reg2->WriteInteger("Left",Form1->Left);
	reg2->CloseKey();


	} catch(...){};

try{DeleteFile("comand.bat");} catch(...){};
delete list;
delete uninstall;
delete appsnames;
delete reg2;
delete cleansizes;
delete tempnames;


}
//---------------------------------------------------------------------------
// Для поиска файлов в каталоге и его подкаталогах напишите следующий код:
void __fastcall TForm1::ListFiles(String dpath)
{
// Эта процедура выводит список файлов и вызывает
// саму себя для всех каталогов
StringGrid1->RowCount=0;
TSearchRec sr;
String foldername,startpath=dpath;
int size;
if (FindFirst(dpath+"\\*", faAnyFile, sr) == 0)
{
 do
 {
 if (sr.Attr & faDirectory)
  {     ShowMessage(sr.Name+"  "+dpath+"\\"+sr.Name+"\\*");
  if (sr.Name!=".")
  if (sr.Name!="..")
   {
   foldername=dpath+"\\"+sr.Name;
  // if (foldername!=Memo3->Lines[0].Text);
   ListFiles(dpath+"\\"+sr.Name+"\\*");// Рекурсивный вызов
   }
  }
  else
   {
   //AnsiString Ext=ExtractFileExt(sr.Name).UpperCase();
	if (sr.Size>100000000){
		StringGrid1->RowCount = StringGrid1->RowCount + 1;
		StringGrid1->Cells[0][StringGrid1->RowCount-1]="Загрузки";
		StringGrid1->Cells[1][StringGrid1->RowCount-1] = sr.Name;
		if(sr.Size<1073741824) {
			size=sr.Size/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Мб";
			} else {
			size=sr.Size/1024/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Гб";
			}
	}
   }
   //переходим в следующую папку

   if(FindNext(sr) == 0)
   {
//	Memo3->Lines->Add(dpath);
	//dpath=dpath-foldername;

	//ListFiles(dpath);
   }
 }
 while (FindNext(sr) == 0);
 FindClose(sr);
}
  StringGrid1->FixedRows=1;
  StringGrid1->ColWidths[0]=StringGrid1->Width*0.17;
  StringGrid1->ColWidths[1]=StringGrid1->Width*0.63;
  StringGrid1->ColWidths[2]=StringGrid1->Width*0.14;
//Application->ProcessMessages();
}
//----------------------------------------------------------


void __fastcall TForm1::Button17Click(TObject *Sender)
{
CardPanel3->ActiveCard=Card10;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
 CardPanel3->ActiveCard=Card11;

 StringGrid1->RowCount=0;
TSearchRec sr;
  int iAttributes = 0;
  int size=0;
  String spath=GetEnvironmentVariable("USERPROFILE")+"\\Downloads\\*.*";
  /*iAttributes |= faReadOnly * CheckBox1->Checked;
  iAttributes |= faHidden * CheckBox2->Checked;
  iAttributes |= faSysFile * CheckBox3->Checked;
  iAttributes |= faVolumeID * CheckBox4->Checked;
  iAttributes |= faDirectory * CheckBox5->Checked;
  iAttributes |= faArchive * CheckBox6->Checked;     */
  iAttributes = faAnyFile;
  //поиск по загрузкам
  if (FindFirst(spath, iAttributes, sr) == 0)
  { do
	{ if ((sr.Attr & iAttributes) == sr.Attr && sr.Size>100000000)
	  {
		StringGrid1->RowCount = StringGrid1->RowCount + 1;
		StringGrid1->Cells[0][StringGrid1->RowCount-1]="Загрузки";
		StringGrid1->Cells[1][StringGrid1->RowCount-1] = sr.Name;
		if(sr.Size<1073741824) {
			size=sr.Size/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Мб";
			} else {
			size=sr.Size/1024/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Гб";
			}

		StringGrid1->Cells[3][StringGrid1->RowCount-1] = spath.SubString1(0,spath.Length()-4)+"\\"+sr.Name;
		//ShowMessage(StringGrid1->Cells[3][StringGrid1->RowCount-1]);
		}
	} while (FindNext(sr) == 0);
	FindClose(sr);
  }
  //поиск по рабочему столу
  if(DirectoryExists(GetEnvironmentVariable("USERPROFILE")+"\\Desktop"))
	  spath=GetEnvironmentVariable("USERPROFILE")+"\\Desktop\\*.*";
  else spath=GetEnvironmentVariable("USERPROFILE")+"\\OneDrive\\Рабочий стол\\*.*";
  if(DirectoryExists(GetEnvironmentVariable("USERPROFILE")+"\\Рабочий стол"))
	 spath=GetEnvironmentVariable("USERPROFILE")+"\\Рабочий стол\\*.*";
  if (FindFirst(spath, iAttributes, sr) == 0)
  { do
	{ if ((sr.Attr & iAttributes) == sr.Attr && sr.Size>100000000)
	  {
		StringGrid1->RowCount = StringGrid1->RowCount + 1;
		StringGrid1->Cells[0][StringGrid1->RowCount-1]="Рабочий стол";
		StringGrid1->Cells[1][StringGrid1->RowCount-1] = sr.Name;
		if(sr.Size<1073741824) {
			size=sr.Size/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Мб";
			} else {
			size=sr.Size/1024/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Гб";
			}
		StringGrid1->Cells[3][StringGrid1->RowCount-1] = spath.SubString1(0,spath.Length()-4)+"\\"+sr.Name;
	  }
	} while (FindNext(sr) == 0);
	FindClose(sr);
  }
  //поиск по документам

  spath=GetEnvironmentVariable("USERPROFILE")+"\\OneDrive\\Документы\\*.*";
  if (FindFirst(spath, iAttributes, sr) == 0)
  { do
	{ if ((sr.Attr & iAttributes) == sr.Attr && sr.Size>100000000)
	  {
		StringGrid1->RowCount = StringGrid1->RowCount + 1;
		StringGrid1->Cells[0][StringGrid1->RowCount-1]="Документы";
		StringGrid1->Cells[1][StringGrid1->RowCount-1] = sr.Name;
		if(sr.Size<1073741824) {
			size=sr.Size/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Мб";
			} else {
			size=sr.Size/1024/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Гб";
			}
		StringGrid1->Cells[3][StringGrid1->RowCount-1] = spath.SubString1(0,spath.Length()-4)+"\\"+sr.Name;
	  }
	} while (FindNext(sr) == 0);
	FindClose(sr);
  }
  //поиск по C:
  spath="C:\\*.*";
  if (FindFirst(spath, iAttributes, sr) == 0)
  { do
	{ if ((sr.Attr & iAttributes) == sr.Attr && sr.Size>100000000 && ExtractFileExt(sr.Name)!=".sys")
	  {
		StringGrid1->RowCount = StringGrid1->RowCount + 1;
		StringGrid1->Cells[0][StringGrid1->RowCount-1]="C:";
		StringGrid1->Cells[1][StringGrid1->RowCount-1] = sr.Name;
		if(sr.Size<1073741824) {
			size=sr.Size/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Мб";
			} else {
			size=sr.Size/1024/1024/1024;
			StringGrid1->Cells[2][StringGrid1->RowCount-1] = IntToStr(size)+" Гб";
			}
		StringGrid1->Cells[3][StringGrid1->RowCount-1] = spath.SubString1(0,spath.Length()-4)+"\\"+sr.Name;
	  }
	} while (FindNext(sr) == 0);
	FindClose(sr);
  }


  StringGrid1->FixedRows=1;
  StringGrid1->ColWidths[0]=StringGrid1->Width*0.27;
  StringGrid1->ColWidths[1]=StringGrid1->Width*0.53;
  StringGrid1->ColWidths[2]=StringGrid1->Width*0.14;
  StringGrid1->ColWidths[3]=1;

  StringGrid1->Cells[0][0]="Папка";
  StringGrid1->Cells[1][0]="Имя файла";
  StringGrid1->Cells[2][0]="Размер";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button16Click(TObject *Sender)
{
String delpath=StringGrid1->Cells[3][StringGrid1->Row];
String q="Вы точно хотите удалить файл: "+delpath;
 if(MessageBox(NULL, q.w_str(), L"Вы уверены?",  MB_YESNO) == IDYES)
{
   DeleteFile(delpath);
   StringGrid1->Cells[0][StringGrid1->Row]="";
   StringGrid1->Cells[1][StringGrid1->Row]="";
   StringGrid1->Cells[2][StringGrid1->Row]="";
   StringGrid1->Cells[3][StringGrid1->Row]="";


}
}
//---------------------------------------------------------------------------
//   УДАЛЕНИЕ СТАНАДАРТНЫХ ПРОГРАММ
// --------------------------------------------------------------------------

void __fastcall TForm1::TabUtilsShow(TObject *Sender)
{       // ПРОВЕРКА СТАНДАРТНЫХ УТИЛИТ
//AppBox->Width=TabUtils->Width-5;
//Memo3->Clear();
//Memo3->Visible=false;
list->Clear();  //ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c powercfg -duplicatescheme e9a42b02-d5df-448d-aa00-03f14749eb61").c_str(), 0, SW_SHOWNORMAL);
AppBox->Clear();
if(!FileExists("shell.txt")) list->SaveToFile("shell.txt");
String command="Get-AppxPackage | Select Name | Out-File -FilePath shell.txt";
ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_HIDE);
Sleep(1000);
list->LoadFromFile("shell.txt");
s="";
String news="";
for(int i=3;i<list->Count;i++) {
	s=list->Strings[i];
	if(s!="") {
			for(int i=0;i<s.Length();i++) {
			   if(s.SubString0(i,1)!=" ") news+=s.SubString0(i,1);
			  }
			AppBox->Items->Add(news);
			news="";
	}
}
list->Clear();
DeleteFile("shell.txt");
Label128->Caption="Найдено "+String(AppBox->Count)+" стандартных приложений";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button19Click(TObject *Sender)
{
 if(MessageBox(NULL, L"Вы уверены, что хотите восстановить все стандартные приложения?", L"Восстановление приложений",  MB_YESNO) == IDYES)
{
String command="Get-AppxPackage -AllUsers| Foreach {Add-AppxPackage -DisableDevelopmentMode -Register \"$($_.InstallLocation)\\AppXManifest.xml\"}";
ShowMessage(command);
ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_SHOW);

//TForm1::TabUtilsShow(Sender);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button20Click(TObject *Sender)
{
AppBox->CheckAll(cbChecked, false, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button18Click(TObject *Sender)
{//удаление выбранных программ
if(MessageBox(NULL, L"Вы уверены, что хотите удалить выбранные приложения?", L"Удаление приложений",  MB_YESNO) == IDYES)
{
int delnum=0;
String command="";
for(int i=AppBox->Items->Count-1;i>=0;i--)
{
	if (AppBox->Checked[i])
	{
	command="Get-AppxPackage -allusers *"+AppBox->Items->Strings[i]+"* | Remove-AppxPackage";
	//ShowMessage(command);
	ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_HIDE);
	Sleep(200);
	AppBox->Items->Delete(i);
	delnum++;
	}
}
Label128->Caption="Удалено "+String(delnum)+", осталось "+String(AppBox->Count)+" стандартных приложений";
}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::N5Click(TObject *Sender)
{
if(Language=="RUS") {
	if(N5->Caption=="Светлая тема")
	{
	reg1->RootKey=HKEY_CURRENT_USER;
	reg1->OpenKey("Software\\PCNP",true);
	Form1->StyleName="Windows10 Clear Day";
	N5->Caption="Темная тема";
	reg1->WriteString("ThemeStyle","Light");
	reg1->CloseKey();
	} else {
		reg1->RootKey=HKEY_CURRENT_USER;
		reg1->OpenKey("Software\\PCNP",true);
	  Form1->StyleName="Material Oxford Blue SE";
	  N5->Caption="Светлая тема";
	  reg1->WriteString("ThemeStyle","Dark");
	 reg1->CloseKey();
	 }
} else {
	 if(N5->Caption=="Light theme")
	{
    reg1->RootKey=HKEY_CURRENT_USER;
		reg1->OpenKey("Software\\PCNP",true);
	 Form1->StyleName="Windows10 Clear Day";
	 N5->Caption="Dark theme";
	 reg1->WriteString("ThemeStyle","Light");
	 reg1->CloseKey();
	}
	 else {
     reg1->RootKey=HKEY_CURRENT_USER;
	 reg1->OpenKey("Software\\PCNP",true);
	  Form1->StyleName="Material Oxford Blue SE";
	  N5->Caption="Light theme";
	  reg1->WriteString("ThemeStyle","Dark");
	 reg1->CloseKey();
	}
}
 }
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage100Click(TObject *Sender)
{                        /*
if(!Memo3->Visible) {
//AppBox->Width=TabUtils->Width/2.0-5;
Memo3->Visible=true;
Memo3->Lines->Add("Не рекомендуется удалять:");
Memo3->Lines->Add(" ");
Memo3->Lines->Add("Net.Framework - нужен для работы других программ");
Memo3->Lines->Add("VCLibs - библиотеки Visual C (нужны играм и программам)");
Memo3->Lines->Add("immersivecontrolpanel - настройки панели управления ");
Memo3->Lines->Add("Client.CBS - компонент, связанный с небольшими обновлениями в системе");
Memo3->Lines->Add("creddialoghost - использоваться для учетных данных");
Memo3->Lines->Add("Apprep.ChxApp - участвует в антивирусе и браузере ");
Memo3->Lines->Add("AsyncTextService ");
Memo3->Lines->Add("AcountsControl ");
Memo3->Lines->Add("BioEnrollment - нужно для входа в систему через Windows Hello ");
Memo3->Lines->Add(" ");


Memo3->Lines->Add("Можно удалить, если не нужен Магазин Майкрософт и его приложения: ");
Memo3->Lines->Add(" ");
Memo3->Lines->Add("WindowsStore - магазин Майкрософт");
Memo3->Lines->Add("UI.Xaml - необходим для приложений Майкрософт, далее UWP");
Memo3->Lines->Add("DestopAppInstaller - установщик приложений .appx");
Memo3->Lines->Add("StorePurchaseApp - оплата в магазине ");
Memo3->Lines->Add("shellexperiencehost - отвечает за отображение универсальных приложений в оконном интерфейс");
Memo3->Lines->Add("startmenuexperiencehost - связано с Пуском и приложениями UWP");
Memo3->Lines->Add("Client.WebExperience - обновление приложений из магазина ");

Memo3->Lines->Add(" ");
Memo3->Lines->Add("На ваше усмотрение:");
Memo3->Lines->Add(" ");
Memo3->Lines->Add("Microsoft.549981C3F5F10 - Кортана ");
Memo3->Lines->Add("OOBE - приложения-помощники для визуальной настройки ");
Memo3->Lines->Add("LockApp - необходимо для виджетов на экране блокировки, но не обязательно ");
Memo3->Lines->Add("CapturePicker - необходимо для скриншотов ");
Memo3->Lines->Add("aad.brokerplugin - средство устранения неполадок ");
Memo3->Lines->Add("CBSPreview - Дает возможность сканировать бар-коды при подключении веб-камеры ");
Memo3->Lines->Add("mountaindwelling - тема оформления с горами ");
Memo3->Lines->Add("ContentDeliveryManager - само устанавливает новые приложения ");
Memo3->Lines->Add("ECApp - в перспективе может включить управление мыши глазами ");
Memo3->Lines->Add("*xbox* - приложения, связанные с приставкой XBOX");
Memo3->Lines->Add("People - приложение Люди");
Memo3->Lines->Add("Your phone - программа для связи с телефоном ");
Memo3->Lines->Add("HEIF, Web, VP9, RAW, AV1 - открытие определенных типов файлов");
Memo3->Lines->Add("Paint - программа для рисования");
Memo3->Lines->Add("Thunderbolt - управление соответствующим разъемом ");
Memo3->Lines->Add("SoundRecorder - диктофон ");
Memo3->Lines->Add("StickyNotes - программа стикеров с заметками ");
Memo3->Lines->Add("ScreenSketch - удобные скриншоты Win+Shift+S");
Memo3->Lines->Add("OfficeHub - средство для загрузки Офис 365");
Memo3->Lines->Add("GetHelp - Помощь");
Memo3->Lines->Add("BingWeather - погода");
Memo3->Lines->Add("Zune video, Zune Music - для работы с устройством-плеером Zune");
Memo3->Lines->Add("BingNews - новости ");
Memo3->Lines->Add("GetStarted - настройка Виндовс для новичков ");
Memo3->Lines->Add("WindowsCamera - необязательно для работы камеры ");
Memo3->Lines->Add("Notepad - блокнот ");
Memo3->Lines->Add("SolitaireCollection - карточные игры ");
Memo3->Lines->Add("PowerAutomateDesktop - создание автоматизированных бизнес-процессов между программами");
Memo3->Lines->Add("Calculator - это понятно :) ");
Memo3->Lines->Add("Teams - программа для видеоконференций ");
Memo3->Lines->Add("Terminal - улучшенная командная строка ");
 Memo3->Lines->Add("ClipChamp - встроенный видеоредактор ");
Memo3->Lines->Add("Photos - программа просмотра изображений ");
Memo3->Lines->Add("Alarms - будильники ");
Memo3->Lines->Add("windowscommunicationsapps - Почта и Календарь Outlook ");
Memo3->Lines->Add("Gamingapp - игровой режим в Windows ");
Memo3->Lines->Add("Todos - список дел и задачи ");
Memo3->Lines->Add("Edge - браузер ");
Memo3->Lines->Add("secureassesmentbrowser - средство защиты ПК и оценки рисков для бизнеса ");
Memo3->Lines->Add("Client.Core - не обязательное приложение ");
Memo3->Lines->Add("OneDrive - облачное хранилище");

 Memo3->Lines->Add("");
  Memo3->Lines->Add("Ни одно из этих приложений не является критически важным для системы, но вы можете лишиться полезных функций");
}
	else {
	AppBox->Width=TabUtils->Width-5;
	Memo3->Visible=false;
}                       */
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SoftSheetShow(TObject *Sender)
{
//получаем список приложений и их данные с деинсталляторами

StepBox->Visible=false;
list->Clear();
TStringList *list2=new TStringList;
uninstall->Clear();
appsnames->Clear();
ListBox2->Clear();
String kname="";
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall",true);
reg2->GetKeyNames(list);
reg2->CloseKey();
String Aname="";
   for(int i=0;i<list->Count;i++) {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		kname="SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"+list->Strings[i];
		reg2->OpenKey(kname,true);
		if(reg2->ValueExists("DisplayName") )
			{
			if(reg2->ValueExists("WindowsInstaller")) {
				if(reg2->ReadInteger("WindowsInstaller")!=1) {
				Aname=reg2->ReadString("DisplayName");
					if(reg2->ValueExists("UninstallString")){
						ListBox2->Items->Add(Aname);
						uninstall->Add(list->Strings[i]);                    //добавлем в список имя директории
						appsnames->Add(reg2->ReadString("UninstallString")); //сохраняем код для удаления

						}
				}
			}  else {
					Aname=reg2->ReadString("DisplayName");
					if(reg2->ValueExists("UninstallString")){
						ListBox2->Items->Add(Aname);
						uninstall->Add(list->Strings[i]);
						appsnames->Add(reg2->ReadString("UninstallString"));
						}
				}
			}
		reg2->CloseKey();
   }
// вторая папка
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",true);
reg2->GetKeyNames(list2);
reg2->CloseKey();
int notorigin=0;
   for(int i=0;i<list2->Count;i++) {
		notorigin=0;
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		kname="SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"+list2->Strings[i];
		reg2->OpenKey(kname,true);
		if(reg2->ValueExists("DisplayName") )
			{
			 Aname=reg2->ReadString("DisplayName");
			 for(int x=0;x<uninstall->Count;x++) {
				 if(Aname==uninstall->Strings[x]) notorigin=1;
				}
				   if(notorigin==0) {
						if(reg2->ValueExists("WindowsInstaller")) {
							if(reg2->ReadInteger("WindowsInstaller")!=1) {

								if(reg2->ValueExists("UninstallString")){
									ListBox2->Items->Add(Aname);
									uninstall->Add(list2->Strings[i]);                    //добавлем в список имя директории
									appsnames->Add(reg2->ReadString("UninstallString")); //сохраняем код для удаления

									}
							}
						}  else {
								Aname=reg2->ReadString("DisplayName");
								if(reg2->ValueExists("UninstallString")){
									ListBox2->Items->Add(Aname);
									uninstall->Add(list2->Strings[i]);
									appsnames->Add(reg2->ReadString("UninstallString"));
									}
							}
						}
			}

		reg2->CloseKey();
   }








   ListBox2->Height=SoftSheet->Height-ListBox2->Top-80;
   delete list2;
   }
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn16Click(TObject *Sender)
{
int sel;
list->Clear();
// Подробности о программе.
try{
sel=ListBox2->ItemIndex;
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"+uninstall->Strings[sel],true);
list->Add("Имя: "+reg2->ReadString("DisplayName"));
list->Add("Путь: "+reg2->ReadString("InstallLocation"));

 ShowMessage(list->Text);

reg2->CloseKey();


} catch(...) {};
// ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c powercfg -duplicatescheme e9a42b02-d5df-448d-aa00-03f14749eb61").c_str(), 0, SW_SHOWNORMAL);
// Sleep(800);
// ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c powercfg.cpl").c_str(), 0, SW_SHOWNORMAL);

}
void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{             //   ДЕИНСТАЛЛЯТОР
if(MessageBox(NULL, L"Вы уверены, что хотите удалить выбранное приложение? (Не все деинсталляторы могут работать)", L"Удаление приложений",  MB_YESNO) == IDYES)
{
int sel;
String command;
list->Clear();
try{
sel=ListBox2->ItemIndex;
command=appsnames->Strings[sel];
//ShowMessage(command);
ShellExecute(Handle, L"open", command.w_str(),0, 0, SW_SHOWNORMAL);
} catch(...) {};
}
}
//---------------------------------------------------------------------------
 // РЕКУРСИЯ ДЛЯ ПОИСКА СЛЕДОВ ПРИЛОЖЕНИЙ
void __fastcall TForm1::StepsFind(String dpath,String key1,int k1)
{
TSearchRec SR;
String foldername,startpath=dpath;

if (FindFirst(dpath + "\\*.*", faAnyFile, SR) == 0)
		do
		{
			if (SR.Name != "." && SR.Name != "..")
			{
				if ((SR.Attr & faDirectory) != 0)
					{
						   foldername=dpath+"\\"+SR.Name;
						   for (int x=1;x<SR.Name.Length(); x++) {
							   if(SR.Name.SubString(x,k1)==key1)      //ищем без первой буквы
							   {
								  StepBox->Items->Add(foldername);
							   }
							}
						   StepsFind(dpath+"\\"+SR.Name,key1,k1);// Рекурсивный вызов
					//StepsFind(Folder + "\\" + SR.Name, Size, fCount, folCount);
					//folCount++;
				}
				else
				{
					//Size = Size + SR.Size;
					//fCount++;
						}
			}
		} while (FindNext(SR) == 0);
		FindClose(SR);


}
//0-------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton4Click(TObject *Sender)
{
if(Edit1->Text!="") {
ListBox2->Height=SoftSheet->Height/2.0-ListBox2->Top;
StepBox->Visible=true;
StepBox->Top=ListBox2->Top+ListBox2->Height+10;
StepBox->Height=SoftSheet->Height-StepBox->Top-80;
StepBox->Clear();
//Поиск по следам данной программы
String key=Edit1->Text;
String key1=key.SubString(2,key.Length()-1);
list->Clear();

String str;
int k=key.Length();
int k1=key1.Length();

reg2->RootKey=HKEY_CURRENT_USER;
reg2->OpenKey("Software",true);        // РЕЕСТР
reg2->GetKeyNames(list);
reg2->CloseKey();

for(int i=0;i<list->Count;i++) {
	str=list->Strings[i];

	for (x=1;x<str.Length(); x++) {
	   if(str.SubString(x,k1)==key1)      //ищем без первой буквы
	   {   //reg2->OpenKey(str,true);
		   StepBox->Items->Add("HKEY_CURRENT_USER\\Software\\"+str);
		   //reg2->CloseKey();
		   //reg2->RootKey=HKEY_CURRENT_USER;
		   //reg2->OpenKey("Software",true);
	   }
	}
}
reg2->CloseKey();

list->Clear();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE",true);        // РЕЕСТР
reg2->GetKeyNames(list);

for(int i=0;i<list->Count;i++) {
	str=list->Strings[i];
	for (x=1;x<str.Length(); x++) {
	   if(str.SubString(x,k1)==key1)      //ищем без первой буквы
	   {
			StepBox->Items->Add("HKEY_LOCAL_MACHINE\\SOFTWARE\\"+str);
		}
	}
}
reg2->CloseKey();

list->Clear();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall",true);        // РЕЕСТР
reg2->GetKeyNames(list);
 reg2->CloseKey();
for(int i=0;i<list->Count;i++) {
	str=list->Strings[i];
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall",true);        // РЕЕСТР
		reg2->OpenKey(list->Strings[i],true);
		str=reg2->ReadString("DisplayName");
		reg2->CloseKey();
	} catch (...) {}
	for (x=1;x<str.Length(); x++) {
	   if(str.SubString(x,k1)==key1)      //ищем без первой буквы
	   {
			StepBox->Items->Add("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"+list->Strings[i]);
		}
	}
}
reg2->CloseKey();

list->Clear();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",true);        // РЕЕСТР
reg2->GetKeyNames(list);
reg2->CloseKey();
for(int i=0;i<list->Count;i++) {
	str=list->Strings[i];
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",true);        // РЕЕСТР
		reg2->OpenKey(list->Strings[i],true);
		str=reg2->ReadString("DisplayName");
		reg2->CloseKey();

	} catch (...) {
	}
	for (x=1;x<str.Length(); x++) {
	   if(str.SubString(x,k1)==key1)      //ищем без первой буквы
	   {
			StepBox->Items->Add("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"+list->Strings[i]);
		}
	}
}
reg2->CloseKey();


//ищем в АППДАТА
StepsFind(GetEnvironmentVariable("USERPROFILE")+"\\AppData",key1,k1);
//ищем в programfiles
StepsFind("C:\\Program Files",key1,k1);
StepsFind("C:\\Program Files (x86)",key1,k1);
StepsFind("C:\\ProgramData",key1,k1);

} else ShowMessage("Введите ключевое слово");

}
//---------------------------------------------------------------------------


void __fastcall TForm1::VirtualImage103Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.youtube.com/watch?v=nI7PvXFGYxk", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton7Click(TObject *Sender)
{
StepBox->CheckAll(cbChecked, false, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton6Click(TObject *Sender)
{
//удаление выбранных следов
if(MessageBox(NULL, L"Вы уверены, что хотите удалить выбранные пункты?", L"Удаление следов приложений",  MB_YESNO) == IDYES)
{
int delnum=0;
String elem,command;
for(int i=StepBox->Items->Count-1;i>=0;i--)
{
	if (StepBox->Checked[i])
	{   elem=StepBox->Items->Strings[i];
		if(elem.SubString(1,17)=="HKEY_CURRENT_USER") {
			//ключ реестра
			try {
			reg2->RootKey=HKEY_CURRENT_USER;
			//ShowMessage(elem.SubString(19,elem.Length()));
			reg2->DeleteKey(elem.SubString(19,elem.Length()));
			StepBox->Items->Delete(i);
			delnum++;
				} catch (...) {
			}
		}
		if(elem.SubString(1,18)=="HKEY_LOCAL_MACHINE") {
			//ключ реестра
			try {
			reg2->RootKey=HKEY_LOCAL_MACHINE;
			//ShowMessage(elem.SubString(19,elem.Length()));
			reg2->DeleteKey(elem.SubString(20,elem.Length()));
			StepBox->Items->Delete(i);
			delnum++;
				} catch (...) {
			}
		}
		if(elem.SubString(1,2)=="C:") {
			//папка
			try {
				 //ShowMessage(elem);
				 //RemoveDir("C:/Users/coola/AppData/Local/Apps");
				 command="Remove-Item -Recurse -Force -Path '"+elem+"'";
				 ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_HIDE);
				 Sleep(200);
				 if(!DirectoryExists(elem,true)) {
				 StepBox->Items->Delete(i);
				 delnum++;
				 }
				} catch (...) {
			}
		}



	}
}
ShowMessage("Удалено "+String(delnum)+" элементов");
//Label128->Caption="Удалено "+String(delnum)+", осталось "+String(AppBox->Count)+" стандартных приложений";
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
if(Key==VK_RETURN) {
	SpeedButton4Click(Sender);
}
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Label117Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://ulap.top/digital-licenses", NULL, NULL, SW_NORMAL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label117MouseEnter(TObject *Sender)
{
Label117->Font->Style = TFontStyles() << fsUnderline << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label117MouseLeave(TObject *Sender)
{
Label117->Font->Style = TFontStyles() << fsItalic;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button22Click(TObject *Sender)
{
BitBtn9Click(Sender);
PageControl2->ActivePage=TabUtils;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AppBoxClick(TObject *Sender)
{
if(Language=="RUS") {
TIniFile *DescIni = new TIniFile(ExtractFilePath(Application->ExeName)+"AppDesc.ini");

DescIni->ReadSection("AppBox_ru",Memo1->Lines);
String an=AppBox->Items->Strings[AppBox->ItemIndex];
int ac=Memo1->Lines->Count-1;
String s;
Memo3->Text="Нет информации по этому приложению";
for(int x=0;x<ac;x++) {
	s=Memo1->Lines->Strings[x];
	for(int i=1;i<an.Length();i++)
{
  if(an.SubString(i,s.Length())==s) {Memo3->Text=DescIni->ReadString("AppBox_ru",s,"Нет информации об этой программе");}
}
}
delete DescIni;
}

if(Language=="EN") {
TIniFile *DescIni = new TIniFile(ExtractFilePath(Application->ExeName)+"AppDesc.ini");

DescIni->ReadSection("AppBox",Memo1->Lines);
String an=AppBox->Items->Strings[AppBox->ItemIndex];
int ac=Memo1->Lines->Count-1;
String s;
Memo3->Text="I don`t have info about this app";
for(int x=0;x<ac;x++) {
	s=Memo1->Lines->Strings[x];
	for(int i=1;i<an.Length();i++)
{
  if(an.SubString(i,s.Length())==s) {Memo3->Text=DescIni->ReadString("AppBox",s,"I don`t have info about this appе");}
}
}
delete DescIni;
}


}
//---------------------------------------------------------------------------

void __fastcall TForm1::StepBoxClick(TObject *Sender)
{
Label118->Caption=StepBox->Items->Strings[StepBox->ItemIndex];
Label118->Width=SoftSheet->Width-15;
Label118->Height=32;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::VirtualImage104Click(TObject *Sender)
{
if(Language=="RUS") {
list->Clear();
list->Add("Как использовать этот раздел?");
list->Add(" ");
list->Add("1. Используйте фильтр, чтобы найти службы, которые можно отключить.");
list->Add("2. Внимательно прочитайте описание, т.к. некоторые службы связаны с другими программами. Если вы отключите не ту службу, некоторые программы или функции Windows, которые вам нужны, не будут работать.");
list->Add("3. Перед изменением рекомендую создать точку восстановления (используйте верхнее меню). Некоторые сервисы могут меняться с новыми обновлениями. Поэтому детали в описании сервисов могут быть неточными.");
list->Add("4. Используйте функцию блокировки, только если служба защищена (не хочет отключаться или восстанавливается после перезагрузки), но вы уверены, что так и должно быть. Блокировка заберет права у системы на управление службой и оставит их только у вас - администратора.");
list->Add("5. Все изменения вступят в силу после перезагрузки ПК.");
ShowMessage(list->Text);
}
if(Language=="EN") {
list->Clear();
list->Add("How to use this section?");
list->Add(" ");
list->Add("1. Use the filter to find services that can be disabled.");
list->Add("2. Read the description carefully, as some services are linked to other programs. If you disable the wrong service, some Windows programs or features you need won't work.");
list->Add("3. Before changing, I recommend to create a restore point (use the top menu). Some services may change with new updates. Therefore, the details in the description of the services may not be accurate.");
list->Add("4. Use the block feature if the service is secure (doesn't want to shut down or recovers after a reboot), but you're sure it should be. Blocking will take away the rights from the system to manage the service and leave them only to you - the administrator.");
list->Add("5. All changes will take effect after restarting the PC.");
ShowMessage(list->Text);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServTabShow(TObject *Sender)
{
// Вкладка службы - скан служб из реестра и создание списка с описанием.
if(openserv==0 && PageControl2->ActivePage==ServTab) {
list->Clear();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services",true);
reg2->GetKeyNames(list);
reg2->CloseKey();
ServGrid->RowCount=1;

ServGrid->ColWidths[0]=ServGrid->Width*0.27;
ServGrid->ColWidths[1]=ServGrid->Width*0.45;
ServGrid->ColWidths[2]=ServGrid->Width*0.14;
ServGrid->ColWidths[3]=ServGrid->Width*0.14;
int i1=0,d=0;
String command;    String stat;
	command="Get-Service | Select Name,Status | Out-File res.txt";
	ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_HIDE);
	Sleep(1000);
	fs->Clear();
	try{ fs->LoadFromFile(ExtractFilePath(Application->ExeName)+"res.txt");} catch(...) {ShowMessage("error servtabres");}
for (int i = 1; i < list->Count; i++) {
//
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services",true);
reg2->OpenKey(list->Strings[i],true);
if(reg2->ValueExists("Start")==true) {
int start=reg2->ReadInteger("Start");

if(start>1) {
String DN=reg2->ReadString("DisplayName");
int p1=1;
if(DN.SubString(1,1)=="@") {
	for (int x=1; x < DN.Length(); x++) {
		if(DN.SubString(x,1)==";") p1=x;
	}
}
	DN=DN.SubString(p1,DN.Length()-p1+1);
	if(DN.SubString(1,1)!="") {
	if(DN.SubString(1,1)==";") ServGrid->Cells[1][i1]=DN.SubString(2,DN.Length()-1);
	else ServGrid->Cells[1][i1]=DN;
	ServGrid->Cells[0][i1]=list->Strings[i];
	if(start==1) ServGrid->Cells[2][i1]="Авто(По триггеру)";
	if(start==2) ServGrid->Cells[2][i1]="Автоматически";
	if(start==3) ServGrid->Cells[2][i1]="Вручную";
	if(start==4) ServGrid->Cells[2][i1]="Отключена";
	//Запущена или нет?
	d=list->Strings[i].Length();
	for(int x=3;x<fs->Count;x++) {
		if(list->Strings[i]==fs->Strings[x].SubString(1,d)) {
			stat=fs->Strings[x];
			//ShowMessage(stat.SubString(stat.Length()-6,6));
			if (stat.SubString(stat.Length()-6,1)=="S") {if(Language=="EN") ServGrid->Cells[3][i1]="Stopped"; else ServGrid->Cells[3][i1]="Останов.";   }
			if (stat.SubString(stat.Length()-6,1)=="R") {if(Language=="EN") ServGrid->Cells[3][i1]="Running"; else ServGrid->Cells[3][i1]="Работает";    }
		}
	}
	i1++;
	ServGrid->RowCount++;
	}
	}
}

reg2->CloseKey();
}

fs->Clear();
DeleteFile("res.txt");
openserv=1;
}
ServGridClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServGridClick(TObject *Sender)
{
if(Language=="RUS") {
TIniFile *ServDesc = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDesc.ini");
String an=ServGrid->Cells[0][sel];
String s;
int tchk1=0,tchk2=0;
Memo4->Text="У меня пока нет информации по этой службе";
for(int i=1;i<an.Length();i++) {
  if(an.SubString(i,1)=="_") an=an.SubString(1,i-1);
  }
  s=ServDesc->ReadString("ServDesc",an,"У меня пока нет информации по этой службе");
  for(int i=1;i<s.Length()+1;i++) {
  if(s.SubString(i,1)=="." && tchk1!=0 && tchk2==0) tchk2=i;
	if(s.SubString(i,1)=="." && tchk1==0) tchk1=i;
  }
 Label120->Caption="Имя: "+s.SubString(1,tchk1-1);
 Label121->Caption="Рекомендация: "+s.SubString(tchk1+1,tchk2-tchk1);
 Label121->Hint=Label121->Caption;
 Memo4->Text=s.SubString(tchk2+1,s.Length()-tchk2);

 an=ServGrid->Cells[1][sel];
  for(int i=1;i<an.Length();i++) {
  if(Memo4->Text=="У меня пока нет информации по этой службе" && an.SubString(i,5)=="river") {Memo4->Text=ServDesc->ReadString("ServDesc","river","У меня пока нет информации по этой службе");}
  }
delete ServDesc;
}

if(Language=="EN") {
TIniFile *ServDesc = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDesc.ini");
String an=ServGrid->Cells[0][sel];
String s;
int tchk1=0,tchk2=0;
Memo4->Text="I don`t have information about this service yet";
for(int i=1;i<an.Length();i++) {
  if(an.SubString(i,1)=="_") an=an.SubString(1,i-1);
  }
  s=ServDesc->ReadString("ServDescEn",an,"I don`t have information about this service yet");
  for(int i=1;i<s.Length()+1;i++) {
  if(s.SubString(i,1)=="." && tchk1!=0 && tchk2==0) tchk2=i;
	if(s.SubString(i,1)=="." && tchk1==0) tchk1=i;
  }
 Label120->Caption="Name: "+s.SubString(1,tchk1-1);
 Label121->Caption="Recommend: "+s.SubString(tchk1+1,tchk2-tchk1);
 Label121->Hint=Label121->Caption;
 Memo4->Text=s.SubString(tchk2+1,s.Length()-tchk2);

 an=ServGrid->Cells[1][sel];
  for(int i=1;i<an.Length();i++) {
  if(Memo4->Text=="I don`t have information about this service yet" && an.SubString(i,5)=="river") {Memo4->Text=ServDesc->ReadString("ServDescEn","river","I don`t have information about this service yet");}
  }
delete ServDesc;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox2Change(TObject *Sender)
{
bool ok=false;
String d1,d2,d3;

 if(ComboBox2->ItemIndex==0) {
 openserv=0;
 ServTabShow(Sender);
 }

 if(ComboBox2->ItemIndex==1) { //службы, которые можно отключить
 openserv=0;
 ServTabShow(Sender);
 TIniFile *ServDesc = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDesc.ini");


 for(int x=ServGrid->RowCount;x>=0;x--)
 {
	ok=false;
	d1=ServGrid->Cells[0][x];
	d2=ServDesc->ReadString("ServDesc",d1,"");
	for(int z=1;z<d2.Length();z++) {      // "can disable it"
	if(d2.SubString(z,16)=="можете отключить") ok=true;
	}
	  if(ok==false) {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
 delete ServDesc;
 }

if(ComboBox2->ItemIndex==2) { //показать только драйверы
openserv=0;
ServTabShow(Sender);
 for(int x=ServGrid->RowCount;x>=0;x--)
 {
	ok=false;
	d1=ServGrid->Cells[0][x];
	d2=ServGrid->Cells[1][x];
	for(int z=1;z<d2.Length();z++) {      //
	if(d2.SubString(z,5)=="river" || d2.SubString(z,6)=="райвер") ok=true;
	}
	  if(ok==false) {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }


}
if(ComboBox2->ItemIndex==3) { //спрятать драйверы
 for(int x=ServGrid->RowCount;x>=0;x--)
 {
	ok=true;
	d1=ServGrid->Cells[0][x];
	d2=ServGrid->Cells[1][x];
	for(int z=1;z<d2.Length();z++) {      //
	if(d2.SubString(z,5)=="river" || d2.SubString(z,6)=="райвер") ok=false;
	}
	  if(ok==false) {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
}

if(ComboBox2->ItemIndex==4) { //Show spy services
openserv=0;
ServTabShow(Sender);
TIniFile *ServDesc = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDesc.ini");
 for(int x=ServGrid->RowCount;x>=0;x--)
 {
	ok=false;
	d1=ServGrid->Cells[0][x];
	d2=ServDesc->ReadString("ServDesc",d1,"");
	for(int z=1;z<d2.Length();z++) {      // "can disable it"
	if(d2.SubString(z,4)=="пион") ok=true;
	}
	  if(ok==false) {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
 delete ServDesc;
}

if(ComboBox2->ItemIndex==5) { //Show connected with MS Store
openserv=0;
ServTabShow(Sender);
TIniFile *ServDesc = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDesc.ini");
 for(int x=ServGrid->RowCount;x>=0;x--)
 {
	ok=false;
	d1=ServGrid->Cells[0][x];
	d2=ServDesc->ReadString("ServDesc",d1,"");
	for(int z=1;z<d2.Length();z++) {      // "can disable it"
	if(d2.SubString(z,6)=="агазин" || d2.SubString(z,5)=="store" || d2.SubString(z,5)=="Store")  ok=true;
	}
	  if(ok==false) {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
 delete ServDesc;
}

if(ComboBox2->ItemIndex==6) { //Show defender
openserv=0;
ServTabShow(Sender);
TIniFile *ServDesc = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDesc.ini");
 for(int x=ServGrid->RowCount;x>=0;x--)
 {
	ok=false;
	d1=ServGrid->Cells[0][x];
	d2=ServDesc->ReadString("ServDesc",d1,"");
	for(int z=1;z<d2.Length();z++) {      // "can disable it"
	if(d2.SubString(z,7)=="ащитник" || d2.SubString(z,8)=="нтивирус") ok=true;
	}
	  if(ok==false) {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
 delete ServDesc;
}

if(ComboBox2->ItemIndex==7) { //хбокс
openserv=0;
ServTabShow(Sender);
TIniFile *ServDesc = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDesc.ini");
 for(int x=ServGrid->RowCount;x>=0;x--)
 {
	ok=false;
	d1=ServGrid->Cells[0][x];
	d2=ServDesc->ReadString("ServDesc",d1,"");
	for(int z=1;z<d2.Length();z++) {      // "can disable it"
	if(d2.SubString(z,4)=="Xbox" || d2.SubString(z,4)=="xbox") ok=true;
	}
	  if(ok==false) {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
 delete ServDesc;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button24Click(TObject *Sender)
{
String svcname=ServGrid->Cells[0][sel];
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\"+svcname,true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();
		ServGrid->Cells[2][sel]="Отключена";
	} catch (...) {   ShowMessage("Ошибка отключения службы");}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)

{
sel=ARow;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button25Click(TObject *Sender)
{
//AUTOMATIC SERVICE
 String svcname=ServGrid->Cells[0][sel];
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\"+svcname,true);
		reg2->WriteInteger("Start",2);
		reg2->CloseKey();
		ServGrid->Cells[2][sel]="Автоматически";
	} catch (...) {   ShowMessage("Ошибка переключения службы в автоматический режим");}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button27Click(TObject *Sender)
{
openserv=0;
ServTabShow(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button26Click(TObject *Sender)
{
//Block SERVICE
 String svcname=ServGrid->Cells[0][sel];
 String com1="/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\"+svcname+"\" -ot reg -actn setowner -ownr \"n:Administrators\"";
 String com2="/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\"+svcname+"\" -ot reg -actn setowner -ownr \"n:Администраторы\"";
 ShellExecute(Handle, L"open", L"cmd.exe",com1.w_str(), 0, SW_SHOWNORMAL);
	Sleep(1000);
	ShellExecute(Handle, L"open", L"cmd.exe",com2.w_str(), 0, SW_SHOWNORMAL);
	Sleep(1000);
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\"+svcname+" [1 8 19]");
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\"+svcname,true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();
		ServGrid->Cells[2][sel]="Заблокировано";
		ShowMessage("Перезагрузите ПК");
		} catch (...) {   ShowMessage("Ошибка блокировки");}
}
//---------------------------------------------------------------------------



void __fastcall TForm1::N6Click(TObject *Sender)
{                                       // БЕЗОПАСНЫЙ РЕЖИМ
if (Language=="EN") {
 try {
	reg2->CloseKey();
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	if(reg2->ValueExists("BootMode")) {
	   if(reg2->ReadInteger("BootMode")==0) {
		 if(MessageBox(NULL, L"Вы уверены, что хотите перезагрузить компьютер в безопасный режим? (При нажатии 'ДА' начнется перезагрузка - сохраните и закройте другие программы)", L"Вход в безопасный режим",  MB_YESNO) == IDYES)
			{
			 reg2->WriteInteger("BootMode",1);
			  String com="/c bcdedit /set {default} safeboot Minimal";
			  ShellExecute(Handle, L"open", L"cmd.exe",com.w_str(), 0, SW_SHOWNORMAL);
			  Sleep(500);
			  ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c shutdown /r /t 0").c_str(), 0, SW_SHOWNORMAL);
			}
	   } else {
		 if(MessageBox(NULL, L"Вы уверены, что хотите перезагрузить компьютер в обычном режиме?", L"Выход из безопасного режима",  MB_YESNO) == IDYES)
			{
			 reg2->WriteInteger("BootMode",0);
			  String com="/c bcdedit /deletevalue {default} safeboot";
			  ShellExecute(Handle, L"open", L"cmd.exe",com.w_str(), 0, SW_SHOWNORMAL);
			  Sleep(500);
			  ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c shutdown /r /t 0").c_str(), 0, SW_SHOWNORMAL);
			}
	   }

	}
	reg2->CloseKey();
	} catch(...) {ShowMessage("Ошибка перехода в безопасный режим. Нет доступа к реестру.");}

} else {
     try {
	reg2->CloseKey();
	reg2->RootKey=HKEY_CURRENT_USER;
	reg2->OpenKey("Software\\PCNP",true);
	if(reg2->ValueExists("BootMode")) {
	   if(reg2->ReadInteger("BootMode")==0) {
		 if(MessageBox(NULL, L"Are you sure you want to restart your computer in safe mode? (Pressing 'YES' will restart - save and close other programs)", L"Enter the safe mode",  MB_YESNO) == IDYES)
			{
			 reg2->WriteInteger("BootMode",1);
			  String com="/c bcdedit /set {default} safeboot Minimal";
			  ShellExecute(Handle, L"open", L"cmd.exe",com.w_str(), 0, SW_SHOWNORMAL);
			  Sleep(500);
			  ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c shutdown /r /t 0").c_str(), 0, SW_SHOWNORMAL);
			}
	   } else {
		 if(MessageBox(NULL, L"Are you sure you want to restart your computer in normal mode?", L"Exit the safe mode",  MB_YESNO) == IDYES)
			{
			 reg2->WriteInteger("BootMode",0);
			  String com="/c bcdedit /deletevalue {default} safeboot";
			  ShellExecute(Handle, L"open", L"cmd.exe",com.w_str(), 0, SW_SHOWNORMAL);
			  Sleep(500);
			  ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c shutdown /r /t 0").c_str(), 0, SW_SHOWNORMAL);
			}
	   }

	}
	reg2->CloseKey();
	} catch(...) {ShowMessage("Error N6 - Ошибка перехода в безопасный режим. Нет доступа к реестру.");}
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button28Click(TObject *Sender)
{
 // Блокировка службы wsearch
if(Button28->Caption=="Заблокировать" || Button28->Caption=="Disable") {
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WSearch",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();
		if(Language=="EN") Label126->Caption="Disabled"; else Label126->Caption="Заблокировано";
		Label126->Font->Color=clRed;
		if(Language=="EN") Button28->Caption="Enable"; else Button28->Caption="Включить";
		//ShowMessage("Перезагрузите ПК");
		} catch (...) {   ShowMessage("Error btn28! Не удалось отключить службу");}
} else {
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WSearch",true);
		reg2->WriteInteger("Start",2);
		reg2->CloseKey();
		if(Language=="EN") Label126->Caption="Enabled"; else  Label126->Caption="Работает";
		Label126->Font->Color=clLime;
		if(Language=="EN") Button28->Caption="Disable"; else Button28->Caption="Заблокировать";
		} catch (...) {   ShowMessage("Error btn28! Не удалось включить службу");}
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox3Change(TObject *Sender)
{
ComboBox2Change(Sender);
String d1;
if(ComboBox3->ItemIndex==0) { //автоматич
 for(int x=ServGrid->RowCount-1;x>=0;x--)
 {
	d1=ServGrid->Cells[2][x];
	  if(d1!="Автоматически") {
		if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
}

if(ComboBox3->ItemIndex==1) { //вручную
 for(int x=ServGrid->RowCount-1;x>=0;x--)
 {
	d1=ServGrid->Cells[2][x];
	  if(d1!="Вручную") {
		//if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
}

if(ComboBox3->ItemIndex==2) { //откл
 for(int x=ServGrid->RowCount-1;x>=0;x--)
 {

	d1=ServGrid->Cells[2][x];
	  if(d1!="Отключена") {
		//if ( ServGrid->RowCount - ServGrid->FixedRows <= 1 ) return; // оставляет по крайней мере одну строку
		// --------------------------- Смещаем строки вверх ------------------------
		for ( int i = x; i < ServGrid->RowCount; i++ )
		{
				ServGrid->Rows[i] = ServGrid->Rows[i+1];
		}
		// --------------------------------------------------------------------------

		ServGrid->RowCount--;
		ServGrid->SetFocus();

	  }
 }
}
if(ComboBox3->ItemIndex==3) {

}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button29Click(TObject *Sender)
{
TabUtilsShow(Sender);
}
//---------------------------------------------------------------------------
 void FindFlPath(String Folder, String FlName, String &FPath, String &FFolder)
{         // найти путь к определенному файлу в каталоге
	TSearchRec SR;
	String fp1;
	if (Folder[Folder.Length()] == '\\')
		Folder.SetLength(Folder.Length() - 1);
	if (FindFirst(Folder + "\\*.*", faAnyFile, SR) == 0)
		do
		{
			if (SR.Name != "." && SR.Name != "..")
			{
				if ((SR.Attr & faDirectory) != 0)
				{
				FindFlPath(Folder + "\\" + SR.Name, FlName, FPath,FFolder);
				//ShowMessage(Folder + "\\" + SR.Name);

				}
				else
				{
					if(SR.Name==FlName) {
					//ShowMessage(Folder+SR.Name);
								FPath=Folder+"\\"+SR.Name;
								FFolder=Folder;

						}

			}
			}
		} while (FindNext(SR) == 0);
		FindClose(SR);
	   //	FPath=fpl;
}

//---------------------------------------------------------
void __fastcall TForm1::Button30Click(TObject *Sender)
{        //удаление браузера
if(Button30->Caption=="Удалить" || Button30->Caption=="Remove" ) {
if(MessageBox(NULL, L"Вы уверены, что хотите удалить браузер MS Edge? Он будет удален полностью без возможности включения. Для его возвращения нужно будет скачать установщик с сайта Майкрософт.", L"Удаление браузера Edge",  MB_YESNO) == IDYES)
{

String elem,command="@taskkill /IM msedge.exe /T /F";
ShellExecute(Handle, L"open", L"cmd.exe",command.w_str(), 0, SW_HIDE);

 reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SOFTWARE\\Microsoft",true);
		reg2->CreateKey("EdgeUpdate");
		reg2->OpenKey("EdgeUpdate",true);
		reg2->WriteInteger("DoNotUpdateToEdgeWithChromium",1);
		reg2->CloseKey();

 list->Clear();
 list->Add("C:\\Program Files (x86)\\Microsoft\\Edge");
 list->Add("C:\\Program Files (x86)\\Microsoft\\EdgeCore");
 list->Add("C:\\Program Files (x86)\\Microsoft\\EdgeUpdate");
 list->Add("C:\\ProgramData\\Microsoft\\EdgeUpdate");
 list->Add(GetEnvironmentVariable("USERPROFILE")+"\\AppData\\Local\\Microsoft\\Edge");
 list->Add("C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Microsoft Edge");



		for(i=0;i<list->Count;i++){
 try {			 elem=list->Strings[i];   //
				// ShowMessage(elem);
				 command="Remove-Item -Recurse -Force -Path '"+elem+"'";
				 ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_HIDE);
				 Sleep(500);

				} catch (...) { };
		}

reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall",true);        // РЕЕСТР
reg2->DeleteKey("Microsoft Edge");
reg2->DeleteKey("Microsoft Edge Update");
reg2->CloseKey();

reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SOFTWARE\\WOW6432Node\\Microsoft",true);        // РЕЕСТР
reg2->DeleteKey("Edge");
reg2->DeleteKey("Edge Update");
reg2->CloseKey();

	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\edgeupdate",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();
	} catch (...) {   ShowMessage("Ошибка отключения службы");}
    	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\edgeupdatem",true);
		reg2->WriteInteger("Start",4);
		reg2->CloseKey();
	} catch (...) {   ShowMessage("Ошибка отключения службы");}

//ShowMessage("Edge был удален. На случай, если я что-то пропустил, воспользуйтесь вкладкой ПРОГРАММЫ и удалите его следы по запросу Edge");
Label141->Caption="Удален";
Button30->Caption="Установить";
Label141->Font->Color=clRed;
}
}
if(Button30->Caption=="Установить") {
  ShellExecute(0,L"open",L"https://www.microsoft.com/ru-ru/edge/download", NULL, NULL, SW_NORMAL);

}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::English1Click(TObject *Sender)
{
Language="EN";
ShellExecute(0,0,Application->ExeName.c_str(),0,0,SW_SHOW); //Запускаем новую копию
Close(); //Закрываем старую
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N10Click(TObject *Sender)
{
Language="RUS";
ShellExecute(0,0,Application->ExeName.c_str(),0,0,SW_SHOW); //Запускаем новую копию
Close(); //Закрываем старую
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage111Click(TObject *Sender)
{
if (Language=="RUS") ShowMessage("Если ошибки возникают при установке какого-то одного обновления, вы можете попробовать вручную скачать его из каталога центра обновления Майкрософт. В поле поиска на странице введите номер обновления, отображаемый в центре обновлений в формате: KB1234567.");
else ShowMessage("If you see errors while installing a single update, you can try manually download it from the Microsoft Update Catalog. In the search field on the page, enter the update number displayed in the update center in the format: KB1234567.");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch27Click(TObject *Sender)
{
//wuauaserv отключение
if(upd==1){
ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c SetACL.exe -on \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wuauserv\" -ot reg -actn setowner -ownr \"n:Администраторы\"").c_str(), 0, SW_SHOWNORMAL);
	Sleep(1000);       // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\wuauserv
	EditRegistryRight("\\Registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\wuauserv [1 8 19]");
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\wuauserv",true);
		if(reg2->ReadInteger("Start")!=4) {reg2->WriteInteger("Start",4); ToggleSwitch27->State=tssOff; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net stop wuauserv").c_str(), 0, SW_HIDE);}
			else {reg2->WriteInteger("Start",2); ToggleSwitch27->State=tssOn; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net start wuauserv").c_str(), 0, SW_HIDE);}
		reg2->CloseKey();
} catch (...) {   ShowMessage("ToggleSwitch27Error");}
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToggleSwitch29Click(TObject *Sender)
{
//отключение
if(upd==1){
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\DoSvc",true);
		if(reg2->ReadInteger("Start")!=4) {reg2->WriteInteger("Start",4); ToggleSwitch29->State=tssOff; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net stop DoSvc").c_str(), 0, SW_HIDE);}
			else {reg2->WriteInteger("Start",2); ToggleSwitch29->State=tssOn; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net start DoSvc").c_str(), 0, SW_HIDE);}
		reg2->CloseKey();
} catch (...) {   ShowMessage("ToggleSwitch29Error");}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch30Click(TObject *Sender)
{
//отключение
if(upd==1){
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\WaaSMedicSvc",true);
		if(reg2->ReadInteger("Start")!=4) {reg2->WriteInteger("Start",4); ToggleSwitch30->State=tssOff; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net stop WaaSMedicSvc").c_str(), 0, SW_HIDE);}
			else {reg2->WriteInteger("Start",3); ToggleSwitch30->State=tssOn; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net start WaaSMedicSvc").c_str(), 0, SW_HIDE);}
		reg2->CloseKey();
} catch (...) {   ShowMessage("ToggleSwitch30Error");}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch34Click(TObject *Sender)
{
if(upd==1){
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\UsoSvc",true);
		if(reg2->ReadInteger("Start")!=4) {reg2->WriteInteger("Start",4); ToggleSwitch34->State=tssOff; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net stop UsoSvc").c_str(), 0, SW_HIDE);}
			else {reg2->WriteInteger("Start",2); ToggleSwitch34->State=tssOn; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net start UsoSvc").c_str(), 0, SW_HIDE);}
		reg2->CloseKey();
} catch (...) {   ShowMessage("ToggleSwitch34Error");}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleSwitch35Click(TObject *Sender)
{
if(upd==1){
	try {
		reg2->RootKey=HKEY_LOCAL_MACHINE;
		reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\BITS",true);
		if(reg2->ReadInteger("Start")!=4) {reg2->WriteInteger("Start",4); ToggleSwitch35->State=tssOff; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net stop BITS").c_str(), 0, SW_HIDE);}
			else {reg2->WriteInteger("Start",2); ToggleSwitch35->State=tssOn; ShellExecute(Handle, L"open", L"cmd.exe",String().sprintf(L"/c net start BITS").c_str(), 0, SW_HIDE);}
		reg2->CloseKey();
} catch (...) {   ShowMessage("ToggleSwitch35Error");}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button32Click(TObject *Sender)
{
				ToggleSwitch27->State=tssOn;
				ToggleSwitch29->State=tssOn;
				ToggleSwitch30->State=tssOn;
				ToggleSwitch34->State=tssOn;
				ToggleSwitch35->State=tssOn;
	if(Language=="EN") Label144->Caption="Enabled"; else 	Label144->Caption="Работает";
		Label144->Font->Color=clLime;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button34Click(TObject *Sender)
{
ShellExecute(0,L"open",L"https://www.catalog.update.microsoft.com/Home.aspx", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button33Click(TObject *Sender)
{

list->Clear();
try{DeleteFile("comand.bat");} catch(...){};
String command="";
  list->Add("wuauclt.exe /updatenow");
  list->Add("gpupdate /force");
list->SaveToFile("comand.bat");
ShellExecute(Handle, L"open", L"comand.bat",0, 0, SW_HIDE);
if(Language=="EN") ShowMessage("Done"); else ShowMessage("Готово");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn18Click(TObject *Sender)
{
ShellExecute(Handle, L"open", L"powershell.exe", L"appwiz.cpl", 0, SW_HIDE);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button35Click(TObject *Sender)
{
  list->Clear();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services",true);
reg2->GetKeyNames(list);
reg2->CloseKey();

String s;
TIniFile *ServDef = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDef.psl");

for (int i = 1; i < list->Count; i++) {
//
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\"+list->Strings[i],true);
if(reg2->ValueExists("Start")) {
s=IntToStr(reg2->ReadInteger("Start"));
ServDef->WriteString("Def",list->Strings[i],s);
}
reg2->CloseKey();
}
delete ServDef;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualImage86Click(TObject *Sender)
{
String s1="Сейчас я сброшу параметры всех служб к значениям по умолчанию. Вы уверены, что хотите продолжить?";
String s2="Сброс служб до значений по умолчанию";
if(Language=="EN") {s1="Now I will reset all services to default state. Are you sure you wanna continue?";
s2="Services default reset";}
if(MessageBox(NULL, s1.c_str(), s2.c_str(),  MB_YESNO) == IDYES)
 {
list->Clear();
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services",true);
reg2->GetKeyNames(list);
reg2->CloseKey();

String s;
TIniFile *ServDef = new TIniFile(ExtractFilePath(Application->ExeName)+"ServDef.psl");
int i1;
for (int i = 1; i < list->Count; i++) {
//
try {
reg2->RootKey=HKEY_LOCAL_MACHINE;
reg2->OpenKey("SYSTEM\\CurrentControlSet\\Services\\"+list->Strings[i],true);
if(reg2->ValueExists("Start")) {
i1=ServDef->ReadInteger("Def",list->Strings[i],2);
reg2->WriteInteger("Start",i1);
}
reg2->CloseKey();
} catch (...) { ShowMessage("Error VI86");}
}
if (Language=="EN")  ShowMessage("Restart the PC"); else ShowMessage("Перезагрузите ПК");
}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button36Click(TObject *Sender)
{
String svcname=ServGrid->Cells[0][sel];
String command="Stop-Service "+svcname+" –Force";
	ShellExecute(Handle, L"open", L"powershell.exe",command.w_str(), 0, SW_HIDE);
    if(Language=="EN") ServGrid->Cells[3][sel]="Stopped"; else ServGrid->Cells[3][sel]="Остановл.";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TabSheet2Show(TObject *Sender)
{
ComboBoxEx1Change(Sender);
}
//---------------------------------------------------------------------------

