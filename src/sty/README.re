
= review-jsbook.cls Users Guide


現時点における最新版 @<tt>{jsbook.cls  2018/06/23 jsclasses (okumura, texjporg)} をベースに、Re:VIEW 向け review-jsbook.cls を実装しました。



過去の Re:VIEW 2 で jsbook.cls で作っていた資産を、ほとんどそのまま Re:VIEW 3 でも利用できます。


== 特徴
 * クラスオプション @<tt>{media} により、「印刷用」「電子用」の用途を明示的な意思表示として与えることで、用途に応じた PDF ファイル生成を行えます。
 * （基本的に）クラスオプションを @<tt>{<key>=<value>} で与えられます。
 * クラスオプション内で、用紙サイズや基本版面を設計できます。



ここで、クラスオプションとは、親 LaTeX 文章ファイルにおいて、以下のような位置にカンマ（,）区切りで記述するオプションです。


//emlist[][latex]{
\documentclass[クラスオプションたち（省略可能）]{review-jsbook}
//}

== Re:VIEW で利用する


クラスオプションオプションたちは、Re:VIEW 設定ファイル config.yml 内の texdocumentclass において、以下のような位置に記述します。


//emlist[][yaml]{
texdocumentclass: ["review-jsbook", "クラスオプションたち（省略可能）"]
//}

== 利用可能なクラスオプションたち

=== 用途別 PDF データ作成 @<tt>{media=<用途名>}


印刷用 @<tt>{print}、電子用 @<tt>{ebook} のいずれかの用途名を指定します。

 * @<tt>{print}［デフォルト］：印刷用 PDF ファイルを生成します。
 ** トンボあり、デジタルトンボあり、hyperref パッケージを @<tt>{draft} モードで読み込み、表紙は入れない
 * @<tt>{ebook}：電子用PDFファイルを生成します。
 ** トンボなし、hyperref パッケージを読み込み、表紙を入れる


=== 表紙の挿入有無 @<tt>{cover=<trueまたはfalse>}


@<tt>{media} の値によって表紙（config.yml の coverimage に指定した画像）の配置の有無は自動で切り替わりますが、@<tt>{cover=true} とすれば必ず表紙を入れるようになります。



なお、config.yml の coverimage で指定する画像ファイルは、原寸を想定しています。


=== 特定の用紙サイズ @<tt>{paper=<用紙サイズ>}


利用可能な特定の用紙サイズを指定できます。

 * @<tt>{a3}
 * @<tt>{a4} ［デフォルト］
 * @<tt>{a5}
 * @<tt>{a6}
 * @<tt>{b4}：JIS B4
 * @<tt>{b5}：JIS B5
 * @<tt>{b6}：JIS B6
 * @<tt>{a4var}：210mm x 283mm
 * @<tt>{b5var}：182mm x 230mm
 * @<tt>{letter}
 * @<tt>{legal}
 * @<tt>{executive}


=== トンボ用紙サイズ @<tt>{tombopaper=<用紙サイズ>} および塗り足し幅 @<tt>{bleed_margin=<幅>}


@<tt>{tombopaper} ではトンボ用紙サイズを指定できます。
［デフォルト］値は自動判定します。



@<tt>{bleed_margin} では塗り足し領域の幅を指定できます。
［デフォルト］3mm になります。


=== カスタム用紙サイズ @<tt>{paperwidth=<用紙横幅>}, @<tt>{paperheight=<用紙縦幅>}


カスタム用紙サイズ @<tt>{paperwidth=<用紙横幅>}, @<tt>{paperheight=<用紙縦幅>} （両方とも与える必要があります）を与えることで、特定の用紙サイズで設定できない用紙サイズを与えられます。



たとえば、B5変形 @<tt>{paperwidth=182mm}, @<tt>{paperheight=235mm}。


=== 基本版面設計 @<tt>{fontsize=<文字サイズ>}, @<tt>{baselineskip=<行送り>}, @<tt>{line_length=<字詰>}, @<tt>{number_of_lines=<行数>}, @<tt>{head_space=<天>}, @<tt>{gutter=<ノド>}, @<tt>{linegap=<幅>}, @<tt>{headheight=<幅>}, @<tt>{headsep=<幅>}, @<tt>{footskip=<幅>}


基本版面情報を与えます。
天、ノドをそれぞれ与えない場合、それぞれ天地、左右中央になります。

 * @<tt>{fontsize=10pt}［デフォルト］：標準の文字（normalfontsize）の文字サイズを与えます。pt のほか、Q や mm といった単位も指定可能です。ただし、文字サイズは jsbook の挙動に合わせるために 8pt, 9pt, 10pt, 11pt, 12pt, 14pt, 17pt, 20pt, 21pt, 25pt, 30pt, 36pt, 43pt のいずれか近いサイズに丸められます。
 * @<tt>{baselineskip=16pt}［デフォルト］：行送りを与えます。
 * @<tt>{line_length=<字詰め幅>}：1行字詰めを与えます。字詰め幅には単位を付ける必要があります。文字数であれば「zw」を使うとよいでしょう（例：35zw＝35文字）。デフォルトでは jsbook の挙動に従い、紙サイズに基いて決定します。
 * @<tt>{number_of_lines=<行数>}：行数を与えます。デフォルトでは jsbook の挙動に従い、紙サイズに基いて決定します。
 * @<tt>{head_space=<幅>}：天を与えます。［デフォルト］は天地中央です。
 * @<tt>{gutter=<幅>}：ノドを与えます。［デフォルト］は左右中央です。
 * @<tt>{linegap=<幅>}：行送りを baselineskip で指定する代わりに、通常の文字の高さにこのオプションで指定する幅を加えたものを行送りとします。



例をいくつか挙げます。

 * @<tt>{paper=a5, fontsize=10pt, line_length=35zw, number_of_lines=32, baselineskip=16pt,}
 * @<tt>{paper=b5, fontsize=13Q, baselineskip=20.5H, head_space=20mm, gutter=20mm,}



さらに、ヘッダー、フッターに関する位置調整は、TeX のパラメータ @<tt>{\headheight}, @<tt>{\headsep}, @<tt>{\footskip} に対応しており、それぞれ @<tt>{headheight}, @<tt>{headsep}, @<tt>{footskip} を与えられます。


== 開始ページ番号 @<tt>{startpage=<ページ番号>}


大扉からのページ開始番号を指定します。



［デフォルト］は1です。表紙・表紙裏（表1・表2）のぶんを飛ばしたければ、@<tt>{startpage=3} とします。


== 通しページ番号（通しノンブル） @<tt>{serial_pagination=<trueまたはfalse>}


大扉からアラビア数字でページ番号を通すかどうかを指定します。

 * @<tt>{true}：大扉を開始ページとして、前付（catalog.yml で PREDEF に指定したもの）、さらに本文（catalog.yml で CHAPS に指定したもの）に連続したページ番号をアラビア数字で振ります（通しノンブルと言います）。
 * @<tt>{false}［デフォルト］：大扉を開始ページとして前付の終わり（通常は目次）までのページ番号をローマ数字で振ります。本文は 1 を開始ページとしてアラビア数字で振り直します（別ノンブルと言います）。


=== 隠しノンブル 'hiddenfolio=<プリセット>'


印刷所固有の要件に合わせて、ノドの目立たない位置に小さくノンブルを入れます。
'hiddenfolio` にプリセットを与えることで、特定の印刷所さん対応の隠しノンブルを出力することができます。
利用可能なプリセットは、以下のとおりです。

 * @<tt>{default}：トンボ左上（塗り足しの外）にページ番号を入れます。
 * @<tt>{marusho-ink}（丸正インキ）：塗り足し幅を5mmに設定、ノド中央にページ番号を入れます。
 * @<tt>{nikko-pc}（日光企画）, @<tt>{shippo}（ねこのしっぽ）：ノド中央にページ番号を入れます。



独自の設定を追加したいときには、review-jsbook.cls の実装を参照してください。



ページ番号は紙面に入れるものと同じものが入ります。アラビア数字で通したいときには、上記の @<tt>{serial_pagination=true} も指定してください。


== 標準で review-jsbook.cls を実行したときの jsbook.cls との違い
 * jsbook.cls のクラスオプション @<tt>{uplatex}：これまで texdocumentclass に指定が必要だった @<tt>{uplatex} オプションは不要となっています。
 * jsbook.cls のクラスオプション @<tt>{nomag}：用紙サイズや版面設計は、review-jsbook.cls 側で行います。
 * hyperref パッケージ：あらかじめ hyperref パッケージを組み込んでおり、@<tt>{media} オプションにより用途別で挙動を制御します。

