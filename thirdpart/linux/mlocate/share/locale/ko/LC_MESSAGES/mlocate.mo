??    3      ?  G   L      h  4   i  F   ?  #   ?     	     "     7  1   L  ?   ~     0     >     U     o  *   ?  ?  ?  +   ?
     ?
  #   ?
  0         @      a  )   ?  '   ?     ?     ?  %        *     <  &   N  .   u     ?     ?     ?  )   ?  !     8   5  $   n     ?     ?  '   ?     ?  "     .   %     T  $   p  "   ?     ?  "   ?  !   ?          *  ?  H  (   ?  G        d     x  "   ?     ?  :   ?  ?        ?  &   ?  *   ?  )      2   J  `  }  J   ?  :   )  G   d  <   ?  -   ?  4     =   L  2   ?  &   ?  (   ?  8     )   F     p  6   ?  N   ?  "     #   7  #   [  C     7   ?  Y   ?  2   U  %   ?     ?  *   ?     ?  3     E   B  3   ?  5   ?  1   ?     $  >   @  2     "   ?  2   ?     	   1   !       *       "            $                0                    +                 )   (   '       2             
   &                       3   .                                ,           -                 /   %                         #    	%'ju byte in file names
 	%'ju bytes in file names
 	%'ju byte used to store database
 	%'ju bytes used to store database
 	%'ju directory
 	%'ju directories
 	%'ju file
 	%'ju files
 
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
 `%s' does not seem to be a mlocate database `%s' has unknown version %u `%s' has unknown visibility flag %u `%s' is locked (probably by an earlier updatedb) `=' expected after variable name can not change directory to `%s' can not change group of file `%s' to `%s' can not change permissions of file `%s' can not drop privileges can not find group `%s' can not get current working directory can not lock `%s' can not open `%s' can not open a temporary file for `%s' can not read two databases from standard input can not stat () `%s' configuration is too large error replacing `%s' file name length %zu in `%s' is too large file name length %zu is too large file system error: zero-length file name in directory %s invalid empty directory name in `%s' invalid regexp `%s': %s invalid value `%s' of --%s invalid value `%s' of PRUNE_BIND_MOUNTS missing closing `"' no pattern to search for specified non-option arguments are not allowed with --%s unexpected EOF reading `%s' unexpected data after variable value unexpected operand on command line unknown variable `%s' value in quotes expected after `=' variable `%s' was already defined variable name expected warning: Line number overflow Project-Id-Version: mlocate
Report-Msgid-Bugs-To: https://fedorahosted.org/mlocate/
POT-Creation-Date: 2012-09-22 04:14+0200
PO-Revision-Date: 2012-03-07 03:59+0000
Last-Translator: eukim <eukim@redhat.com>
Language-Team: Korean (http://www.transifex.com/projects/p/fedora/language/ko/)
Language: ko
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=1; plural=0;
 파일 이름에 있는 	%'ju 바이트
 데이터베이스를 저장하기 위해 사용되는 	%'ju 바이트
 	%'ju 디렉토리
 	%'ju 파일
 
%s에 버그를 보고합니다.
 --%s이(가) 두 번 지정됨  --%s은(는) 이전 명령행 인수를 덮어 씁니다   Copyright (C) 2007 Red Hat, Inc. All rights reserved.
This software is distributed under the GPL v.2.

This program is provided with NO WARRANTY, to the extent permitted by law. 데이터베이스 %s:
 `%s' 읽는 도중 I/O 오류 발생   `%s'에서 검색하는 도중 I/O 오류  `%s'에 쓰는 도중 I/O 오류 발생   표준 출력에 쓰기 도중 I/O 오류 발생   사용법: updatedb [옵션]...
mlocate 데이터베이스 업데이트 사항.

  -f, --add-prunefs FS           FS 생략
  -n, --add-prunenames NAMES     NAMES 생략
  -e, --add-prunepaths PATHS     PATHS 생략
  -U, --database-root PATH       데이터베이스에 저장하기 위한 하위 트리 (기본값 "/")
  -h, --help                     도움말 인쇄
  -o, --output FILE              업데이트할 데이터베이스 (기본값
                                 `%s')
      --prune-bind-mounts FLAG   bind 마운트 생략 (기본값 "no")
      --prunefs FS               데이터베이스에서 생략할 파일 시스템
      --prunenames NAMES         데이터베이스에서 생략할 디렉토리 이름
      --prunepaths PATHS         데이터베이스에서 생략할 경로
  -l, --require-visibility FLAG  파일을 보고하기 전 가시성 확인
                                 (기본값 "yes")
  -v, --verbose                  검색한 파일의 경로 인쇄
  -V, --version                  버전 정보 인쇄

값으로의 설정 기본값은
`%s'에서 읽어옵니다.
 `%s'은(는) mlocate 데이터베이스에 있을것 같지 않습니다   `%s'에는 알 수 없는 버전 %u이(가) 있습니다.  `%s'에는 알 수 없는 가시성 플래그 %u이(가) 있습니다   `%s'은(는) 잠겨있습니다 (이전 updatedb를 통해)  변수 이름 뒤에 `='이 예상됩니다   디렉토리를 `%s'로 변경할 수 없습니다   `%s' 파일의 그룹을 `%s'로 변경할 수 없습니다   `%s' 파일 권한을 변경할 수 없습니다   권한을 삭제할 수 없습니다   `%s' 그룹을 찾을 수 없습니다   현재 작업 중인 디렉토리를 얻을 수 없음   `%s'을(를) 잠금할 수 없습니다   `%s'을(를) 열 수 없음   `%s'에 대한 임시 파일을 열 수 없습니다   표준 입력에서 두 개의 데이터베이스를 읽을 수 없습니다   stat () `%s' 할 수 없습니다  설정 사항이 너무 큽니다   `%s' 교체 도중 오류 발생    파일 이름의 길이 %zu가 (`%s'에 있는) 너무 큽니다    파일 이름의 길이 %zu이(가) 너무 큽니다    파일 시스템 오류: 디렉토리 %s에 길이가 0인 파일 이름이 있습니다  `%s'에 있는 잘못된 빈 디렉토리 이름   잘못된 정규 표현식 `%s': %s   --%s의 잘못된 값 `%s'    PRUNE_BIND_MOUNTS의 잘못된 값 `%s'   `"' 종료 중 부재   검색 패턴이 지정되어 있지 않습니다   옵션이 아닌 인수는 --%s와 함께 허용되지 않습니다   `%s'을(를) 읽는 도중 기대하지 않은 EOF  변수 값 다음에 예상하지 않은 데이터    명령행에서 기대하지 않은 피연산자  알 수 없는 변수 `%s'  `=' 다음에 인용 부호루 묶인 값이 예상됩니다   `%s' 변수는 이미 정의되어 있습니다    변수 이름이 예상됩니다   경고: 줄 번호가 초과되어 있습니다    