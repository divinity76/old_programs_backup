program bictrainer;

{%ToDo 'bictrainer.todo'}

uses
  Forms,
  tiksbot in 'tiksbot.pas' {Form1},
  grafico in 'grafico.pas',
  tfunctions in 'tfunctions.pas',
  tempinjecter in 'tempinjecter.pas' {Form2},
  broad in 'broad.pas' {fmbroad},
  shot in 'shot.pas' {shotwho},
  ssrune in 'ssrune.pas' {whorune};

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'BicTrainer';
  Application.CreateForm(TForm2, Form2);
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(Tfmbroad, fmbroad);
  Application.CreateForm(Tshotwho, shotwho);
  Application.CreateForm(Twhorune, whorune);
  Application.Run;
end.
