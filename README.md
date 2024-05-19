
# FlushMouse
|![Icon](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/FlushMouse.png)|**システムトレイ(タスクトレイ)に常駐して、マウスカーソルへIMEモードの表示をするユーティリティです<br>FlushMouse is a software developed by JeffyTS. It is a mouse gesture tool that allows you to perform various operations by drawing gestures with your mouse. (by Bing)**|
| :---: | :--- |

#### リリース情報

* **2024/05/19　　Version 2.2.3.1 をリリースしました**

* **(主な変更点)**
   * SynTP Helper を VMware Workstation 17 Pro にも対応しました
   * β機能 SynTP Helperの動作を改善しました。また、Guest 側用にインストール不要の SynTP_Receiver を作成しました。詳しくは[下記](https://github.com/JeffyTS-JP/FlushMouse/tree/master?tab=readme-ov-file#syntp_helper%E6%A9%9F%E8%83%BD-synaptics%E7%A4%BE%E8%A3%BD%E3%82%BF%E3%83%83%E3%83%81%E3%83%91%E3%83%83%E3%83%89%E3%81%A8vmware-player%E3%81%A7%E3%81%AE-two-finger-scroll-%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)を参照してください
   * 細かなバグを修正しました
   
   * Version 2 から、Windows UI3に対応しました (といっても、設定画面などだけですし、あえて従来と似せたので、あまりそれらしくありませんが…)
     * なお、デスクトップ版と、UI3版の共存はできません。切り替える場合は、いったんアンインストールしてください


* **(今後について)**
   * ~~OSDN上ではここまでの機能で開発は終了としたいと考えています(バグ対応は除く)~~
   * ~~今後は、プレゼンテーションモードの追加や、マウスカーソルの形状や、表示する内容などをカスタマイズする機能をつけて有償に移行しようと考えています~~
   * と、上記のように考えていたのですが、この FlushMouse は Microsoft Storeに登録できないやり方を使って処理しているので無理なことが判明しました(フックを使用しいるとどうしてもだめなようです。ただ、SDKなどで、だいぶ検証しました)
   * ***ということで、今後も暇を見つけて、機能向上を目指していきますのでよろしくお願いします***

#### 対応OS
   * Windows 10 / 11 64bit  (Latest Release Versionで検証しています)
   * Version 2 以降のUI3対応版では、.NET Framework 8 のランタイムが必要になります。インストーラーで自動的にインストール画面が出ます

#### プライバシーポリシー
   * デスクトップ版、UI3版共に個人情報の収集などは一切行っていません
     * UI3版から、ネットワークへのアクセスとして、インストール時のランタイムのダウンロードと、バージョン情報にGitHubへのリンクがあります
     * また、Version 2.2から Synaptics社製タッチパッドと、VMware Playerへの対応として、ネットワークにアクセスします (詳しくは[下記](https://github.com/JeffyTS-JP/FlushMouse/tree/master?tab=readme-ov-file#syntp_helper%E6%A9%9F%E8%83%BD-synaptics%E7%A4%BE%E8%A3%BD%E3%82%BF%E3%83%83%E3%83%81%E3%83%91%E3%83%83%E3%83%89%E3%81%A8vmware-player%E3%81%A7%E3%81%AE-two-finger-scroll-%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)をご覧ください)

#### インストール / アンインストール
   * Windows Installer (exe)形式になっています。実行してインストールしてください
   * アンインストールは「設定」の「アプリと機能」、または「コントロールパネル」の「プログラムと機能」からできます
     * または、同じバージョンのインストーラーがあればそれをもう一度実行してもアンインストールできます
     * こちらの場合は、本プログラムの設定(レジストリ)を削除するかどうかを選べます(デフォルトでは残すようにしてあります)

#### 使い方
   * スタートメニュー、デスクトップのアイコンをクリックするか、PCを起動するとシステムトレイ(タスクトレイ)に常駐します
   * 不要であればデスクトップのショートカットアイコンは、削除してしまっても問題ありません

#### マウスカーソル
   * IME ONの時にモードを表示するマウスカーソルは、「矢印(通常の選択)」「Ｉビーム(テキスト選択)」「ハンド(リンクの選択)」の3種類です
   * IME OFF(直接入力)ではマウスカーソル左にはモードを表示しません(キャレットのモード表示は行います)
   * キャレットとマウスカーソル左へのIMEモード表示に、Caps Lock ONの時はアンダーラインを表示します (マウスカーソル右下の表示にはアンダーラインは表示されません。ご了承ください)
   
![Cursors](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/Cursors.png)

#### 設定ダイアログ画面
   * システムトレイ(タスクトレイ)の中にあるFlushMouseのアイコンをクリックすることで表示します
   * UI3版
![SettingDialogUI3](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/SettingDlgUI3.png)
   * デスクトップアプリ版
![SettingDialog](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/SettingDlg.png)

#### 設定のヒント
   * Windows 10 / 11ではマウスカーソルの形状や大きさを決める方法が複数あるため、下記の順番で設定することをお勧めします 
  1. 設定→ディスプレイ→拡大縮小とレイアウトで、「ディスプレイの解像度」、「テキスト、アプリ、その他の項目のサイズ」、「表示スケールの詳細設定」を好みに設定します
  2. 設定→デバイス→マウス→「マウスとカーソルのサイズを調整する」で好みに設定します。ただし、この変更を行うとマウスカーソルが、Windows 10 / 11特有のものになります。もし、Windows 7などの以前のマウスカーソルを使いたい場合は３の設定をします。必要なければ４に進んでください
  3. 設定→デバイス→マウス→「その他のマウスオプション」(これはコントロールパネルのマウスのプロパティと同じ設定画面です)の、ポインタータブで、好みのカーソル形状、大きさに設定します
  4. システムトレイ(タスクトレイ)のFlushMouseのアイコンをクリックして設定を選択し、「マウスカーソルのサイズ」を好みの大きさに設定します
   * FlushMouseで設定するカーソルの大きさは、アプリケーション独自に表示するもの(例えばExcelの十字型のカーソルなど)には影響しませんので、１から３を繰り返して、それらが好みの大きさになるようにしてから、FlushMouseでコントロールするカーソルの大きさを合わせると使いやすくなると思います

#### 設定項目について (わかりづらい項目のみ説明します)
   * **IMEモードの取得方法**
     * 「マウスカーソル下のウィンドウのIMEモードを表示する」 / 「フォーカスウィンドウのIMEモードを表示する」から選べます
     * 設定→時刻と言語→言語→キーボードの「入力方式の切り替え」や、旧IMEのプロパティにある「IME入力モード切替の通知の画面中央に表示する」と関わってきますので、ご自分の設定に合わせて好みな方を選択してください
   * **IME関連キーがクリックされたときにIMEモードを表示する**
     * IME関連キーとは「半角/全角(漢字)」「変換」「無変換」「カタカナ/ひらがな」「Ctrl + F6～F9」などを指します。加えて、「TAB」「Enter」キーでも表示します
   * **IMEモードをできるだけキャレット(テキストカーソル)の左に表示する**
     * アプリによってはキャレットの位置が取得できなかったり、表示位置がおかしくなることがあります(これは制限事項となります)。ただし、位置が取得できない場合は、マウスカーソルの左に表示します
   * **IME 関連キーのサポートを有効にする**
   * **ExplorerPatcher Simple Window Switcher Helperを有効にする**
     * [下記](https://github.com/JeffyTS-JP/FlushMouse/tree/master?tab=readme-ov-file#ime-%E9%96%A2%E9%80%A3%E3%82%AD%E3%83%BC%E3%81%AE%E3%82%B5%E3%83%9D%E3%83%BC%E3%83%88-%E3%81%A8-explorerpatcher-simple-window-switcher-helper-%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)説明をご覧ください
   * **モード表示の時間**
     * マウスカーソルの左に「あ」「ア」「Ａ」「_ｱ」「_A」を表示をしている時間を調整できます
   * **モード表示までの時間**
     * マウスカーソルの左に「あ」「ア」「Ａ」「_ｱ」「_A」を表示するまでの時間を調整できます (一部のアプリケーションでは、モードが切り替わるまで時間がかかる場合がありますので、誤判定が発生する場合があります。お好みで調整してください)
   * **Ctrlキーを離してからモードを表示するまでの時間**
     * 設定の「マウスのプロパティ」にある、「Ctrlキーを押すとポインターの位置を表示する」をONにしているとき、この表示と重なるのを避けるためにモードを表示するまでの時間を調整できます
   * 本FlushMouseでは、「矢印」「Ｉビーム」「ハンド」を 「aero_」から抽出したものを表示しています(これは固定になります。ご了承ください)


#### IME 関連キーのサポート と ExplorerPatcher Simple Window Switcher Helper について
  * まず、[ExplorerPatcher](https://github.com/valinet) の作者であるValentin-Gabrielに謝意を伝えます　Thanks, Valentin-Gabriel !
  * さて、マルチモニターの環境でSimple Window Switcherを有効にすると、Alt + Tabでマウスカーソルのあるカレントモニターのウィンドウだけを切り替えられて非常に便利です。もちろん他にも便利な機能があります

  * しかし、[Valinetのブログ](https://valinet.ro/2021/11/18/Functional-Windows-10-flyouts-on-Windows-11.html)を読むとわかるのですが、英語キーボード(101/102/104キーボード)のドライバーをロードして実現しています。このため、日本語キーボード(106/109)メインの我々にはちょっと困ったことになってしまいます。例えば、漢字(半角/全角)キーで「 ` 」が入力されてしまったり、「@」マークなどの記号の位置が変わってしまったり。何より日本語変換ができません。そこで、FlushMouseに、強制的に「US / ENG」を排除する機能を加えました
  * ~~Windowsキー + スペースで入力言語を切り替えた時、IME関連キー押したときやマウスクリックしたとき、フォーカスが切り替わったときなどにIMEの状態をチェックして、後述するように動作します~~
  * FlushMouse Version 2 以降から、EP Helperの機能を独立させました。ON になっている場合、IME関連キーを押下したかどうかにかかわらず「US / ENG」にならないように動作します
  * ただし、以下のような制限事項があります
    * ★**物理的に英語(101/102/104)キーボードを接続している場合は動きません。必ず日本語(106/109)キーボードの環境で使用してください**(日本語キーボードが見つからないときには、本機能が動作しないようにチェックしています)

  * IME関連キーのサポートの動作は下記のようになります(IMEの設定でキーマップを変更している場合は異なることがあります)
    * 動作としては、キーボード標記の動作になるようにする機能です
    *  * 検索をすると良く出てくる、Edge と Chromeで、漢字変換ができない問題(変換モードが「一般」から「無変換」に時々変わってしまう)に対して、FlushMouse内で軽減できますので対応しました(IME 関連キーのサポートを ONにした時のみ動作します)
    * 「JA / ENG」 の時、キーボードロケールを返さないアプリがあります。この場合は、前のIMEの状態に戻すことができません。これは制限事項となります。ご了承ください
    * 一部のアプリでは「漢字(半角/全角)」などのIME関連キーで「JA / ENG」から切り替わらないことがあります。現時点では制限事項となります。ご了承ください
    * ★**EP Helper、IME関連キーのサポート のどちらも、Windowsに付属されているMicrosoft IMEでしか検証していません**
 
|↓押下キー　　　IMEの状態→|　　JA / IME 　　|　　JA / ENG 　　|
| :---: | :---: | :---: |
|半角 / 全角 (漢字)         |　　 あ / A　　　|　　　 あ　　　　|
|無変換                     |　　　 A 　　　　|　　　 －　　　　|
|変換                       |(前のIME ON状態) |(前のIME ON状態) |
|ひらがな                   |　　　 あ　　　　|(前のIME ON状態) |
|カタカナ (Shift + ひらがな)|　　　 カ　　　　|(前のIME ON状態) |
|英数                       |　　　 A 　　　　|　　　 －　　　　|

  * Windowsのデフォルト(参考)
 
|↓押下キー　　　IMEの状態→|　　JA / IME 　　|　　JA / ENG 　　|　　US / ENG 　　|
| :---: | :---: | :---: | :---: |
|半角 / 全角 (漢字)         |　　 あ / A　　　|　　　 －　　　　|　「 ` 」の入力　|
|無変換                     |　　　 －　　　　|　　　 －　　　　|　　　 －　　　　|
|無変換 (IME ONの状態から)  |　あ / ア / _ｱ 　|　　　 －　　　　|　　　 －　　　　|
|変換 |(前のIME ON状態)     |　　　 －　　　　|　　　 －　　　　|　　　 －　　　　|
|ひらがな                   |　　   あ   　　|　　　 －　　　　|　　　 －　　　　|
|カタカナ (Shift + ひらがな)|　　   ア   　　|　　　 －　　　　|　　　 －　　　　|
|英数                       |　　 あ / A　　　|　　　 －　　　　|　　　 －　　　　|


#### SynTP_Helper機能 (Synaptics社製タッチパッドと、VMware Workstation / Playerでの Two Finger Scroll) について
  * 動作環境 (検証環境)
    * Panasonic社製 Let's note CF-SV1 搭載の Synaptics社製 Wheel Pad (Touch Pad) および Windows 11 Pro (Version 23H2  Build 22631.3296)
    * Broadcom (VMware)社製 VMware(R) Workstation 17 Pro (Version 17.5.2) / VMware(R) Workstation 17 Player (Version 17.5.2)
    * Guest OS は、主に Windows 11 Pro (Version 23H2  Build 22635.2552)
    
  * 機能概要
    * 上記環境で、Wheel Padの　Two Finger Scroll、および Wheel Pad 沿いを回転するようになぞることでのスクロール(右端が起点)を可能にしました

  * 仕組み (内部動作)
    * Host OS で、Wheel Padの動作を RawInputを使用して受取り、UDPを使用して、Guest OSに送ります。Guest OS では送られてきたデータを SendInputで出力しています
 
  * 使用方法
    * Host には、FlushMouse をインストールしてください
    * Guest には、FlushMouse をインストールするか、SynTP_Receiver_vx.x.x.x.zip を展開し、SynTP_Receiver.exe をコピーしてください
    * タスクトレイにある、FlushMouse のアイコンをクリックすると出てくるメニューから、SynTP Helperを選び設定画面を出します
      ![SynTPHelperDlg](https://github.com/JeffyTS-JP/FlushMouse/blob/master/OSDN%20Wiki/SynTPHelperDlg.png)
      * Host 側では、Senderとして登録します。IP Address の欄には、Guest OSの IPv4 アドレスを入力します (一般的には Ethernet1のアドレスでよいと思います)。また、UDP Portには、空いている番号を入れてください
      * Guest 側では、Receiverとして登録します。UDP Portには、Host と同じ番号を入れてください (Guest 側では、UDPを Anyで受けますので、IP Addressは必要ありません)
      * 設定 -> ネットワークとインターネット -> ネットワークの詳細設定 -> 共有の詳細設定 -> プライベートネットワークのネットワーク探索を ON にしてください
      * (初回のみ)ネットワークへのアクセス許可のダイアログが出ますので、許可してください
      * Host 側で Hostname を使用して、FlushMouse 起動時からスタートする場合、名前解決ができるようにネットワークが構成されている必要が有ります。名前解決ができない場合はストップ状態となります
    * SynTP_Receiver の場合は、ショートカットを作成し、プロパティを開いて「Your folder path\SynTP_Receiver.exe /Port 50008」 のように UDP Portを指定します
      * 管理者権限で起動しない場合、タスクマネージャーなど一部のアプリでスクロールしません。ショートカットのプロパティの詳細設定で「管理者権限で実行する」にチェックをしてください
      * SynTP_Receiverを終了するためには、タスクマネージャーからタスクの終了を行います
   
  * その他 (制限事項、今後など)
    * ネットワークを介在してマウスの動きを送受信していますので、ネットワークがビジーの時など少々動きがにぶくなります。これは制限事項となります
    * いろいろと調べると、他社製はもちろん、Synaptics社の Touch Padでさえも、仕様がいろいろとあるようです。このため、Panasonic社製 Let's note CF-SV1に限定しています (特にプログラム内で制限はしていませんので、動いたらラッキーと思ってください。実機を貸していただければ、対応できるかもしれませんが)
    * スクロールの量や方向などは、私の好みにしています。設定できるようにするのは、今後の課題です
    * Guest OS で、UNIX系については、私は詳しくありませんので、どなたか受信側を書いていただければ。。。
    * 設定画面は、手を抜いてUI3でも従来のやり方にしています。今後考えます
    * VMware社が Broadcom社に買収され、先日、Player をやめて Workstation Proに移行するとのアナウンスがありました。商用利用ではない場合は、無償とのことです。~~今後、開発環境を整えて、移行を考えたいと思います~~　Ver.2.2.3.1で対応しました
    


#### ビルド方法

* まず、Visual Studio 2022 の開発環境が必要になりますので、二つの方法を紹介します

  (A). ご自分のPCに直接Visual Studioを入れる場合は、
  このリンク [https://visualstudio.microsoft.com/ja/downloads/](https://visualstudio.microsoft.com/ja/downloads/) から入手してください(Communityで十分だと思います)
  
  (B). 仮想マシンで開発環境を構築したい方は、このリンク [https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/](https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/) から入手してください
  

* 以下では、(B)の方法を紹介します(私はVMwareで作業していますので、その手順を紹介しますが、他の仮想マシンでも同じような手順だと思います)。なお、ライセンス等については、よく読んでご判断ください

* (B) のWindows 11開発環境を取得する　から、仮想マシンをダウンロードしてインストールしたのち、日本語化するために最低設定する項目です(他にも設定する項目はありますが割愛します)

  1. Settings -> Time & language -> Language & regionで、日本語を追加します
  2. Japaneseを上に移動します
  3. 右側の「…」をクリックして、Option -> Keyboards -> Change layout -> Japanese keyboard(106/109 key)を選択し、Restart nowで再起動します
  4. Settings -> Time & language -> Date & Time -> Time zone -> UTC+09:00) Osaka, Sapporo, Tokyoを設定します
  5. Settings -> Time & language -> Language & region -> Windows display language -> 日本語 を選んで、 Sign out、Sign inします
  6. 設定 -> 時刻と言語 -> 言語と地域 -> 管理用言語の設定で表示されるダイアログで、システムロケールの変更を選んで 日本語(日本)を選択します
  7. ここで、Windows Updateをしておいたほうが良いと思います

* 次に、Visual Studioの設定を行います

  1. まず、Visual Studio **Installer** (本体ではなく**Installer**です。Windowsキーを押して「vis」と入力すると候補に出てきます)を起動します
  2. 変更を選んで、 「C++ によるデスクトップ開発」にチェックを入れます。次に「言語パック」タブで日本語にチェックを入れます。その後、右下の変更ボタンで、インストールを行います
  3. 次に、WiXを使用するために、Windowsの設定 -> アプリ -> オプション機能 -> Windowsのその他の機能 -> Windowsの機能の有効化または無効化 -> .NET Framework 3.5.1(.NET 2.0及び3.0を含む)にチェックを入れてOKを押して、インストール、再起動します
  4. [https://github.com/wixtoolset/wix3](https://github.com/wixtoolset/wix3) から、Wix311.exeをダウンロードしてインストールします
  5. Visual Studioを起動して 右下の「コードなし」で起動します
  6. Tools -> Options -> Environment -> International Settings をクリックして、Languageを日本語にします
  7. 拡張機能 -> 検索で　WiXと入力し「 WiX v3 - Visual Studio 2022 Extension」を選択して、Visual Studioを終了するとインストールが始まります
  8. Visual Studioを再起動して、リポジトリのクローン -> リポジトリの場所に [https://github.com/JeffyTS-JP/FlushMouse.git](https://github.com/JeffyTS-JP/FlushMouse.git)　を入力してクローンします

* 以下はUI3のために必要な作業です
  1. .NET Framework SDK 8 を[https://dotnet.microsoft.com/ja-jp/download/dotnet/thank-you/sdk-8.0.101-windows-x64-installer](https://dotnet.microsoft.com/ja-jp/download/dotnet/thank-you/sdk-8.0.101-windows-x64-installer)からダウンロードしてインストールします
  2. NuGet CLI(nuget.exe)を[https://dist.nuget.org/win-x86-commandline/latest/nuget.exe](https://dist.nuget.org/win-x86-commandline/latest/nuget.exe)からダウンロードして、FlushMouseをクローンしたフォルダの直下の「NuGet」フォルダ(デフォルトでは C:\Users\User\source\repos\FlushMouse\NuGet です)に入れます
  3. Windowsの 設定 -> システム -> 開発者向け で 開発者モードを ON にします
  5. 次に、ツール -> NuGet パッケージマネージャー -> ソリューションの NuGet パッケージの管理 (Altキーを押しながら続けて T N N) を開き、右側にある歯車(⚙)を選択して、ダイアログを開きます
  6. まず、右上のプラス(＋)を押してから、下にある「ソース」欄には、nuget.exeを入れた「NuGet」フォルダ(デフォルトでは C:\Users\User\source\repos\FlushMouse\NuGet)を選択して「更新」を押します(名前については適当で構いません)
  7. その後 OK を押してダイアログを閉じ、上の歯車の左にある「パッケージソース」を「すべて」に変更します。次に「復元」選択します

* ビルド方法です
  1. ソリューションプラットフォームを「**MixedPlatform**」にしてビルドします (Ctrl + Shift + B)
  2. ソリューション構成を「Release」でビルドすると、デフォルトでは C:\Users\User\source\repos\FlushMouseにFlushMouse_x64.exe (Version 1.2以前は FlushMouse_x64.msi)と、FlushMouseUI3_x64.exeができるのでインストールしてご利用ください


