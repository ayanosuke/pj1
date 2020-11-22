= 信号機を作る

本誌で取り扱う信号機は閉塞信号機と出発信号機をご紹介します。閉鎖信号機はぬっきーさんと同じ信号機アクセサリデコーダタイプとTOMIXの信号機のように
線路に設置した位置検出センサで在線を検出して信号機を点灯させるタイプになります。出発信号機はアクセサリデコーダですが線路に設置した位置検出センサと
連動して動きます。

== 信号機機器の紹介

信号機を作るに為にHOサイズのスケールですが@<img>{signal3}の様に、３現示、4現示警戒、４現示減速の3種類の信号機を作りました。

//image[signal3][３現示、4現示警戒、４現示減速信号機][scale=1]

信号機台は@<img>{signal7}の様に信号機をコネクタに差し込みその上にHOレールを置いて設置します。

//image[signal7][信号機台][scale=1]

在線検出センサは@<img>{signal7}の様にCDSセンサやフォトリフレクタセンサ、リードスイッチが使用できます。またこのセンサボードは Nゲージ、HOゲージに対応しています。

//image[signal8][在線検出センサ][scale=1]


== 閉塞信号機（在線検出タイプ）
アナログでもDCCでも使用できる閉塞信号機（閉そく信号機：へいそくしんごうき）です。実際の閉塞信号機とは違って車両のATSを働かせて車両を停止させることはできません。
在線検出センサで車両を検出したら、決まったパターンで信号機のライトの点灯を変化させます。

//image[heisoku1][閉塞信号機][scale=1]

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/BlockSignalling,[ BlockSignallingスケッチ ]}

https://github.com/DccShield/BlockSignalling

=== スケッチの構成
以下のソースファイルが @<tt>{BlockSignalling}のスケッチに含まれています。

@<tt>{cds.cpp　:　Cds}センサライブラリ

@<tt>{cds.h  :　Cds}センサライブラリ用ヘッダファイル

@<tt>{3ColorLightSignal.cpp : }３現示信号機ライブラリ

@<tt>{3ColorLightSignal.h : }３現示信号機ライブラリ用ヘッダファイル

@<tt>{FourDeceleration.cpp : }４現示減速・抑速信号機ライブラリ

@<tt>{FourDeceleration.h : }４現示減速・抑速信号機ライブラリ用ヘッダファイル

@<tt>{FourVigillance.cpp : }４現示警戒信号機ライブラリ

@<tt>{FourVigillance.h : }４現示警戒信号機ライブラリ用ヘッダファイル

@<tt>{BlockSignalling.ino :　}閉塞信号機スケッチ


=== BlockSignalling.ino に含まれている関数





== 閉塞信号機（アクセサリデコーダタイプ）


== 出発信号機（アクセサリデコーダタイプ）
