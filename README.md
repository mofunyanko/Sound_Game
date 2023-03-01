# 音楽ゲーム
## 作成した経緯
夢考房の組込みソフトウェアプロジェクトで、C/C++の勉強の最終課題として、Visual Studioでゲームを１つ作成する課題が課されたため、当時はまっていた音ゲー制作にチャレンジした
（2020年8月）

## ゲーム概要
フリー音源から譜面を自作し、その譜面のリズムに合わせてノーツが打てる音楽ゲーム（音ゲー）

## ゲームのルール
- 曲は1曲（ファンタジー13）のみ
- 押すキーは、第1レーンはX、第2レーンはC、第3レーンはN、第4レーンはM
- 曲に合わせてノーツが流れてくるので、レーン付近に来たときに、上記のキーを押す
- タイミングが上手ければノーツは消え、失敗するとそのまま流れていく

## 実行結果
![image](https://user-images.githubusercontent.com/95011869/222024426-c3274fbf-4fd4-4d38-bf34-e2d0b7949fd4.png)

### 成功
![image](https://user-images.githubusercontent.com/95011869/222024547-2af5b932-a242-4237-9d79-4d2b840969b4.png)

### 失敗
![image](https://user-images.githubusercontent.com/95011869/222024585-8a3f4c11-1215-4a4a-bcb0-f1c730bd48ab.png)

## 工夫点
- 最初に作ったコードは、関数が多く、かなり見にくいものになってしまったので構造体やコメントを使って、他の人が見ても何の関数か分かるようにした
- 判定やノーツの遅れが生じるのを防ぐために、すべてゲームの稼働時間に依存したコードを作成した
- 最初は、簡単なリズムに、途中から少し難しいリズムにすることで、初心者でも楽しめるような譜面を作成した
- txtファイルに譜面データを作成することで、人間でも読めるようにした
例）1111は、1小節に四分音符が4つを意味する

## 感想・今後の展望
- 譜面を作る際、自分が想定していた通りのリズムになかなかならず、苦労した
- 複数の曲を作りたかったが、とても時間がかかるので、１曲の譜面を作るのに精一杯だった
・コメントを残しておかないと、プログラムを見直した際、何を意味するプログラムなのか分らなくなってしまう。また、関数や使用データが増えると読みづらいコードになる
→コメント、構造体の重要性を感じた
- 次に作製する機会があれば、タイトル画面や曲選択画面、失敗したら即終了などの機能を付けてみたい

### 参考文献
#### Webサイト
-[『C言語とDxLibで音ゲーのような何かをつくってみる』](https://iconcreator.hatenablog.com/entry/2018/12/27/235450)
#### 音源
-[『魔王魂』](https://maoudamashii.jokersounds.com/)
曲名:「ファンタジー13」
