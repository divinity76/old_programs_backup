___________________________________________________________________________

                                              
      ExeInfo PE ver. 0.0.2.9  by A.S.L  ( c ) 2006.03 - 2011              
                                              
                     freeware version     for Windows XP                    
                                              
   Windows 32 PE executable  file checker , compilators, exe packers ....           
                                              
       with solve hint for unpack  /  internal exe tools / rippers                 
                                              
___________________________________________________________________________




		Internal Tools Menu :
		---------------------

- overlay remover / adder ...                   - generate new file without overlay data , added data from file
- save overlay as external file
- EP Corrector ( for Delphi / C++ 5/6 )         - generate many exe file with Entry Point  
- EP Corrector ( for Delphi v.5 ) Runtime       - correct EP  
- XoR permutator (xor,or,shl..)       		- create one file with xor data ( 255x2000 bytes )
- Section splitter                    		- save exe sections as files & exe header
- Search strin 8 / 16 bit           		- enter 8 bit string = double search - 16 bit strings & 8 bit ( F7 key )
- REGistry call finder + CLSID        		- find registry call &  regedit.exe strings
- overlay xor uncrypter               		- uncrypt one byte crypted exe in ovl. 
- External exe file runner            		- ( from exeinfopeRUN.cfg ) - txt user file list to run
- External exe detector runner                  - from shell registry : Fast Scaner , CFF Explorer , Bit Detector , Quick Unpack ....
- Add fake signer Visual Basic for UPX          - You can add VBasic v5/6 signature to exe file packed with upx.exe



		File Menu :
		--------------------

+ Rename file
+ Copy file As.. *.bak
+ Execute - create executable process  ( exe )
+ Execute - windows ext. associate     ( dll ,zip ... )
+ Delete file ( ALt+Del) - work in multiscan mode
+ Run multifile scanner mode ( Directory scan )
+ kill Windows process
+ - view global log file ( c:\Raport-exeinfo-log.txt ) 
  - delete global log file ( no confirm )


		Rippers Menu : 
		--------------------

- www address searcher inside exe   - work on any file

- ExE inside ExE ( Win32 Pe windows executable)      - work on any file

Archives ripper :

- Zip archives inside ExE  www.winzip.com  - work on any file
- Rar archives inside ExE  www.rarlab.com  - work on any file
- 7z  archives inside ExE  www.7-zip.org   - work on any file
- CAB MS archives inside ExE  (for MSI installers ) - work on any file

- SWF flash Adobe animation files ( internal length fixer for non exe files )
- ICO nonstandard icon ripper

Graphics ripper submenu : 

BMP , 
PNG , 
JPG ( JFIF only !!! Exif - not supported ) , 
GIF ( static pic. only )


- ( All in one ) - for lazy boys ( without 'www address' )


- XML ripper


keys :

F1 key  - keyboard help
F2 key  - Multiple file scanner for *.exe files 
F3 key  - external view ( hiewdemo.exe or hiew32.exe ) path directory
F4 key  - external test ( peid.exe ) path directory
F5 key  - external test RDG Packer Detector ( I read location from Win registry )
F6 key  - external test DiE.exe Detect it Easy  ( I read location from Win registry - shell integration req.)
F7 key  - 8 / 16 bit String finder

F9 key  - UPX.exe  pack ( LZMA ) from path directory
Alt+F9  - UPX.exe  pack ( LZMA + Force mode ) from path directory
F10 key - UPX.exe unpack
Alt+S   - ZOOM Window x2 !
Right Mouse Button - Run external exe detectors - Fast Scanner, CFF Explorer , Quick Unpack ... - required shell integration before !

Alt+Delete - delete file

"+" ,"-" - Numeric KEY =  adjust transparent Form


Non executable file detection :

Image file   - jpg , png , mng ,gif (87/89) , bmp , tiff , TGA new only .
Sound file   - mp3 (ID3/noID) ,wma , ogg , .mid , .xm , .mod , AAC
Video file   - avi (divx/xvid + info ) , wmv , mpg , 3GP , mov , mp4/m4v , .RM , FLV
Archive file - 7zip ,zip ,rar , gzip , bzip

other : chm (Microsoft HTML Help), msi , pdf , xml , fws , cws , php , html , hlp , mdb , lnk ,reg , rtf.
        Linux ELF , .dylib - Mac OS X Universal Binary executable , .class - java






Overlay detector : 


01. zip archives
02. cab archives  
03. SWF Flash object  ( packed & unpacked format )
04. Executable PE file
05. 7zip archives
06. RAR archives
07. MSI/DOC/XLS

- Plugins like a Peid.exe ( 70 % compatible  )



-------------------------

 Multiscaner use - command line : 

- Exeinfope *.sys  ( show all .sys files )

- Exeinfope *.* /s ( Show All PE files and sent to log file ( s = silent mode no GUI !  -> !ExEinfo-Multiscan.log  ) 
  Exeinfope *.exe /s
 
 

-------------------------





*ACM - anti cheat mechanism ( anti fake sign )

_______________________________________________________________________


	www site :      www.exeinfo.xwp.pl


_______________________________________________________________________





        ExeInfo detection list :
        ----------------------------------------------  


001. RealArcade Wrapper ( Microsoft Visual C++ )  50% detection not all versions
002. Borland Delphi ( 2.0 - 7.0 )
003. Microsoft Visual C++ ver. 5.0 ~ 6.0 ( exe )
004. Microsoft Visual C++ ver. 7.x ( exe ,dll)
005. PEtite 2.x -> Ian Luck
006. UPX exe 0.89.6 - 1.02 / 1.05 - 1.93B -> Markus & Laszlo
007. UPX dll file - 1.93Beta -> Markus & Laszlo
008. Aspack v2.12 -> Alexey Solodovnikov
009. EXECryptor v.2.3.1-6  ( www.strongbit.com )    
010. Morphine ver.2.7b  ( plugin Peid.exe )
 
011. AC protect 2.0 by  RIScO Software Inc. ( www.ultraprotect.com )
012. ASprotect 2.1 reg ( www.aspack.com/asprotect.htm )  only exe files DLL files detect as ASpack :)
013. AHTeam EP Protector ver.0.3 priv
014. WinUpack 0.39 final by Dwing  ( http://dwing.51.net )  :-((
015. Software Compress ver. 1.2 Lite - www.bgsopt.com
016. PEcompact ver.2.78a - 3.0.3.9  - www.bitsum.com
017. nsPack ver.2.3 unreg - by North Star  -  www.nsdsn.com
018. nsPack ver.3.0 - 4.1 reg - by North Star  -  www.nsdsn.com
019. Mole Box 2.5.7  by Teggo. - www.molebox.com

020. Microsoft Visual C++ ver. 8  ( ??? )
021. EXE Guarder 1.8 - 2.1 (2006/2008 unreg)  www.exeicon.com/exeguarder
022. EXE Wrapper ver. 2.3-2.5 ( www.533soft.com/exewrapper ) - how to remove password
023. Exe password protector 1.0.5.100  (protect/unprotect)
024. TASM / MASM / FASM - assembler language 
025. MS Visual Basic 5.0-6.0   dll
026. MS Visual Basic 5.0-6.0   exe

027. Armadillo 4.4x ~ 4.62..5.0~7.0  32bit   -  www.siliconrealms.com   (60e8)
028. Enigma protector v1.1x   - www.enigma.izmuroma.ru © Sukhov Vladimir 2004-2006
029. SVK-Protector v1.32 demo  -  Pavol Cerven - www.anticracking.sk

030. Generic check : ASprotect 1.? old version  ( www.aspack.com/asprotect.htm )  exe only
031. Generic check - AC protect 1.? by  RIScO Software Inc. ( www.ultraprotect.com )
032. Packman v1.0 Brandon LaCombe  ( http://packman.cjb.net )
033. modified exe , EP code = Borland Delphi ( 2.0 - 7.0 )
034. ExeStealth V2.76  www.webtoolmaster.com
035. FSG v2.0   F[ast] S[mall] G[ood]  - www.xtreeme.prv.pl

036. Generic check - Aspack v2.1x -> Alexey Solodovnikov
037. Aspack v2.12b? -> Alexey Solodovnikov
038. Program protector v2.1unreg ( exe password - DECODE PASS ! ) - www.blumentals.net
039. Obsidium v1.3 software protection system (demo) - www.obsidium.de
040. ARMprotector v0.1 by SMOKE 2004
041. ARMprotector v0.3 by SMOKE 2004
042. SDProtector Profesional Edition v1.12  ( 2003 ) -   www.sdprotector.com

043. Themida 1.0 -1.3? - Adv.Win.Software Protection System (c) 2004-2005 Oreans Technologies - www.oreans.com
044. yodas Protector v1.03.3 -  http://yodap.has.it  2004-2006
045. yoda's Crypter v1.3 - Ashkbiz Danehkar  2004-2005
046. PE-Pack v0.99 (c) 1998 by ANAKiN
047. WATCOM C/C++ 1988-1995
048. Microsoft CAB SFX module
049. Generic check : Microsoft Visual C++ vx.x
050. UPX -> Markus & Laszlo ver. [ 2.00 ] <- version info from file  
051. PeSpin v1.304 public by CyberBob - http://pespin.w.interia.pl
052. UPX -> Markus & Laszlo ver. [ ] - EXE modified!!!
053. UPX -> with extra sections - Real EP resolver   ( [ ] - required Fast scan unchecked )
054. PolyEnE v0.01+ Polymorphic Encryptor (c) 2001 Lennart Hedlund   ( [ ] - required Fast scan unchecked )
055. Nullsoft PiMP Stub - ( read from Ovl  : NullsoftInst3" )
056. eXpressor PE Packer v1.4.5.1  - www.cgsoftlabs.ro   ( exe , dll )
057. Thinstall 2.4x - 2.5x -> Jitit Software  - www.thinstall.com
058. Thinstall 2.7x  -> Jitit Software  - www.thinstall.com
059. Nullsoft scriptable install system 2.xx - ( read from Ovl  : NullsoftInst )
060. Inno Setup Module [SFX] - Borland Delphi Inno Setup Module [unknown] 

061. Private EXE Protector 1.7 ( 2003-2006 )  www.setisoft.com
062. Excalibur v1.03r (c) by forgot -> read from file [ Excalibur (c) DFCG ] , http://www.breezer.ful.cn 
063. MSLRH v.032a - SISTEMA DE PROTECCION ANTICRACKEO
064. ShareGuard Loader V3.6 Zapper Software - www.zapperSoftware.com
065. Borland C++ 1999
066. Zip Sfx Archive 
067. Rar Sfx Archive 
068. 7-Zip Sfx Archive 
069. WinZip Sfx ver. 8.x   www.winzip.com
070. Zylom Game Installer zip Sfx ( MS Visual C++ 7.0 ) 
071. Borland C++  2002 /2005 - Copyright 200X Borland Corporation
072. WinZip Sfx ( generic check ) www.winzip.com
073. Lock Express 2.0 Build 9.2 - 1997-2006 Sciensoft Research Inc
074. FreeBASIC Compiler v0.14-0.17  (c) 2004-2006 Andre Victor T.Vicentini - console App.
075. generic check : InstallShield 2003 ( MS Visual C++ 5/6.0 )

076. InstallAware Setup Squeezer InstallShield - www.installaware.com ( 7zip archive )
077. Installer Nullsoft PiMP Stub ( UPX pack )
078. Generic check : Nullsoft PiMP Stub installer
079. ASprotect 1.1c old version  ( www.aspack.com/asprotect.htm )
080. Microsoft Visual C# / Basic.NET
081. Setup Dev INSTALLER – Version 1.3 © Shere Khan – November 2005 ( MS Visual C++ 5/6.0 )
082. Dev-C++ Compiler v4.9.9.2 - Bloodshed Software ( www.bloodshed.net )
083. Generic check : EXE STICKER like DotFix FakeSigner
084. DotFix FakeSigner v3.4 ( ASPR Stub ) http://fakesigner.dotfix.net
085. PeLock v.1.x Bartosz Wójcik www.pelock.prv.pl
086. MS IExpress 2.0 - Win32 Cabinet Self-Extractor
087. generic check : MS IExpress x.x - CAB installer (  in section II )
088. InstallShield (R) Setup Launcher  v.7.x  CAB file ( MS Visual C++ 5/6.0 )
089. PEcompact ver.1.41 - v1.84  - www.bitsum.com
090. ORiEN ver.2.11~2.12  - ( 1994-2003 http://zalexf.narod.ru )
091. VMProtect v.1.2x (demo) 2003-2006 PolyTech - www.polytech.ural.ru ( only EP protection )
092. FASM ver. 1.67  50% detection
093. Private exe Protector v1.9x - www.setisoft.com ( morph )
094. Krypton The Krypter ver.0.3 by Yado - www.lockless.com
095. MEW 11 SE 1.2 by Northfox (2004)  -  Northfox.uw.hu
096. PEncrypt 4.0 Public Release / 4.0 Phi -> junkcode -  www.junkcode.cjb.net  
097. SDProtector Pro Edition v.1.16  ( 1.1 SDP! ) <- info from file.   www.sdprotector.com
098. PE Diminisher v.0.1 ( 1999 ) - www.phrozencrew.com/~teraphy
099. !EP (EXE Pack) v1.0   g-l-u-k [TeaM - X] 2005 -  www.softprot.cjb.net
100. [G!X]'s Protector v1.2   -  http://breezer.ys168.com
101. Active PE Scrambler / APES / v. 1.0   (2005)  [TeaM - X]  - www.team-x.ru
102. (UPX) PowerArchiver 2006 [ ZIP/ CAB/ unknown ] SFX v.9.63.x - www.powerarchiver.com
103. GameHouse.com installer ( MS Visual C++ )  inside  Wise Installer
104. Dev-C++ Compiler v4.9.9.2 ( MINGW 32 v5.x.x ) - Bloodshed Software ( www.bloodshed.net )
105. Hide&Protect v1.0x ( 2005 ) - www.SoftWar-protect.com
106. WWPack32 ver 1.xx  ( 1997,98 ) by P. Warezak and R.Wierzbicki
107. CHAOS Self Extractor 3.9 (1998-2006) ( WWPack-ed ) http://safeSofthome.com
!108. Xtreme-Protector v.1.08 (c) 2003 www.oreans.com/xprotector/xprot.htm  
109. LCC Win32 v1.x  ( Jacob Navia )  http://www.cs.virginia.edu/~lcc-win32/
110. LCC Win32 v1.x DLL ( Jacob Navia ) www.cs.virginia.edu/~lcc-win32
111. Hmimys-Packer v1.0
112. ExeFog v.1.1x - 2005 - www.bagie.xost.ru
113. PolyCrypt PE v.2.1.x  ( 2004-2005 ) - www.jlabsoftware.com (exe/dll)
114. SimplePack v1.0 - 1.2 ( LZMA / APLIB - Packman compression library 1999-2005 Igor Pavlov )
115. SimplePack v1.11 - 1.2x ( Method 2 NT )
116. Unopix Version 1.10 Final 2006 Scrambler for PE files ( exe/dll )
!117. PPC PROTECT ver 1.1 ( 2006 )  Alexey Gorchakov   www.ppc-protect.com
118. Inno Setup Uninstaller - Borland Delphi 
119. Armadillo v2.5x - v2.6x -  www.siliconrealms.com
120. DotFix NiceProtect v1.2  by GPcH Soft ( 2006 ) - www.niceprotect.com
121. CreateInstall v4.x Gentee ( 2004 - 2008 ) - www.createinstall.com ( free/light/full)
122. Gentee Programming Language © 2004-2006  www.gentee.com
123. RLPack v.1.11 BasicEdition ( uses aPLib 0.42 ) http://ap0x.jezgra.net
124. ReversingLabsProtector 0.7.4beta  http://ap0x.headcoders.net
125. Install Creator /Pro ver.2.0 ( 2003-2009 )  - www.clickteam.com
126. PowerBasic /CC 3.0x/CC 4.0/Win 7.0x/Win 8.0x -  www.powerbasic.com
127. WinUHA ver.2.0  Sfx Archive - www.winuha.com  ( UPX ) 
128. ZipGenius 6.0.x  Sfx Archive - www.zipgenius.it ( Borland Delphi )
129. PEbundle ver.3.20 ( 2003 ) Jeremy Collake  - www.bitsum.com /
     / Alloy Executable Compressor v.4.x- Copyright © 2000-2010 PGWARE - www.pgware.com
130. Lazy Assembler  Version 0.53 (26 Sep 2006) Freeware (c) 2000-2006 Stepan Polovnikov
131. nPack v1.1.300 (aPlib ) by NEOx ( 2006 )  www.uinc.ru
132. Installer - Setup Factory 6.0 - 7.0  Indigo Rose Corporation ( 2006 ) MS V C++ 6.0 
133. dePack by deNULL - www.ooooQ.cn
134. Goat's PE Mutilator v.1.6 ( 2005 ) - www.geocities.com/killereaglesoftware
135. RLPack v.1.14-1.18 BasicEdition ( uses aPLib 0.43 / LZMA 4.30  ) http://ap0x.jezgra.net
136. VBOWatch protector v2.0  Copyright [c] 2006 MoonLight - www.ooooQ.cn
137. Generic check : build like - Private exe Protector v2.0 - www.setisoft.com
138. Easy Code v.1.0x ( GUI for assembler ) Ramon Sala - www.easycoder.org
139. Mole Box 2.6.1  by Teggo. - www.molebox.com
140. SLVcOdeProtector v.1.12 by SLV  - www.ooooQ.cn
141. Exewrap MFC Application v.1.0 ( 2003 )
142. Microsoft Visual C++ 8 compiler ( 2006 )
143. RosAsm -V2.039c - http://betov.free.fr ( effectiveness 80 % )
144. Software Compress ver. 1.4 Lite - www.bgsopt.com
145. Intel (R) C++ Compiler
146. FreePascal ver : FPC 1 - 2 Win32 -> (Berczi Gabor, Pierre Muller & Peter Vreman)
147. Open WATCOM C/C++32 Portions Copyright (c) Sybase 1988-2002
148. File2Pack SFX v.2.0 2006 (F2P Self Extractor ) SHOW PASSWORD! - www.mental9production.com ( MS VB5/6 )
149. PV Logiciels dotNet Protector 4.0 2003-2005   http://dotnetprotector.pvlog.com
150. ReflexiveArcade Game wrapped file   ( *.RWG )
151. DAStub Dragon Armor (BamBam0.0.4.1) from Orient 2006 www.ooooQ.cn
152. Akala EXE Lock ver.3.20 www.zero2000.com (Aspack v2.12 -> Alexey Solodovnikov) - PASSWORD DECODER(N) OR HOW TO REMOVE PASSWORD
153. BeRoEXEPacker - Version 1.00 - Copyright (C) 2006, Benjamin BeRo Rosseaux  ( Exe/DLL )
154. EXE Password Protector v.1.1  (MSV C++ v7) - www.eltima.com/products/exe-password - INFO HOW TO REMOVE PASSWORD
155. AGInstaller 1.9.12 ( UPX pack ) Copyright (c) 2001-2006 Agentix Software  - www.aginstaller.com
156. CreateInstall v2003.3.5  www.createinstall.com/www.gentee.com ( EP check & OVL )
157. Protection PLUS - Instant plus (software key) 2.0.98.0 (2005) - www.softwarekey.com  Concept Software
158. Wise Installation System! std/pro 9.02 (c) Wise Solutions Inc. - www.wise.com
159. Wise Installation System! ver. ?.? (c) Wise Solutions Inc. - www.wise.com
160. Wise Uninstaller Wizard (sec3)  - www.wise.com - MS Visual C++ ver.6
161. m9P Editor Plus v.1.0.300 Distributable Executable Rich Text - DERT™ X ©mental9Production, 2005 -  www.mental9Production.com - INFO HOW TO REMOVE PASSWORD
162. Nullsoft uninstaller - www.nullsoft.com - ( UPX packed )
163. Nullsoft uninstaller - www.nullsoft.com
164. Softwrap (XTREAMLOK) ver. 1.x~3.x - www.softwrap.com ( exe/dll )
165. RLPack v.1.14-16 Full Edition - False signatures unichecker
166. RLPack v.1.14-16 Full Edition ( uses aPLib 0.43 / LZMA 4.3x  ) http://ap0x.jezgra.net
167. Salfeld Computer EXE Password  2004 v 7.114.0.0 trial - www.salfeld.com ( Borland Delphi )
168. Wise for Windows Installer pro 4.21 ( CAB )  - www.wise.com
169. Tarma Installer ver. 2.99.xx (2005)  Tarma Software Research Pty Ltd. - www.tarma.com ( MS Visual C++ ) 
170. NTkrnl Secure Suite v.01 packer or protector - www.ntkrnl.com ( exe )
171. NTkrnl Secure Suite v.01 packer or protector - www.ntkrnl.com ( dll )
172. [dUP2 -> diablo2oo2]  v.2.1x patchengine ( patch ) - Mircosoft MacroAssembler  - http://diablo2oo2.cjb.net
173. [dUP2 -> diablo2oo2]  v.2.1x patchengine ( loader installer ) - Mircosoft MacroAssembler  - http://diablo2oo2.cjb.net
174. PE password encryptor 31-01-2000 by SMT ( asm ) - [ OEP finder included ]
175. WinUDA 0.271 sfx ( 2004 ) by Dwing  http://dwing.51.net
176. kkrunchy 0.1x >> radical exe packer - www.farbrausch.de/~fg/kkrunchy OR www.farb-rausch.com
177. kkrunchy 0.23 alpha 2 >> radical exe packer (c) f. giesen 2003-2005  - www.farbrausch.de/~fg/kkrunchy
178. CyberInstaller Suite 2006 1.1 - SilverCyberTech 2003-2007 
179. Eurora3D - free installator - www.extramedia.co.yu/eurora3d  ( ASM ) 
180. Microsoft Visual C++ ver. 7.1 [DEBUG] exe
181. Fucking Fake File 1.0 by wspomagacz 2005.11( EXE Binder exe,jpg hidden inside] )
182. Anskya Polymorphic Packer V 1.3 Code By Anskya
183. Self-Extracting Archive Utility (SEAU) ver. 15.0  2006 ( Aspack v2.12 -> Alexey Solodovnikov ) - http://gammadyne.com
184. PE-Pack v 1.0 (c) 1998 by ANAKiN
185. PKLITE32(tm) - Version 1.1 02-15-1999 ( exe )
186. PKLITE32(tm) - Version 1.1 02-15-1999 ( DLL )
187. EncryptPE V2.2006.10.25 China Cracking Group - www.encryptpe.com
188. CC386 Version 3.28.1.6 Copyright (C) (GPL)  LADSoft 1994-2006
189. PC Guard for Win32 V5.01  -  www.sofpro.com
190. JDPack ver 1.01 ( 2005 ) - www.tlzj18.com ???
191. Netopsystems AG INSTALLER FEAD(R) SFX (MS C++)  - www.netopsystems.com ( packed UPX & not packed )
192. Borland C++  1995~1998  - www.borland.com
193. eXpressor PE Packer v1.5.0.1  - www.cgsoftlabs.ro
194. Excelsior Installer v1.0 2003-2007 ( MS Visual C++ 6.0 ) - www.excelsior-usa.com
195. tElock v0.98 Freeware PE-Compressor/Encryptor (c) 2000-2001 by tE!
196. UPX Lock v1.02  (2007.02) - www.team-x.ru
197. softSENTRY 3.00  1999 - 20/20 Software Inc. www.twenty.com ( site closed )
198. DxPack ver 0.86  ( 2001.06 )
199. Neolite 2.0 -> Neoworx Inc. ( 1999.03.20 ) - www.neoworx.com  ( site closed )
200. ZipWorx SecureEXE v3.0 (2004-2007) www.zipworx.com (Neolite packed)
201. [ PE-DIY Tools V1.10 2004 ] by A.Young (PoJieYong) - www.w-yong.com  ( how to unprotect,oep info )
!202. aUS v0.5 beta ( upx scrambler 2005.08 ) - http://ap0x.headcoders.net ( bad link? )
203. EXE protector 2.01a  Eyhab Hillail ( 1998-2003 )- http://oxygen72.tripod.com ( how unprotect pass )
204. 32Lite 0.03a -> Oleg Prokhorov   www.????
205. aPackage SFX v.1.14 2001-2002 Joergen Ibsen [32Lite v0.03a packed]
206. NTPacker V2.1 by ErazerZ (2005.12) ErazerZ@gmail.com ( zPlib / XOR / aPlib+xor )
207. WinHKI v1.77 SFX 2000-2007 by Hanspeter Imp ( hki archive only ) www.winhki.com (packed PEcompact ver.2.7x) 
208. nBinder 5.1.0 ( 24.03.2007 MSV C++ 8.0 ) NKProds Software -  www.nkprods.com
.209. (Basic check) : Securom 7.1 -> Sony DADC  - www.securom.com
210. Cexe Executable Compressor v1.0b Copyright 1999, Tinyware, Inc. - www.tinyware.com by Scott Ludwig
211. ASprotect 2.3 SKE ( www.aspack.com/asprotect.htm ) 25%
212. Easypano Virtual Tour player ( MSV C++ )  - www.easypano.com
213. PeX v0.99  bart/CrackPl (2000) (APLib 0.26 by J.Ibsen)  - longdiy.myrice.com
214. YZPack v.2.0b.aplib (c) UsAr ( 2007.03 )
215. YZPack v.1.1 LZMA (c) UsAr ( 2006.08 )
216. YZPack v.1.2 aplib/LZMA (c) UsAr ( 2007.03 )
217. ExeStealth V2.72 (Share.ver) -   www.webtoolmaster.com
218. Generic check : ExeStealth V?.? (share.ver) -   www.webtoolmaster.com
219. ExeStealth V2.x (Regg.ver) -   www.webtoolmaster.com
220. nsPack ver.1.x - x.x by North Star  -  www.nsdsn.com
221. Microsoft Visual C++ 6 DLL
222. exe32pack 1.42  Copyright 1999-2004 www.SteelBytes.com
223. Protect Exe 0.4 Beta ( PROEX ) 2002 - www.dpaehl.de.cx ( UPX packed )
224. SexyPacker v.1.0.1.0 ( c ) 2001 - www.smalleranimals.com ( SFX ) MSV C++ 5.0
225. ID Executable Password 1.2 (c) 2005 Fastlink2 Build: 08/08/2005 - www.idsecuritysuite.com - !SHOW PASSWORD!
226. ID Application Protector v.1.2 Unreg (c) 2005 Fastlink2 - www.idsecuritysuite.com ( OEP info ,how to clear TRIAL)
227. Pelles C for Windows v2.xx - 4.50 ExE ( 1999-2006 ) - www.smorgasbordet.com/pellesc
228. Wise for Windows Installer pro ?.?? ( CAB in section 4 ) MS C++  - www.wise.com
229. WinUtilities 5.2 EXE Protector 1.0 ( 2002-2007 ) YL Computing Inc. - www.ylcomputing.com -  ( Info how Pass remove/unprotect )
230.  [section protection] VMProtect v.1.25 - 1.x (demo) 2003-2006 PolyTech - www.polytech.ural.ru
231. REALbasic 2007 R2 Standard Edition ( 1997-2007 REAL Software ) - www.realbasic.com ( exe only )
232. UPX 3.0 -> Markus & Laszlo ver. [ 3.00 ] <- info from file. ( sign for DEV C++ compiler )
233. Microsoft Visual C++ ver. 7.1 EXE/DLL  (3 bytes sign - easy to false)
234. Beria v0.07 public WIP ( 2005 ) - symbiont ( aPlib )
235. NoodleCrypt version 2 by NoodleSpa ( 2000.08 )
236. VPacker v0.02.10 by tt.t (exe only 2006.04 aPlib)
237. Private exe Protector v.2.00-2.15 ( 18.04.2007 ) www.setisoft.com
238. Free Pascal Compiler v.2.1.4 i386 GUI APP ( 11.05.2007 ) Berczi Gabor - www.freepascal.org
239. Free Pascal Compiler v.2.1.4 - 2.2.4 i386 CON APP  Berczi Gabor - www.freepascal.org
240. Free Pascal Compiler v.2.1.4 i386 DLL APP ( 11.05.2007 ) Berczi Gabor - www.freepascal.org
241. Installshield v.12 (MSV C++ )  www.installshield.com / www.macrovision.com
242. generic check2 : InstallShield v.12-14 2008 ( MS Visual C++) www.installshield.com / www.macrovision.com
243. FASM ( 1.3x -1.67 ) 2004-2007 http://flatassembler.net - Tomasz Grysztar
244. Thinstall VS 3.0.x  -> Jitit Software  - www.thinstall.com  
245. Astrum InstallWizard v2.24.20 ( 1999-2006 ) - www.thraexsoftware.com ( MS Visual C++ )
246. WinZip SelfExtractor 3.0 ( MSV C++ v7 ) 1996-2006 WinZip Int. LCC - www.winzip.com
247. Wise Instalation Express v7.0 2006 (SFX CAB) MSV C++ - wise.com / ALTIRIS
248. VisageSoft Installer ? WISE for Win/.msi ( MSCF CAB ) Borland C++ - www.visagesoft.com

249. ST Protector v1.5 SE ( 2006 ) - Silent Software - www. ???
250. (exe) Visual Protect v2.5.7 ( 2000.12  www.visagesoft.com
251. (dll) Visual Protect v2.5.7 ( 2000.12  www.visagesoft.com
252. eXpressor PE Packer v1.5.0.1 (MODE: Protection) - www.cgsoftlabs.ro
253. The Enigma Protector 1.31 unreg (2007.06.15) - Vladimir Sukhov - www.enigmaprotector.com ( exe/dll )
254. generic check: (exe) Visual Protect ( 2000? )  www.visagesoft.com 
255. RCryptor 1.6d by Vaska ( 2007.01 ) only exe file protector - ( OEP info )
256. Polymorph Crypter,Beta Morphnah (c) puccxak.com ( 2007.05 ) - ( OEP info )
257. Pohernah v1.0.3 puccxak.com ( 2007.03 )
258. QIP[Crypt] ( 2007.06 ) Borland Delphi Crypter
259. SimbiOZ (RUS)  ! Rootkit exe hider ! ( OEP info - for C++/Delphi )
260. AsdPack2 ( EP overflow exe - Delphi or C++ detector )  [ detection 75% ]
261. QSetup Instalation Suite 8.5.0.4 - 26.05.2007 - www.pantaray.com
262. Perplex PE-protector v1.01devel  2002-2003 by [tc] GiveMe5/BliZZaRD
263. Mole Box 2.6.4  by Teggo. - www.molebox.com
264. !EP (exe pack) v1.4 (lite) final  - Team-X  ( 2007.04 ) www.team-x.ru , http://exetools.blog.com.cn
265. DalKrypt 1.0 by DalKiT - www.dalkit.fr.st (26.10.2003) Anti-SI, Anti-Debug, Anti-Dump
266. NackedPacker v1.0 by BigBoote ( 2004.01-2007.06? )- www.PEArmor.com
     / KaOs undetecter by  A R E S - http://www.north-team.org/com/net
267. WATCOM C/C++32 Run-Time system (c) Sybase Inc, 1988-2000
268. MS Visual C++ v.5 DLL Method 1 ( MS VBasic kit library )   ACM*
269. Open Source Code Crypter 1.0 by p0ke (9.06.2007) - www.swerat.com - http://unnamed.bot.nu ( Borland Delphi )
270. Private Personal Packer (PPP) Version 1.0.2 (13.03.2007) - www.ConquestOfTroy.com  ACM*
271. Wise for Windows Installer v.?.?? ( CAB in section 4 ) MS C++ 7.0 
272. Inteli check : unknown Installer - MSCF Cab file
273. Armadillo x.x ~ 5.0  32bit  [exe -low protection only] 
274. Armadillo x.x ~ 5.0  32bit  [Dll-std protection]
275. Inteli check : MASM assembler ( no signature )
276. Inteli check : unknown ver. WATCOM C/C++32 (c) Sybase 1988-200?
277. inteli check : Dev - ( MINGW 32 v ?.?.? ) - Bloodshed Software ( www.bloodshed.net )
278. Borland Delphi 2006/2007 ( 2009-2010 )- www.borland.com
279. Borland C++ - ( DLL ) Copyright 1994/96 , 1999  Borland Intl.
280. CRYPToCRACk's PE Protector 0.9.3 (2007.01) Lukas Fleischer - cryptocrack.de
281. Break-Into-Pattern, a.k.a BIP, v0.1 (2006.01) - http://n0name.exmuros.net http://undergroundkonnekt.net
282. DotFix NiceProtect 2.5 (with internal packer) GPcH Soft - www.niceprotect.com
283. DotFix NiceProtect 2.5 (Krypton sign) GPcH Soft - www.niceprotect.com
284. DotFix NiceProtect 2.5 (SVKP 1.3x sign) GPcH Soft - www.niceprotect.com
285. DotFix NiceProtect 2.5 (Visual C++ sign) GPcH Soft - www.niceprotect.com
286. Borland Delphi ( Component ) xxxx - www.borland.com
287. Microsoft Visual C++ ver. x.x DLL (5-8)
288. Microsoft Visual C++ ver. 8.0 DLL ( 83 )  ACM*
289. Microsoft Visual C++ ver. 7.xx DLL ( 83 )

290. Private exe Protector v.2.25 ( 28.06.2007 ) www.setisoft.com
291. Microsoft Visual C++ ver. 9.0 exe ( E8 )
292. Microsoft Visual C++ ver. 9.0 DLL ( 8B )
293. PEiD Plugin -> Exe Converter v.1.00 ( BobSoft )
294. MarjinZ EXE-Scrambler SE ( MS Visual C++ 8.0 )
295. Microsoft Visual C++ v7.10/8.0/9.0 DLL ( 8B )
296. Borland VCL Component for .NET ( Borland Developer Studio 4 (c) 2006 v.10.0.2 )
297. PDF2EXE v1.0 CoolPDF Software -  www.pdf2exe.com ( 2006.10 ) - PASSWORD DECODER :-)
298. RealBasic v.?.? ExE  - www.realbasic.com
299. RealBasic v.?.? DLL  - www.realbasic.com
300. Generic check - Aspack vx.x -> Alexey Solodovnikov
301. generic ckeck : FreePascal ver : FPC 1.x.x 
302. UPX -> (exe) Markus & Laszlo ver. 0.72 OBSOLETE VER. ( 12.05.1999 ) ACM*
303. UPX -> (dll) Markus & Laszlo ver. 0.72 OBSOLETE VER. ( 12.05.1999 ) ACM*
304. ScanTime UnDetectable by MarjinZ ( STUD RC4 1.0 ) Marjinz-Crypter.exe
305. Free Pascal Compiler version 2.0.4 [2006/08/21] for i386 ACM*
306. Active Basic v4.24.00 © 2006.04.08 (exe) Discoversoft  - www.activebasic.com ( Japan ) *ACM
307. Aspack v2.0/2.001  -> Alexey Solodovnikov - www.aspack.com
308. Play Basic v.1.0x - 1.63 ( 2D game creator ) www.playbasic.com

309. (exe) UPX obsolete ver. 0.50 - 0.72 -> Markus & Laszlo 
310. ANDpakk2 v0.06 (Jul 18 2006) Dmitry "AND" Andreev - http://and.intercon.ru
311. ANDpakk2 v0.18 (Jul 16 2007) 2006,2007 Dmitry "AND" Andreev  - http://and.intercon.ru
312. PEiD-Bundle v1.03 by BoB (2007.03.30) - www.secretashell.com/BobSoft
313. Exe Stealth Packer or Protector v.3.16   -  www.webtoolmaster.com (NTkrnl)
314. 20to4 v2004.04.18 Copyright 2001-2004 20to4.net
315. Borland C++ 1995 DLL    *ACM 
316. nBinder LIMITED v4.0 2006 - www.nkprod.ro  ( MSV C++ 8.0 ) 
317. mkfpack llydd (aPlib) 28.05.2007
318. KByS 0.28 beta EXE ( shoooo ) china 2006.05.23 *ACM
319. KByS 0.28 beta DLL ( shoooo ) china 2006.05.23 *ACM
320. Microsoft Visual C++ ver. 8.0 DEBUG / Visual Studio 2005 (FF) *ACM
321. mPack - mario PACKer version 0.0.2 (c) DeltaAziz
322. WinUDA 0.291 clasic sfx 2005 by Dwing  http://dwing.51.net 
323. Cryptic v2.1 - EXE Crypter Copyright [c] 2007.09.26 Tughack ( MS Visual Basic exe stub )
324. aSm Protector v1.0 Copyright [c] 2007.09.29 AT4RE
325. AverCryptor v.1.02beta by Sec|Null os1r1s ( 2007.08.23 ) - www.secnull.org
326. Muckis Protector 2 coded 2007 by Mucki    *ACM
327. Rewolf DLL packager v1.0 V.2007 http://rewolf.prv.pl    ( OEP info )
328. x86 Virtualizer ReWolf ( VIII.2007 ) - http://rewolf.pl

329. BeRo Tiny Pascal Compiler ( EXE ) http://bero.0ok.de
330. CDS SS V1.0 beta1 (c) CyberDoom [Team-X member] ( 2005.12.18 ) *ACM
331. [dUP2 -> diablo2oo2]  v.2.16 patchengine ( loader installer ) - Microsoft MacroAssembler  - http://diablo2oo2.cjb.net
332. Borland C++ 2002 & 2005 DLL - www.borland.com
333. WinUpack 0.37-0.39 by Dwing  ---  http://dwing.51.net (BE&60 sign)
334. Flash2X EXE Packager ver.2.1.0 2007 - http://flash2x.net/exepackager ( Borland Delphi ) - RIP HINTs
335. D1S1G PEiD Plugin by D1N  ( 10-24-2007 ) PEiD Signature and PE Overlay Tool  ( only OVL protection )
336. WinUtilities EXE Protect 2.1 - www.ylcomputing.com (MS C++ 6.0) ( how to pass remove )
337. Hacker's Patcher version 0.07 Veacheslav Patkov ( 2007.09.21 ) - http://patkov-site.narod.ru/eng.html
338. Enigma Protector 1.35 (2007.10.12)- www.enigmaprotector.com ,Vladimir Sukhov
339. FSG v1.33  F[ast] S[mall] G[ood]  - www.xtreeme.prv.pl  *ACM
340. FishPE Shield v.1.1x Crypt by HellFish ( http://hellfish.ys168.com ) - sign NOT TESTED trojan
341. Microsoft Visual C++ v4.2 DLL  *ACM


~ ~ ~ ~ ~ ~ ~ ~ ~ 

.... ....

571. PPT to EXE Converter Vaysoft v5.36 ( 2009.12.24 Delphi stub ) - www.vaysoft.com ( packed or not packed stub )
572. EXEForger (SignsImitator) v1.0.40.10 by STRELiTZIA ( 15.04.2010 ) - www.at4re.com
573. VMProtect v.2.05  2003-2010 VMProtect Software - www.vmprotect.ru
574. KiXtart 2010 (version 4.61) scripting language - www.kixtart.org
575. BoxedAppPacker v2.2.0.45  ( 2004 - 2010 Softanics ) - www.boxedapp.com
576. Obsidium v1.4.3.7 ( exe/dll ) Obsidium Software - www.obsidium.de
577. Private exe Protector v.3.2.0 or newer ( Generic detector 2010.05.xx )  - www.setisoft.com
578. DeployMaster Instaler v.3.3.2 ( 01.07.2010 ) Jan Goyvaerts - www.deploymaster.com
579. Zylom Game wrapper ( MS Visual C++ 7.x ) www.zylom.com
580. Zylom (wrapper) Crypted Game.exe file - www.zylom.com
581. Aspack v2.24-2.25 ( 03/11.2010 ) -> Alexey Solodovnikov - www.aspack.com     *ACM  
582. [dUP] diablo2oo2''s Universal Patcher [ loader installer ] - Version: 2.20 - 2.21 beta2 ( 05.10.2010 ) - http://diablo2oo2.cjb.net *ACM
583. [dUP] diablo2oo2''s Universal Patcher [ simple loader ] - Version: 2.20 - 2.21 beta2 ( 05.10.2010 ) - http://diablo2oo2.cjb.net *ACM
584. [dUP] diablo2oo2''s Universal Patcher [ patch ] - Version: 2.20 - 2.21 beta2 ( 05.10.2010 ) - http://diablo2oo2.cjb.net *ACM
585. Xenocode Virtual Application Studio 2010 ( v.8.0.xx ) - 2003-2009 Code Systems Corporation - www.xenocode.com    *ACM
586. GENERIC CHECK : ? Gentee Programming Language © 2004-2006  www.gentee.com
587. ASPROTECT generic / modified / unknown version detector
588. Enigma Virtual Box v1.30 free - 21.11.2010 - www.enigmaprotector.com
589. *generic check - Free Pascal compiler (MinGW/FPC) - familly *unknown version
590. VProtect ver.1.9.0.0 - VirtualizeProtect ( 2010.12.23 ) www.vprotect.net    *ACM 
591. VProtect ver.1.x - Generic detector - VirtualizeProtect  www.vprotect.net
592. Generic check : VMProtect v.2.07 - X.X  2003-2010 VMProtect Software - www.vmprotect.ru


_______________________________________________________________________

                   www.exeinfo.xwp.pl
_______________________________________________________________________


		2011.01.28 ( c ) A.S.L.


