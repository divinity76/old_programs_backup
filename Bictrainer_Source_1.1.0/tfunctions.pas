unit tfunctions;

interface

uses
  Windows,SysUtils,madCodeHook,Dialogs;

type
  TBLPosition = record
Position:integer;
N:integer;
X:Integer;
Y:Integer;
Z:Integer;
end;

procedure SBText(Text: String; Time: Integer);
function VerExp(Ver: Boolean):integer; // Check exp left in status
function GetBLPosition(PlayerID:integer): TBLPosition;
function GetNameBLPosition(PlayerName:string): TBLPosition;
procedure DoLight(Size, Color : Integer);
procedure AllHaveLight(size,color:integer);
function ReadMemInteger(Address: Cardinal): Cardinal;
function ReadMemByte(Address: Cardinal): Cardinal;
function ReadMemString(addr, len : dword): string;
procedure MWint(Address: Integer; buf: Integer; Length: DWORD);
procedure MWStr(Address: Integer; buf: String; Length: DWORD);
function GetLastUsed:integer;
function OutfitValido(ID:integer):boolean;
procedure DanceP(HowMany:integer);
procedure Rainbow;
procedure Comer(FoodID:integer;Slot,SubSlot:byte);
procedure GroupItems;
procedure ShotRune(IdRuna:Integer;IDatt:integer);
//function FindFishAble:TFishAble;

type
TItem = Record
   ItemNro:integer;
      Pos1:Integer;
    BPNro:integer;
    BPAberta:boolean;
 Slot:byte; //BP, Hand ect...
 SubSlot:byte; //If applicable, ie: BP
 Found:boolean; //The function has found the item?
 ID:integer;//repeated, i know
end;

type
TGItem = Record
    Slot1:byte; //BP, Hand ect...
 SubSlot1:byte; //If applicable, ie: BP
    Slot2:byte; //BP, Hand ect...
 SubSlot2:byte; //If applicable, ie: BP
 Quant:integer;
 ID:integer;//repeated, i know
end;



function FindItem(ItemID:integer):TItem;
var
ItemNr:integer=0;
Items:array[0..400] of TItem;
qual:integer;
CustomRune:integer;
const
Version=110;
tsen =$5F6CBC;
tcon = $5F6CDC;
//IPChanger
S76_1 = $5EFBC0;
S76_2 = $5EFC30;
S76_3 = $5EFCA0;
S76_4 = $5EFD10;
pS76_1 = $5EFC24;
pS76_2 = $5EFC94;
pS76_3 = $5EFD04;
pS76_4 = $5EFD74;
//IPChanger
Adr_Map = $4AF478;  //76
DistSpeed              = $84;      //78
RHandID                = $05;
LHandID                = $06;
AmmoID                 = $0A;
BPID                   = $40;
BLEnd = $4a2014;//$4a2ce0;
BPisOpen               = $10;  //for minus
ItemsVisible           = $28; //For more
  BLStart = $4A12AC;
  SBTxt = $5F7058;
  SBTime = $5F7054;
  SelfID = $4A1244;
  maxhp =   $4A123C;
  maxmana = $4A1220;
  ALevel = $4A1234;
  Experience = $4A1238;
  DistChar = 156;
  DLightSize = $70;
  DLightColor = $74;
  OutfitDist = 92;
  PUse = $5F6D44;
  PrepUse = $5F6D44;
  BPStart = $4a9640;
  DistBP = 492;
  FirstItem = 44;
  DistItems = 12;
  aLife = $4A1240;
    DItem = 12;
  Mana=$4A1224;
  DistMap = $A8;
  Soul=$4A121C;
  L_Hand=$4A95E8;
  R_Hand=$4A95F4;
  DCharP=$88;
  ArrowSlot=$4A9624;
  Status=$4A11D0;
  BPUsed=$5F6CF0;
  SlotUsed=$5F6CEC;
  LastUsedID=$5F6D64;
  DistX = $20;
  DistY = $24;
  DistZ = $28;
  Atacked = $4A1214;
  sdrune = 3155;
  hmmrune = 3198;
  uhrune = 3160;
  ihrune = 3152;
  explosionrune = 3200;
  gfbrune = 3191;
  lmmrune = 3187;
  DistWalking = $48;
  DistDir = $4C;
  LClick = $5F6D58;
  LastItem = $5F6D70;
implementation

uses grafico, tiksbot, tempinjecter;

{$REGION 'Memory Functions'}

function ReadMemInteger(Address: Cardinal): Cardinal;       //Read adress:value
var
ProcId: Cardinal;
tProc: THandle;
NBR: Cardinal;
value:integer;
begin
    GetWindowThreadProcessId(TibiaHandle, @ProcId);
    tProc:= OpenProcess(PROCESS_ALL_ACCESS, False, ProcId);
    ReadProcessMemory(tProc, Ptr(Address), @value, 4, NBR);
    CloseHandle(tProc);
    Result:=value;
end;

function ReadMemByte(Address: Cardinal): Cardinal;       //Read adress:value
var
ProcId: Cardinal;
tProc: THandle;
NBR: Cardinal;
value:byte;
begin
    GetWindowThreadProcessId(TibiaHandle, @ProcId);
    tProc:= OpenProcess(PROCESS_ALL_ACCESS, False, ProcId);
    ReadProcessMemory(tProc, Ptr(Address), @value, 1, NBR);
    CloseHandle(tProc);
    Result:=value;
end;


function ReadMemString(addr, len : dword): string;
var
 str : string;
 dwread : dword;
 IDProc: Integer;
 THandl: hWnd;
begin
  GetWindowThreadProcessId(TibiaHandle, @IdProc);
  THandl := OpenProcess(PROCESS_VM_READ, False, IdProc);
  setlength(str, len);
  readprocessmemory(THandl, pointer(addr), @str[1], len, dwread);
  result := str;
end;

procedure MWint(Address: Integer; buf: Integer; Length: DWORD);
var ProcID, THandle: Integer;
    e: DWORD;
begin
   GetWindowThreadProcessId(TibiaHandle, @ProcID);
   THandle := OpenProcess(PROCESS_ALL_ACCESS, False, ProcID);
   WriteProcessMemory(THandle, Ptr(Address), @buf, Length, e);
   CloseHandle(THandle);
end;

procedure MWStr(Address: Integer; buf: String; Length: DWORD);
var ProcID: Integer;
    THandle: hWnd;
    e: DWORD;
begin
  GetWindowThreadProcessId(TibiaHandle, @ProcID);
  THandle := OpenProcess(PROCESS_ALL_ACCESS, False, ProcID);
  WriteProcessMemory(THandle, Pointer(Address), PChar(buf), Length, e);
  CloseHandle(THandle);
end;

{$ENDREGION 'Memory Functions'}


//function FindFishAble:TFishAble;
//var
//  Px:integer;
//  Py:integer;
//  Pz:integer;
//  z,x,y,quant,
//  Player: Integer;
//  Mapa:array[0..8,0..14,0..18] of Byte;
//  I: Integer;
//begin
//quant:=0;
//Px:=999;
//Py:=999;
//Pz:=999;
//for Player := 1 to 2016 do begin
//  z:= Player div (14*18);
//  y:=(Player-z*14*18) div 18;
//  x:=(Player-z*14*18-y*18);
//  Mapa[z][y][x]:=ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1));
//  //Loop pra ver qntos items tem no mapa
//  if ReadMemInteger(ReadMemInteger(Adr_Map)+$AC*(Player-1)) >= 2 then begin
//    if ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)+$C) = $63 then
//    if ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)+$C+$4) = ReadMemInteger(SelfID) then begin
//    //here we take the tile were the player are
//    Px:=x;
//    Py:=y;
//    Pz:=z;
//    end;
//Form1.ProcMsg;
//  end;
//  if
//    (ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)) = 4597)
//     or (ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)) = 4598)
//     or (ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)) = 4599)
//     or (ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)) = 4600)
//     or (ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)) = 4601)
//     or (ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1)) = 4602)
//    then begin
//    //Fishable found, let mark
//    inc(Quant);
//    Result[Quant].ID := ReadMemInteger(ReadMemInteger(Adr_Map)+4+$AC*(Player-1));
//    Result[Quant].X := x;
//    Result[Quant].Y := y;
//    Result[Quant].Z := z;
//  end;
//
//end;
//Result[0].Quant := Quant;
////Player position not found, unable to calc coordenates
//if (Px = 999) or (Py = 999) or (Pz = 999)then showmessage('eero');
//  for Z := 1 to Result[0].Quant do begin
//  Form1.ProcMsg;
//    if Pz - Result[Z].Z <> 0 then break; //Floors aren't the same
//    Result[Z].Z := ReadMemInteger(getBlPosition+$28);
//    if Result[Z].X > Px then begin
//      Result[Z].X := Result[Z].X - Px;
//      Result[Z].X := Result[Z].X + ReadMemInteger(GetBlPosition+$20);
//    end
//    else begin
//      Result[Z].X := Px - Result[Z].X;
//      Result[Z].X := ReadMemInteger(GetBlPosition+$20) - Result[Z].X;
//    end;
//
//    if Result[Z].Y > Py then begin
//      Result[Z].Y := Result[Z].Y - Py;
//      Result[Z].y := Result[Z].Y + ReadMemInteger(GetBlPosition+$24);
//    end
//    else begin
//      Result[Z].Y := Py - Result[Z].Y;
//      Result[Z].Y := ReadMemInteger(GetBlPosition+$24) - Result[Z].Y;
//    end;
//Form1.ProcMsg;
//  end;
//end;

function BPOpen(BP:integer):boolean;
begin
Result:=false;
    if ReadMemInteger($4a9640+(DistBP*(BP-1))) <> 0 then
      if ReadMemInteger($4a9640-16+DistBP*(BP-1)) <> 0 then
       Result:=true
       else Result:=false;
end;

procedure GetBPItems;
var
z:integer;
bpnr:integer;//check the number of the current bp
begin
  for Z := 1 to 280-1 do begin
    if Z mod 20 <> 0 then bpnr := Z div 20+1
      else bpnr := Z div 20;
        Items[Z].ID:= ReadMemInteger(($4a9640+FirstItem)+(DItem*(Z-(20*(bpnr-1))))-DItem+(DistBP*(bpnr-1)));
        Items[Z].BPNro := bpnr;
        Items[Z].ItemNro := Z-(20*(bpnr-1));
        if BPOpen(bpnr) then Items[Z].BPAberta := true
        else Items[Z].BPAberta := False;
  end;
end;


function GetItemPos(ID:Integer):integer;
var
  z: Integer;
  LHand,RHand,ASlot:integer;
begin
  Result := -1; if ID <> 3147 then begin
  LHand := ReadMemInteger(L_Hand);
  if LHand = ID then begin
  Result:=-2;
  Exit;
end;
RHand:=ReadMemInteger(R_Hand);
if RHand = ID then begin
  Result:=-3;
  Exit;
end;
ASlot:=ReadMemInteger(ArrowSlot);
if ASlot = ID then begin
  Result:=-4;
  Exit;
end;
  end;
  GetBPItems;
  for z := 1 to 280 do begin
    if  Items[Z].ID = ID then begin
      Result:=Z;
      Exit;
      end;
  end;
end;

//function ConvertDataToHex(Buffer: pointer; Length: Word): string;
//var
//  I: integer;
//  HexBuffer: string;
//begin
//  HexBuffer := '';
//  for I := 0 to Length - 1 do
//  begin
//    HexBuffer := HexBuffer + IntToHex(Ord(char(pointer(integer(Buffer) + I)^)), 2) + ' ';
//  end;
//  Result := HexBuffer;
//end;


procedure ShotRune(IdRuna:Integer;IDatt:integer);
var
Packet:array[0..18] of byte;
bp,slot:byte;
X,Y:integer;
begin
slot:=0;
bp:=0;
if GetItemPos(IdRuna) = -2 then begin
bp := $05;
slot := $00;
end;
if GetItemPos(IdRuna) = -3 then begin
bp := $06;
slot := $00;
end;
if GetItemPos(IdRuna) = -4 then begin
bp := $0A;
slot := $00;
end;
if GetItemPos(IdRuna) > 0 then begin
bp := $40+Items[GetItemPos(IdRuna)].BPNro-1;
slot := $00+Items[GetItemPos(IdRuna)].ItemNro-1;
end;
if ReadMemByte(GetBLPosition(IDatt).Position-1) <> $40 then begin
  if GetItemPos(IdRuna) <> -1 then begin
      Y:= GetBLPosition(IDatt).Y;
      X:= GetBLPosition(IDatt).X;
  if ReadMemByte(GetBLPosition(IDatt).Position+DistWalking) = 1 then begin
    if ReadMemByte(GetBLPosition(IDatt).Position+$4C) = 0 then
      Y:= Y
    else if ReadMemByte(GetBLPosition(IDatt).Position+$4C) = 2 then
      Y:= Y
    else if ReadMemByte(GetBLPosition(IDatt).Position+$4C) = 3 then
      X:= X
    else if ReadMemByte(GetBLPosition(IDatt).Position+$4C) = 1 then
      X:= X;
  end;
    Packet[0]:=$11;
    Packet[1]:=$00;
    Packet[2]:=$83;
    Packet[3]:=$FF;
    Packet[4]:=$FF;
    Packet[5]:=bp;///BP
    Packet[6]:=$00;
    Packet[7]:=slot;//Slot
    Packet[8]:=Lo(IdRuna); //Id Da runa
    Packet[9]:=Hi(IdRuna); //ID da Runa
    Packet[10]:=slot;//Slot
    Packet[11]:=$00+Lo(X);
    Packet[12]:=$00+Hi(X);
    Packet[13]:=$00+Lo(Y);
    Packet[14]:=$00+Hi(Y);
    Packet[15]:=$00+GetBLPosition(IDatt).Z;
    Packet[16]:=$63;
    Packet[17]:=$00;
    Packet[18]:=$01;
//    Mwint(sbtime,3,4);
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,19);
  end;
  end
  else begin
  Packet[0]:=$0D;
  Packet[1]:=$00;
  Packet[2]:=$84;
  Packet[3]:=$FF;
  Packet[4]:=$FF;
  Packet[5]:=Bp;
  Packet[6]:=$00;
  Packet[7]:=Slot;
  Packet[8]:=Lo(IdRuna);
  Packet[9]:=Hi(IdRuna);
  Packet[10]:=Slot;
  Packet[11]:=Lo(IdAtt);
  Packet[12]:=Hi(IDAtt);
  Packet[13]:=ReadMemByte(GetBLPosition(ReadMemInteger(atacked)).Position-2);
  Packet[14]:=$40;

//  showmessage(convertdatatohex(@packet,15));
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,15);
  end;
//end;
end;



function OutfitValido(ID:integer):boolean;
begin
 Result := true;
  if ID < 0 then Result := false;
  if ID > 142 then  Result := false;
  if ID = 135 then  Result := false;
  if ID = 20 then   Result := false;
  if ID = 98 then   Result := false;
  if ID = 97 then   Result := false;
  if ID = 96 then   Result := false;
  if ID = 93 then  Result := false;
  if ID = 1 then   Result := false;
  if ID = 12 then  Result := false;
  if ID = 77 then   Result := false;
  if ID = 72 then   Result := false;
  if ID = 47 then   Result := false;
  if ID = 46 then   Result := false;
  if ID = 10 then   Result := false;
  if ID = 11 then   Result := false;
end;

procedure SBText(Text: String; Time: Integer);
begin
    MWStr(SBTxt,Text,120);
    MWint(SBTime,Time*10,4);
end;

function GetLastUsed:integer;
begin
result:=ReadMemByte(LastUsedID);
end;

procedure GroupItems;
var BpNr2,BpNr,CurrentBP,ItemNrLoc,ItemNr2,ItemParou,BpParou:Integer;
Position:TGItem;
packet:array[0..255] of byte;
pos,VisibleItems:integer;
begin
{$REGION 'InicVariaveis'}
Position.Slot1:=0;
Position.Slot2:=0;
Position.SubSlot1:=0;
Position.SubSlot2:=0;
Position.Quant := 0;
ItemParou:=0;
BpParou := 0;
{$ENDREGION 'InicVariaveis'}
//form1.agroup.Interval := 100; //Come back the normal interval of time
//Dist between a item and your amount is 4
for BpNr := 0 to 12 do begin
  CurrentBP := BPStart+DistBP*BpNr+FirstItem;
  if ReadMemByte(CurrentBP-FirstItem-BPisOpen) = 0 then begin
    //BPClosed
    break;
  end;
  for ItemNrLoc := ItemNr to ReadMemInteger(CurrentBP-FirstItem+$20)-1 do begin
  ItemNr := ItemNrLoc;
      if ReadMemInteger(CurrentBP+DistItems*ItemNr+4) > 0 then begin
    if ReadMemInteger(CurrentBP+DistItems*ItemNr+4) < 100 then begin
      //We Found... But the item realy exist?
      pos := ReadMemInteger(CurrentBP-FirstItem+ItemsVisible);
      if pos >= ItemNr+1 then begin
        //We Found, so come on check it is grouppable or simplely already have 100
        if ReadMemInteger(CurrentBP+DistItems*ItemNr+4) = 0 then
          exit; //Ins't grouppable
        if ReadMemInteger(CurrentBP+DistItems*ItemNr+4) < 100 then begin
          //All right, save this position
          Position.Slot1 := BPID+BpNr;
          Position.ID := ReadMemInteger(CurrentBP+DistItems*ItemNr);
          Position.SubSlot1 := $00+ItemNr;
          Position.Quant := ReadMemInteger(CurrentBP+DistItems*ItemNr+4);    //Yes, We Found!
          ItemParou := ItemNr;
          BpParou := BpNr;
          break;
          //we have one, ok, let find another
        end;
      end;
    end;
  end;
  end;
end;
for BpNr2 := BpParou to 20 do begin
  CurrentBP := BPStart+DistBP*BpNr2+FirstItem;
  if ReadMemByte(CurrentBP-FirstItem-BPisOpen) = 0 then begin
    //BPClosed
    break;
  end;
  for ItemNr2 := ItemParou+1 to ReadMemInteger(CurrentBP-FirstItem+$20)-1 do begin
      if ReadMemInteger(CurrentBP+DistItems*ItemNr2+4) > 0 then begin
    if ReadMemInteger(CurrentBP+DistItems*ItemNr2+4) < 100 then  begin
//    Items2:=ReadMemInteger(CurrentBP+DistItems*ItemNr2);
//    if Items2 = Position.ID then begin

      //We Found... But the item realy exist?
      VisibleItems :=  ReadMemInteger(CurrentBP-FirstItem+ItemsVisible);
      if VisibleItems >= ItemNr2+1 then begin
        //We Found, so come on check it is grouppable or simplely already have 100
        if ReadMemInteger(CurrentBP+DistItems*ItemNr2+4) = 0 then
          exit; //Ins't grouppable
        if ReadMemInteger(CurrentBP+DistItems*ItemNr2+4) < 100 then begin
          //All right, save this position
          Position.Slot2 := BPID+BpNr2;
          Position.SubSlot2 := $00+ItemNr2;     //Yes, We Found!
          break;
          //ok, let group
//        end;
      end;
    end;
      end;
      end;
  end;
end;
//showmessage('Item/BP Nr: '+inttostr(itemNr)+'/'+inttostr(bpNr)+#13+'Quant: '+inttostr(position.Quant)+#13+'Position 1 BP = '+inttohex(position.Slot1,2)+#13+'Position 1 Slot = '+inttohex(position.SubSlot1,2)+#13+'Position 2 BP = '+inttohex(position.Slot2,2)+#13+'Position 2 Slot = '+inttohex(position.SubSlot2,2)+#13+'ID: '+inttostr(Position.ID));
if position.Slot1 < $40 then begin
ItemNr := 0;
exit;
end;
if position.Slot2 < $40 then begin
itemNr:=itemNr+1;
exit;
end;
if not grouping then begin
grouping := true;
//showmessage('Position 1 BP = '+inttohex(position.Slot1,2)+#13+'Position 1 Slot = '+inttohex(position.SubSlot1,2)+#13+'Position 2 BP = '+inttohex(position.Slot2,2)+#13+'Position 2 Slot = '+inttohex(position.SubSlot2,2)+#13+'ID: '+inttostr(Position.ID));
packet[0]:=$0F;
packet[1]:=$00;
packet[2]:=$78;
packet[3]:=$FF;
packet[4]:=$FF;
packet[5]:=Position.Slot1;
packet[6]:=$00;
packet[7]:=Position.SubSlot1;
packet[8]:=Lo(Position.ID);
packet[9]:=Hi(Position.ID);
packet[10]:=Position.SubSlot1;
packet[11]:=$FF;
packet[12]:=$FF;
packet[13]:=Position.Slot2;
packet[14]:=$00;
packet[15]:=Position.SubSlot2;
packet[16]:=Position.Quant;
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,17);
sleep(200);
form1.ProcMsg;
sleep(200);
form1.ProcMsg;
sleep(200);
form1.ProcMsg;
sleep(200);
form1.ProcMsg;
sleep(200);
form1.ProcMsg;
grouping:=false;
end;

end;

Function VerExp(Ver: Boolean):integer; // Check exp left in status
var
  Level:integer;
  ExpToNextLevel, exper:integer;
begin
Result:=-1;
      Level := ReadMemInteger(ALevel);
      if (Level > 0) then
        begin
          ExpToNextLevel := Round((50 * (Level) * (Level) * (Level) - 150 * (Level) * (Level) + 400 * (Level)) / 3);
          exper := ReadMemInteger(Experience);
          Result := ExpToNextLevel - exper;
          if ver=true then
          SBText('You need of '+inttostr(Result)+' exp to level '+inttostr(Level+1)+'. ('+inttostr(ExpToNextLevel)+')',3);
  end;
end;

function GetBLPosition(PlayerID:integer): TBLPosition;
var BLPosition:integer;
Current,i:integer;
begin
if botclose then
exit;
i:=0;
Result.Position := -1;
BLPosition := BLStart;
Current := ReadMemInteger(blposition-4);
while (Current <> PlayerID) do begin
inc(i);
BLPosition := BLPosition+Distchar;
Current := ReadMemInteger(blposition-4);
if BLPosition >= BLEnd+BlEnd then exit;
end;
Result.N:=i;
Result.Position:= BLPosition;
Result.X:=ReadMemInteger(BlPosition+DistX);
Result.Y:=ReadMemInteger(BlPosition+DistY);
Result.Z:=ReadMemInteger(BlPosition+DistZ);
end;

function GetNameBLPosition(PlayerName:string): TBLPosition;
var BLPosition:integer;
Current:string;
//i:integer;
begin
if botclose then
exit;
//i:=0;
Result.Position := -1;
BLPosition := BLStart;
Current := ReadMemString(blposition,length(PlayerName));
while (Current <> PlayerName) do begin
//inc(i);
BLPosition := BLPosition+Distchar;
while ReadMemByte(BLPosition+DCharP) <> $1 do
BLPosition:=BLPosition+Distchar;
Current := ReadMemString(blposition,length(PlayerName));
if BLPosition >= BLEnd+BlEnd then exit;
end;
Result.N:=ReadMemInteger(BLPosition-4);
Result.Position:= BLPosition;
Result.X:=ReadMemInteger(BlPosition+DistX);
Result.Y:=ReadMemInteger(BlPosition+DistY);
Result.Z:=ReadMemInteger(BlPosition+DistZ);
//showmessage(inttostr(Result.N));
end;

procedure DoLight(Size, Color : Integer);
begin
if Size < 20 then
 if Color > 99 then
  MWInt(GetBLPosition(ReadMemInteger(SelfID)).Position+DLightSize,Size,4);
  MWInt(GetBLPosition(ReadMemInteger(SelfID)).Position+DLightColor,Color,4);
end;

procedure DanceP(HowMany:integer);
var
  Packet: array [0..255] of Byte;
  I: Integer;
begin
for I := 0 to HowMany - 1 do begin
Packet[0] := $01;
Packet[1] := $00;
Packet[2] := $6F;
//EncryptPacket(Packet,10);
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,3);
sleep(100);
Packet[0] := $01;
Packet[1] := $00;
Packet[2] := $71;
//EncryptPacket(Packet,10);
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,3);
sleep(100);
Packet[0] := $01;
Packet[1] := $00;
Packet[2] := $72;
//EncryptPacket(Packet,10);
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,3);
sleep(100);
Packet[0] := $01;
Packet[1] := $00;
Packet[2] := $70;
//EncryptPacket(Packet,10);
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,3);
sleep(100);
end;
end;

procedure Comer(FoodID:integer;Slot,SubSlot:byte);
var packet:array[0..255] of byte;
begin
packet[0]:=$0A;
packet[1]:=$00;
packet[2]:=$82;
packet[3]:=$FF;
packet[4]:=$FF;
packet[5]:=Slot;
packet[6]:=$00;
packet[7]:=SubSlot;
packet[8]:=Lo(FoodID);
packet[9]:=Hi(FoodID);
packet[10]:=SubSlot;
packet[11]:=$00;
//showmessage(inttostr(slot)+'  '+inttostr(SubSLot)+'  '+inttostr(foodid));
//EncryptPacket(packet,18);
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,12);
end;

function FindItem(ItemID:integer):TItem;
var
  ItemNr,
 VisibileItems,CheckingItem, Rhand,Lhand,ASlot,CurrentBP,BpNr: Integer;
begin
{
 First, TiksBOT will check if the items is on
 Player Hand, Ammo Slot and last, if it is on
 Some Opened Container
}
Rhand :=ReadMemInteger(R_Hand);
  if Rhand = ItemID then begin
    Result.Slot := RHandID;
    Result.SubSlot := $00;
    Result.Found := true;
    Result.ID := ItemID;
    Exit; //Already found, can exit
  end;
Lhand :=ReadMemInteger(L_Hand);
  if LHand = ItemID then begin
    Result.Slot := LHandID;
    Result.SubSlot := $00;
    Result.Found := true;
    Result.ID := ItemID;
    Exit; //Already found, can exit
  end;
  ASlot := ReadMemInteger(arrowslot);
  if ASlot = ItemID then begin
    Result.Slot := AmmoID;
    Result.SubSlot := $00;
    Result.Found := true;
    Result.ID := ItemID;
    Exit; //Already found, can exit
  end;
//If not found on hands and ammo slot, so, let we find on bp
  for BpNr := 0 to 20 do begin
  CurrentBP := BPStart+DistBP*BpNr+FirstItem;
//  ShowMessage('Lendo Memória do Tibia'+#13+'BP Atual: '+inttostr(BpNr)+#13+'Memoria Aberta: '+inttohex(CurrentBP-FirstItem-BPisOpen,5)+#13+'Aberta: '+inttostr(ReadMemByte(CurrentBP-FirstItem-BPisOpen)));
  if ReadMemByte(CurrentBP-FirstItem-BPisOpen) = 0 then begin
      Result.Slot := $99;   //The BP isn't open, if the item
      Result.SubSlot := $99;//Not found until now, so, not found
      Result.Found := false;
      Result.ID := 0;
    exit;
  end;
  for ItemNr := 0 to ReadMemInteger(CurrentBP-FirstItem+$20)-1 do begin
  CheckingItem := ReadMemInteger(CurrentBP+DistItems*ItemNr);
     if CheckingItem = ItemID then begin
       //We Found... But the item realy exist?
       VisibileItems := ReadMemInteger(CurrentBP-FirstItem+ItemsVisible);
       if VisibileItems >= ItemNr+1 then begin
         Result.Slot := BPID+BpNr;
         Result.SubSlot := $00+ItemNr;     //Yes, We Found!
         Result.Found := true;
         Result.ID := ItemID;
       exit;
     end;
  end;
  end;
  Result.Slot := $99;
  Result.SubSlot := $99;
  Result.Found := false;
  Result.ID := 0;
end;
end;

procedure Rainbow;
var
packet:array[0..255] of byte;
begin
Packet[0]:=$06;
Packet[1]:=$00;
Packet[2]:=$D3;
Packet[3]:=ReadMemInteger(GetBLPosition(ReadMemInteger(SelfID)).Position+92);
Randomize;
Packet[4]:=Random(254);
Randomize;
Packet[5]:=Random(254);
Randomize;
Packet[6]:=Random(254);
Randomize;
SendIpcMessage(pchar(inttostr(ProcID)),@Packet,8);
end;


procedure AllHaveLight(size,color:integer);
var
CurrentPos:Integer;
begin
if botclose then
exit;
CurrentPos:=BLStart;
while currentPos <> BLEnd do begin
if currentpos = getblposition(ReadMemInteger(SelfID)).Position then
CurrentPos := CurrentPos+DistChar;
MWInt(CurrentPos+DLightSize,size,4);
MWInt(CurrentPos+DLightColor,color,4);
CurrentPos := CurrentPos+DistChar;
end;

end;

end.
