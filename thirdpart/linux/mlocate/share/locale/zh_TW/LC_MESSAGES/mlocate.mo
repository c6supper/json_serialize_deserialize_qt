??    3      ?  G   L      h  4   i  F   ?  #   ?     	     "     7  1   L  ?   ~     0     >     U     o  *   ?  ?  ?  +   ?
     ?
  #   ?
  0         @      a  )   ?  '   ?     ?     ?  %        *     <  &   N  .   u     ?     ?     ?  )   ?  !     8   5  $   n     ?     ?  '   ?     ?  "     .   %     T  $   p  "   ?     ?  "   ?  !   ?          *  ?  H  %   ?  %         F     T      b     ?  &   ?  ?   ?     f     w  (   ?  "   ?  &   ?  ?    &   ?        &   0  8   W  %   ?     ?  4   ?  #   	     -     @  !   [     }     ?  #   ?  *   ?     ?            .   =     l  =   ?  &   ?  $   ?       ,   )     V     m  3   ?  2   ?  $   ?  '        @  "   X     {     ?     ?     	   1   !       *       "            $                0                    +                 )   (   '       2             
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
PO-Revision-Date: 2012-03-07 18:21+0000
Last-Translator: Cheng-Chia Tseng <pswo10680@gmail.com>
Language-Team: Chinese (Taiwan) <trans-zh_TW@lists.fedoraproject.org>
Language: zh_TW
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=1; plural=0;
 	%'ju 檔案名稱中的位元組數
 	%'ju 位元組用於儲存資料庫
 	%'ju 目錄
 	%'ju 檔案
 
將程式錯誤報告給 %s。
 --%s 被指定兩次 --%s 將覆寫先前的命令列參數 著作權 (C) 2007 Red Hat, Inc。保留所有權利。
本軟體在 GPL v.2 下發布。

本程式不提供任何擔保，在法律允許的範圍內予以排除。 資料庫 %s：
 I/O 讀取錯誤「%s」 在「%s」中搜尋時發生 I/O 錯誤 寫入「%s」時發生 I/O 錯誤 寫入標準輸出時發生 I/O 錯誤 用法：updatedb [選項]…
更新 mlocate 資料庫。

  -f, --add-prunefs FS           也忽略 FS
  -n, --add-prunenames NAMES     也忽略 NAMES
  -e, --add-prunepaths PATHS     也忽略 PATHS
  -U, --database-root PATH       向資料庫中儲存的子樹 (預設 "/")
  -h, --help                     顯示這份說明
  -o, --output FILE              要升級的資料庫 (預設
                                 「%s」)
      --prune-bind-mounts FLAG   忽略繫結載入 (預設 "no")
      --prunefs FS               從資料庫中忽略的檔案系統
      --prunenames NAMES         從資料庫中忽略的目錄名稱
      --prunepaths PATHS         從資料庫中忽略的路徑
  -l, --require-visibility FLAG  在報告檔案之前檢查可見性
                                 (預設 "yes")
  -v, --verbose                  當檔案被找到時顯示其路徑
  -V, --version                  顯示版本資訊
組態預設為從檔案「%s」讀取的值。
 「%s」不是一個 mlocate 資料庫 「%s」具有不明的版本 %u 「%s」有不明的可視性旗標 %u 「%s」已被鎖定 (可能是由於先前的 updatedb) 變數名稱之後需要一個「=」 無法變更目錄到「%s」 無法將檔案所在群組「%s」變更為「%s」 無法變更檔案「%s」的權限 無法釋放特權 無法找到群組「%s」 無法得到目前的工作目錄 無法鎖定「%s」 無法開啟「%s」 無法為「%s」開啟暫時檔案 無法從標準輸入讀取兩個資料庫 無法執行 stat()「%s」 組態過大 替換「%s」時發生錯誤 檔案名稱的長度 %zu 在「%s」中過大 檔案名稱 %zu 過長 檔案系統錯誤：目錄 %s 中的檔案名稱長度為零 「%s」中有無效的空目錄名稱 無效的正規表示式「%s」: %s %s 有無效的值「%s」 PRUNE_BIND_MOUNTS 之後的值「%s」無效 缺少匹配的「"」 沒有指定任何搜尋樣式 沒有選項的參數不允許與 --%s 一起使用 讀取「%s」時，出現未預期的檔案結束 變數值之後有未預期的資料 命令列包含未預期的作業物件 不明的變數「%s」 「=」之後需要引號內的值 變數「%s」已被定義 需要一個變數名稱 警告：列數溢出 