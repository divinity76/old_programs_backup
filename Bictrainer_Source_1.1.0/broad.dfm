object fmbroad: Tfmbroad
  Left = 640
  Top = 337
  BorderStyle = bsDialog
  Caption = 'Auto Broadcaster'
  ClientHeight = 254
  ClientWidth = 267
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 255
    Height = 13
    Caption = 'Please wait while BicTrainer sends msg to all your list!'
  end
  object Label2: TLabel
    Left = 8
    Top = 20
    Width = 214
    Height = 13
    Caption = 'You can stop the process anytime you want!'
  end
  object Label3: TLabel
    Left = 175
    Top = 39
    Width = 76
    Height = 13
    Caption = 'Time estimated:'
  end
  object lbtempo: TLabel
    Left = 180
    Top = 55
    Width = 3
    Height = 13
  end
  object Label7: TLabel
    Left = 175
    Top = 83
    Width = 31
    Height = 13
    Caption = 'Status'
  end
  object lbstatus: TLabel
    Left = 180
    Top = 96
    Width = 3
    Height = 13
  end
  object charlist: TValueListEditor
    Left = 8
    Top = 39
    Width = 161
    Height = 209
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goAlwaysShowEditor, goThumbTracking]
    TabOrder = 0
    TitleCaptions.Strings = (
      'Char Name'
      'Msg Status')
    ColWidths = (
      77
      78)
  end
  object btstop: TButton
    Left = 184
    Top = 147
    Width = 75
    Height = 25
    Caption = '&Stop'
    TabOrder = 1
    OnClick = btstopClick
  end
  object tcharlist: TTimer
    Interval = 100
    OnTimer = tcharlistTimer
    Left = 120
    Top = 128
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 128
    Top = 136
  end
end
