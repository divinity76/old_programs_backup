library bic;

uses
  SysUtils,
  madCodeHook,
  Winsock,
windows;

{$R *.res}
var
sendNext: function(s: TSocket; Buf: Pointer;  len, flags: Integer): Integer; stdcall;
recvNext: function(s: TSocket; Buf: Pointer;  len, flags: Integer): Integer; stdcall;
TibiaSocket:TSocket;



procedure EnviaPacket(name: pchar; messageBuf: pointer;
  messageLen: dword; answerBuf: pointer; answerLen: dword);stdcall;
  begin

  if messageLen = 2 then begin
    DestroyIPCqueue(name);
    exit;
  end;

  SendNext(TibiaSocket,messageBuf,messageLen,0);
end;

{$Region 'APIs Callbacks'}
function sendCallBack(s: TSocket; Buf: Pointer; len, flags: Integer): Integer; stdcall;
begin
TibiaSocket:=s;
SendIPCMessage(pchar(inttostr(GetCurrentProcessId+1)),Buf,len);
Result := sendNext(s, Buf, len, flags);
end;

function recvCallBack(s: TSocket; Buf: Pointer; len, flags: Integer): Integer; stdcall;
begin
TibiaSocket:=s;
Result := recvNext(s, Buf, len, flags);
end;
{$ENDREGION}

begin
  HookAPI('ws2_32.dll', 'send', @SendCallBack, @SendNext);
  HookAPI('ws2_32.dll', 'recv', @recvCallBack, @recvNext);
  createIPCQueueEx(pchar(inttostr(GetCurrentProcessId)),EnviaPacket);
end.
