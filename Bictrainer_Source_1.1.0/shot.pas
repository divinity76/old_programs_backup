unit shot;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  Tshotwho = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    Button1: TButton;
    procedure FormShow(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  shotwho: Tshotwho;
  f1name,f2name,f3name,f4name,f5name,f6name,f7name,f8name,f9name,f10name,f11name,f12name:string;
implementation

uses tiksbot;

{$R *.dfm}

procedure Tshotwho.Button1Click(Sender: TObject);
begin
if form1.hotkey.itemindex = 0 then
f1name := edit1.Text;
if form1.hotkey.itemindex = 1 then
f2name := edit1.Text;
if form1.hotkey.itemindex = 2 then
f3name := edit1.Text;
if form1.hotkey.itemindex = 3 then
f4name := edit1.Text;
if form1.hotkey.itemindex = 4 then
f5name := edit1.Text;
if form1.hotkey.itemindex = 5 then
f6name := edit1.Text;
if form1.hotkey.itemindex = 6 then
f7name := edit1.Text;
if form1.hotkey.itemindex = 7 then
f8name := edit1.Text;
if form1.hotkey.itemindex = 8 then
f9name := edit1.Text;
if form1.hotkey.itemindex = 9 then
f10name := edit1.Text;
if form1.hotkey.itemindex = 10 then
f11name := edit1.Text;
if form1.hotkey.itemindex = 11 then
f12name := edit1.Text;
form1.Enabled := true;
form1.shotat.clear;
form1.shotat.items.Insert(0,'Target (BattleList)');
form1.shotat.Items.Insert(1,'Yourself');
form1.shotat.Items.Insert(2,edit1.Text+'...');
form1.shotat.ItemIndex := 2;
shotwho.Close;
end;

procedure Tshotwho.FormShow(Sender: TObject);
begin
form1.Enabled := false;
end;

end.
