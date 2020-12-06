= スマイルライター

== スマイルデコーダにスケッチを書き込む

@<tt>{Arduino Nano DCC Shield}の書き換えは@<tt>{USB}でパソコンと接続するとスケッチの書き換えができますが、スマイルデコーダシリーズは専用のコネクタ
で書き換える様になっています。
@<tt>{DCC}電子工作連合では、@<tt>{Smile Writer / }スマイルライター が数種類用意されています。

=== 書き込み機の紹介

@<tt>{9910}スマイルライター コンボ-@<tt>{A}キット@<tt>{（TYPE-A}専用

@<tt>{https://nagoden.cart.fc2.com/ca4/15/p-r-s/}
//image[writer3][現行販売品][scale=0.5]

//embed[latex]{
\clearpage
//}


@<tt>{Arduino Nano}にコネクタを取り付けるコンパクトタイプ@<tt>{[DIY]}

書き込み機@<tt>{(}スマイルライタ@<tt>{ Type-A)}

@<tt>{https://desktopstation.net/wiki/doku.php/ds_smile_decoder_r4}
//image[writer2][Arduino NANO改造タイプ][scale=0.5]

@<tt>{isolation Smile writer[}非売品@<tt>{]}

//image[writer4][isolation Smile writer][scale=0.5]
ヘビーユーザー用のライターです。

・デコーダの板厚さ2種類に対応。

・絶縁用フォトカプラーを使用してデコーダと電気的絶縁をしています。

・絶縁用@<tt>{USB} シリアル変換基板を搭載。デコーダと電気的に絶縁して通信できます。

・購入したい方は要ご相談

=== デコーダをスマイルライターに取り付けます

スマイルデコーダシリーズは板厚さが@<tt>{1.6mm}のタイプと@<tt>{0.8mm}タイプの2種類があります。スマイルライターコンボAキットは
両方に対応しています。

//image[writer5][ノーマル板厚タイプ][scale=0.5]

//image[writer6][スリム板厚タイプ][scale=0.5]


=== 書き込み操作（ATmega328タイプ）

@<tt>{ATmega328}タイプ@<tt>{(SmileDecoderN18,MP3}サウンドデコーダ@<tt>{V6N18, MP3}サウンドデコーダ@<tt>{V6 SE（SlimEdition)）}が
該当します。

設定は以下の図の様に設定します。
//image[writer7][ツール設定][scale=0.5]

スケッチ→書き込み装置を使って書き込むを洗濯してスケッチをデコーダに書き込みます。
//image[writer8][スケッチ書き込み][scale=0.5]

スマイルライターのスケッチを書き換えるボタンですので赤矢印の書き込みボタンは押さないでください。
//image[writer9a][押してはダメ][scale=0.5]

※詳細は各デコーダの書き込み説明書を参照してください。


=== 書き込み操作（ATtiny85タイプ）

@<tt>{ATtiny85}タイプ@<tt>{(DCC}館で配布しているデコーダ@<tt>{)}が該当します。

設定は以下の図の様に設定します。
//image[writer10][Attiny85の設定][scale=0.5]

スケッチ→書き込み装置を使って書き込むを洗濯してスケッチをデコーダに書き込みます。
//image[writer8][スケッチ書き込み][scale=0.5]

※詳細は各デコーダの書き込み説明書を参照してください。
