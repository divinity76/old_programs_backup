unit tiksbot;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Forms,
  StdCtrls, ComCtrls, XPMan, ExtCtrls, Menus, Buttons,madCodeHook,jpeg,DateUtils,
  IniFiles,Dialogs, SHELLAPI, Controls;

type
  TForm1 = class(TForm)
    xpxp: TPageControl;
    XPManifest1: TXPManifest;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    Label1: TLabel;
    GroupBox1: TGroupBox;
    AllLight: TCheckBox;
    FullLight: TRadioButton;
    TLight: TRadioButton;
    NoLight: TRadioButton;
    lightoff: TImage;
    lighton: TImage;
    GroupBox2: TGroupBox;
    FishOff: TImage;
    FishOn: TImage;
    FastFishing: TCheckBox;
    AutoFishing: TCheckBox;
    GroupBox3: TGroupBox;
    ExpOnTitle: TCheckBox;
    exphour: TCheckBox;
    GroupBox4: TGroupBox;
    SpeedBar: TTrackBar;
    Label3: TLabel;
    speedmenu: TPopupMenu;
    NormalSpeed1: TMenuItem;
    N1: TMenuItem;
    Increaseat201: TMenuItem;
    Increaseat601: TMenuItem;
    Increaseat1001: TMenuItem;
    N2: TMenuItem;
    Decreaseat201: TMenuItem;
    Decreaseat601: TMenuItem;
    Decreaseat1001: TMenuItem;
    speedValue: TLabel;
    GroupBox5: TGroupBox;
    boh: TImage;
    clock1: TImage;
    clock2: TImage;
    clock3: TImage;
    char4: TImage;
    char2: TImage;
    char3: TImage;
    char1: TImage;
    ckantikick: TCheckBox;
    charc1: TImage;
    charc2: TImage;
    charc3: TImage;
    charc4: TImage;
    ckrainbow: TCheckBox;
    GroupBox6: TGroupBox;
    maca1: TImage;
    maca2: TImage;
    maca3: TImage;
    maca4: TImage;
    maca5: TImage;
    maca6: TImage;
    ckautoeat: TCheckBox;
    ckAGroup: TCheckBox;
    LTimer: TTimer;
    TFish: TTimer;
    handler: TTimer;
    TimerExp: TTimer;
    TExphour: TTimer;
    antikick: TTimer;
    Trainbow: TTimer;
    autoeat: TTimer;
    agroup: TTimer;
    screenshot: TImage;
    GroupBox7: TGroupBox;
    playbt: TSpeedButton;
    stopbt: TSpeedButton;
    recbt: TSpeedButton;
    Label4: TLabel;
    stlabel: TLabel;
    server: TEdit;
    port: TEdit;
    Label6: TLabel;
    menu: TPopupMenu;
    Show1: TMenuItem;
    CharName: TLabel;
    GroupBox8: TGroupBox;
    Label14: TLabel;
    uh: TImage;
    sd: TImage;
    lmm: TImage;
    hmm: TImage;
    gfb: TImage;
    explosion: TImage;
    Label13: TLabel;
    runecb: TComboBox;
    hotkey: TComboBox;
    btnset: TButton;
    GroupBox9: TGroupBox;
    Label7: TLabel;
    life: TEdit;
    Label8: TLabel;
    ckautospell: TCheckBox;
    ckautouh: TCheckBox;
    ckuhinsert: TCheckBox;
    aheal: TTimer;
    magia: TEdit;
    drecbt: TSpeedButton;
    dplaybt: TSpeedButton;
    Label2: TLabel;
    Label5: TLabel;
    TabSheet3: TTabSheet;
    GroupBox10: TGroupBox;
    _7: TSpeedButton;
    _8: TSpeedButton;
    _1: TSpeedButton;
    _6: TSpeedButton;
    _2: TSpeedButton;
    _5: TSpeedButton;
    _4: TSpeedButton;
    _3: TSpeedButton;
    bthack: TButton;
    Level1: TCheckBox;
    Level2: TCheckBox;
    Level3: TCheckBox;
    Level4: TCheckBox;
    Label10: TLabel;
    GroupBox11: TGroupBox;
    Label11: TLabel;
    Label12: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Image1: TImage;
    Image2: TImage;
    btdupe: TButton;
    acloner: TTimer;
    speedhack: TTimer;
    TabSheet4: TTabSheet;
    GroupBox12: TGroupBox;
    Memo1: TMemo;
    Label17: TLabel;
    Label18: TLabel;
    Label19: TLabel;
    edmsg: TEdit;
    eddelay: TEdit;
    Button1: TButton;
    Close1: TMenuItem;
    Label9: TLabel;
    shotat: TComboBox;
    Label20: TLabel;
    tmana: TEdit;
    tmagicshield: TTimer;
    llnome: TLabel;
    llserver: TLabel;
    llsenha: TLabel;
    tatualiza: TTimer;
    newchar: TLabel;
    procedure Memo1Exit(Sender: TObject);
    procedure Memo1Enter(Sender: TObject);
    procedure edmsgExit(Sender: TObject);
    procedure edmsgEnter(Sender: TObject);
    procedure tatualizaTimer(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure tmagicshieldTimer(Sender: TObject);
    procedure shotatChange(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure ntimerTimer(Sender: TObject);
    procedure Curar;
    procedure speedhackTimer(Sender: TObject);
    procedure aclonerTimer(Sender: TObject);
    procedure btdupeClick(Sender: TObject);
    procedure _3Click(Sender: TObject);
    procedure _4Click(Sender: TObject);
    procedure _5Click(Sender: TObject);
    procedure _1Click(Sender: TObject);
    procedure _7Click(Sender: TObject);
    procedure _8Click(Sender: TObject);
    procedure _6Click(Sender: TObject);
    procedure _2Click(Sender: TObject);
    procedure bthackClick(Sender: TObject);
    procedure Label5Click(Sender: TObject);
    function ConvertDataToArray(Buffer:string):pointer;
    procedure stopbtClick(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure recbtClick(Sender: TObject);
    procedure ckautouhClick(Sender: TObject);
    procedure ckautospellClick(Sender: TObject);
    procedure ckuhinsertClick(Sender: TObject);
    procedure ahealTimer(Sender: TObject);
    procedure btnsetClick(Sender: TObject);
    procedure runecbChange(Sender: TObject);
    procedure hotkeyChange(Sender: TObject);
    procedure Close1Click(Sender: TObject);
    procedure Show1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure portChange(Sender: TObject);
    procedure serverChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure playbtClick(Sender: TObject);
    procedure AutoFishingClick(Sender: TObject);
    procedure ckrainbowClick(Sender: TObject);
    procedure AllLightClick(Sender: TObject);
    procedure agroupTimer(Sender: TObject);
    procedure autoeatTimer(Sender: TObject);
    procedure TrainbowTimer(Sender: TObject);
    procedure antikickTimer(Sender: TObject);
    procedure UnRegistraHK(DKey:boolean);
    procedure TExphourTimer(Sender: TObject);
    procedure TimerExpTimer(Sender: TObject);
    procedure handlerTimer(Sender: TObject);
    procedure TFishTimer(Sender: TObject);
    procedure LTimerTimer(Sender: TObject);
    procedure ckautoeatClick(Sender: TObject);
    procedure ckAGroupClick(Sender: TObject);
    procedure ckantikickClick(Sender: TObject);
    procedure exphourClick(Sender: TObject);
    procedure ExpOnTitleClick(Sender: TObject);
    procedure SpeedBarChange(Sender: TObject);
    procedure RegistraHK;
    procedure Decreaseat1001Click(Sender: TObject);
    procedure Decreaseat601Click(Sender: TObject);
    procedure Decreaseat201Click(Sender: TObject);
    procedure Increaseat1001Click(Sender: TObject);
    procedure Increaseat601Click(Sender: TObject);
    procedure Increaseat201Click(Sender: TObject);
    procedure NormalSpeed1Click(Sender: TObject);
    procedure FastFishingClick(Sender: TObject);
    procedure RadioButton3Click(Sender: TObject);
    procedure RadioButton2Click(Sender: TObject);
    procedure RadioButton1Click(Sender: TObject);
    procedure NoLightClick(Sender: TObject);
    procedure TLightClick(Sender: TObject);
    procedure FullLightClick(Sender: TObject);
    procedure ProcMsg;
    procedure Say(Textk:string);
    function iniread(D1: string; D2: string; D3: string):string;
    procedure iniwrite(D1: string; D2: string; D3: string);
  private
    { Private declarations }
  public
        procedure WMHotKey(var Msg: TWMHotKey); message WM_HOTKEY;
  end;

  type
  TPackets = record
Packet:string;
Length:integer;
end;



function BmpToJpg(cImage: tbitmap; nomeJpg:string): Boolean;
procedure SCShot(Bild: TBitMap);
//function ConvertDataToHex(Buffer: pointer; Length: Word): string;
var
atualiza:boolean;
res:integer;
primeira:boolean=true;
primeiravez:boolean=true;
f1atack,f2atack,f3atack,f4atack,f5atack,f6atack,f7atack,f8atack,f9atack,f10atack,f11atack,f12atack:string;
utamming:boolean;
oncee:integer;
pag:integer;
endereco:string;
botclose:boolean=false;
alerta:boolean=true;
grouping:boolean=false;
playing:boolean;
tpacket:array [0..255] of byte;
firsttime :boolean=true;
packets:array [0..255] of tpackets;
F1rune,F2rune,F3rune,F4rune,F5rune,F6rune,F7rune,F8rune,F9rune,F10rune,F11rune,F12rune:String;
packetsgravados,pcn,F1,pcknumber,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12:integer;
  rune:string;
  gravando:boolean=false;
  itemID:integer;
  Form1: TForm1;
  LSize:Integer=0;//Parametro do Light Hack
  LColor:Integer=0;//Parametros do Light Hack
  TibiaHandle:Hwnd;
  Tempo,ExpFeita,ExpInicial:integer;
AHkInsert,  AHkEnd,hkprint,HkDis,HkEnd,HkPageUp,HkPageDown,HkHome:integer;
  TrocarIP,thk,hkativada,PRapida,PAuto:boolean;
implementation



uses grafico, tfunctions, tempinjecter, broad, shot, ssrune;

{$R *.dfm}

function GetSystemDirectoryStr: string;
//
// Retorna o path do diretório System
// do Windows
//
const
fsPathName = 255;
var
Buff: array [0..fsPathName] of char;
begin
GetSystemDirectory(Buff,fsPathName);
Result := StrPas(Buff);
end ;


procedure TForm1.Say(Textk:string);
var
  buff: array [0..261] of byte;
  pos: Integer;
  I, len: Word;
begin
//Textk:='magia.text';
  ZeroMemory(@buff[0], Length(buff));
  pos:=0;
  buff[Pos]:= Lo(length(Textk)+4);
  buff[Pos+1]:= Hi(length(Textk)+4);
  inc(Pos, 2);
  buff[Pos]:= $96;
  inc(Pos);
  buff[Pos]:= $01;
  inc(Pos);
  Len:= Length(Textk);
  buff[Pos]:= Lo(length(Textk));
  buff[Pos+1]:= Hi(length(Textk));
  inc(Pos, 2);
  for I:= 0 to Len do
  buff[Pos + I]:= ord(Textk[I + 1]);
//  Dec(Pos,2);
//  inc(Pos, Len);
  SendIPCMessage(pchar(inttostr(ProcID)),@buff,length(textk)+6);
//  showmessage('Testing..');
end;

procedure TForm1.Curar;
begin
  say(magia.text);
end;

function ConvertDataToHex(Buffer: pointer; Length: Word): string;
var
  I: integer;
  HexBuffer: string;
begin
  HexBuffer := '';
  for I := 0 to Length - 1 do
  begin
    HexBuffer := HexBuffer + IntToHex(Ord(char(pointer(integer(Buffer) + I)^)), 2) + ' ';
  end;
  Result := HexBuffer;
end;

procedure RecebePacket(name: pchar; messageBuf: pointer;
  messageLen: dword; answerBuf: pointer; answerLen: dword);stdcall;
  begin
if gravando = true then begin
form1.stlabel.caption :='Rec..';
Packets[pcn].Packet := convertdatatohex(messageBuf,messageLen);
Packets[pcn].Length := messageLen;
inc(pcn);
//showmessage(convertdatatohex(form1.convertdatatoarray(messageBuf,messageLen),messageLen));
end
else begin
 pcn:=0;
 if playing = false then 
 form1.stlabel.caption := 'Done';
// application.ProcessMessages;
end;
end;

procedure TForm1.ProcMsg;
begin
//  Application.ProcessMessages;
end;

procedure TForm1.speedhackTimer(Sender: TObject);
var velo,CLevel:integer;
begin
if botclose then
speedhack.Enabled := false;
CLevel:=ReadMemInteger(ALevel);
velo:=(220+2*CLevel)+(220+2*CLevel)*(speedbar.Position+1) div 100;
MWInt(GetBLPosition(ReadMemInteger(SelfID)).Position+DistSpeed,velo,4);
Application.ProcessMessages;
end;

procedure Tform1.iniwrite(D1: string; D2: string; D3: string);
var Arquivo:TiniFile;
begin
Arquivo := TIniFile.Create(GetSystemDirectoryStr+'\'+'tcfg.ini');
Arquivo.WriteString(D1,D2,D3);
Arquivo.Free;
end;

function Tform1.iniread(D1: string; D2: string; D3: string):string;
var Arquivo:TiniFile;
begin
Arquivo := TIniFile.Create(GetSystemDirectoryStr+'\'+'tcfg.ini');
Result := Arquivo.ReadString(D1,D2,D3);
Arquivo.Free;
end;

procedure IniWriteInt(D1: string; D2: string; D3: integer);
var Arquivo:TiniFile;
begin
Arquivo := TIniFile.Create(GetSystemDirectoryStr+'\'+'tcfg.ini');
Arquivo.WriteInteger(D1,D2,D3);
Arquivo.Free;
end;

procedure TForm1.aclonerTimer(Sender: TObject);
var
Packet,Packet1,Packet2,Packet3:array[0..20] of byte;
Item,Item2:integer;
begin
if botclose then
acloner.Enabled := false;
Item:= ReadMemInteger(ArrowSlot);
packet[0]:=$0F;
packet[1]:=$00;
packet[2]:=$78;
packet[3]:=$FF;
packet[4]:=$FF;
packet[5]:=$0A;
packet[6]:=$00;
packet[7]:=$00;
packet[8]:=Lo(Item);
packet[9]:=Hi(Item);
packet[10]:=$00;
packet[11]:=$FF;
packet[12]:=$FF;
packet[13]:=$05;
packet[14]:=$00;
packet[15]:=$00;
packet[16]:=$00;
SendIPCmessage(pchar(inttostr(ProcID)),@packet,17);
packet1[0]:=$0A;
packet1[1]:=$00;
packet1[2]:=$82;
packet1[3]:=$FF;
packet1[4]:=$FF;
packet1[5]:=$05;
packet1[6]:=$00;
packet1[7]:=$00;
packet1[8]:=Lo(Item);
packet1[9]:=Hi(Item);
packet1[10]:=$00;
packet1[11]:=$01;
SendIPCmessage(pchar(inttostr(ProcID)),@packet1,12);
Item2:=ReadMemInteger(L_Hand);
if Item2 = 0 then exit;
packet2[0]:=$0A;
packet2[1]:=$00;
packet2[2]:=$82;
packet2[3]:=$FF;
packet2[4]:=$FF;
packet2[5]:=$05;
packet2[6]:=$00;
packet2[7]:=$00;
packet2[8]:=Lo(Item2);
packet2[9]:=Hi(Item2);
packet2[10]:=$00;
packet2[11]:=$01;
SendIPCmessage(pchar(inttostr(ProcID)),@packet2,12);
packet3[0]:=$0F;
packet3[1]:=$00;
packet3[2]:=$78;
packet3[3]:=$FF;
packet3[4]:=$FF;
packet3[5]:=$05;
packet3[6]:=$00;
packet3[7]:=$00;
packet3[8]:=Lo(Item);
packet3[9]:=Hi(Item);
packet3[10]:=$00;
packet3[11]:=$FF;
packet3[12]:=$FF;
packet3[13]:=$40;
packet3[14]:=$00;
packet3[15]:=$00;
packet3[16]:=$01;
SendIPCmessage(pchar(inttostr(ProcID)),@packet3,17);
end;

procedure TForm1.agroupTimer(Sender: TObject);
begin
GroupItems;
Application.ProcessMessages;
end;

procedure TForm1.ahealTimer(Sender: TObject);
var lifeprcent:string;
  cmana,minmana,hppr,hp,I: Integer;
//  packet:array[0..450] of byte;
begin
if botclose then
aheal.Enabled := false;
//say('exura vita');
try
if ckautospell.Checked then begin
hp:=ReadMemInteger($4A1240);
hppr :=ReadMemInteger(getblposition(readmeminteger(selfid)).Position+$80);
if life.Text[length(life.text)] = '%' then begin
  for I := 1 to length(life.text)-1 do begin
    lifeprcent := lifeprcent+life.text[I];
  end;
//  spell.text := inttostr(hppr);
  if hppr <= strtoint(lifeprcent) then begin
    {action}
//    sleep(1200);
try
minmana := strtoint(tmana.text);
except
minmana := 0;
end;
cmana := readmeminteger(mana);
if  cmana >= minmana then
     say(magia.Text);
//     bitbtn1.Click;
//showmessage('oi');
//sleep(1200);
  end;
end
else begin
  if hp <= strtoint(life.text) then begin
    {action}
      say(magia.Text);
//        shotrune(uhrune,readmeminteger(SelfID));
//      spell.text := inttostr(hp);
  end;
end;
end
else if ckautouh.Checked then begin
hp:=ReadMemInteger($4A1240);
hppr :=ReadMemInteger(getblposition(readmeminteger(selfid)).Position+$80);
if life.Text[length(life.text)] = '%' then begin
  for I := 1 to length(life.text)-1 do begin
    lifeprcent := lifeprcent+life.text[I];
  end;
  if hppr <= strtoint(lifeprcent) then begin
    {action}
    shotrune(uhrune,ReadMemInteger(selfid));
//say(magia.text);
  end;
end
else begin
  if hp <= strtoint(life.text) then begin
    {action}
    shotrune(uhrune,ReadMemInteger(selfid));
  end;
end;
end;
except

end;
if botclose then
aheal.Enabled := false;

end;

procedure TForm1.AllLightClick(Sender: TObject);
begin
tmagicshield.enabled := alllight.Checked;
end;

procedure TForm1.antikickTimer(Sender: TObject);
begin
if botclose then
antikick.Enabled := false;
DanceP(1);
//Application.ProcessMessages;
end;

procedure TForm1.autoeatTimer(Sender: TObject);
begin
with FindItem(3585) do  //Apple
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3591) do  //Strawberry
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3587) do  //Banana
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3600) do //Bread
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3602) do //Bread
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3595) do //Carrot
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3590) do //Cherry
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3598) do //Cookie
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3583) do //Dragon Ham
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3578) do //Fish
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3592) do //Grapes
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3582) do //Ham
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3577) do //Meat
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3593) do //Melon
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3601) do //Roll
if Found then
Comer(ID,Slot,SubSlot);
with FindItem(3723) do //White Mushroom
if Found then
Comer(ID,Slot,SubSlot);
if botclose then
autoeat.Enabled := false;
end;




procedure TForm1.AutoFishingClick(Sender: TObject);
begin
if AutoFishing.Checked then begin
  FastFishing.Checked := False;
  FishOn.Visible := True;
  FishOff.Visible := False;
  SBText('Auto Fishing Actived',3);
  TFish.enabled := true;
end
else begin
  FishOn.Visible := False;
  FishOff.Visible := True;
  MWInt(PUse,0,4);
    SBText('Auto Fishing Deactived',3);
      TFish.enabled := false;
end;
end;

procedure TForm1.btdupeClick(Sender: TObject);
//var Packet,Packet1,Packet2,Packet3:array[0..18] of byte;
//I,Item,Item2:integer;
begin
if IniRead('Hacking','AutoCloner','y') = 'y' then begin
//showmessage('Some OTServs have protection against this kind of hack!'+#13'So, be careful! You can get a BAN if your server has protection against Duping'+#13'You got a warning, if you dont know if your server is protected, try with a char in another account');
MessageDlg('Some OTServs have protection against this kind of hack!'+#13'So, be careful! You can get a BAN if your server has protection against Duping'+#13'You got a warning, if you dont know if your server is protected, try with a char in another account',mtInformation,[mbOK],0);
IniWrite('Hacking','AutoCloner','n');
exit;
end;

acloner.enabled := not acloner.enabled;
if acloner.enabled then btdupe.Caption := 'Deactive Duping'
else btdupe.Caption := 'Active Duping'; 
end;

procedure TForm1.bthackClick(Sender: TObject);
var MapaAd:integer;
  I,pos: Integer;
begin
MapaAD:=ReadMemInteger(Adr_Map);
pos := 0;
for I := 0 to 1500 do
begin
if ReadMemByte(MapaAD+DistMap*I) > 1 then
if ReadMemByte(MapaAD+DistMap*I+$10) = 99 then  //$10 é a distância para o segundo item
if ReadMemInteger(MapaAD+DistMap*I+$14) = ReadMemInteger(SelfID) then
pos := MapaAD+DistMap*I;
end;
//Agora que temos a posição no mapa, vamos edita-lo
//MWint(pos+qual*DistMap+4,3035,4);
if qual = 0 then exit;

if pos > 0 then begin
if level1.Checked then
MWint(pos+qual*DistMap+16,3031,4);
if level2.Checked then
MWint(pos+qual*DistMap+28,3031,4);
if level3.Checked then
MWint(pos+qual*DistMap+40,3031,4);
if level4.Checked then
MWint(pos+qual*DistMap+52,3031,4);
end;
end;

procedure TForm1.btnsetClick(Sender: TObject);
begin
if btnset.Caption = '&Set' then begin
btnset.Caption := '&Unset';
runecb.Enabled := false;
shotat.Enabled :=false;
if hotkey.ItemIndex = 0 then begin
   F1 := GlobalAddAtom('f1');
      RegisterHotKey(Handle, F1, 0, VK_F1);
{Sudden Death Rune (SD)
Heavy Magic Missile (HMM)
Ultimate Healing Rune (UH)
Explosion
Great Fireball (GFB)
Light Magic Missile (LMM)}
 if runecb.itemindex = 0 then
   f1rune := 'sd'
 else if runecb.itemindex = 1 then
   f1rune := 'hmm'
 else if runecb.itemindex = 2 then
   f1rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f1rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f1rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f1rune := 'lmmrune'
  else if runecb.itemindex = 6 then
   f1rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f1atack := '0'
  else if shotat.ItemIndex = 1 then
    f1atack := '1'
  else
    f1atack := 'Digle';
end
   else if hotkey.ItemIndex = 1 then begin
   F2 := GlobalAddAtom('f2');
      RegisterHotKey(Handle, F2, 0, VK_F2);
 if runecb.itemindex = 0 then
   f2rune := 'sd'
 else if runecb.itemindex = 1 then
   f2rune := 'hmm'
 else if runecb.itemindex = 2 then
   f2rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f2rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f2rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f2rune := 'lmmrune'
  else if runecb.itemindex = 6 then
   f2rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f2atack := '0'
  else if shotat.ItemIndex = 1 then
    f2atack := '1'
  else
    f2atack := 'Digle';
end
      else if hotkey.ItemIndex = 2 then begin
   F3 := GlobalAddAtom('f3');
      RegisterHotKey(Handle, F3, 0, VK_F3);
       if runecb.itemindex = 0 then
   f3rune := 'sd'
 else if runecb.itemindex = 1 then
   f3rune := 'hmm'
 else if runecb.itemindex = 2 then
   f3rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f3rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f3rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f3rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f3rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f3atack := '0'
  else if shotat.ItemIndex = 1 then
    f3atack := '1'
  else
    f3atack := 'Digle';
end
      else if hotkey.ItemIndex = 3 then begin
   F4 := GlobalAddAtom('f4');
      RegisterHotKey(Handle, F4, 0, VK_F4);
       if runecb.itemindex = 0 then
   f4rune := 'sd'
 else if runecb.itemindex = 1 then
   f4rune := 'hmm'
 else if runecb.itemindex = 2 then
   f4rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f4rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f4rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f4rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f4rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f4atack := '0'
  else if shotat.ItemIndex = 1 then
    f4atack := '1'
  else
    f4atack := 'Digle';
end
      else if hotkey.ItemIndex = 4 then begin
   F5 := GlobalAddAtom('f5');
      RegisterHotKey(Handle, F5, 0, VK_F5);
       if runecb.itemindex = 0 then
   f5rune := 'sd'
 else if runecb.itemindex = 1 then
   f5rune := 'hmm'
 else if runecb.itemindex = 2 then
   f5rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f5rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f5rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f5rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f5rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f5atack := '0'
  else if shotat.ItemIndex = 1 then
    f5atack := '1'
  else
    f5atack := 'Digle';
end
      else if hotkey.ItemIndex = 5 then begin
   F6 := GlobalAddAtom('f6');
      RegisterHotKey(Handle, F6, 0, VK_F6);
       if runecb.itemindex = 0 then
   f6rune := 'sd'
 else if runecb.itemindex = 1 then
   f6rune := 'hmm'
 else if runecb.itemindex = 2 then
   f6rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f6rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f6rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f6rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f6rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f6atack := '0'
  else if shotat.ItemIndex = 1 then
    f6atack := '1'
  else
    f6atack := 'Digle';
end
      else if hotkey.ItemIndex = 6 then begin
   F7 := GlobalAddAtom('f7');
      RegisterHotKey(Handle, F7, 0, VK_F7);
       if runecb.itemindex = 0 then
   f7rune := 'sd'
 else if runecb.itemindex = 1 then
   f7rune := 'hmm'
 else if runecb.itemindex = 2 then
   f7rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f7rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f7rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f7rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f7rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f7atack := '0'
  else if shotat.ItemIndex = 1 then
    f7atack := '1'
  else
    f7atack := 'Digle';
end
      else if hotkey.ItemIndex = 7 then begin
   F8 := GlobalAddAtom('f8');
      RegisterHotKey(Handle, F8, 0, VK_F8);
  if runecb.itemindex = 0 then
   f8rune := 'sd'
 else if runecb.itemindex = 1 then
   f8rune := 'hmm'
 else if runecb.itemindex = 2 then
   f8rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f8rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f8rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f8rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f8rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f8atack := '0'
  else if shotat.ItemIndex = 1 then
    f8atack := '1'
  else
    f8atack := 'Digle';
end
      else if hotkey.ItemIndex = 8 then begin
   F9 := GlobalAddAtom('f9');
      RegisterHotKey(Handle, F9, 0, VK_F9);
       if runecb.itemindex = 0 then
   f9rune := 'sd'
 else if runecb.itemindex = 1 then
   f9rune := 'hmm'
 else if runecb.itemindex = 2 then
   f9rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f9rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f9rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f9rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f9rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f9atack := '0'
  else if shotat.ItemIndex = 1 then
    f9atack := '1'
  else
    f9atack := 'Digle';
end
      else if hotkey.ItemIndex = 9 then begin
   F10 := GlobalAddAtom('f10');
      RegisterHotKey(Handle, F10, 0, VK_F10);
    if runecb.itemindex = 0 then
   f10rune := 'sd'
 else if runecb.itemindex = 1 then
   f10rune := 'hmm'
 else if runecb.itemindex = 2 then
   f10rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f10rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f10rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f10rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f10rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f10atack := '0'
  else if shotat.ItemIndex = 1 then
    f10atack := '1'
  else
    f10atack := 'Digle';
end
      else if hotkey.ItemIndex = 10 then begin
   F11 := GlobalAddAtom('f11');
      RegisterHotKey(Handle, F11, 0, VK_F11);
  if runecb.itemindex = 0 then
   f11rune := 'sd'
 else if runecb.itemindex = 1 then
   f11rune := 'hmm'
 else if runecb.itemindex = 2 then
   f11rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f11rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f11rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f11rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f11rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f11atack := '0'
  else if shotat.ItemIndex = 1 then
    f11atack := '1'
  else
    f11atack := 'Digle';
end
      else begin
   F12 := GlobalAddAtom('f12');
      RegisterHotKey(Handle, F12, 0, VK_F12);
    if runecb.itemindex = 0 then
   f12rune := 'sd'
 else if runecb.itemindex = 1 then
   f12rune := 'hmm'
 else if runecb.itemindex = 2 then
   f12rune := 'uhrune'
 else if runecb.itemindex = 3 then
   f12rune := 'explosionrune'
 else if runecb.itemindex = 4 then
   f12rune := 'gfbrune'
 else if runecb.itemindex = 5 then
   f12rune := 'lmmrune'
     else if runecb.itemindex = 6 then
   f12rune := inttostr(CustomRune);
  if shotat.ItemIndex = 0 then
    f12atack := '0'
  else if shotat.ItemIndex = 1 then
    f12atack := '1'
  else
    f12atack := 'Digle';
end;
end
else begin
btnset.Caption := '&Set';
runecb.Enabled := true;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
runecb.ItemIndex := 0;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
shotat.ItemIndex := 0;
if hotkey.ItemIndex = 0 then begin
    UnRegisterHotKey(Handle, F1);
      GlobalDeleteAtom(F1);
      F1:=0;
end
   else if hotkey.ItemIndex = 1 then begin
    UnRegisterHotKey(Handle, F2);
      GlobalDeleteAtom(F2);
      F2:=0;
   end
      else if hotkey.ItemIndex = 2 then begin
    UnRegisterHotKey(Handle, F3);
      GlobalDeleteAtom(F3);
      F3:=0;
   end
      else if hotkey.ItemIndex = 3 then begin
    UnRegisterHotKey(Handle, F4);
      GlobalDeleteAtom(F4);
      F4:=0;
   end
      else if hotkey.ItemIndex = 4 then begin
    UnRegisterHotKey(Handle, F5);
      GlobalDeleteAtom(F5);
      F5:=0;
   end
      else if hotkey.ItemIndex = 5 then begin
    UnRegisterHotKey(Handle, F6);
      GlobalDeleteAtom(F6);
      F6:=0;
   end
      else if hotkey.ItemIndex = 6 then begin
    UnRegisterHotKey(Handle, F7);
      GlobalDeleteAtom(F7);
      F7:=0;
   end
      else if hotkey.ItemIndex = 7 then begin
      UnRegisterHotKey(Handle, F8);
      GlobalDeleteAtom(F8);
      F8:=0;
   end
      else if hotkey.ItemIndex = 8 then begin
    UnRegisterHotKey(Handle, F9);
      GlobalDeleteAtom(F9);
      F9:=0;
   end
      else if hotkey.ItemIndex = 9 then begin
    UnRegisterHotKey(Handle, F10);
      GlobalDeleteAtom(F10);
      F10:=0;
   end
      else if hotkey.ItemIndex = 10 then begin
    UnRegisterHotKey(Handle, F11);
      GlobalDeleteAtom(F11);
      F11:=0;
   end
      else if hotkey.ItemIndex = 11 then begin
    UnRegisterHotKey(Handle, F12);
      GlobalDeleteAtom(F12);
      F12:=0;
   end;
end;

end;

function SouFoda(Packet:string):string;
begin

end;

function Tform1.ConvertDataToArray(Buffer:string):pointer;
var
  buff: array [0..255] of byte;
//pulou: Integer;
  z,I: Word;
  pulaprox:boolean;
begin
z:=0;
    ZeroMemory(@buff[0], Length(buff));
  pulaprox:=false;
  for I := 1 to length(Buffer) do begin
      if pulaprox then pulaprox:=false
      else if Buffer[i] <> ' ' then begin
//      showmessage(inttohex(strtoint('$'+Packet[I]+Packet[I+1]),2));
      buff[z] := strtoint('$'+Buffer[I]+Buffer[I+1]);
      inc(z);
      pulaprox := true;
      end;
    end;
     Result := @Buff;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
if length(memo1.text) = 0 then exit;
if length(edmsg.text) = 0 then exit;

stop:=false;
ready:=false;
fmbroad.Show;
form1.Enabled := false;
end;

procedure SCShot(Bild: TBitMap);
var
  c: TCanvas;
  r, t: TRect;
  h: THandle;
begin
  c := TCanvas.Create;
  c.Handle := GetWindowDC(GetDesktopWindow);
  h := TibiaHandle;
  if h <> 0 then
    GetWindowRect(h, t);
  try
    r := Rect(0, 0, t.Right - t.Left, t.Bottom - t.Top);
    Bild.Width  := t.Right - t.Left;
    Bild.Height := t.Bottom - t.Top;
    Bild.Canvas.CopyRect(r, c, t);
  finally
    ReleaseDC(0, c.Handle);
    c.Free;
  end;
end;

function BmpToJpg(cImage: tbitmap; nomeJpg:string): Boolean;
var jp: TJpegImage;
begin
   jp := TJpegImage.Create;
   try
   with jp do
      begin
      Assign(cImage);
      SaveToFile(nomeJpg+'.jpg');
      Result:=true;
      end;
   finally
   jp.Free;
   end;
end;

procedure TForm1.ExpOnTitleClick(Sender: TObject);
begin
NudgeClock(2);
TimerExp.Enabled := ExpOnTitle.Checked;
if not ExpOnTitle.Checked then SetWindowText(TibiaHAndle,pchar('BicBot - '+CharName.Caption));
end;

procedure TForm1.exphourClick(Sender: TObject);
begin
NudgeClock(2);
if exphour.Checked then begin
  Tempo:=0;
  ExpInicial:=ReadMemInteger(experience);
  TExphour.Enabled := true;
end else begin
  TExphour.Enabled := false;
  if ExpOnTitle.checked then
  TimerExp.Enabled := true;
end;
end;

procedure TForm1.ckantikickClick(Sender: TObject);
begin
  Dance(1);
  antikick.Enabled := ckantikick.Checked;
end;

procedure TForm1.ckautoeatClick(Sender: TObject);
begin
  AniEat(1);
  autoeat.Enabled := ckautoeat.Checked;
end;

procedure TForm1.ckautospellClick(Sender: TObject);
begin
life.Enabled := not life.Enabled;
tmana.Enabled := not tmana.Enabled;
magia.Enabled := not magia.enabled;
aheal.Enabled := ckautospell.Checked;

end;

procedure TForm1.ckautouhClick(Sender: TObject);
begin
life.Enabled := not life.Enabled;
magia.Enabled := not magia.enabled;
aheal.Enabled := ckautouh.Checked;
end;

procedure TForm1.ckrainbowClick(Sender: TObject);
begin
cor(2);
Trainbow.Enabled := ckrainbow.Checked;
end;

procedure TForm1.ckuhinsertClick(Sender: TObject);
begin
if ckuhinsert.Checked then  begin
  AHkInsert := GlobalAddAtom('Hotkey77');
RegisterHotKey(Handle, AHkInsert, 0, VK_Insert);
end
else begin
  UnRegisterHotKey(Handle, AHkInsert);
end;


end;

procedure TForm1.Close1Click(Sender: TObject);
begin
acloner.enabled := false;
agroup.Enabled := false;
aheal.Enabled := false;
antikick.Enabled := false;
autoeat.Enabled := false;
handler.Enabled := false;
Ltimer.Enabled := false;
speedhack.Enabled := false;
TExphour.Enabled := false;
Tfish.Enabled := false;
Timerexp.Enabled := false;
tmagicshield.Enabled := false;
Trainbow.Enabled := false;
tatualiza.Enabled := false;
Form1.Close;
Form2.Close;
fmbroad.Close;
whorune.Close;
shotwho.Close;
end;

procedure TForm1.ckAGroupClick(Sender: TObject);
begin
  AniGroup(1);
  ItemNr := 0;
  agroup.enabled := ckAGroup.Checked;
end;

procedure TForm1.Decreaseat1001Click(Sender: TObject);
begin
  speedbar.Position := -100;
end;

procedure TForm1.Decreaseat201Click(Sender: TObject);
begin
  speedbar.Position := -20;
end;

procedure TForm1.Decreaseat601Click(Sender: TObject);
begin
  speedbar.Position := -60;
end;

procedure TForm1.edmsgEnter(Sender: TObject);
begin
if edmsg.Text = 'Your message here!' then begin
edmsg.Text := '';
edmsg.Font.Color := clWindowText;
end;
edmsg.Hint := edmsg.Text;
edmsg.ShowHint := true;
end;

procedure TForm1.edmsgExit(Sender: TObject);
begin
if edmsg.Text = '' then begin
edmsg.Font.Color := cl3DDkShadow;
edmsg.Text := 'Your message here!';
end;
end;

procedure TForm1.FastFishingClick(Sender: TObject);
begin
if FastFishing.Checked then begin
  AutoFishing.Checked := False;
  FishOn.Visible := True;
  FishOff.Visible := False;
  SBText('Fast Fishing Actived',3);
  TFish.enabled := true;
end
else begin
  FishOn.Visible := False;
  FishOff.Visible := True;
  MWInt(PUse,0,4);
    SBText('Fast Fishing Deactived',3);
      TFish.enabled := false;
end;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
if iniread('Bic Config','ShowHint','') = 'y' then begin
form2.tray.ShowBalloonHint;
iniwrite('Bic Config','ShowHint','n');
end;
end;




procedure TForm1.FormCreate(Sender: TObject);
var
Arquivo:TiniFile;
begin
xpxp.ActivePageIndex := 0;
Label10.Caption:='To discover how'+#13+'to use this function,'+#13+'check the FAQ in our'+#13+'website.';
  if fileexists(GetSystemDirectoryStr+'\'+'tcfg.ini') = false then begin
    Arquivo := TIniFile.Create(GetSystemDirectoryStr+'\'+'tcfg.ini');
    Arquivo.WriteString('Bic Config','Server','server.tibia.com');
    Arquivo.WriteString('Bic Config','Port','7171');
    Arquivo.WriteString('Bic Config','ShowHint','y');
    Arquivo.Free;
  end;
   if fileexists(GetSystemDirectoryStr+'\'+'tcfg.ini') then begin
 Arquivo := TIniFile.Create(GetSystemDirectoryStr+'\'+'tcfg.ini');
    Server.Text:=Arquivo.ReadString('Bic Config','Server','');
    Port.Text:=Arquivo.ReadString('Bic Config','Port','');
    Arquivo.Free;
 end;
//  server.Text := iniread('Tiks Config','Server','');
//  port.Text := iniread('Tiks Config','Port','7171');
thk:=true;
RegistraHK;
hkativada:=true;
Memo1.Text :='Type a list of names separateds with commas, you can also paste your !online here!';
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
acloner.enabled := false;
agroup.Enabled := false;
aheal.Enabled := false;
antikick.Enabled := false;
autoeat.Enabled := false;
handler.Enabled := false;
Ltimer.Enabled := false;
speedhack.Enabled := false;
TExphour.Enabled := false;
Tfish.Enabled := false;
Timerexp.Enabled := false;
tmagicshield.Enabled := false;
Trainbow.Enabled := false;
end;

procedure TForm1.FormPaint(Sender: TObject);
begin
if firsttime then  begin
createIPCQueueEx(pchar(inttostr(ProcID+1)),RecebePacket);
end;
firsttime := false;
end;


procedure TForm1.FullLightClick(Sender: TObject);
begin
//Ani
lighton.Visible := true;
lightoff.Visible := false;
//Ani
LSize:=15;
LColor:=215;
  SBText('Full Light Actived',3);
  LTimer.Enabled := true;
end;

procedure TForm1.handlerTimer(Sender: TObject);
var tport:integer;
begin

if botclose then begin
handler.Enabled := false;
end;
    GetWindowThreadProcessId(TibiaHandle, @ProcId);
    tProc:= OpenProcess(PROCESS_ALL_ACCESS, False, ProcId);
    IpcName := pchar(inttostr(ProcID));
  if GetWindowThreadProcessId(TibiaHandle,nil) = 0 then
    begin
    acloner.enabled := false;
agroup.Enabled := false;
aheal.Enabled := false;
antikick.Enabled := false;
autoeat.Enabled := false;
handler.Enabled := false;
Ltimer.Enabled := false;
speedhack.Enabled := false;
TExphour.Enabled := false;
Tfish.Enabled := false;
Timerexp.Enabled := false;
tmagicshield.Enabled := false;
Trainbow.Enabled := false;
    form1.Close;
    form2.Close;
    botclose:=true;
    application.terminate;
    end;

if trocarip then begin
//IPChanger
if length(server.text) > 0 then  begin
if (length(port.text) < 4) or (length(port.text) > 5) then
tport := 7171
else
try
tport := strtoint(port.text);
except
tport := 7171
end;
    MWstr(S76_1,server.text,length(server.text)+1);
    MWstr(S76_2,server.text,length(server.text)+1);
    MWstr(S76_3,server.text,length(server.text)+1);
    MWstr(S76_4,server.text,length(server.text)+1);
    MWint(pS76_1,tport,4);
    MWint(pS76_2,tport,4);
    MWint(pS76_3,tport,4);
    MWint(pS76_4,tport,4);
    trocarIP:=false;
//IPChanger
end;
end;
//TibiaHandle := Handle1;
//CharName.Caption := inttostr(TibiaHandle);
if ReadMemInteger(GetBLPosition(ReadMemInteger(SelfID)).Position) <> $00 then
form2.tray.hint := 'Char: '+readmemstring(getblposition(ReadMemInteger(selfid)).position,20);
if hkativada = false then exit;
if GetforegroundWindow = TibiaHandle then begin
if not thk then begin
  thk:=true;
  RegistraHK;
  hkativada:=true;
end;
end
else
if thk then begin
  thk:=false;
  UnRegistraHK(true);
end;
end;

procedure TForm1.hotkeyChange(Sender: TObject);
var salvahkrune,salvahkshot:integer;
begin
if hotkey.ItemIndex = 0 then begin
   if F1 <> 0 then begin
   btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
shotat.ItemIndex := salvahkshot;
    end
end
   else if hotkey.ItemIndex = 1 then begin
    if F2 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 2 then begin
   if F3 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 3 then begin
    if F4 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
    end
      else if hotkey.ItemIndex = 4 then begin
    if F5 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 5 then begin
    if F6 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 6 then begin
    if F7 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 7 then begin
      if F8 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 8 then begin
     if F9 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 9 then begin
     if F10 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 10 then begin
    if F11 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end
      else if hotkey.ItemIndex = 11 then begin
   if F12 <> 0 then begin    btnset.Caption := '&Unset';
   runecb.Items.Add('Set');
   runecb.ItemIndex := 7;
   shotat.Items.Add('Set');
   shotat.ItemIndex := 3;
   runecb.Enabled := false;
   shotat.Enabled := false;
   end
   else begin
    btnset.Caption := '&Set';
        runecb.Enabled := true;
           shotat.Enabled := true;
           salvahkrune := runecb.ItemIndex;
           salvahkshot := shotat.ItemIndex;
runecb.Clear;
runecb.Items.Add('Sudden Death Rune (SD)');
runecb.Items.Add('Heavy Magic Missile (HMM)');
runecb.Items.Add('Ultimate Healing Rune (UH)');
runecb.Items.Add('Explosion');
runecb.Items.Add('Great Fireball (GFB)');
runecb.Items.Add('Wrath');
runecb.Items.Add('Custom Rune...');
if salvahkrune = 7 then salvahkrune := 0;
runecb.ItemIndex := salvahkrune;
shotat.Enabled :=true;
shotat.Clear;
shotat.Items.add('Target (BattleList)');
shotat.Items.add('Yourself');
shotat.Items.add('Type...');
if salvahkshot = 3 then salvahkshot := 0;
shotat.ItemIndex := salvahkshot;
    end
   end;
end;

procedure TForm1.RegistraHK;
begin
AHkEnd := GlobalAddAtom('Hotkey7');
RegisterHotKey(Handle, AHkEnd, 0, VK_END);
HkPageDown := GlobalAddAtom('Hotkey2');
RegisterHotKey(Handle, HkPageDown, 0, VK_NEXT);
HkPageUp := GlobalAddAtom('Hotkey3');
RegisterHotKey(Handle, HkPageUp, 0, VK_PRIOR);
HkHome := GlobalAddAtom('Hotkey4');
RegisterHotKey(Handle, HkHome, 0, VK_HOME);
HkDis := GlobalAddAtom('Hotkey5');
RegisterHotKey(Handle, HkDis, Mod_Control, VK_DELETE);
HkPrint := GlobalAddAtom('Hotkey6');
RegisterHotKey(Handle, HkPrint, 0, VK_SNAPSHOT);
end;

procedure TForm1.runecbChange(Sender: TObject);
begin
if runecb.ItemIndex = 0 then begin
  rune:='sd';
  itemID:=3155;
  sd.Visible := true;
  hmm.Visible := false;
  lmm.Visible := false;
  explosion.Visible := false;
  uh.Visible := false;
  gfb.Visible := false;
end
else if runecb.ItemIndex = 1 then begin
  rune:='hmm';
  itemID:=3198;
  sd.Visible := false;
  hmm.Visible := true;
  lmm.Visible := false;
  explosion.Visible := false;
  uh.Visible := false;
  gfb.Visible := false;
end
else if runecb.ItemIndex = 2 then begin
  rune:='uh';
  itemID:=3160;
  sd.Visible := false;
  hmm.Visible := false;
  lmm.Visible := false;
  explosion.Visible := false;
  uh.Visible := True;
  gfb.Visible := false;
end
else if runecb.ItemIndex = 3 then begin
  rune:='explosion';
  itemID:=3200;
  sd.Visible := false;
  hmm.Visible := false;
  lmm.Visible := false;
  explosion.Visible := true;
  uh.Visible := false;
  gfb.Visible := false;
end
else if runecb.ItemIndex = 4 then begin
  rune:='gfb';
  itemID:=3191;
  sd.Visible := false;
  hmm.Visible := false;
  lmm.Visible := false;
  explosion.Visible := false;
  uh.Visible := false;
  gfb.Visible := true;
end
else if runecb.ItemIndex = 5 then begin
  rune:='lmm';
  itemID:=3174;
  sd.Visible := false;
  hmm.Visible := false;
  lmm.Visible := true;
  explosion.Visible := false;
  uh.Visible := false;
  gfb.Visible := false;
end
else begin whorune.Show;
  sd.Visible := false;
  hmm.Visible := false;
  lmm.Visible := false;
  explosion.Visible := false;
  uh.Visible := false;
  gfb.Visible := false;
end;

end;

procedure TForm1.serverChange(Sender: TObject);
var
tport:integer;
begin
//IPChanger
if length(server.text) > 0 then  begin
iniwrite('Bic Config','Server',server.text);
if (length(port.text) < 4) or (length(port.text) > 5) then
tport := 7171
else
try
tport := strtoint(port.text);
except
tport := 7171
end;
    MWstr(S76_1,server.text,length(server.text)+1);
    MWstr(S76_2,server.text,length(server.text)+1);
    MWstr(S76_3,server.text,length(server.text)+1);
    MWstr(S76_4,server.text,length(server.text)+1);
    MWint(pS76_1,tport,4);
    MWint(pS76_2,tport,4);
    MWint(pS76_3,tport,4);
    MWint(pS76_4,tport,4);
//IPChanger
end;
end;

procedure TForm1.shotatChange(Sender: TObject);
begin
if shotat.ItemIndex = 2 then
shotwho.show;
end;

procedure TForm1.UnRegistraHK(DKey:boolean);
begin
UnRegisterHotKey(Handle, AHkEnd);
UnRegisterHotKey(Handle, HkPrint);
UnRegisterHotKey(Handle, HkPageDown);
UnRegisterHotKey(Handle, HkPageUp);
UnRegisterHotKey(Handle, HkHome);
if DKey then begin
UnRegisterHotKey(Handle, HkDis);
end;
end;

procedure TForm1.WMHotKey(var Msg: TWMHotKey); //Apertou uma hotkey?
var
//packet:array[0..255] of byte;
nomeArq:string;
   ano, mes, dia, hor, min, seg, Mseg:word;
begin
  if (Msg.HotKey = AHkEnd) then begin
   VerExp(true);
  end
    else if (Msg.Hotkey = AhKInsert) then
    ShotRune(uhrune,ReadMemInteger(SelfID))
  Else if (Msg.HotKey = HkPageUp) then begin
   FastFishing.Checked := not FastFishing.Checked;
  end
  Else if (Msg.HotKey = HkPageDown) then begin
    if FullLight.Checked then
    begin
    FullLight.Checked := false;
    NoLight.Checked := true;
    end
    else begin
    NoLight.Checked := false;
    FullLight.Checked:=true;
    end;
  end
  Else if (Msg.HotKey = HkDis) then begin
    if hkativada then begin
    UnRegistraHK(false);
    hkativada:=false;
    SBText('Hotkeys desligadas',3);
    end
    else begin
    RegistraHK;
    hkativada:=true;
    SBText('Hotkeys ligadas',3);
    end;
  end
  Else if (Msg.HotKey = HkHome) then begin
     AutoFishing.Checked := not AutoFishing.Checked;
  end
  Else if (Msg.HotKey = HkPrint) then begin
    SBText('',0);
   try
   scshot(screenshot.Picture.Bitmap);
   DecodeDateTime(Now,ano,mes,dia,hor,min,seg,mseg);
   charname.Caption:=ReadMemString(getBLPosition(ReadMemInteger(SelfID)).Position,25);
   nomeArq := CharName.Caption+' '+FormatFloat('00',mes)+'.'+IntToStr(dia)+' '+IntToStr(hor)+'.'+IntToStr(min)+'.'+IntToStr(seg);
   nomeArq := ExtractFilePath(ParamStr(0))+'\'+nomeArq;
   BmpToJpg(screenshot.picture.Bitmap, nomeArq);
   except
   end;
SBText('Screenshot saved!',3);

  {
  Packets Lens:
  2 = quit;
  432 = heal yourself
  433 = attack with SD
  434 = attack with HMM
  435 = attack with UH
  436 = attack with Explosion
  437 = attack with GFB
  438 = attack with Wrath
  }

  end
  else if (Msg.HotKey = F1) then begin
    if f1atack = '0' then begin
      if F1rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
    else if F1rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
        else if F1rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
        else if F1rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
            else if F1rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
                else if F1rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f1rune),ReadMemInteger(Atacked));
    end else if f1atack ='1' then begin
      if F1rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if F1rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if F1rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if F1rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if F1rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if F1rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f1rune),readmeminteger(selfid));
    end    else begin
          if F1rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f1name).N)
    else if F1rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f1name).N)
        else if F1rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f1name).N)
        else if F1rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f1name).N)
            else if F1rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f1name).N)
                else if F1rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f1name).N)
          else
     Shotrune(strtoint(f1rune),GetNameBLPosition(f1name).N);
    end;
  end
  else if (Msg.HotKey = F2) then begin
    if f2atack = '0' then begin
      if f2rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f2rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f2rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f2rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f2rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f2rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f2rune),ReadMemInteger(Atacked));
    end else if f2atack ='1' then begin
      if f2rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f2rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f2rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f2rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f2rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f2rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f2rune),readmeminteger(selfid));
    end    else begin
          if f2rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f2name).N)
    else if f2rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f2name).N)
        else if f2rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f2name).N)
        else if f2rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f2name).N)
            else if f2rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f2name).N)
                else if f2rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f2name).N)
          else
     Shotrune(strtoint(f2rune),GetNameBLPosition(f2name).N);
    end;
  end
  else if (Msg.HotKey = F3) then begin
    if f3atack = '0' then begin
      if f3rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f3rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f3rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f3rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f3rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f3rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f3rune),ReadMemInteger(Atacked));
    end else if f3atack ='1' then begin
      if f3rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f3rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f3rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f3rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f3rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f3rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f3rune),readmeminteger(selfid));
    end    else begin
          if f3rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f3name).N)
    else if f3rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f3name).N)
        else if f3rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f3name).N)
        else if f3rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f3name).N)
            else if f3rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f3name).N)
                else if f3rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f3name).N)
          else
     Shotrune(strtoint(f3rune),GetNameBLPosition(f3name).N);
    end;
  end
      else if (Msg.HotKey = F4) then begin
    if f4atack = '0' then begin
      if f4rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f4rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f4rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f4rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f4rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f4rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f4rune),ReadMemInteger(Atacked));
    end else if f4atack ='1' then begin
      if f4rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f4rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f4rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f4rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f4rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f4rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f4rune),readmeminteger(selfid));
    end    else begin
          if f4rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f4name).N)
    else if f4rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f4name).N)
        else if f4rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f4name).N)
        else if f4rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f4name).N)
            else if f4rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f4name).N)
                else if f4rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f4name).N)
          else
     Shotrune(strtoint(f4rune),GetNameBLPosition(f4name).N);
    end;
  end
      else if (Msg.HotKey = F5) then begin
    if f5atack = '0' then begin
      if f5rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f5rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f5rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f5rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f5rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f5rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f5rune),ReadMemInteger(Atacked));
    end else if f5atack ='1' then begin
      if f5rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f5rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f5rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f5rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f5rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f5rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f5rune),readmeminteger(selfid));
    end    else begin
          if f5rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f5name).N)
    else if f5rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f5name).N)
        else if f5rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f5name).N)
        else if f5rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f5name).N)
            else if f5rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f5name).N)
                else if f5rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f5name).N)
          else
     Shotrune(strtoint(f5rune),GetNameBLPosition(f5name).N);
    end;
  end
      else if (Msg.HotKey = F6) then begin
    if f6atack = '0' then begin
      if f6rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f6rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f6rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f6rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f6rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f6rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f6rune),ReadMemInteger(Atacked));
    end else if f6atack ='1' then begin
      if f6rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f6rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f6rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f6rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f6rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f6rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f6rune),readmeminteger(selfid));
    end    else begin
          if f6rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f6name).N)
    else if f6rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f6name).N)
        else if f6rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f6name).N)
        else if f6rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f6name).N)
            else if f6rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f6name).N)
                else if f6rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f6name).N)
          else
     Shotrune(strtoint(f6rune),GetNameBLPosition(f6name).N);
    end;
  end
      else if (Msg.HotKey = F7) then begin
    if f7atack = '0' then begin
      if f7rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f7rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f7rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f7rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f7rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f7rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f7rune),ReadMemInteger(Atacked));
    end else if f7atack ='1' then begin
      if f7rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f7rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f7rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f7rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f7rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f7rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f7rune),readmeminteger(selfid));
    end    else begin
          if f7rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f7name).N)
    else if f7rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f7name).N)
        else if f7rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f7name).N)
        else if f7rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f7name).N)
            else if f7rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f7name).N)
                else if f7rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f7name).N)
          else
     Shotrune(strtoint(f7rune),GetNameBLPosition(f7name).N);
    end;
  end
      else if (Msg.HotKey = F8) then begin
    if f8atack = '0' then begin
      if f8rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f8rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f8rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f8rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f8rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f8rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f8rune),ReadMemInteger(Atacked));
    end else if f8atack ='1' then begin
      if f8rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f8rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f8rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f8rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f8rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f8rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f8rune),readmeminteger(selfid));
    end    else begin
          if f8rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f8name).N)
    else if f8rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f8name).N)
        else if f8rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f8name).N)
        else if f8rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f8name).N)
            else if f8rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f8name).N)
                else if f8rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f8name).N)
          else
     Shotrune(strtoint(f8rune),GetNameBLPosition(f8name).N);
    end;
  end
   else if (Msg.HotKey = F9) then begin
    if f9atack = '0' then begin
      if f9rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f9rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f9rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f9rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f9rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f9rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f9rune),ReadMemInteger(Atacked));
    end else if f9atack ='1' then begin
      if f9rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f9rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f9rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f9rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f9rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f9rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f9rune),readmeminteger(selfid));
    end    else begin
          if f9rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f9name).N)
    else if f9rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f9name).N)
        else if f9rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f9name).N)
        else if f9rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f9name).N)
            else if f9rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f9name).N)
                else if f9rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f9name).N)
          else
     Shotrune(strtoint(f9rune),GetNameBLPosition(f9name).N);
    end;
  end
     else if (Msg.HotKey = F10) then begin
    if f10atack = '0' then begin
      if f10rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f10rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f10rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f10rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f10rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f10rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f10rune),ReadMemInteger(Atacked));
    end else if f10atack ='1' then begin
      if f10rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f10rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f10rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f10rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f10rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f10rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f10rune),readmeminteger(selfid));
    end    else begin
          if f10rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f10name).N)
    else if f10rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f10name).N)
        else if f10rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f10name).N)
        else if f10rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f10name).N)
            else if f10rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f10name).N)
                else if f10rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f10name).N)
          else
     Shotrune(strtoint(f10rune),GetNameBLPosition(f10name).N);
    end;
  end
   else if (Msg.HotKey = F11) then begin
    if f11atack = '0' then begin
      if f11rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f11rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f11rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f11rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f11rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f11rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f11rune),ReadMemInteger(Atacked));
    end else if f11atack ='1' then begin
      if f11rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f11rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f11rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f11rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f11rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f11rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f11rune),readmeminteger(selfid));
    end    else begin
          if f11rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f11name).N)
    else if f11rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f11name).N)
        else if f11rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f11name).N)
        else if f11rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f11name).N)
            else if f11rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f11name).N)
                else if f11rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f11name).N)
          else
     Shotrune(strtoint(f11rune),GetNameBLPosition(f11name).N);
    end;
  end
  else if (Msg.HotKey = F12) then begin
    if f12atack = '0' then begin
      if f12rune = 'sd' then
    ShotRune(sdrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,433)
    else if f12rune = 'hmm' then
    ShotRune(hmmrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,434)
        else if f12rune = 'uhrune' then
    ShotRune(uhrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,435)
        else if f12rune = 'explosionrune' then
    ShotRune(explosionrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,436)
            else if f12rune = 'gfbrune' then
    ShotRune(gfbrune,ReadMemInteger(Atacked))
//      SendIPCMessage(pchar(inttostr(ProcID)),@packet,437)
                else if f12rune = 'lmmrune' then
    ShotRune(lmmrune,ReadMemInteger(Atacked))
     else
     Shotrune(strtoint(f12rune),ReadMemInteger(Atacked));
    end else if f12atack ='1' then begin
      if f12rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,readmeminteger(selfid))
    else if f12rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,readmeminteger(selfid))
        else if f12rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,readmeminteger(selfid))
        else if f12rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,readmeminteger(selfid))
            else if f12rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,readmeminteger(selfid))
                else if f12rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,readmeminteger(selfid))
     else
     Shotrune(strtoint(f12rune),readmeminteger(selfid));
    end    else begin
          if f12rune = 'sd' then
//    ShotRune(sdrune,ReadMemInteger(Atacked))
      Shotrune(sdrune,GetNameBLPosition(f12name).N)
    else if f12rune = 'hmm' then
//    ShotRune(hmmrune,ReadMemInteger(Atacked))
      Shotrune(hmmrune,GetNameBLPosition(f12name).N)
        else if f12rune = 'uhrune' then
//    ShotRune(uhrune,ReadMemInteger(Atacked))
      Shotrune(uhrune,GetNameBLPosition(f12name).N)
        else if f12rune = 'explosionrune' then
//    ShotRune(explosionrune,ReadMemInteger(Atacked))
      Shotrune(explosionrune,GetNameBLPosition(f12name).N)
            else if f12rune = 'gfbrune' then
//    ShotRune(gfbrune,ReadMemInteger(Atacked))
      Shotrune(gfbrune,GetNameBLPosition(f12name).N)
                else if f12rune = 'lmmrune' then
//    ShotRune(lmmrune,ReadMemInteger(Atacked))
     Shotrune(lmmrune,GetNameBLPosition(f12name).N)
          else
     Shotrune(strtoint(f12rune),GetNameBLPosition(f12name).N);
    end;
  end;
end;

procedure TForm1._1Click(Sender: TObject);
begin
qual:=-17;
end;

procedure TForm1._2Click(Sender: TObject);
begin
qual := 1;
end;

procedure TForm1._3Click(Sender: TObject);
begin
qual:=19;
end;

procedure TForm1._4Click(Sender: TObject);
begin
qual:=18;
end;

procedure TForm1._5Click(Sender: TObject);
begin
qual:=17;
end;

procedure TForm1._6Click(Sender: TObject);
begin
qual := -1;
end;

procedure TForm1._7Click(Sender: TObject);
begin
qual:=-19;
end;

procedure TForm1._8Click(Sender: TObject);
begin
qual:=-18;
end;

procedure TForm1.Increaseat1001Click(Sender: TObject);
begin
speedbar.Position := 100;
end;

procedure TForm1.Increaseat201Click(Sender: TObject);
begin
speedbar.Position := 20;
end;

procedure TForm1.Increaseat601Click(Sender: TObject);
begin
speedbar.Position := 60;
end;

procedure TForm1.Label5Click(Sender: TObject);
begin
 If (Sender is TLabel) then
  with (Sender as Tlabel) do
 ShellExecute(Application.Handle,
             PChar('open'),
             PChar('http://bictrainer.awardspace.com'), 
             PChar(0),
             nil,
             SW_NORMAL);
end;

procedure TForm1.LTimerTimer(Sender: TObject);
begin
if botclose then
LTimer.Enabled := false;
//Application.ProcessMessages;
if noLight.Checked then exit;
DoLight(LSize,LColor);
Application.ProcessMessages;
end;

procedure TForm1.Memo1Enter(Sender: TObject);
begin
if memo1.Text = 'Type a list of names separateds with commas, you can also paste your !online here!' then begin
memo1.Text := '';
memo1.Font.Color := clWindowText;
end;
end;

procedure TForm1.Memo1Exit(Sender: TObject);
begin
if memo1.Text = '' then begin
memo1.Font.Color := cl3DDkShadow;
memo1.Text := 'Type a list of names separateds with commas, you can also paste your !online here!';
end;
end;

procedure TForm1.TExphourTimer(Sender: TObject);
var
Media:double;
tempostr:string;
ExpAtual:integer;
begin
if botclose then
texphour.Enabled := false;
Tempo:=Tempo+1;
ExpAtual:= ReadMemInteger(Experience);
ExpFeita:=ExpAtual-ExpInicial;
Media := ExpFeita / (Tempo / 60) * 60;
  if tempo > 3599 then
  tempostr := inttostr(tempo div 60 div 60)+'h '+inttostr(tempo div 60-(tempo div 60 div 60)*60)+'min '+inttostr(tempo -(tempo  div 60)*60)+'s'
  else if tempo > 59 then
  tempostr := inttostr(tempo div 60-(tempo div 60 div 60)*60)+'min '+inttostr(tempo -(tempo div 60)*60)+'s'
  else
  tempostr := inttostr(tempo)+'s';
  if ExpOnTitle.checked then begin
  SetWindowText(TibiaHandle,PChar('BicTrainer - '+TrimRight(ReadMemString(GetBLPosition(readmeminteger(selfid)).Position,20))+' - Exp/Hour: '+floattostr(Round(media))+' - Exp left: '+inttostr(VerExp(false))+' - Hunting time: '+tempostr+' - Exp Made: '+floattostr(expfeita)));
  TimerExp.Enabled:=false;
  end
  else
  SetWindowText(TibiaHandle,PChar('BicTrainer - '+TrimRight(ReadMemString(GetBLPosition(readmeminteger(selfid)).Position,20))+' - Exp/Hour: '+floattostr(Round(media))+' - Hunting time: '+tempostr+' - Exp Made: '+floattostr(expfeita)));
end;

procedure TForm1.TFishTimer(Sender: TObject);
begin
if botclose then
tfish.Enabled := false;
if FastFishing.Checked then begin
  if getlastused = 0 then exit;
  MWInt(PUse,7,4);
end;
if AutoFishing.Checked then begin
  if getlastused = 0 then exit;
  if GetForegroundWindow <> TibiaHandle then begin
    AutoFishing.Checked := false;
  end;
  MWInt(PUse,7,4);
  mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 ); //click mouse
  sleep(1); //wait 1 ms
  mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
end;
Application.ProcessMessages;
end;

procedure TForm1.TimerExpTimer(Sender: TObject);
begin
if botclose then
timerexp.Enabled := false;
SetWindowText(TibiaHandle,pchar('BicTrainer - '+TrimRight(ReadMemString(GetBLPosition(readmeminteger(selfid)).Position,20))+' - Exp left: '+inttostr(VerExp(false))));
end;

procedure TForm1.TLightClick(Sender: TObject);
begin
//Ani
lighton.Visible := true;
lightoff.Visible := false;
//Ani
LSize:=7;
LColor:=215;
  SBText('Torch Light Actived',3);
    LTimer.Enabled := true;  
end;

procedure TForm1.tmagicshieldTimer(Sender: TObject);
begin
if utamming then begin
  sleep(1000);
  utamming := false;
end;
if ReadMemInteger(Mana) > 49 then begin
if   ReadMemInteger(Status) < 16 then begin
    Say('utamo vita');
    utamming := true;
    exit;
end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 31 then
    if   ReadMemInteger(Status) < 48 then begin
        Say('utamo vita');
    utamming := true;
        exit;
    end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 63 then
    if   ReadMemInteger(Status) < 80 then begin
          Say('utamo vita');
        utamming := true;
          exit;
    end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 95 then
    if   ReadMemInteger(Status) < 112 then begin
      Say('utamo vita');
       utamming := true;
      exit;
    end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 127 then
    if   ReadMemInteger(Status) < 144 then begin
      Say('utamo vita');
    utamming := true;
      exit;
    end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 159 then
    if   ReadMemInteger(Status) < 176 then begin
      Say('utamo vita');
        utamming := true;
      exit;
    end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 191 then
    if   ReadMemInteger(Status) < 208 then begin
      Say('utamo vita');
       utamming := true;
      exit;
    end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 223 then
    if   ReadMemInteger(Status) < 240 then begin
      Say('utamo vita');
        utamming := true;
      exit;
    end;
application.ProcessMessages;
  if   ReadMemInteger(Status) > 255 then begin
      Say('utamo vita');
    utamming := true;
      exit;
  end;
application.ProcessMessages;
end;
application.ProcessMessages;
end;

procedure TForm1.TrainbowTimer(Sender: TObject);
begin
if botclose then
trainbow.Enabled := false;
Rainbow;
Application.ProcessMessages;
end;

procedure TForm1.NoLightClick(Sender: TObject);
begin
  LTimer.Enabled := false;
//Ani
lighton.Visible := false;
lightoff.Visible := true;
//Ani
DoLight(2,215);
  SBText('Light Hack Deactived',3);
end;

procedure TForm1.NormalSpeed1Click(Sender: TObject);
begin
speedbar.Position := 0;
end;

procedure TForm1.ntimerTimer(Sender: TObject);
var lifeprcent:string;
  hppr,hp,I: Integer;
begin
if ckautospell.Checked then begin
hp:=ReadMemInteger($4A1240);
hppr :=ReadMemInteger(getblposition(readmeminteger(selfid)).Position+$80);
if life.Text[length(life.text)] = '%' then begin
  for I := 1 to length(life.text)-1 do begin
    lifeprcent := lifeprcent+life.text[I];
  end;
  if hppr <= strtoint(lifeprcent) then begin
     say(magia.Text);
  end;
end
else begin
  if hp <= strtoint(life.text) then begin
      say(magia.Text);
  end;
end;
end
else if ckautouh.Checked then begin
hp:=ReadMemInteger($4A1240);
hppr :=ReadMemInteger(getblposition(readmeminteger(selfid)).Position+$80);
if life.Text[length(life.text)] = '%' then begin
  for I := 1 to length(life.text)-1 do begin
    lifeprcent := lifeprcent+life.text[I];
  end;
  if hppr <= strtoint(lifeprcent) then begin
  shotrune(uhrune,readmeminteger(SelfID));
  end;
end
else begin
  if hp <= strtoint(life.text) then begin
  shotrune(uhrune,readmeminteger(SelfID));
  end;
end;
end;
end;

procedure TForm1.portChange(Sender: TObject);
var
tport:integer;
begin
//IPChanger
if length(server.text) > 0 then  begin
if (length(port.text) < 4) or (length(port.text) > 5) then begin
tport := 7171;
iniwrite('Bic Config','Port',port.text);
end
else begin
try
tport := strtoint(port.text);
except
tport := 7171
end;
iniwrite('Bic Config','Port',port.text);
end;
    MWstr(S76_1,server.text,length(server.text)+1);
    MWstr(S76_2,server.text,length(server.text)+1);
    MWstr(S76_3,server.text,length(server.text)+1);
    MWstr(S76_4,server.text,length(server.text)+1);
    MWint(pS76_1,tport,4);
    MWint(pS76_2,tport,4);
    MWint(pS76_3,tport,4);
    MWint(pS76_4,tport,4);
//IPChanger
end;
end;

procedure TForm1.RadioButton1Click(Sender: TObject);
begin
FishOn.Visible := True;
FishOff.Visible := False;
end;

procedure TForm1.RadioButton2Click(Sender: TObject);
begin
FishOn.Visible := True;
FishOff.Visible := False;
end;

procedure TForm1.RadioButton3Click(Sender: TObject);
begin
FishOn.Visible := True;
FishOff.Visible := False;
end;

procedure TForm1.Show1Click(Sender: TObject);
begin
Form1.Show;
end;

procedure TForm1.SpeedBarChange(Sender: TObject);
var velo,CLevel:integer;
begin
if speedbar.Position <> 0 then speedhack.enabled := true
else speedhack.enabled := false;

CLevel:=ReadMemInteger(ALevel);
SpeedValue.Caption := inttostr(speedbar.Position)+'%';
NudgeBoh(1);
velo:=(220+2*CLevel)+(220+2*CLevel)*(speedbar.Position+1) div 100;
MWInt(GetBLPosition(ReadMemInteger(SelfID)).Position+DistSpeed,velo,4);
end;



procedure TForm1.playbtClick(Sender: TObject);
var I:integer;
begin
playing := true;
drecbt.Visible := true;
stlabel.Caption := 'Playing';
while playing do
for I := 0 to packetsgravados-1 do begin
SendIPCMessage(pchar(inttostr(ProcID)),convertdatatoarray(packets[I].Packet),packets[i].Length);
sleep(100);
application.ProcessMessages;
end;
end;



procedure TForm1.stopbtClick(Sender: TObject);
begin
if gravando then begin
packetsgravados := pcn;
gravando := false;
end;
if playing then begin
playing := false;
end;
stlabel.caption := 'Done';
if pcn > 0 then
dplaybt.Visible := false;
drecbt.Visible := false;

end;

procedure TForm1.tatualizaTimer(Sender: TObject);
begin
if ReadMemByte(GetBLPosition(ReadMemInteger(SelfID)).Position) = $00 then
exit;
if primeira then begin
  primeira := false;
  charname.Caption := ReadMemString(GetBLPosition(ReadMemInteger(SelfID)).Position,20);
newchar.Caption := ReadMemString(GetBLPosition(ReadMemInteger(SelfID)).Position,20);
if charname.Caption <> newchar.Caption then begin
  charname.caption := ReadMemString(GetBLPosition(ReadMemInteger(SelfID)).Position,20);
end;
end;
end;


procedure TForm1.recbtClick(Sender: TObject);
begin
stlabel.Caption := 'Rec...';
gravando := true;
dplaybt.visible := true;
end;

end.
