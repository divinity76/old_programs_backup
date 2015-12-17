object shotwho: Tshotwho
  Left = 547
  Top = 283
  BorderStyle = bsNone
  Caption = 'shotwho'
  ClientHeight = 57
  ClientWidth = 232
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 209
    Height = 13
    Caption = 'Type here the name of who you want shot:'
  end
  object Edit1: TEdit
    Left = 8
    Top = 27
    Width = 145
    Height = 21
    TabOrder = 0
  end
  object Button1: TButton
    Left = 159
    Top = 24
    Width = 65
    Height = 25
    Caption = 'Ok'
    Default = True
    TabOrder = 1
    OnClick = Button1Click
  end
end
