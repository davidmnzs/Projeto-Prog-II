[Setup]
AppName=Projeto-Prog-II
AppVersion=1.0
DefaultDirName={autopf}\Projeto-Prog-II
DefaultGroupName=Projeto-Prog-II
OutputDir=C:\output
OutputBaseFilename=Projeto-Prog-II-Setup
Compression=lzma
SolidCompression=yes

[Files]
Source: "Projeto-Prog-II.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "arquivos\*"; DestDir: "{app}\arquivos"; Flags: recursesubdirs createallsubdirs

[Icons]
; Atalho no Menu Iniciar
Name: "{group}\Projeto-Prog-II"; Filename: "{app}\Projeto-Prog-II.exe"

; Atalho na Área de Trabalho
Name: "{commondesktop}\Projeto-Prog-II"; Filename: "{app}\Projeto-Prog-II.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Criar um ícone na área de trabalho"; GroupDescription: "Ícones adicionais:"
