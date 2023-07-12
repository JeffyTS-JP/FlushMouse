
# FlushMouse　![Icon](https://osdn.net/users/jeffyts/pf/FlushMouse/wiki/FrontPage/thumb/48x48/FlushMouse.png)

**タスクトレイに常駐して、マウスカーソルへIMEモードの表示をするユーティリティです**

**FlushMouse is a software developed by JeffyTS. It is a mouse gesture tool that allows you to perform various operations by drawing gestures with your mouse. (by Bing)**




使い方については [OSDN/FlushMouse](https://osdn.net/users/jeffyts/pf/FlushMouse/wiki/FrontPage "https://osdn.net/users/JeffyTS/pf/FlushMouse/wiki/FrontPage)") を参照してください (どちらにも同じソースとバイナリが置いてあります)

![Cursors](https://osdn.net/users/jeffyts/pf/FlushMouse/wiki/FrontPage/thumb/230x181/Cursors.png)

![SettingDialog](https://osdn.net/users/jeffyts/pf/FlushMouse/wiki/FrontPage/thumb/572x645/SettingDlg.png)



**ビルド方法**

* まず、Visual Studio 2022 の開発環境が必要になりますので、二つの方法を紹介します
  A. ご自分のPCに直接Visual Studioを入れる場合は、
  このリンク [https://visualstudio.microsoft.com/ja/downloads/](https://visualstudio.microsoft.com/ja/downloads/) から入手してください(Communityで十分だと思います)
  
  B. 仮想環境で開発環境を構築したい方は、このリンク [https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/](https://developer.microsoft.com/ja-jp/windows/downloads/virtual-machines/) から入手してください
  

* 以下では、Bの方法を紹介します(私はVMwareで作業していますので、その方法を紹介しますが、他の仮想マシンでも同じような手順だと思います)。なお、ライセンス等については、よく読んでご判断ください

* BのWindows 11開発環境を取得する　から、仮想マシンをダウンロードしてインストールしたのち、日本語化するために最低設定する項目です(他にも設定する項目はありますが割愛します)

  1. Settings -> Time & language -> Language & regionで、日本語を追加します
  2. Japaneseを上に移動します
  3. 右側の「…」をクリックして、Option -> Keyboards -> Chage layout -> Japanese keyboard(106/109 key)を選択し、Restart nowで再起動します
  4. Settings -> Time & language -> Date & Time -> Time zone -> UTC+09:00) Osaka, Sapporo, Tokyoを設定します
  5. Settings -> Time & language -> Language & region -> Windows display language -> 日本語 を選んで、 Sign out、Sign inします
  6. 設定 -> 時刻と言語 -> 言語と地域 -> 管理用言語の設定で表示されるダイアログで、システムロケールの変更を選んで 日本語(日本)を選択します
  7. ここで、Windows Updateをしておいたほうが良いと思います

* 次に、Visual Studioの設定を行います

  1. まず、Visual Studio Installer (必ずInstallerを選んでください。Windowsキーを押して「vis」と入力すると出てきます)を起動します
  2. 上の変更タブを選んで、 C++ によるデスクトップ開発にチェックを入れます。次に言語パックタブで日本語にチェックを入れます。その後、右下の変更ボタンで、インストールを行います
  3. 次に、WiXを使用するために、設定 -> アプリ -> オプション機能 -> Windowsのその他の機能 -> Windowsの機能の有効化または無効化 -> .NET Framework 3.5.1(.NET 2.0及び3.0を含む)にチェックを入れてOKを押して再起動します
  4. [https://github.com/wixtoolset/wix3](https://github.com/wixtoolset/wix3) から、Wix311.exeをダウンロードしてインストールします
  5. Visual Studioを起動して 右下の「コードなし」で起動します
  6. 拡張機能 -> 検索で　WiXと入力し「 WiX v3 - Visual Studio 2022 Extension」を選択して、再起動するとインストールが始まります

* ビルド方法です
  1. Visual Studioを起動して、リポジトリのクローン -> リポジトリの場所に [https://github.com/JeffyTS-JP/FlushMouse.git](https://github.com/JeffyTS-JP/FlushMouse.git)　を入力してクローンします
  2. クローンが終わったら構成を MixedPlatform にしてビルドします
  3. Releaseでビルドすると、デフォルトでは C:\Users\User\source\repos\FlushMouseにFlushMouse_x64.msiができるのでインストールしてください


