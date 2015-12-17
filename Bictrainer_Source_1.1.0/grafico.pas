{
On this file we control all grafics that have "animation" except
the light images and fish images.
}

unit grafico;

interface
uses tiksbot,SysUtils,Forms;
procedure NudgeClock(HowMany:Integer);
procedure NudgeBoh(HowMany:Integer);
procedure Dance(HowMany:Integer);
procedure Cor(HowMany:Integer);
procedure AniGroup(HowMany:Integer);
procedure AniEat(HowMany:integer);

var
ClockEmUso:boolean;
BohEmUso:Boolean;
Dancing:Boolean;
implementation

procedure AniEat(HowMany:integer);
var
  I: Integer;
begin
for I := 0 to HowMany - 1 do
begin
Form1.maca6.Visible := true;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := true;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := true;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := true;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := true;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := true;
sleep(80);
Application.ProcessMessages;
//
sleep(120);
Application.ProcessMessages;
sleep(120);
Application.ProcessMessages;
sleep(120);
Application.ProcessMessages;
Form1.maca6.Visible := true;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
end;
end;

procedure AniGroup(HowMany:Integer);
var
  I: Integer;
begin
for I := 0 to howmany - 1 do
begin
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := true;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := true;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := true;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := false;
Form1.maca4.Visible := true;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := false;
Form1.maca5.Visible := true;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
//
Form1.maca6.Visible := true;
Form1.maca5.Visible := false;
Form1.maca4.Visible := false;
Form1.maca3.Visible := false;
Form1.maca2.Visible := false;
Form1.maca1.Visible := false;
sleep(80);
Application.ProcessMessages;
end;
end;

procedure Cor(HowMany:Integer);
var
  I: Integer;
begin
if Dancing then exit;
Dancing:=true;
for I := 0 to HowMany - 1 do begin
//ChangeColor
  Form1.charc1.Visible := true;
  Form1.charc2.Visible := False;
  Form1.charc3.Visible := false;
  Form1.charc4.Visible := true;
  sleep(20);
  application.ProcessMessages;
//ChangeColor
  Form1.charc1.Visible := false;
  Form1.charc2.Visible := True;
  Form1.charc3.Visible := false;
  Form1.charc4.Visible := false;
  sleep(20);
  application.ProcessMessages;
//ChangeColor
  Form1.charc1.Visible := false;
  Form1.charc2.Visible := False;
  Form1.charc3.Visible := true;
  Form1.charc4.Visible := false;
  sleep(20);
  application.ProcessMessages;
//ChangeColor to back
  Form1.charc1.Visible := false;
  Form1.charc2.Visible := False;
  Form1.charc3.Visible := false;
  Form1.charc4.Visible := false;
  sleep(20);
  application.ProcessMessages;
end;
Dancing:=false;
end;

procedure Dance(HowMany:Integer);
var
  I: Integer;
begin
if Dancing then exit;
Dancing:=true;
for I := 0 to HowMany - 1 do begin
//Turn to Left
  Form1.char1.Visible := false;
  Form1.char2.Visible := False;
  Form1.char3.Visible := false;
  Form1.char4.Visible := true;
  sleep(20);
  application.ProcessMessages;
//Turn to Up
  Form1.char1.Visible := false;
  Form1.char2.Visible := True;
  Form1.char3.Visible := false;
  Form1.char4.Visible := false;
  sleep(20);
  application.ProcessMessages;
//Turn to Right
  Form1.char1.Visible := false;
  Form1.char2.Visible := False;
  Form1.char3.Visible := true;
  Form1.char4.Visible := false;
  sleep(20);
  application.ProcessMessages;
//Turn to Down
  Form1.char1.Visible := true;
  Form1.char2.Visible := False;
  Form1.char3.Visible := false;
  Form1.char4.Visible := false;
  sleep(20);
  application.ProcessMessages;
end;
  Dancing:=false;
end;

procedure NudgeClock(HowMany:Integer);
var I :integer;
begin
if ClockEmUso then exit;
ClockEmUso:=true;
for i := 0 to HowMany - 1 do begin
//Show the Second Clock
  Form1.clock1.Visible := False;
  Form1.clock2.Visible := True;
  Form1.clock3.Visible := False;
  sleep(20);
  application.ProcessMessages;
//Show the Third Clock
  Form1.clock1.Visible := False;
  Form1.clock2.Visible := False;
  Form1.clock3.Visible := True;
  sleep(20);
  application.ProcessMessages;
//Show the First Clock
  Form1.clock1.Visible := True;
  Form1.clock2.Visible := False;
  Form1.clock3.Visible := False;
  sleep(20);
  application.ProcessMessages;
end;
ClockEmUso:=false;
end;

procedure NudgeBoh(HowMany:Integer);
var
I:Integer;
begin
if BohEmUso then Exit;
BohEmUso:=true;
for I := 0 to HowMany - 1 do begin
Form1.boh.Top := 16;
sleep(15);
Application.ProcessMessages;
Form1.boh.Top := 18;
sleep(15);
Application.ProcessMessages;
Form1.boh.Top := 20;
sleep(15);
Application.ProcessMessages;
Form1.boh.Top := 18;
sleep(15);
Application.ProcessMessages;
Form1.boh.Top := 16;
end;
BohEmUso:=false;
end;

end.
