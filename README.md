
# FlushMouse
|![Icon](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/FlushMouse.png)|**タスクトレイに常駐して、マウスカーソルへIMEモードの表示をするユーティリティです<br>FlushMouse is a software developed by JeffyTS. It is a mouse gesture tool that allows you to perform various operations by drawing gestures with your mouse. (by Bing)**|
| :---: | :--- |

#### リリース情報

* **2023/07/12　　Version 1.1.2.9 をリリースしました**

* **(主な変更点)**
   * EP Helperの機能を強化して、US / ENGになってしまうのをさらに防ぐようにしました
   * 最近(2023/07以降)、[OSDN/FlushMouse](https://osdn.net/users/jeffyts/pf/FlushMouse/wiki/FrontPage) が、不安定なようですので、 [GitHub/FlushMouse](https://github.com/JeffyTS-JP/FlushMouse)の方をチェックしていただいたほうが良いかもしれません(できるだけ、どちらにも同じソースとバイナリが置くようにはします)
   * 上記のような事情で、OSDNの Wikiは簡単な説明に変更して、詳しい内容はREADME.mdで行うようにしました。ビルド方法については下の方に書いてあります


   * IMEモードの表示の色指定でαチャネルが有効になるようにしてあります(設定画面は作成していません。レジストリを編集できる方は好みの色にしてみてください  (ただし、レジストリの編集は十分注意して行ってください)
   * HKEY_CURRENT_USER\Software\JeffyTS\FlushMouse の NearDrawCaretColor と NearDrawMouseColor です。上位バイトから、α B G R の順です
   * αの値は、通常と違って、大きいほうが透明度が高くなります。これは従来と互換性を持たせるためですのでご了承ください。なお、レジストリを変更した方がいたら、お手数をおかけしますが修正をお願いします
   * マウスカーソル右下のモード表示の色は、現状では変更できません。ご了承ください

* **(今後について)**
   * ~~OSDN上ではここまでの機能で開発は終了としたいと考えています(バグ対応は除く)~~
   * ~~今後は、プレゼンテーションモードの追加や、マウスカーソルの形状や、表示する内容などをカスタマイズする機能をつけて有償に移行しようと考えています~~
   * と、上記のように考えていたのですが、この FlushMouse は Microsoft Storeに登録できないやり方を使って処理しているので無理なことが判明しました(フックを使用しいるとどうしてもだめなようです。ただ、SDKなどで、だいぶ検証しました)
   * ***ということで、今後も暇を見つけて、機能向上を目指していきますのでよろしくお願いします***

#### 対応OS
   * Windows 10 / 11 64bit  (Latest Release Versionで検証しています)

#### インストール / アンインストール
   * Windows Installer (msi)形式になっています。実行してインストールしてください
   * アンインストールは「設定」の「アプリと機能」、または「コントロールパネル」の「プログラムと機能」からできます
     * または、同じバージョンのインストーラーがあればそれをもう一度実行してもアンインストールできます[[BR]]
     * こちらの場合は、本プログラムの設定(レジストリ)を削除するかどうかを選べます(デフォルトでは残すようにしてあります)[[BR]]

#### 使い方
   * スタートメニュー、デスクトップのアイコンをクリックするか、PCを起動するとタスクトレイに常駐します(ただし、PC再起動で起動しないと、一部のアプリでモード表示されない現象が発生します)
   * PC起動時に、タスクスケジューラーからFlushMouseを自動的に起動するようになっています
     * (これにより、以前の制限事項であった「一部のアプリ、例えば「タスクマネージャー」などがフォーカスを持っているときは、モード表示ができません」は解消されました)
   * 不要であればデスクトップのショートカットアイコンは、削除してしまっても問題ありません

#### マウスカーソル
   * IME ONの時にモードを表示するマウスカーソルは、「矢印(通常の選択)」「Ｉビーム(テキスト選択)」「ハンド(リンクの選択)」の3種類です
   * IME OFF(直接入力)ではマウスカーソル左にはモードを表示しません(キャレットのモード表示は行います)
   * キャレットとマウスカーソル左へのIMEモード表示に、Caps Lock ONの時はアンダーラインを表示します (マウスカーソル右下の表示にはアンダーラインは表示されません。ご了承ください)
   
![Cursors](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/Cursors.png)

#### 設定ダイアログ画面
   * タスクトレイの中にあるFlushMouseのアイコンをクリックすることで表示します
   
![SettingDialog](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/SettingDlg.png)

#### 設定のヒント
   * Windows 10 / 11ではマウスカーソルの形状や大きさを決める方法が複数あるため、下記の順番で設定することをお勧めします
  1. 設定→ディスプレイ→拡大縮小とレイアウトで、「ディスプレイの解像度」、「テキスト、アプリ、その他の項目のサイズ」、「表示スケールの詳細設定」を好みに設定します
  2. 設定→デバイス→マウス→「マウスとカーソルのサイズを調整する」で好みに設定します
    * この変更を行うとマウスカーソルが、Windows 10 / 11特有のものになります
    * もし、Windows 7などの以前のマウスカーソルを使いたい場合は３の設定をします。必要なければ４に進んでください
  3. 設定→デバイス→マウス→「その他のマウスオプション」(これはコントロールパネルのマウスのプロパティと同じ設定画面です)の、ポインタータブで、好みのカーソル形状、大きさに設定します
  4. タスクトレイのFlushMouseのアイコンをクリックして設定を選択し、「マウスカーソルのサイズ」を好みの大きさに設定します
   * FlushMouseで設定するカーソルの大きさは、アプリケーション独自に表示するもの(例えばExcelの十字型のカーソルなど)には影響しませんので、１から３を繰り返して、それらが好みの大きさになるようにしてから、FlushMouseでコントロールするカーソルの大きさを合わせると使いやすくなると思います

#### 設定項目について (わかりづらい項目のみ説明します)
   * **IMEモードの取得方法**
     * 「マウスカーソル下のウィンドウのIMEモードを表示する」 / 「フォーカスウィンドウのIMEモードを表示する」から選べます
     * 設定→時刻と言語→言語→キーボードの「入力方式の切り替え」や、旧IMEのプロパティにある「IME入力モード切替の通知の画面中央に表示する」と関わってきますので、ご自分の設定に合わせて好みな方を選択してください
   * **IME関連キーが押されたときにIMEモードを表示する**
     * IME関連キーとは「半角/全角(漢字)」「変換」「無変換」「カタカナ/ひらがな」「Ctrl + F6～F9」などを指します
   * **モード表示の時間**
     * マウスカーソルの左に「あ」「ア」「Ａ」「_ｱ」「_A」を表示をしている時間を調整できます
   * **Ctrlキーを離してからモードを表示するまでの時間**
     * 設定の「マウスのプロパティ」にある、「Ctrlキーを押すとポインターの位置を表示する」をONにしているとき、この表示と重なるのを避けるためにモードを表示するまでの時間を調整できます
   * **IMEモードをキャレット(テキストカーソル)の左に表示する**
     * アプリによってはキャレットの位置が取得できなかったり、表示位置がおかしくなることがあります(これは制限事項となります)。ただし、位置が取得できない場合は、マウスカーソルの左に表示します
   * 本FlushMouseでは、「矢印」「Ｉビーム」「ハンド」を 「aero_」から抽出したものを表示しています(これは固定になります。ご了承ください)


#### β機能　ExplorerPatcher Simple Window Switcher Helper について
  * まず、[ExplorerPatcher](https://github.com/valinet) の作者であるValentin-Gabrielに謝意を伝えます　Thanks, Valentin-Gabriel !
  * さて、マルチモニターの環境でSimple Window Switcherを有効にすると、Alt + Tabでマウスカーソルのあるカレントモニターのウィンドウだけを切り替えられて非常に便利です。もちろん他にも便利な機能があります

  * しかし、[Valinetのブログ](https://valinet.ro/2021/11/18/Functional-Windows-10-flyouts-on-Windows-11.html)を読むとわかるのですが、英語キーボード(101/102/104キーボード)のドライバーをロードして実現しています。このため、日本語キーボード(106/109)メインの我々にはちょっと困ったことになってしまいます。例えば、漢字(半角/全角)キーで「 ` 」が入力されてしまったり、「@」マークなどの記号の位置が変わってしまったり。何より日本語変換ができません。そこで、FlushMouseに、強制的にUS / ENGを排除する機能を加えました
  * Windowsキー + スペースで入力言語を切り替えた時、IME関連キー押したときやマウスクリックしたとき、フォーカスが切り替わったときなどにIMEの状態をチェックして、後述するように動作します

  * ただし、以下のような制限事項があります
    * ★**物理的に英語(101/102/104)キーボードを接続している場合は動きません。必ず日本語(106/109)キーボードの環境で使用してください**(日本語キーボードが見つからないときには、本機能が動作しないようにチェックしています)
    * ★**Windowsに付属されているMicrosoft IMEでしか検証していません**
    * 一部のアプリケーションでは「漢字(半角/全角)」などのIME関連キーで「US / ENG」「JA / ENG」から切り替わらないことがあります。現時点では制限事項となります。ご了承ください

  * IME関連キーの動作は下記のようになります(IMEの設定でキーマップを変更している場合は異なることがあります)
  * FlushMouse 動作時 (ExplorerPatcher Simple Window Switcher HelperをONにした場合)
     * JA / IME の時は、英数、ひらがな、カタカナ(Shift + ひらがな)のみコントロールしています。その他は,Windowsのデフォルトの動作です (以前のバージョンの IMEを使用してキーマップを変更すると使いやすくなると思います)
    
|↓押下キー　　　IMEの状態→|　　JA / IME 　　|　　JA / ENG 　　|　　US / ENG 　　|
| :---: | :---: | :---: | :---: |
|半角 / 全角 (漢字)         |　　 あ / A　　　|(前のIME ON状態) |(前のIME ON状態) |
|無変換                     |　　 IME OFF 　　|　　 IME OFF 　　|　　 IME OFF 　　|
|無変換(IME ONの状態から)   |　あ / ア / _ｱ 　|(前のIME ON状態) |(前のIME ON状態) |
|変換                       |(前のIME ON状態) |(前のIME ON状態) |(前のIME ON状態) |
|ひらがな                   |　　　 あ　　　　|(前のIME ON状態) |(前のIME ON状態) |
|カタカナ (Shift + ひらがな)|　　　 カ　　　　|(前のIME ON状態) |(前のIME ON状態) |
|英数                       |　　　 A 　　　　|(前のIME ON状態) |(前のIME ON状態) |

  * Windowsのデフォルト(参考)
 
|↓押下キー　　　IMEの状態→|　　JA / IME 　　|　　JA / ENG 　　|　　US / ENG 　　|
| :---: | :---: | :---: | :---: |
|半角 / 全角 (漢字)         |　　 あ / A　　　|　　　 －　　　　|　「 ` 」の入力　|
|無変換                     |　　　 －　　　　|　　　 －　　　　|　　　 －　　　　|
|無変換 (IME ONの状態から)  |　あ / ア / _ｱ 　|　　　 －　　　　|　　　 －　　　　|
|変換 |(前のIME ON状態)     |　　　 －　　　　|　　　 －　　　　|　　　 －　　　　|
|ひらがな                   |　　 あ / カ 　　|　　　 －　　　　|　　　 －　　　　|
|カタカナ (Shift + ひらがな)|あ(もう一度で カ)|　　　 －　　　　|　　　 －　　　　|
|英数                       |　　 あ / A　　　|　　　 －　　　　|　　　 －　　　　|


#### ビルド方法**

* まず、Visual Studio 2022 の開発環境が必要になりますので、二つの方法を紹介します

  A. ご自分のPCに直接Visual Studioを入れる場合は、
  このリンク [https://visualstudio.microsoft.com/ja/downloads/](https://visualstudio.microsoft.com/ja/downloads/) から入手してください(Communityで十分だと思います)
  
  B. 仮想マシンで開発環境を構築したい方は、このリンク [https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/](https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/) から入手してください
  

* 以下では、Bの方法を紹介します(私はVMwareで作業していますので、その手順を紹介しますが、他の仮想マシンでも同じような手順だと思います)。なお、ライセンス等については、よく読んでご判断ください

* BのWindows 11開発環境を取得する　から、仮想マシンをダウンロードしてインストールしたのち、日本語化するために最低設定する項目です(他にも設定する項目はありますが割愛します)

  1. Settings -> Time & language -> Language & regionで、日本語を追加します
  2. Japaneseを上に移動します
  3. 右側の「…」をクリックして、Option -> Keyboards -> Change layout -> Japanese keyboard(106/109 key)を選択し、Restart nowで再起動します
  4. Settings -> Time & language -> Date & Time -> Time zone -> UTC+09:00) Osaka, Sapporo, Tokyoを設定します
  5. Settings -> Time & language -> Language & region -> Windows display language -> 日本語 を選んで、 Sign out、Sign inします
  6. 設定 -> 時刻と言語 -> 言語と地域 -> 管理用言語の設定で表示されるダイアログで、システムロケールの変更を選んで 日本語(日本)を選択します
  7. ここで、Windows Updateをしておいたほうが良いと思います

* 次に、Visual Studioの設定を行います

  1. まず、Visual Studio **Installer** (必ず**Installer**を選んでください。Windowsキーを押して「vis」と入力すると候補に出てきます)を起動します
  2. 変更を選んで、 「C++ によるデスクトップ開発」にチェックを入れます。次に「言語パック」タブで日本語にチェックを入れます。その後、右下の変更ボタンで、インストールを行います
  3. 次に、WiXを使用するために、Windowsの設定 -> アプリ -> オプション機能 -> Windowsのその他の機能 -> Windowsの機能の有効化または無効化 -> .NET Framework 3.5.1(.NET 2.0及び3.0を含む)にチェックを入れてOKを押して、インストール、再起動します
  4. [https://github.com/wixtoolset/wix3](https://github.com/wixtoolset/wix3) から、Wix311.exeをダウンロードしてインストールします
  5. Visual Studioを起動して 右下の「コードなし」で起動します
  6. 拡張機能 -> 検索で　WiXと入力し「 WiX v3 - Visual Studio 2022 Extension」を選択して、Visual Studioを終了するとインストールが始まります

* ビルド方法です
  1. Visual Studioを起動して、リポジトリのクローン -> リポジトリの場所に [https://github.com/JeffyTS-JP/FlushMouse.git](https://github.com/JeffyTS-JP/FlushMouse.git)　を入力してクローンします
  2. クローンが終わったら、構成を **MixedPlatform** にしてビルドします
  3. Releaseでビルドすると、デフォルトでは C:\Users\User\source\repos\FlushMouseにFlushMouse_x64.msiができるのでインストールしてください

