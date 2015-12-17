unit broad;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Grids, ValEdit, StdCtrls, ExtCtrls,madCodeHook;

type
  Tfmbroad = class(TForm)
    charlist: TValueListEditor;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    lbtempo: TLabel;
    Label7: TLabel;
    lbstatus: TLabel;
    btstop: TButton;
    tcharlist: TTimer;
    Timer1: TTimer;
    procedure Timer1Timer(Sender: TObject);
    procedure btstopClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure tcharlistTimer(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure EnviaMsgs;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmbroad: Tfmbroad;
atualizar:boolean=false;
ready,stop:boolean;
implementation

uses tiksbot, tfunctions, tempinjecter;

{$R *.dfm}

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

procedure Tfmbroad.btstopClick(Sender: TObject);
begin
if btstop.caption = '&Stop' then begin
stop:=true;
ready:=true;
lbstatus.caption := 'Stopped';
btstop.caption := '&Close';
end
else begin
  fmbroad.Close;
end;
end;

procedure Tfmbroad.EnviaMsgs;
var
test:string;
  Palavra:array[0..280] of string;
  trestante,ddelay,TSleep,pcnb,Z,I: Integer;
  pulaesp,pulou:boolean;
pcmsg,X,C: Integer;
packet:array[0..400] of byte;
begin
ready := false;
z:=0;
C:=0;
pulou:=false;
pulaesp:=false;
with form1 do
for I := 1 to length(memo1.Text) do begin
  if memo1.Text[I] = ':' then begin
   test := memo1.Text[I-6]+memo1.Text[I-5]+memo1.Text[I-4]+memo1.Text[I-3]+memo1.Text[I-2]+memo1.Text[I-1];
    if test  = 'online' then begin
        palavra[z]:='';
//        z:=z-1;
    end;
    pulou:=true;
  end;

  if memo1.Text[I] = #$A then begin
    pulou:=true;
  end;

  if memo1.Text[I] = ',' then begin
    if I <> length(memo1.text) then
    inc(z);
    pulou := true;
  end;
if not pulou then
    try
    strtoint(memo1.Text[I])
  except
    Palavra[z] := Palavra[z] + memo1.text[I];
  end
  else pulou := false;
end;
charlist.Strings.Clear;
try
ddelay := strtoint(form1.eddelay.text);
except
ddelay := 2;
end;
trestante:=ddelay*(z);
lbtempo.Caption := inttostr(ddelay*z)+' s';
for X := 0 to Z do begin

packet[2]:=$96;
packet[3]:=$04;
if palavra[x][1] = ' ' then pulaesp:=true;
if pulaesp then
packet[4]:=length(palavra[x])-1
else
packet[4]:=length(palavra[x]);
packet[5]:=$00;
pcnb:=6;
if pulaesp then begin
for i := 2 to length(palavra[x])+1 do begin
  packet[pcnb-2+i]:=ord(palavra[x][i]);
  C:=i-1;
end;
pcmsg:=pcnb+c-1;
packet[pcmsg]:=length(form1.edmsg.text);
packet[pcmsg+1]:=$00;
inc(pcmsg,2);
for Z := 0 to length(form1.edmsg.text) do begin
packet[pcmsg+Z]:=ord(form1.edmsg.text[Z+1]);
C := z;
end;
end else begin
for i := 1 to length(palavra[x])+1 do begin
  packet[pcnb-1+i]:=ord(palavra[x][i]);
  C:=i;
end;
pcmsg:=pcnb+c-1;
packet[pcmsg]:=length(form1.edmsg.text);
packet[pcmsg+1]:=$00;
inc(pcmsg,2);
for Z := 0 to length(form1.edmsg.text) do begin
packet[pcmsg+Z]:=ord(form1.edmsg.text[Z+1]);
C := z;
end;
end;
pcmsg:=pcmsg+C;
if length(palavra[x]) > 1 then begin
charlist.InsertRow(trim(palavra[x]),'Sent',true);
packet[0]:=lo(pcmsg-2);
packet[1]:=Hi(pcmsg-2);
//showmessage(convertdatatohex(@packet,pcmsg)+#13+inttostr(pcmsg));
sendIPCMessage(pchar(inttostr(ProcID)),@packet,pcmsg);
try
ddelay := strtoint(form1.eddelay.text)
except
ddelay := 2;
end;
if stop then exit;
trestante := trestante - ddelay;
if trestante < 1 then trestante := 0;
lbtempo.Caption := inttostr(trestante)+' s';

lbstatus.Caption := 'Sending msgs';
for TSleep := 0 to ddelay - 1 do begin
for I := 0 to 10 do begin
  sleep(100);
application.ProcessMessages;
end;
end;
end;
end;
if lbtempo.caption = '0 s' then begin
ready := true;
lbstatus.Caption := 'Done';
btstop.caption := '&Close';
end;
end;

procedure Tfmbroad.FormClose(Sender: TObject; var Action: TCloseAction);
begin
stop:=true;
atualizar := false;
tcharlist.Enabled := true;
form1.Enabled := true;
end;

procedure Tfmbroad.FormShow(Sender: TObject);
begin
atualizar := true;
btstop.caption :='&Stop';
end;

procedure Tfmbroad.tcharlistTimer(Sender: TObject);
begin
if atualizar then begin
tcharlist.Enabled := false;
Enviamsgs;
end;
end;

procedure Tfmbroad.Timer1Timer(Sender: TObject);
begin
if lbtempo.Caption = '0 s' then btstop.Caption := '&Close';
end;

end.
