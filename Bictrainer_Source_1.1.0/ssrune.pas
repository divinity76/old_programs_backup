unit ssrune;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  Twhorune = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    trune: TTimer;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure truneTimer(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  whorune: Twhorune;
  prune:integer;
implementation

uses tiksbot, tfunctions;

{$R *.dfm}

procedure Twhorune.FormClose(Sender: TObject; var Action: TCloseAction);
begin
form1.runecb.Clear;
form1.runecb.items.Insert(0,'Sudden Death Rune (SD)');
form1.runecb.items.Insert(1,'Heavy Magic Missile (HMM)');
form1.runecb.items.Insert(2,'Ultimate Healing Rune (UH)');
form1.runecb.items.Insert(3,'Explosion');
form1.runecb.items.Insert(4,'Great Fireball (GFB)');
form1.runecb.items.Insert(5,'Wrath');
form1.runecb.items.Insert(6,'Rune ID: '+inttostr(CustomRune)+'...');
form1.runecb.ItemIndex:=6;
form1.Enabled := true;
end;

procedure Twhorune.FormShow(Sender: TObject);
begin
prune := readmeminteger(LClick);
form1.enabled := false;
trune.Enabled := true;
end;

procedure Twhorune.truneTimer(Sender: TObject);
begin
  while PRune = ReadMemInteger(LClick) do
    application.ProcessMessages;
  CustomRune := ReadMemInteger(LClick);
  trune.Enabled := false;
  whorune.Close;
end;

end.
