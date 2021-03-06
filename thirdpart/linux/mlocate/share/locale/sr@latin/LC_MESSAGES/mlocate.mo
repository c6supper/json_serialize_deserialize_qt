??    .      ?  =   ?      ?     ?       1     ?   M     ?          $     >  *   ^  ?  ?  +   r	     ?	  #   ?	  0   ?	      
      0
  )   Q
  '   {
     ?
     ?
  %   ?
     ?
       &     .   D     s     ?     ?  )   ?  !   ?  $        )     A  '   \     ?  "   ?  .   ?     ?  $     "   +     N  "   d  !   ?     ?     ?  ?  ?     ?     ?  2   ?  ?   "     ?  !   ?  $      "   %  .   H  e  w  .   ?  !     /   .  <   ^  ,   ?  ,   ?  6   ?  -   ,     Z  #   y  -   ?     ?     ?  2     ;   9  1   u     ?     ?  3   ?  (     1   :  '   l  $   ?  :   ?     ?        2   0  =   c  /   ?  %   ?     ?  2     '   I     q  #   ?              !   -   %   
       &                                 '          ,         *              "                         .   )   +                        #   	                            (             $       
Report bugs to %s.
 --%s specified twice --%s would override earlier command-line argument Copyright (C) 2007 Red Hat, Inc. All rights reserved.
This software is distributed under the GPL v.2.

This program is provided with NO WARRANTY, to the extent permitted by law. Database %s:
 I/O error reading `%s' I/O error seeking in `%s' I/O error while writing to `%s' I/O error while writing to standard output Usage: updatedb [OPTION]...
Update a mlocate database.

  -f, --add-prunefs FS           omit also FS
  -n, --add-prunenames NAMES     omit also NAMES
  -e, --add-prunepaths PATHS     omit also PATHS
  -U, --database-root PATH       the subtree to store in database (default "/")
  -h, --help                     print this help
  -o, --output FILE              database to update (default
                                 `%s')
      --prune-bind-mounts FLAG   omit bind mounts (default "no")
      --prunefs FS               filesystems to omit from database
      --prunenames NAMES         directory names to omit from database
      --prunepaths PATHS         paths to omit from database
  -l, --require-visibility FLAG  check visibility before reporting files
                                 (default "yes")
  -v, --verbose                  print paths of files as they are found
  -V, --version                  print version information

The configuration defaults to values read from
`%s'.
 `%s' does not seem to be a mlocate database `%s' has unknown version %u `%s' has unknown visibility flag %u `%s' is locked (probably by an earlier updatedb) `=' expected after variable name can not change directory to `%s' can not change group of file `%s' to `%s' can not change permissions of file `%s' can not drop privileges can not find group `%s' can not get current working directory can not lock `%s' can not open `%s' can not open a temporary file for `%s' can not read two databases from standard input can not stat () `%s' configuration is too large error replacing `%s' file name length %zu in `%s' is too large file name length %zu is too large invalid empty directory name in `%s' invalid regexp `%s': %s invalid value `%s' of --%s invalid value `%s' of PRUNE_BIND_MOUNTS missing closing `"' no pattern to search for specified non-option arguments are not allowed with --%s unexpected EOF reading `%s' unexpected data after variable value unexpected operand on command line unknown variable `%s' value in quotes expected after `=' variable `%s' was already defined variable name expected warning: Line number overflow Project-Id-Version: mlocate
Report-Msgid-Bugs-To: https://fedorahosted.org/mlocate/
POT-Creation-Date: 2012-09-22 04:14+0200
PO-Revision-Date: 2012-02-14 08:16+0000
Last-Translator: Miloslav Trmač <mitr@volny.cz>
Language-Team: LANGUAGE <LL@li.org>
Language: sr@latin
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
 
Prijavite greške na %s.
 --%s definisano dva puta --%s bi premostilo raniji argument komandne linije Autorska prava 2007 Red Hat, Inc.  Sva prava zadržana.
Ovaj softver se distribuira pod GJL verzije 2.

Ovaj program dolazi BEZ GARANCIJE, u okviru dozvoljenim zakonom. Baza podataka %s:
 U/I greška pri čitanju „%s“ U/I greška pri traženju u „%s“ U/I greška pri pisanju u „%s“ U/I greška pri čitanju sa standardnog izlaza Upotreba: updatedb [OPCIJE]...
Ažurira mlocate bazu podataka.

  -f, --add-prunefs SD             izostavi sistem datoteka
  -n, --add-prunenames NAZIVI      izostavi NAZIVE
  -e, --add-prunepaths PUTANJE     izostavi PUTANJE
  -U, --database-root PUTANJA      podstablo za čuvanje baze podataka
                                   (podrazumevano „/“)
  -h, --help                       odštampaj ovu pomoć
  -o, --output DATOTEKA            baza podataka za ažuriranje (podrazumevano
                                   „%s“)
      --prune-bind-mounts OZNAKA   izostavi veznike montiranja (podrazumevano
                                   „ne“)
      --prunefs SD                 sistemi datoteka koji se izostavljaju iz
                                   baze podataka
      --prunenames NAZIVI          nazivi direktorijuma koji se izostavljaju iz
                                   baze podataka
      --prunepaths PUTANJE         putanje koje se izostavljaju iz baze podataka
  -l, --require-visibility OZNAKA  proveri vidljivost pre izveštavanju o
                                   datotekama
                                   (podrazumevano „da“)
  -v, --verbose                    štampa putanje datoteka kako se pronalaze
  -V, --version                    štampa informacije o verziji

Podrazumevane vrednosti podešavanja pročitane iz
„%s“.
 „%s“ izgleda da nije mlocate baza podataka „%s“ ima nepoznatu verziju %u „%s“ ima nepoznati indikator vidljivosti %u „%s“ je zaključana (verovatno ranijom updatedb radnjom) „=“ se očekuje posle naziva promenljive ne mogu da promenim direktorijum na „%s“ ne mogu da promenim grupu datoteke „%s“ u „%s“ ne mogu da promenim dozvole datoteke „%s“ ne mogu da uklonim privilegije ne mogu da pronađem grupu „%s“ ne mogu da dobijem tekući radni direktorijum ne mogu da zaključam „%s“ ne mogu da otvorim „%s“ ne mogu da otvorim privremenu datoteku za „%s“ ne mogu da pročitam dve baze podataka sa standardnog ulaza ne mogu da izvršim funkciju stat () nad „%s“ podešavanje je preveliko greška pri zameni „%s“ dužina %zu naziva datoteke u „%s“ je prevelika dužina %zu naziva datoteke je prevelika neispravan naziv praznog direktorijuma u „%s“ neispravan regularni izraz „%s“: %s neispravna vrednost „%s“ za --%s neispravna vrednost „%s“ PRUNE_BIND_MOUNTS promenljive nedostaje završno „"“ šablon za pretragu nije naveden argumenti koji nisu opcije nisu dozvoljeni sa --%s neočekivani završetak datoteke (EOF ) pri čitanju „%s“ neočekivan podatak posle vrednosti promenljive neočekivani operator komandne linije nepoznata promenljiva „%s“ vrednost u navodnicima je očekivana posle „=“ promenljiva „%s“ je već definisana očekuje se naziv promenljive upozorenje: prekoračen broj linija 