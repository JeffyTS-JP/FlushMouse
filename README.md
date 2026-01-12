
# FlushMouse
|<img width="100" height="100" alt="Image" src="https://github.com/user-attachments/assets/520a32ac-afd6-4af7-b0c3-389f8daf7c79" />|**システムトレイ(タスクトレイ)に常駐して、マウスカーソルやキャレットに IMEモードの表示をするユーティリティです<br>FlushMouse is a software developed by JeffyTS. It is a mouse gesture tool that allows you to perform various operations by drawing gestures with your mouse. (by Bing)**|
| :---: | :--- |

#### リリース情報

* **2026/01/24　　Version 3.8.3.6 を リリースしました**

* **(主な変更点)**
   * 終了時の処理を見直しました
   * IME モード表示、カーソルデータの処理をキャッシュを使用するようにしました
   * インストーラーで .NET Framework 10 ランタイムのインストールを見直しました
   * その他、細かなバグの修正や機能改善をしました
   
#### DEMO
![Image](https://github.com/user-attachments/assets/7f710568-6645-4c80-a26e-94ead6543689)

#### 対応OS
   * Windows 10 / 11 64bit  (Latest Release Versionで検証しています。25H2 に対応しました)
   * WinUI3 のために.NET Framework 10 のランタイムが必要になります。インストーラーで自動的にインストールされます

#### (今後について)
   * ~~OSDN上ではここまでの機能で開発は終了としたいと考えています(バグ対応は除く)~~
   * ~~今後は、プレゼンテーションモードの追加や、マウスカーソルの形状や、表示する内容などをカスタマイズする機能をつけて有償に移行しようと考えています~~
   * と、上記のように考えていたのですが、この FlushMouse は Microsoft Storeに登録できないやり方を使って処理しているので無理なことが判明しました(フックを使用しいるとどうしてもだめなようです。ただ、SDKなどで、だいぶ検証しました)
   * このため、ダウンロードやインストール時に警告が出ますがご容赦ください (ソースコードを公開していますし、ビルド手順も記載していますので気になる方は試してみてください)
   * ***ということで、今後も暇を見つけて、GitHub上で新機能開発や機能向上を目指していきますのでよろしくお願いします***

#### (既知の問題)
   * Ver.3 からの デスクトップ版の UI3 で動く設定画面や、UI3 版の本体において、アプリ終了時にメモリーリークしていることがわかっています (動的に増えていくメモリーリークではありません)
   * プログラム的には、WinMain に入ってくるまでと、抜けた後の処理でリークしているようです (Ver.3.2 から WinMain を使っていませんが同様の症状がでます)。このため、当方に直す方法はなく、Microsoft の SDK 内の問題であり、アプリやシステムには影響はないものと考えています

#### Copyright
   * Copyright (C) 1993 - JeffyTS. All rights reserved. Licensed under the GPL-2.0 License.
   * 詳しくはインストール時に表示される「ソフトウェア使用許諾契約」をお読みください
   * FlushMouse を最初に作成したのは、1993年でしたが、使い物になるようなものではありませんでした。その後、2022年に偶然見つけて開発を続けています

#### プライバシーポリシー
   * デスクトップ版、UI3版共に個人情報の収集などは一切行っていません
     * ネットワークへのアクセスとして、インストール時のランタイムのダウンロードと、バージョン情報にGitHubへのリンクがあります
     * また、Ver.3 から最新のバージョンを確認できるようにしましたので、その際にネットワークへのアクセスがあります
     * Ver.2.2 から Synaptics社製タッチパッドと、VMware Workstation / Playerへの対応として、ネットワークにアクセスします (詳しくは[下記](https://GitHub.com/JeffyTS-JP/FlushMouse/tree/master?tab=readme-ov-file#syntp_helper%E6%A9%9F%E8%83%BD-synaptics%E7%A4%BE%E8%A3%BD%E3%82%BF%E3%83%83%E3%83%81%E3%83%91%E3%83%83%E3%83%89%E3%81%A8vmware-player%E3%81%A7%E3%81%AE-two-finger-scroll-%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)をご覧ください)
     * Ver.3.3 から、直接描画ではないときの、マウスカーソルのリソースをプログラム的に作成しますので、テンポラリーファイルへの書き込みがあります

#### インストール / アンインストール
   * Windows Installer (exe)形式になっています。実行してインストールしてください
   * アンインストールは「設定」の「アプリと機能」、または「コントロールパネル」の「プログラムと機能」からできます
     * または、同じバージョンのインストーラーがあればそれをもう一度実行してもアンインストールできます。この場合は、本プログラムの設定(レジストリ)を削除するかどうかを選べます(デフォルトでは残すようにしてあります)
   * Ver.2 から、WinUI3に対応しました。プログラム構成については、[下記](https://GitHub.com/JeffyTS-JP/FlushMouse?tab=readme-ov-file#%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0%E6%A7%8B%E6%88%90)をご参照ください
     * なお、デスクトップ版と、UI3版の共存はできません。切り替える場合は、いったんアンインストールしてください (設定は共通ですので、レジストリを消す必要はありません)

#### 使い方
   * スタートメニュー、デスクトップのアイコンをクリックするか、PC を起動するとシステムトレイ(タスクトレイ)に常駐します
   * 不要であればデスクトップのショートカットアイコンは、削除してしまっても問題ありません

#### マウスカーソル
   * Ver.3.4 以降では下記の通りとなります
   * 「右下の描画方法」として、「リソース方式を使用する」「直接描画方式を使用する」「リソース方式と直接描画方式を併用する」から選べます
   * 「リソース方式を使用する」と「リソース方式と直接描画方式を併用する」の場合は、モードを表示するマウスカーソルのうち、「矢印(通常の選択)」「Ｉビーム(テキスト選択)」「ハンド(リンクの選択)」の3種類は、本FlushMouseで作成したリソースになります
     * 上記モードで表示する「矢印」「Ｉビーム」「ハンド」を 「aero_」から抽出したものを表示しています(これは固定になります。ご了承ください)
   * 「直接描画方式を使用する」の場合には、システム設定のカーソルをそのまま使用します
   * 「直接描画方式を使用する」と「リソース方式と直接描画方式を併用する」の場合はすべてのマウスカーソルで IME のモード表示が行えます(ただし、システムで表示する、ビジー(砂時計など)や、ウィンドウサイズ変更にかかわるカーソルでは表示しません)
   * 「直接描画方式を使用する」とその他の描画方式では「矢印」「Ｉビーム」「ハンド」への IMEモードの表示が若干異なります(これは制限事項となります。ご了承ください)
   * IME OFF(直接入力)の時にマウスカーソル左側と右下へ表示するかどうかを設定できます
   * IMEモード表示に、Caps Lock ONの時はアンダーラインを表示します (リソースで表示する部分もVer.3.6.1で対応しました)

||方式概要|メリット|デメリット|
| :---: | :---: | :---: | :---: |
|リソース方式を使用する|矢印、Ｉビーム、ハンドのみ IME モードをリソースで表示します|CPUの消費量が少なくて済みます|3種類のマウスカーソル形状がお仕着せになります|
|直接描画方式を使用する|マウスカーソルの右下へ直接描画します|システム設定のカーソルが使用できます|システムがビジーの時は描画が遅れます|
|リソース方式と直接描画方式を併用する|矢印、Ｉビーム、ハンドをリソースで表示し、残りは直接描画します|ほとんどのケースでリソースで表示することになりますので遅れも目立たないと思います|2方式を併用してますのでのマウスサイズなどの設定が少々面倒ですが[こちら](https://GitHub.com/JeffyTS-JP/FlushMouse?tab=readme-ov-file#%E8%A8%AD%E5%AE%9A%E3%81%AE%E3%83%92%E3%83%B3%E3%83%88)を参考にしてみてください|
   
<img alt="Image" src="https://github.com/user-attachments/assets/4e96b099-92e8-4d65-8ee3-6d9b252686d6" width="30%" />

#### 設定のヒント
   * Ver.3.1 以降では、「右下の描画方法」で、「直接描画方式を使用する」を選択している場合には、システムのカーソルをそのまま使用しますので、下記設定は不要です (その他の方式の場合に行ってください)
   * 「リソース方式と直接描画方式を併用する」を使用する際は、最初に「直接描画方式を使用する」にして、距離、サイズを決めてから「リソース方式と直接描画方式を併用する」に戻して「矢印」「Ｉビーム」「ハンド」の大きさを決めると楽だと思います
   * Windows 10 / 11ではマウスカーソルの形状や大きさを決める方法が複数あるため、下記の順番で設定することをお勧めします 
  1. 設定→ディスプレイ→拡大縮小とレイアウトで、「ディスプレイの解像度」、「テキスト、アプリ、その他の項目のサイズ」、「表示スケールの詳細設定」を好みに設定します
  2. 設定→デバイス→マウス→「マウスとカーソルのサイズを調整する」で好みに設定します。ただし、この変更を行うとマウスカーソルが、Windows 10 / 11特有のものになります。もし、Windows 7などの以前のマウスカーソルを使いたい場合は３の設定をします。必要なければ４に進んでください
  3. 設定→デバイス→マウス→「その他のマウスオプション」(これはコントロールパネルのマウスのプロパティと同じ設定画面です)の、ポインタータブで、好みのカーソル形状、大きさに設定します
  4. システムトレイ(タスクトレイ)のFlushMouseのアイコンをクリックして設定を選択し、「マウスカーソルのサイズ」を好みの大きさに設定します
   * FlushMouseで設定するカーソルの大きさは、アプリケーション独自に表示するもの(例えばExcelの十字型のカーソルなど)には影響しませんので、１から３を繰り返して、それらが好みの大きさになるようにしてから、FlushMouseでコントロールするカーソルの大きさを合わせると使いやすくなると思います

#### 設定項目について (わかりづらい項目のみ説明します)
   * 設定ダイアログ画面は右下のシステムトレイ(タスクトレイ)の「∧」マークの中にある FlushMouseのアイコンをクリックすることで表示します
   * Ver.3から、デスクトップアプリ版、UI3版共に同じ設定画面が表示されます

<img alt="Image" src="https://github.com/user-attachments/assets/d770f02c-85bc-46fb-bbe3-ea47c93f5169" width="50%" />

   * **IMEモードの取得方法**
     * 「マウスカーソル下のウィンドウのIMEモードを表示する」 / 「フォーカスウィンドウのIMEモードを表示する」から選べます
     * 設定→時刻と言語→言語→キーボードの「入力方式の切り替え」や、旧IMEのプロパティにある「IME入力モード切替の通知の画面中央に表示する」と関わってきますので、ご自分の設定に合わせて好みな方を選択してください。なお、アプリや入力欄によっては IME のモードを返さない場合があります。この場合は IME OFFとして取り扱います。これは制限事項となります。ご了承ください
   * **右下の描画方法 (Ver.3.4 以降)**
     * [上記](https://GitHub.com/JeffyTS-JP/FlushMouse?tab=readme-ov-file#%E3%83%9E%E3%82%A6%E3%82%B9%E3%82%AB%E3%83%BC%E3%82%BD%E3%83%AB)のマウスカーソルの項目をご参照ください
   * **直接入力 (IME OFF) の時も IME モードの表示をする (Ver.3.1 以降)**
     * 変換モードではない、すなわち直接入力(IME OFF)の時も、マウスカーソル左と右下に IME モードを表示します
   * **IME関連キーがクリックされたときにIMEモードを表示する**
     * IME関連キーとは「半角/全角(漢字)」「変換」「無変換」「カタカナ/ひらがな」「Ctrl + F6～F9」などを指します。加えて、「TAB」「Enter」「F5」キーでも表示します
   * **IMEモードをキャレット(テキストカーソル)左に表示する**
     * アプリによってはキャレットの位置が取得できなかったり、表示位置がおかしくなることがあります(これは制限事項となります)。ただし、位置が取得できない場合は、マウスカーソルの左に表示します
   * **Windows Remote Desktop と VMware guest のウィンドウ内で、できる限り Host の IME モードを表示しない**
     * この機能を ON にすると、Windows Remote Desktop と VMware guest のウィンドウ内では、できる限り Host の IME モードを表示しないようにします
     * ただし、CPU 使用率が若干上がりますので、必要のない方は OFF (デフォルト)にしてください
     * Remote Desktop と VMware guest がフォーカスを得た段階で有効になります。フォーカスの無い場合や、フォーカスを得た時に、多少 Host の IME モードが表示されることがあります(制限事項となります)
   * **IME 関連キーのサポートを有効にする**
   * **ExplorerPatcher Simple Window Switcher Helperを有効にする**
     * 上記 2項目は [こちらの説明](https://GitHub.com/JeffyTS-JP/FlushMouse/tree/master?tab=readme-ov-file#ime-%E9%96%A2%E9%80%A3%E3%82%AD%E3%83%BC%E3%81%AE%E3%82%B5%E3%83%9D%E3%83%BC%E3%83%88-%E3%81%A8-explorerpatcher-simple-window-switcher-helper-%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)をご覧ください
   * **モード表示の時間**
     * マウスカーソルの左に「あ」「ア」「Ａ」「_ｱ」「_A」を表示をしている時間を調整できます
   * **モード表示までの時間**
     * マウスカーソルの左に「あ」「ア」「Ａ」「_ｱ」「_A」を表示するまでの時間を調整できます (一部のアプリケーションでは、モードが切り替わるまで時間がかかる場合がありますので、誤判定が発生する場合があります。お好みで調整してください)
   * **Ctrl キーを離してからモードを表示するまでの時間**
     * 設定の「マウスのプロパティ」にある、「Ctrlキーを押すとポインターの位置を表示する」をONにしているとき、この表示と重なるのを避けるためにモードを表示するまでの時間を調整できます
   * **IMEモード表示設定**
     * Ver.3.2 から、マウスカーソルの左、キャレットの左、マウスカーソルの右下の、モード表示のサイズ、フォント、色、表示文字を変更できます。Ver.3.5 からマウスカーソルとキャレット左の水平方向の表示位置を調整できます
     * 「直接描画方式を使用する」場合、マウスカーソル右下の表示位置の距離を変更できます(正確にはホットスポットからの距離です)。お好みで設定してください
     * 「リソース方式を使用する」と「リソース方式と直接描画方式を併用する」の場合、「矢印」「Ｉビーム」「ハンド」での表示位置の距離は変更できません
     * 表示する文字は、記号などでも設定できますので、お好みで変更してみてください
     * フォントは「Yu Gothic UI (デフォルト)」「Meiryo UI」「MS Gothic」「Consolas」の４種類です。選択したフォントによっては表示が切れたりしますが、現時点では仕様となります。ご了承ください

<img alt="Image" src="https://github.com/user-attachments/assets/072b4a46-b256-48c8-afa3-872f9dab5fbd" width="50%" />

   * **バージョン画面**

<img alt="Image" src="https://github.com/user-attachments/assets/704f1f3c-ed23-4c8b-846c-b011abf38979" width="50%" />


#### IME 関連キーのサポート と ExplorerPatcher Simple Window Switcher Helper について
  * まず、[ExplorerPatcher](https://GitHub.com/valinet) の作者であるValentin-Gabrielに謝意を伝えます　Thanks, Valentin-Gabriel !
  * さて、マルチモニターの環境でSimple Window Switcherを有効にすると、Alt + Tabでマウスカーソルのあるカレントモニターのウィンドウだけを切り替えられて非常に便利です。もちろん他にも便利な機能があります

  * しかし、[Valinetのブログ](https://valinet.ro/2021/11/18/Functional-Windows-10-flyouts-on-Windows-11.html)を読むとわかるのですが、英語キーボード(101/102/104キーボード)のドライバーをロードして実現しています。このため、日本語キーボード(106/109)メインの我々にはちょっと困ったことになってしまいます。例えば、漢字(半角/全角)キーで「 ` 」が入力されてしまったり、「@」マークなどの記号の位置が変わってしまったり。何より日本語変換ができません。そこで、FlushMouseに、強制的に「US / ENG」を排除する機能を加えました
  * ~~Windowsキー + スペースで入力言語を切り替えた時、IME関連キー押したときやマウスクリックしたとき、フォーカスが切り替わったときなどにIMEの状態をチェックして、後述するように動作します~~
  * Ver.2 以降から、[US / ENG] になったことが、Hookで通知されることがわかりましたので、EP Helperの機能を独立させました。設定が ON になっている場合、IME関連キーを押下したかどうかにかかわらず「US / ENG」にならないように動作します
  * ただし、以下のような制限事項があります
    * ★**物理的に英語(101/102/104)キーボードを接続している場合は動きません。必ず日本語(106/109)キーボードの環境で使用してください**(日本語キーボードが見つからないときには、本機能が動作しないようにチェックしています)

  * IME関連キーのサポートの動作は下記のようになります(IME の設定でキーマップを変更している場合は異なることがあります)
    * 動作としては、キーボード標記の動作になるようにする機能です
    * 検索をすると良く出てくる、Edge と Chromeで、漢字変換ができない問題(変換モードが「一般」から「無変換」に時々変わってしまう)に対して、FlushMouse内で軽減できますので対応しました(IME 関連キーのサポートを ONにした時のみ動作します)
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


#### (β機能) SynTP_Helper機能 (Synaptics社製タッチパッドと、VMware Workstation / Playerでの Two Finger Scroll) について
  * 動作環境 (検証環境)
    * Panasonic社製 Let's note CF-SV1 搭載の Synaptics社製 Wheel Pad (Touch Pad) および Windows 11 Pro (Version 25H2)
    * Broadcom (VMware)社製 VMware(R) Workstation Pro 25H2 / VMware(R) Workstation Player 25H2
    * Guest OS は、主に Windows 11 Pro (Version 25H2)
    
  * 機能概要
    * 上記環境で、Wheel Padの Two Finger Scroll、および Wheel Pad 沿いを回転するようになぞることでのスクロール(右端が起点)を可能にしました

  * 仕組み (内部動作)
    * Host OS で、Wheel Padの動作を RawInputを使用して受取り、UDPを使用して、Guest OSに送ります。Guest OS では送られてきたデータを SendInputで WM_MOUSE(H)WHEEL として出力しています
  
  * 設定画面

<img alt="Image" src="https://github.com/user-attachments/assets/82a9f547-63e6-40a1-bf17-24508152c3f9" width="50%" />

  * 使用方法
    * Host には、FlushMouse をインストールしてください
    * Guest には、FlushMouse をインストールするか、SynTP_Receiver_vx.x.x.x.zip を展開し、任意の場所に SynTP_Receiver.exe をコピーしてください
    * タスクトレイにある、FlushMouse のアイコンをクリックすると出てくるメニューから、SynTP Helperを選び設定画面を出します
      * Host 側では、Senderとして登録します。IP Address の欄には、Guest OSの IPv4 アドレス、または、Hostname を入力します (一般的には Ethernet1のアドレスでよいと思います)。また、UDP Portには、空いている番号を入れてください
      * IP Address、Hostname どちらも、プライベートアドレスのみ指定できます (10.0.0.0 - 10.255.255.255, 172.16.0.0 - 172.31.255.255, 192.168.0.0 - 192.168.255.255)
      * Host 側で Hostname を使用して、FlushMouse 起動時からスタートする場合、名前解決ができるようにネットワークが構成されている必要が有ります。名前解決ができない場合はストップ状態となりメッセージを表示します
      * Guest 側では、Receiverとして登録します。UDP Portには、Host と同じ番号を入れてください (Guest 側では、UDPを Anyで受けますので、IP Addressは必要ありません)
      * 設定 -> ネットワークとインターネット -> ネットワークの詳細設定 -> 共有の詳細設定 -> プライベートネットワークのネットワーク探索を ON にしてください
      * (初回のみ)ネットワークへのアクセス許可のダイアログが出ますので、許可してください
      * Ver. 2.2.4.7 から、TouchPadのサイズと Edge motion の幅を、Synaptics のレジストリ (HKEY_LOCAL_MACHINE\SOFTWARE\Synaptics\OEM\TouchPad) を参照するのではなく、独自に諸元を設定できるようにしました。これにより、多少機種依存の部分が緩和され、Touch Pad のデータ出力方法が同じであれば、動く可能性があると思います
        * Let's note CF-SV1 の場合、TouchPad Width = 528, Height = 528, Edge Area Width = 88, Height = 88 (デフォルト値)です
        * TouchPad Width, Height は、左下を原点とした、Touch Pad のサイズです
        * Edge Area Width, Height は、スクロールが始まって、指を置いているとスクロールが継続するエリアの幅です。お好みで調整してみてください。この値を 0 にすると Edge motion は動作しません。
    * SynTP_Receiver.exe の場合は、ショートカットを作成し、プロパティを開いて「[Your folder path]\SynTP_Receiver.exe /Port 50008」 のように UDP Portを指定します
      * 管理者権限で起動しない場合、タスクマネージャーなど一部のアプリでスクロールしません。ショートカットのプロパティの詳細設定で「管理者権限で実行する」にチェックをするか、タスクスケジューラに、例えば「任意のユーザーのログオン時」に「最上位の特権で実行する」ように登録する方法もあります
      * SynTP_Receiverを終了するためには、タスクマネージャーからタスクの終了を行います
   
  * その他 (制限事項、今後など)
    * ネットワークを介在してマウスの動きを送受信していますので、ネットワークがビジーの時など少々動きがにぶくなります。これは制限事項となります
    * いろいろと調べると、他社製はもちろん、Synaptics社の Touch Padでさえも、仕様がいろいろとあるようです。このため、Panasonic社製 Let's note CF-SV1に限定しています (特にプログラム内で制限はしていませんので、動いたらラッキーと思ってください。実機を貸していただければ、対応できるかもしれませんが)
    * スクロールの量や方向などは、私の好みにしています。設定できるようにするのは、今後の課題です
    * Guest OS で、UNIX系については、私は詳しくありませんので、どなたか受信側を書いていただければ。。。
    * VMware社が Broadcom社に買収され、先日、Player をやめて Workstation Proに移行するとのアナウンスがありました。商用利用ではない場合は、無償とのことです。~~今後、開発環境を整えて、移行を考えたいと思います~~　Ver.2.2.3.1で両方とも対応しました

    
#### プログラム構成
  * Ver.3 以降のプログラム構成を説明します
    * デスクトップ版 (FlushMouse_x64.exe)
      * FlushMouse.exe - デスクトップ版のプログラム本体 (Win32 API)
      * FlushMouseDLL.dll - フックなど (Win32 API)
      * FlushMouse32.exe - プロセスの監視など (Win32 API)
      * FlushMouseDLL32.dll - フックなど (Win32 API)
      * FlushMouseSettings.exe - 設定画面のメインプログラム (C++/WinUI3)
      * FlushMouseUI3DLL.dll - 設定画面のプログラム (C#/WinUI3 .NET Framework)
      * FlushMouseCursor.dat - 直接描画ではないときのマウスカーソルリソース (原本は%ProgramData%\JeffyTS\FlushMouseに配置、起動後は%TMP%にコピーの上リソースを作成します)
    * WinUI3版 (FlushMouseUI3_x64.exe - Ver.3 以降リリースはしていませんがビルドはできます)
      * FlushMouseUI3.exe - UI3版のメインプログラム (C++/WinUI3)
      * FlushMouseDLL.dll - フックなど (Win32 API)
      * FlushMouse32.exe - プロセスの監視など (Win32 API)
      * FlushMouseDLL32.dll - フックなど (Win32 API)
      * FlushMouseUI3DLL.dll - 設定画面のプログラム (C#/WinUI3 .NET Framework)
      * FlushMouseCursor.dat - 直接描画ではないときのマウスカーソルリソース (原本は%ProgramData%\JeffyTS\FlushMouseに配置、起動後は%TMP%にコピーの上リソースを作成します)


#### ビルド方法

* まず、Visual Studio 2026 の開発環境が必要になりますので、二つの方法を紹介します

  (A). ご自分のPCに直接Visual Studioを入れる場合は、このリンク [https://visualstudio.microsoft.com/ja/downloads/](https://visualstudio.microsoft.com/ja/downloads/) から入手できます(Communityで十分だと思います)
  
  (B). 仮想マシンで開発環境を構築したい方は、このリンク [https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/](https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/) から入手できます
  

* 以下では、(B)の方法を紹介します(私はVMwareで作業していますので、その手順を紹介しますが、他の仮想マシンでも同じような手順だと思います)。なお、ライセンス等については、よく読んでご判断ください

* (B) の Windows 11 開発環境を取得するから、仮想マシンをダウンロードしてインストールしたのち、日本語化するために最低設定する項目です(他にも設定する項目はありますが割愛します)

  1. Settings -> Time & language -> Language & regionで、日本語を追加します
  2. Japaneseを上に移動します
  3. 右側の「…」をクリックして、Option -> Keyboards -> Change layout -> Japanese keyboard(106/109 key)を選択し、Restart nowで再起動します
  4. Settings -> Time & language -> Date & Time -> Time zone -> UTC+09:00) Osaka, Sapporo, Tokyoを設定します
  5. Settings -> Time & language -> Language & region -> Windows display language -> 日本語 を選んで、 Sign out、Sign inします
  6. 設定 -> 時刻と言語 -> 言語と地域 -> 管理用言語の設定で表示されるダイアログで、システムロケールの変更を選んで 日本語(日本)を選択します
  7. ここで、Windows Updateをしておいたほうが良いと思います

* 次に、Visual Studioの設定を行います

  1. まず、Visual Studio **Installer** (2026 本体ではなく**Installer**です。Windowsキーを押して「vis」と入力すると候補に出てきます)を起動します
  2. 変更を選んで、 「C++ によるデスクトップ開発」にチェックを入れます。次に「言語パック」タブで日本語にチェックを入れます。その後、右下の変更ボタンで、インストールを行います
  3. 次に、WiXを使用するために、Windowsの設定 -> アプリ -> オプション機能 -> Windowsのその他の機能 -> Windowsの機能の有効化または無効化 -> .NET Framework 3.5.1(.NET 2.0及び3.0を含む)にチェックを入れてOKを押して、インストール、再起動します
  4. [https://GitHub.com/wixtoolset/wix3](https://GitHub.com/wixtoolset/wix3) から、Wix311.exeをダウンロードしてインストールします
  5. Visual Studioを起動して 右下の「コードなし」で起動します
  6. Tools -> Options -> Environment -> International Settings をクリックして、Languageを日本語にします
  7. 拡張機能 -> 検索で　WiXと入力し「WiX v3 - Visual Studio 2022 Extension」を選択して、Visual Studio を終了するとインストールが始まります
  8. .NET Framework SDK 10 を[https://dotnet.microsoft.com/ja-jp/download/dotnet/thank-you/sdk-10.0.100-windows-x64-installer](https://dotnet.microsoft.com/ja-jp/download/dotnet/thank-you/sdk-10.0.100-windows-x64-installer)からダウンロードしてインストールします
  9. Visual Studioを再起動して、リポジトリのクローン -> リポジトリの場所に [https://GitHub.com/JeffyTS-JP/FlushMouse.git](https://GitHub.com/JeffyTS-JP/FlushMouse.git)　を入力してクローンします
  10. NuGet CLI(nuget.exe)を[https://dist.nuget.org/win-x86-commandline/latest/nuget.exe](https://dist.nuget.org/win-x86-commandline/latest/nuget.exe)からダウンロードして、FlushMouseをクローンしたフォルダの直下の「NuGet」フォルダ(デフォルトでは C:\Users\\[User]\source\repos\FlushMouse\NuGet です)に入れます
  11. Windowsの 設定 -> システム -> 開発者向け で 開発者モードを ON にします
  12. 次に、ツール -> NuGet パッケージマネージャー -> ソリューションの NuGet パッケージの管理 (Altキーを押しながら続けて T N N) を開き、右側にある歯車(⚙)を選択して、ダイアログを開きます
  13. まず、右上のプラス(＋)を押してから、下にある「ソース」欄には、nuget.exeを入れた「NuGet」フォルダ(デフォルトでは C:\Users\\[User]\source\repos\FlushMouse\NuGet)を選択して「更新」を押します(名前については適当で構いません)
  14. その後 OK を押してダイアログを閉じ、上の歯車の左にある「パッケージソース」を「すべて」に変更します。次に「復元」を選択します

* ビルド方法です
  1. ソリューションプラットフォームを必ず「**MixedPlatform**」にしてビルドします (Ctrl + Shift + B)
  2. ソリューション構成を「Release」でビルドすると、デフォルトでは C:\Users\\[User]\source\repos\FlushMouseにFlushMouse_x64.exe (Ver.1.2 以前は FlushMouse_x64.msi)と、FlushMouseUI3_x64.exeができるのでインストールしてご利用ください

