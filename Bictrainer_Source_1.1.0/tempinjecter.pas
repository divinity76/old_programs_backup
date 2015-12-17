unit tempinjecter;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, madCodeHook, Buttons, DBXpress, DB, SqlExpr;

type
  TForm2 = class(TForm)
    Label1: TLabel;
    Button1: TButton;
    tray: TTrayIcon;
    char: TComboBox;
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure FormCreate(Sender: TObject);
    procedure charDropDown(Sender: TObject);
    procedure trayDblClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2: TForm2;
  TTHandle:hwnd;
  IPCNAme:pchar;
  tProc:THandle;
  ProcId:Cardinal;
    handle1:array[0..99] of hwnd;
    closer:boolean=false;
//    TibiaHandle:hwnd;
implementation

uses tiksbot, tfunctions;

{$R *.dfm}

function Tibia: Boolean;
var
Handle: HWND;
ClassName: array [0..255] of Char;
begin
form2.char.Clear;
//form2.char.Update;
Handle := FindWindow(nil, nil);
while Handle <> 0 do
begin
GetClassName(Handle, ClassName, SizeOf(ClassName));
if ClassName = 'TibiaClient' then
begin
//Result := true;
TibiaHandle := handle;
if ReadMemInteger(GetBLPosition(ReadMemInteger(SelfID)).Position) <> $00 then
form2.char.Items.Add(ReadMemString(GetBLPosition(ReadMemInteger(SelfID)).Position,20))
else
form2.char.Items.Add('Not logged in');
handle1[form2.char.Items.Count-1] := handle;
end;
Handle := GetWindow(Handle, GW_HWNDNEXT);
end;
Result := false;
end;

procedure TForm2.Button1Click(Sender: TObject);
//var injetar:integer;
begin
  if char.ItemIndex <> -1 then begin
//  injetar := handle1[char.ItemIndex];
//  if strtoint(form1.iniread('Injected','Handle','0')) <> injetar then begin
//showmessage(inttostr(handle1[char.itemindex]));
//    form1.iniwrite('Injected','Handle',inttostr(handle1[char.ItemIndex]));
    GetWindowThreadProcessId(handle1[char.ItemIndex], @ProcId);
    tProc:= OpenProcess(PROCESS_ALL_ACCESS, False, ProcId);
    InjectLibrary(tProc,'bic.dll',7000);
      Form1.show;
      TibiaHandle :=handle1[char.ItemIndex];
      TrocarIP:=true;
      Form2.Visible := false;
      Tray.Visible := true;
    end
    else begin
      Form1.show;
      TibiaHandle :=handle1[char.ItemIndex];
      TrocarIP:=true;
      Form2.Visible := false;
      form2.hide;
      Tray.Visible := true;
//    end;
  end;
end;

procedure TForm2.charDropDown(Sender: TObject);
begin
Tibia;
end;

procedure TForm2.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
with form1 do begin
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
playing := false;
end;
end;

procedure TForm2.FormCreate(Sender: TObject);
begin
TibiaHandle := FindWindow(nil, nil);;
end;

procedure TForm2.trayDblClick(Sender: TObject);
begin
form1.Show;
end;

end.
