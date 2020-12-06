= 信号機を作る

本誌で取り扱う信号機は閉塞信号機と出発信号機をご紹介します。閉鎖信号機はぬっきーさんと同じ信号機アクセサリデコーダタイプと@<tt>{TOMIX}の信号機のように
線路に設置した位置検出センサで在線を検出して信号機を点灯させるタイプになります。出発信号機はアクセサリデコーダですが線路に設置した位置検出センサと
連動して動きます。

== 信号機機器の紹介

信号機を作るに為に@<tt>{HO}サイズのスケールですが@<img>{signal3}の様に、３現示、4現示警戒、４現示減速の3種類の信号機を作りました。

//image[signal3][３現示、4現示警戒、４現示減速信号機][scale=0.4]

信号機台は@<img>{signal7}の様に信号機をコネクタに差し込みその上に@<tt>{HO}レールを置いて設置します。

//image[signal7][信号機台][scale=1]

在線検出センサは@<img>{signal7}の様に@<tt>{CDS}センサやフォトリフレクタセンサ、リードスイッチが使用できます。またこのセンサボードは @<tt>{N}ゲージ、@<tt>{HO}ゲージに対応しています。

//image[signal8][在線検出センサ][scale=1]


== 閉塞信号機（在線検出タイプ）
アナログでも@<tt>{DCC}でも使用できる閉塞信号機（閉そく信号機：へいそくしんごうき）です。実際の閉塞信号機とは違って車両のATSを働かせて車両を停止させることはできません。
在線検出センサで車両を検出したら、決まったパターンで信号機のライトの点灯を変化させます。
@<tt>{Arduino Nano DCC Shield 1}台で、信号機@<tt>{２}台、在線検出センサ@<tt>{２}台を制御します。

//image[signal17][閉鎖信号機システム][scale=1]

信号機を通過すると赤信号が点灯し、しばらくすると黄信号→青信号に変わります。

//image[heisoku1][閉塞信号機][scale=1]

=== 接続図
在線検出回路の接続

@<tt>{AYA056-2 CdsPrSensor} 在線検出基板で、@<tt>{Cds}、フォトリフレクタ、リードスイッチが使用できます。

@<tt>{AYA057-1 4con x 2 Board 4}極コネクタをピンヘッダに変換する基板です。

@<tt>{AYA046-2 Arduino Nano DCC Shield}です。

各基板の接続は4極ジャックけーブルを使用しています。

//image[signal9][在線検出センサ回路図][scale=1]

//image[signal11][在線検出センサ実態図][scale=1]


//blankline

=== 信号機回路の接続

@<tt>{AYA052-1　4}現示警戒信号機

@<tt>{AYA053-1　4}現示減速信号機

@<tt>{AYA057-1 4con x 2 Board 4}極コネクタをピンヘッダに変換する基板です。

@<tt>{AYA046-2 Arduino Nano DCC Shield}です。

※在線検出回路で使用する@<tt>{Arduino　Nano DCC Shield}と共存できます。

//image[signal10][信号機回路図][scale=0.8]

//image[signal12][信号機実態図][scale=0.8]


=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/BlockSignalling,[ BlockSignallingスケッチ ]}

@<tt>{https://github.com/DccShield/BlockSignalling}

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


=== ヘッダ部の実装

//emlistnum{
#include <arduino.h>
#include "Cds.h"
//#include "3ColorLightSignal.h"
#include "FourDeceleration.h"
//#include "FourVigillance.h"

unsigned long gPreviousL5 = 0;
char DetectionLevel = LOW;

//各種設定、宣言
//ThirdColorLightSignal FirstSignal(9,10,11,12);
//ThirdColorLightSignal SecondSignal(5,6,7,8);
FourDeceleration FirstSignal(9,10,11,12);
FourDeceleration SecondSignal(5,6,7,8);
Cds FirstSensor(A0, 2, 200,300);
Cds SecondSensor(A1, 2, 200,300);
//}

1行目@<tt>{ Arduino固有}のヘッダファイル

2行目@<tt>{ Cds}センサライブラリヘッダファイル

3行目 コメントアウトになっていますが、なんちゃって英語で3現示信号機ライブラリヘッダファイル

4行目 なんちゃって英語ですが@<tt>{4}現示減速信号機ライブラリヘッダファイル

5行目 コメントアウトになっていますが、なんちゃって英語ですが4現示警戒信号機ライブラリヘッダファイル

よって、今回の閉鎖信号機は@<tt>{4}現示減速信号機を使用して点灯制御させます。

7行目@<tt>{ Arduino}の経過時間格納用変数

8行目@<tt>{ Cds}受信感度設定用変数

11行目 コメントアウトになっていますが、3現示信号機クラスから1台目の信号機を実体化しています。

12行目 コメントアウトになっていますが、3現示信号機クラスから2台目の信号機を実体化しています。

13行目 4現示減速信号機クラスから1台目の信号機を実体化させています。

14行目 4現示減速信号機クラスから2台目の信号機を実体化させています。

15行目@<tt>{ Cds}クラスから1台目の在線検出部を実態化させています。

16行目@<tt>{ Cds}クラスから2台目の在線検出部を実体化させています。

=== loop()の実装
//emlistnum{
void loop() {
  if((millis() - gPreviousL5) >= 100){
    gPreviousL5 = millis();
    BlockingSignalState();
    FirstSignal.statechk();
    SecondSignal.statechk();
  }
}
//}

4行目 在線検出をチェックするステートマシン処理

5行目 1台目の信号機のステートマシン処理、時間都ともに赤→黄→青の点灯シーケンスを制御

6行目 2台目の信号機のステートマシン処理、時間都ともに赤→黄→青の点灯シーケンスを制御

//embed[latex]{
\clearpage
//}

=== BlockingSignalState()の実装
大まかな動きとしては、起動時に1台目、2台目信号機のステートマシーンに青信号表示を指定。
以降は@<tt>{Cds}センサの在線検出をチェックして、信号機のステートマシーンに赤信号表示を指定しています。

//emlistnum{
void BlockingSignalState( void ){
  enum{
      STS_INIT = 0,
      STS_IDLE,
  };
  static int state = STS_INIT;

  switch(state){
    case STS_INIT:
                  FirstSignal.ChangeState(ST_ADVANCE);
                  SecondSignal.ChangeState(ST_ADVANCE);
                  state = STS_IDLE;
                  break;
    case STS_IDLE:
                  if(FirstSensor.statechk(DetectionLevel) == 1){
                    FirstSensor.;
                    FirstSignal.ChangeState(ST_STOPDET);
                  }
                  if(SecondSensor.statechk(DetectionLevel) == 1){
                    SecondSensor.;
                    SecondSignal.ChangeState(ST_STOPDET);
                  }
                  break;
    default:
                  break;
  }
}
//}
2行目〜　ステートマシン用のステート値を宣言

9行目〜　起動時に一度だけ処理されるステートで、1台目、２台目の信号機を青表示に設定

15行目〜　1台目の@<tt>{Cds}センサが検知してら、１台目の信号機を赤信号に設定。

19行目〜　2台目の@<tt>{Cds}センサが検知してら、2台目の信号機を赤信号に設定。

=== Cds.cpp に含まれている関数

以下の関数が、@<tt>{Cds.cpp} ファイルに入っています。

在線検出は、@<tt>{Cds}クラスとして定義しています。

@<tt>{Cds():Cds}センサのコンストラクタ

@<tt>{:Cds}ステートマシンのステートを@<tt>{ST_INIT}に遷移させます

@<tt>{statechk()}ステートマシンを動かします

=== Cds()の実装

@<tt>{Cds}クラスのコンストラクタです。

//emlistnum{
Cds::Cds(char ch , char ct, int LTh, int HTh){
  port = ch;
  cntup = ct;
  LThreshold = LTh;
  HThreshold = HTh;
}
//}

@<tt>{Cds}センサの入力ポートの設定と暗・明の閾値（スレッショルド）の設定を行なっております。

=== Reset()の実装
@<tt>{Cds}クラスは在線を検出後在線状態を保持（ラッチ）した状態になります。
@<tt>{BlockingSignalState()}内のステートマシンで在線を検出した後の処理で@<tt>{Reset()}を実行して@<tt>{Cds}ステートマシンを
リセットしています。

//emlistnum{
void Cds::Reset( void ){
  state = ST_INIT;
}
//}

=== statechk()の実装
//emlistnum{
char Cds::statechk( char range ){
  switch(state){
    case ST_INIT:
              state = ST_MEAS;
              cnt = 0;
              break;
    case ST_MEAS:
              if( cnt > cntup ){
                state = ST_DETECTION;
                return HIGH;
              }
              Ain = analogRead( port );
              if(range == LOW){
                if(Ain <=  LThreshold){
                  cnt++;
                } else
                  cnt = 0;
              }
              if(range == HIGH){
                if(Ain <=  HThreshold){
                  cnt++;
                } else
                  cnt = 0;
              }
              break;
    case ST_DETECTION:
              return HIGH;
              break;
    default:
              break;
  }
  return LOW;
}
//}

3行目〜 初期化のステートです。@<tt>{cnt}フラグをリセットして@<tt>{ST_MEAS}に遷移します。

7行目〜 計測のステートです。@<tt>{A/D}の閾値を超えて@<tt>{cntup}を超えたら在線を検知します。

12行目@<tt>{ analogRead()}で@<tt>{Cds}のアナログ値を読み込んでいます。

13行目〜 @<tt>{A/D}検出レベルが@<tt>{LOW}レンジの時の閾値超えのカウントアップを行います。

19行目〜 @<tt>{A/D}検出レベルが@<tt>{HIGH}レンジの時の閾値超えのカウントアップを行います。

26行目〜 在線検知ステートです。@<tt>{HIGH}を戻り値として買えします。

本スケッチは@<tt>{Cds}センサを使用していますが、フォトリフレクタクラス、リードスイッチクラス、@<tt>{tof}センサクラスに
差し替えることで多種の在線検出に対応可能です。

//embed[latex]{
\clearpage
//}

=== FourDeceleration.cpp に含まれている関数
以下の関数が、@<tt>{FourDeceleration.cpp} ファイルに入っています。

4現示抑速信号機は、@<tt>{FourDeceleration}クラスとして定義しています。

@<tt>{FourDeceleration():4}現示抑速信号機のコンストラクタ（初期化）

@<tt>{Detection():}ステートマシンに強制的に停止状態を設定する

@<tt>{nowState():}ステートマシンのステート番号を取得

@<tt>{ChangeState():}ステートマシンのステート番号を強制的に変更

@<tt>{statechk():}ステートマシンを動かします

@<tt>{advance():}進行状態

@<tt>{Deceleration():}減速状態

@<tt>{caution():}注意状態

@<tt>{caution_stops():}注意状態

@<tt>{stops():} 停止状態

@<tt>{blank():}　消灯状態


=== FourDeceleration()の実装

4現示抑速クラスの@<tt>{FourDeceleration}クラスのコンストラクタです。

@<tt>{LED1〜LED4}に@<tt>{Arduino}のポート番号を設定しています

//emlistnum{
FourDeceleration::FourDeceleration(char L1, char L2, char L3, char L4)
{
  LED1 = L1;
  LED2 = L2;
  LED3 = L3;
  LED4 = L4;

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}
//}

1行目 @<tt>{FourDeceleration}コンストラクタの引数に@<tt>{L1〜L4}があり、クラスを実態かするスケッチ
@<tt>{FourDeceleration FirstSignal(9,10,11,12);}により、@<tt>{Arduino}のポート番号が設定されます。

3行目〜 @<tt>{LED1〜LED4}に、ポート番号が代入されます。

8行目〜 @<tt>{pinMode()}でポートを初期化します。

//embed[latex]{
\clearpage
//}

=== Detection()の実装

在線を検出したというメンバ関数で強制的に在線検出した状態を通知します。

//emlistnum{
void FourDeceleration::Detection(void)
{
  detf = HIGH;
}
//}

=== nowState()の実装

ステートマシン内のステートをコールバックするメンバ関数です。

※本スケッチで使用していません。

//emlistnum{
int FourDeceleration::nowState()
{
  return state;
}
//}

=== ChangeState()の実装

引数を通して、ステートマシン内のステートを強制遷移させます。

//emlistnum{
void FourDeceleration::ChangeState( int st )
{
  state = st;
}
//}

=== statechk()の実装

信号機のステートマシンの本体です。

信号機が変わるときは両方点灯した状態を作り電球の残像を再現させています。
起動時は @<tt>{ST_INIT→ST_IDLE}で待機、@<tt>{loop()}内の初回時に@<tt>{ST_ADVANCE→ST_IDLE}で待機します。
以降は在線検出センサを通過すると@<tt>{ST_STOPDET}に遷移し時間と共に信号機の状態を変化させます。

//image[signal13b][ステートマシン][scale=1]

//emlistnum{
void FourDeceleration::statechk( void )
{
  switch(state){
      case ST_INIT:
              stops();
              state = ST_IDLE;
              break;

      case ST_IDLE:
              break;

      case ST_STOP: // 停止
              blank();
              state = ST_STOP2;
              break;

      case ST_STOP2:
              stops();
              state = ST_IDLE;
              break;

      case ST_STOPDET:
              stops();
              cnt = 0;
              state = ST_STOPWAIT;
              break;

      case ST_STOPWAIT:
              cnt++;
              if( cnt > STOPSTIM ) {
                cnt = 0;
//              caution();
//              state = ST_CAUTIONWAIT;
                state = ST_CAUTION;
              }
              break;

      case ST_CAUTION:     // 停止->注意同時　注意:制限 40km/h
              caution_stops();
              state = ST_CAUTION2;
              break;

      case ST_CAUTION2:
              caution();
              cnt = 0;
              state = ST_CAUTIONWAIT;
              break;

      case ST_CAUTIONWAIT:
              cnt++;
              if( cnt > CAUTIONTIM) {
                cnt = 0;
                cycle = 0;
                Deceleration();
                state = ST_DECELEATIONWAIT;
              }
              break;

      case ST_DECELEATIONWAIT:  //5減速:制限 70km/h
              cnt++;
              if( cnt > DECELEATIONTIM) {
                cnt = 0;
                cycle = 0;
                Deceleration();
                state = ST_SUPPRESSWAIT;
              }
              break;

      case ST_SUPPRESSWAIT:  // 減速:制限 100km/h 点滅処理
              cnt++;
              if( cnt > SUPPRESSTIM) {
                cnt = 0;
                if(onoff == 0) {
                  blank();
                  onoff = 1;
                } else {
                  Deceleration();
                  onoff = 0;
                  cycle++;
                }
                if( cycle >= SUPPRESSCNT ){
                  blank();
                  state = ST_ADVANCE;
                }
              }
              break;

      case ST_ADVANCE:
              advance();
              state = ST_IDLE;
              break;

      default:
              break;
    }
}
//}

本誌では4現示抑速を使用しましたが、３現示信号機ライブラリ、４現示警戒信号機への変更もスケッチの信号機クラスの実体化部分を
変更するだけで簡単に信号機を切り替えることができます。
@<tt>{ThirdColorLightSignal}クラスのコメントを外して、@<tt>{ourDeceleration}クラスをコメントアウトすると
３現示信号機に切り替えることができます。

//emlistnum{
  //各種設定、宣言
  //ThirdColorLightSignal FirstSignal(9,10,11,12);
  //ThirdColorLightSignal SecondSignal(5,6,7,8);
  FourDeceleration FirstSignal(9,10,11,12);
  FourDeceleration SecondSignal(5,6,7,8);
//}

//image[signal14][信号機][scale=1]

※@<tt>{2ColorLightSignal.cpp}と@<tt>{FiveDeceleration.cpp}はスケッチは用意されていません。


//embed[latex]{
\clearpage
//}


====[column] 信号機クラスについて

クラスを使用しない方法で信号機のスケッチを作ると@<img>{signal16}の様に1台目の信号機、2台目の信号機は別々のプログラムになります。
台数が増えるとスケッチの量も増え、スケッチのメンテナンスも大変になります。
//image[signal16][信号機関数][scale=0.5]

そこで信号機クラスを使用すると@<img>{signal15}の様に@<tt>{FourDeceleration}クラスがあると、
@<tt>{Arduino}側のスケッチで1台目の信号機、2台目の信号機として実体化（オブジェクト化）できます。
信号機のクラスは1種類だけメンテすれば良いのとメモリが許す限り信号機を複数簡単に設置ができます。
//image[signal15][信号機クラス][scale=0.5]

====[/column]


//embed[latex]{
\clearpage
//}




== 出発信号機（アクセサリデコーダタイプ）

本線と分岐の島式ホームの出発信号機です。ポイントを直進にすると直進側の信号が青（進行）に変わり、分岐側の信号は赤（停止）になります。
またポイントを分岐にすると直進側の信号は赤になり、分岐側の信号は青になります。
青の状態で車両を出発させて在線検出信号を通過すると青から赤信号に変わります。
@<tt>{Arduino Nano DCC Shield}がポイントの@<tt>{DCC}信号を受信し信号機を制御します。
//image[syupatu1][出発信号機システム][scale=1]

=== 接続図

閉塞信号機と同じ接続になります。

=== 信号機回路の接続

閉塞信号機と同じ接続になります。

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/DepartureSignal,[ DepartureSignalスケッチ ]}

@<tt>{https://github.com/DccShield/DepartureSignal}

=== スケッチの構成
以下のソースファイルが @<tt>{DepartureSignal}のスケッチに含まれています。

@<tt>{cds.cpp　:　Cds}センサライブラリ

@<tt>{cds.h  :　Cds}センサライブラリ用ヘッダファイル

@<tt>{3ColorLightSignal.cpp : }３現示信号機ライブラリ

@<tt>{3ColorLightSignal.h : }３現示信号機ライブラリ用ヘッダファイル

@<tt>{NmraDcc.cpp : NmraDCC}ライブラリ

@<tt>{NmraDcc.h : NmraDCC}ライブラリ用ヘッダファイル

@<tt>{DepartureSignal.ino :　}閉塞信号機スケッチ


=== ヘッダ部の実装
//emlistnum{
#include <arduino.h>
#include <avr/eeprom.h>   //required by notifyCVRead() function if enabled below
#include "DccCV.h"
#include "Cds.h"
#include "3ColorLightSignal.h"
#include "NmraDcc.h"

#define DEBUG      //リリースのときはコメントアウトすること

//各種設定、宣言
void Dccinit(void);

//使用クラスの宣言
NmraDcc   Dcc;
DCC_MSG  Packet;
ThirdColorLightSignal StrSignal(9,10,11,12);
ThirdColorLightSignal DivSignal(5,6,7,8);
Cds CdsSensorStr(A0, 2, 200,200);
Cds CdsSensorDiv(A1, 2, 200,200);

struct CVPair {
  uint16_t  CV;
  uint8_t Value;
};

CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, 1},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
  {CV_29_CONFIG, 0b10000000},             // CV29 Software sw CV29＝128 アクセサリデコーダ
};
//}
1行目 Arduino固有のヘッダファイル

2行目 EEPROMライブラリのヘッダファイル

3行目 DccCVヘッダファイル

4行目 Cdsセンサライブラリヘッダファイル

5行目 3現示信号機ライブラリヘッダファイル

6行目 NnraDccライブラリヘッダファイル

14行目〜 各クラスの実体化（オブジェクト化）、NmraDCCライブラリと、3現示信号機2ch分、Cds在線検出2ch分

26行目〜 アクセサリアドレス1、CV29 CONFIGにしてアクセサリデコーダを設定しています。

=== setup()の実装

@<tt>{DCC}ライブラリの初期化とタイマー変数に現在の経過時刻を代入しています。

//emlistnum{
void setup()
{
  Dccinit();

  //Reset task
  gPreviousL5 = millis();
}
//}

=== loop()の実装

//emlistnum{
void loop() {
  Dcc.process();

  if( FactoryDefaultCVIndex && Dcc.isSetCVReady()){
    FactoryDefaultCVIndex--;
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV,
                                FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
  }

  if((millis() - gPreviousL5) >= 100){
    gPreviousL5 = millis();
    CloseSignalState();
    StrSignal.statechk();
    DivSignal.statechk();
  }
}
//}

2行目 @<tt>{DCC}の処理プロセスを実行します

4行目〜 @<tt>{CV8}等でリセットされた時に、デフォルトの@<tt>{CV}テーブルを元に、再セットする処理

10行目〜 @<tt>{100ms}周期に3つのステートマシンを処理します。

//embed[latex]{
\clearpage
//}

=== CloseSignalState()の実装

出発信号機ステートマシンの本体です。ここでは全体の流れを管理しています。

@<tt>{DCC}のアクセサリ信号の直進か分岐かのイベントによる信号機制御、在線を検出したイベントによる
信号機の制御をを行なっています。

//image[syupatu2][ステートマシン][scale=1]

//emlistnum{
void CloseSignalState( void ){
  enum{
      STS_INIT = 0,
      STS_IDLE,
      STS_STR,
      STS_STR2,
      STS_STRIDLE,
      STS_STRIDLE2,
      STS_DIV,
      STS_DIV2,
      STS_DIVIDLE,
      STS_DIVIDLE2,
  };
  static int state = STS_INIT;
//}

@<tt>{Enum(列挙型)}を使用し、ステートマシンのステートを設定しています。

//emlistnum{
  switch(state){
    case STS_INIT:
                  StrSignal.ChangeState(ST_ADVANCE);
                  DivSignal.ChangeState(ST_STOP);
                  state = STS_STRIDLE;                  // 初回はSTRIDLE
                  break;
    case STS_IDLE:
                  break;
    case STS_STR:
                  DivSignal.ChangeState(ST_STOP);       // 分岐停止
                  state = STS_STR2;
                  break;
    case STS_STR2:  // STR.DIVが同時に切り替わらない様に100ms遅らせる
                  StrSignal.ChangeState(ST_CAUTION);    // 直進注意
                  state = STS_STRIDLE;
                  break;
    case STS_STRIDLE:
                  if(CdsSensorStr.statechk(LOW) == 1){  // STR側 在線検出
                    StrSignal.ChangeState(ST_STOPDET);     // 停止信号
//                  StrSignal.ChangeState(ST_STOP);     // 停止信号
                    CdsSensorStr.;
                    state = STS_STRIDLE;
                  }
                  if( gCTevent == 1 ){ 　// アクセサリのDCCイベント発生
                    gCTevent = 0;
                    if(gDir == 0){
                      state = STS_DIV;
                    } else if(gDir == 1){
                      state = STS_STR;
                    }
                  }
                  break;
    case STS_STRIDLE2:
                  break;
    case STS_DIV:
                  StrSignal.ChangeState(ST_STOP);      // 直進停止
                  state = STS_DIV2;
                  break;
    case STS_DIV2:  // STR.DIVが同時に切り替わらない様に100ms遅らせる
                  DivSignal.ChangeState(ST_CAUTION);    // 分岐注意
                  state = STS_DIVIDLE;
                  break;
    case STS_DIVIDLE:
                  if(CdsSensorDiv.statechk(LOW) == 1){  // DIVR側 在線検出
                    DivSignal.ChangeState(ST_STOPDET);  // 停止信号
//                  DivSignal.ChangeState(ST_STOP);     // 停止信号
                    CdsSensorDiv.;
                    state = STS_DIVIDLE;
                  }
                  if( gCTevent == 1 ){  // アクセサリのDCCイベント発生
                    gCTevent = 0;
                    if(gDir == 0){
                      state = STS_DIV;
                    } else if(gDir == 1){
                      state = STS_STR;
                    }
                  }
                  break;
    default:
                  break;
  }
}
//}

=== getMyAddr_Acc()の実装

アクセサリアドレスを取得します。
@<tt>{CV29}の情報を確認し、アクセサリデコーダならアドレスを格納、2バイトアドレスが設定されていたら2バイトアドレスを
格納します。

//emlistnum{
uint16_t getMyAddr_Acc(void)
{
  uint16_t  Addr ;
  uint8_t   CV29Value ;

  CV29Value = Dcc.getCV( CV_29_CONFIG ) ;

  if( CV29Value & 0b10000000 ) { // Accessory Decoder?
    Addr = ( Dcc.getCV( CV_ACCESSORY_DECODER_ADDRESS_MSB ) << 8 ) |
             Dcc.getCV( CV_ACCESSORY_DECODER_ADDRESS_LSB ) ;
  }
  else   // Multi-Function Decoder?(アドレス4桁？)
  {
    if( CV29Value & 0b00100000 )  // Two Byte Address?
      Addr = ( Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB ) << 8 ) |
               Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB ) ;
    else
      Addr = Dcc.getCV( 1 ) ;
  }
  return Addr ;
}
//}

6行目 @<tt>{CV29}の設定値を取得します。

8行目〜 @<tt>{CV29}の設定値からアクセサリデコーダか判断し、アクセサリデコーダの場合は1バイトアドレスを設定します。

14行目〜 @<tt>{CV29}の設定値から2バイトアドレスが有効かを判断し、2バイトアドレスタイプの場合2バイトアドレスを設定します。

//embed[latex]{
\clearpage
//}

=== notifyDccAccState()の実装

アクセサリ命令が受信されると呼ばれます。@<tt>{gDir}変数に直進または分岐の状態を代入します。

//emlistnum{
void notifyDccAccState(uint16_t Addr,uint16_t BoardAddr,uint8_t OutputAddr,
                                                                 uint8_t State)
{
  uint16_t aAccAdress = Addr;
  uint8_t aDir = (OutputAddr & 0b001);

  if( gAccessoryAddress != aAccAdress){
    return;
  }
  gDir = (OutputAddr & 0b001);
  gCTevent = 1;
}
//}

4行目 受信したアドレスを@<tt>{aAccAdress}に代入します。

5行目 @<tt>{aDir}変数に直進または分岐かのフラグを代入します。

7行目 自分のアドレスと受信したアドレスを比較して異なっていた場合は処理を抜けます。
同じであったら@<tt>{gDir}変数に直進または分岐かのフラグを代入します。

=== その他の関数

他の章を参照してください。

@<tt>{notifyDccSpeed()}

@<tt>{resetCVToDefault()}

@<tt>{notifyCVResetFactoryDefault()}

@<tt>{notifyCVAck()}

@<tt>{void MOTOR_Ack()}

@<tt>{void Dccinit()}

@<tt>{notifyCVChange()}

@<tt>{notifyDccSigState()}

@<tt>{notifyDcc}
