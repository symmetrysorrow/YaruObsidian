# 概要　
- 小説とやる夫の自動更新、更新通知アプリです。小説家になろう、ハーメルン、ノクターンノベル、一部のやる夫まとめサイトに対応しています。マークダウンファイルに保存するため、Obsidianなどのマークダウンリーダーで読むことになるかと思います。起動しておけば自動で一時間ごとに更新します。
- サーバーへの負荷軽減、およびbotであることがバレるのを避けるため一話更新するごとにランダムな1~3秒の待機時間を設定しているため更新は多少時間がかかります。

# 必要なもの
## 必須
- WindowsPC
## 任意
- 更新通知受信用のメールアドレス 
    - 更新があったときの通知や小説が削除されたときの警告などを受信するメールアドレスです。普段使いのもので問題ありません。
- 小説追加用のメールアドレス
    - 更新したい小説をメールに送信することで小説を追加することができるようになります。
- クラウドドライブ 
    - 複数PCで同期したい方、スマホで読みたい方向けです。500話の小説で10Mb程度なので、よほどのことがない限り無料分で十分でしょう。
- FolderSync 
    - クラウドドライブとスマホのフォルダを同期するアプリ。同期できるアプリは他にもあるかもしれませんが、私はこれを使ってます。
- Obsidian 
    - マークダウン(このアプリが保存する形式)を読むアプリです。小説のみであれば他のアプリでも問題ありませんが、やる夫はCSSスニペットを利用して表示しているのでやる夫も見る方はObsidianをお勧めします。

# インストール・設定
- このアプリをPCの適当な場所で解凍する。

- 更新・保存したい小説のIDを控えます。ここでIDとは各小説にアクセスしたときにURLの末尾に出る英数字のコードのことです。やる夫の場合は、[やる夫RSS+インデックス](https://rss.r401.net/)にアクセスし、目的の連載にアクセスするとURLの末尾に数字が出るので、これを控えてください。

- BookshelvesフォルダにNarou.csv、Hameln.csv、Noc,csv、Yaruo,csvがあります。なろうの小説であればNarou.csvに、ハーメルンであればHameln.csvに、ノクターンノベルはNoc.csvに、やる夫はYaruo.csvに対応しています。ここにID,1,1と入力してください。1つの小説につき一行ですので、適宜改行してください。以下はサンプルです。
```
238287,1,1
273914,1,1
243534,1,1
```

- YaruObsidian.exeを実行することで各フォルダに小説が保存されます。

- [Obsidian](https://obsidian.md/)をインストールし、保管庫として開くからアプリフォルダにあるNovel、Yaruoを開いてください。これで小説ややる夫がObsidianで読めるようになります。

- メールでの操作をしたい方はPython.zipをダウンロードし、解凍してください。

- メールで小説を追加したい場合はMailFetcher.pyというファイルがあるのでそれをYaruObsidian.exeと同じフォルダに移動してメモ帳で開いてください。6、7行目にメールを入力する場所とパスワードを入力する場所があります。""で区切られたそこにアプリパスワードを入力してください。どうしてもパスワードをベタ打ちしたくない人は環境変数を使うようにPythonファイルを書き換えてもいいでしょう。アプリパスワードの入力は使い方を誤れば情報漏洩につながる行為ですので、自己責任の下で行ってください。

- 更新通知を受け取りたい方はSendMail.pyというファイルをYaruObsidian.exeと同じフォルダに移動して開き、受信用のメールアドレスと送信用のメールアドレス、および送信用アドレスのパスワードを入力してください。

- スタートアップに登録することでPC起動時に自動で更新するようになります。具体的な設定方法は省きます。

# 使い方
## メールを使った小説の追加
- 自動更新の対象にしたい小説をスマホのブラウザで開いた状態で共有からメールアプリを開いて、Mailfetcher.pyに入力したメールアドレスにそのまま送信します。本文の一行目が小説のURLになっていれば問題ないです。

## その他の使い方

- アプリの解答場所をPCにインストールしたクラウドドライブにして、FolderSync等を利用してクラウドフォルダとスマホのローカルフォルダを紐づけ、そのフォルダをスマホのObsidianで開くことでスマホも自動更新されるようになります。(正確にはFolderSyncで自動更新を設定しなければなりません。)

# 注意点
- このアプリはフリーソフトであり、その利用による被害について作成者は一切の責任を負いません。
- このコードそのものはCC0ですので自由に使っていただいて結構ですが、スクレイピングはコード中の数値を少し変えるだけで犯罪となりえる行為です。また著作物の保存は個人の範疇でのみ許可されるものであり、これらを配布することは禁止されています。十分に注意して利用してください。
- 更新中にアプリを閉じると追加された小説の一部のデータが消えます。
- ハーメルンの短編かつ1話しかない小説には対応していません。数も少ないのでいいかなという判断です。短編でも2話以上ある場合には問題なく更新します。
- やる夫スレは以下のサイトのみに対応しています。それ以外ではエラーが出ることが予想されます。
    - [やる夫まとめくす](https://rss.r401.net/yaruo/sites/185)
    - [やる夫スレ本棚](https://rss.r401.net/yaruo/sites/184)
    - [やる夫の暇つぶし麻亜屈](https://rss.r401.net/yaruo/sites/57)
    - [暇な時にやる夫まとめ](https://rss.r401.net/yaruo/sites/139)
    - [Yaruyomi](https://rss.r401.net/yaruo/sites/3)
    - [泳ぐやる夫シアター](https://rss.r401.net/yaruo/sites/14)
    - [100%自分用やる夫まとめ](100%自分用やる夫まとめ)
    - [面白かったやる夫スレをまとめるサイト](https://rss.r401.net/yaruo/sites/122)
