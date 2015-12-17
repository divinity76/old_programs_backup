object whorune: Twhorune
  Left = 653
  Top = 377
  BorderStyle = bsNone
  Caption = 'whorune'
  ClientHeight = 56
  ClientWidth = 171
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
    Width = 160
    Height = 13
    Caption = 'Please, click in the rune you want'
  end
  object Label2: TLabel
    Left = 8
    Top = 20
    Width = 160
    Height = 13
    Caption = 'to use, when  ready, this window'
  end
  object Label3: TLabel
    Left = 8
    Top = 32
    Width = 74
    Height = 13
    Caption = 'WILL CLOSE!!!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object trune: TTimer
    Enabled = False
    Interval = 100
    OnTimer = truneTimer
    Left = 72
    Top = 16
  end
end
