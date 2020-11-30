= DCC用スケッチを作る
== どんなスケッチを作る？
本@<tt>{Arduino Nano  DCC Shield}と@<tt>{NmraDCC}ライブラリを使うことで、ファンクションデコーダ、アクセサリデコーダ、
@<tt>{DCC}モニタ（@<tt>{DCC}スニファ）を作ります。
以下の順番でファンクションデコーダ、アクセサリデコーダを紹介していきます。

//blankline

・何もしないファンクションデコーダ

　@<tt>{FunctionDecoderDoNothing.ino}

//blankline

・@<tt>{LED}ファンクションデコーダ

　@<tt>{FunctionDecoderLED.ino}

//blankline

・モータファンクションデコーダ

　@<tt>{FunctionDecoderMotorL1.ino}
//blankline

・BEMF制御に対応したモータファンクションデコーダ

　@<tt>{FunctionDecoderMotorL4.ino}
//blankline

・ステッピングファンクションデコーダ

//blankline


・サーボモータファンクションデコーダ

　@<tt>{FunctionDecoderServo.ino}

//blankline

・サーボモータアクセサリデコーダ

　@<tt>{AccDecoderServo0328.ino}

//blankline

・MP3サウンドデコーダ

//blankline


・車両デコーダー

//blankline


・DCC通信モニタ

//embed[latex]{
\clearpage
//}

#@#-----------------------------------------------------------------------------
#@# 何もしないファンクションデコーダを作る
#@#-----------------------------------------------------------------------------

== 何もしないファンクションデコーダを作る
まずは@<tt>{DCC}ファンクションデコーダの元となる、何もしないファンクションデコーダ@<tt>{FunctionDecoderDoNothing}のスケッチを
紹介します。

//image[dccsyori][DCCデコーダ処理ブロック図][scale=1]

@<tt>{Dcc}の受信処理は@<tt>{DCC}ライブラリ側で行っていますので、デコーダを作る際は速度インベントの処理、ファンクションイベントの
処理だけを作成すればよいので簡単に作れます。

@<tt>{DEBUG}フラグを有効にすると、@<tt>{DCC}から受信した速度のイベント、ファンクションのイベントを@<tt>{Arduino Nano }のシリアル出力しますので、
コマンドステーションから送られてきた@<tt>{DCC}コマンドの確認ができます。

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/FunctionDecoderDoNothing,[ FunctionDecoderDoNothingスケッチ ]}

https://github.com/DccShield/FunctionDecoderDoNothing

=== スケッチの構成
以下のソースファイルが @<tt>{FunctionDecoderDoNothing}のスケッチに含まれています。

@<tt>{NmraDcc.cpp                   　:　NmraDcc}ライブラリ

@<tt>{NmraDcc.h　　                   :　NmraDcc}ライブラリ用ヘッダファイル

@<tt>{DccCV.h　　　                   :　}定義ファイルとグローバル変数宣言

@<tt>{FunctionDecoderDoNothing.ino   :　}何もしないファンクションデコーダスケッチ


=== FunctionDecoderDoNothing.ino に含まれている関数
以下の関数が、@<tt>{FunctionDecoderDoNothing.ino} ファイルに入っています。

@<tt>{setup()                        ： }Arduinoの初期化関数

@<tt>{loop()                         ： }Arduinoのループ関数

@<tt>{dccinit()                      ： }デコーダの初期化を行う関数

@<tt>{MOTOR_Ack()                    ： DCC}の@<tt>{ACK}を返答するときにモータを動かして@<tt>{ACK}を応答する関数

@<tt>{resetCVToDefault()             ： CV}テーブルを初期化する関数

@<tt>{notifyDccSpeed()               ： }ノードファイ（通知）@<tt>{DCC}の速度を受信した時に呼ばれる関数

@<tt>{notifyDccFunc()                ： DCC}のファンクション命令を受信した時に呼ばれる関数

@<tt>{notifyCVResetFactoryDefault()　： CV8}を受信した時に呼ばれる関数

@<tt>{notifyCVAck()                  ： CV ack}を通知する時に呼ばれる関数

@<tt>{notifyCVChange()               ： CV}値が変化あった時に呼ばれる関数

@<tt>{notifyDccReset()               ： DCC RESET}が通知された時に呼ばれる関数

//blankline

何もしないファンクションデコーダなのですが、@<tt>{NmrmDcc}ライブラリから最低限必要な関数だけ用意しました。

=== setup()の実装
@<tt>{Arduino}で用意されている@<tt>{setup()}は、@<tt>{Arduino Nano }に電源が入った時に最初の一度だけ処理させる関数です。
この何もしないファクションデコーダでは、@<tt>{DEBUG}用のシリアルポートの初期化、@<tt>{NmraDcc}の初期化、現在時刻の初期化を行なっています。

//emlistnum{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Hello,Smile Function Decoder");
#endif

//ファンクションの割り当てピン初期化
Dccinit();

//Reset task
gPreviousL5 = millis();
//}

1行目@<tt>{:#define DEBUG}と定義されていた時に、@<tt>{#ifdef〜#endif}間をコンパイルします。
スケッチの10行目に@<tt>{ #define DEBUG }の記述があるので、@<tt>{Arduino Nano }のシリアルポートを
@<tt>{115200bps}で初期化、シリアルポートに@<tt>{"Hello,Smile Function Decoder"}という文字を
送信しています。

@<tt>{10行目}の@<tt>{#define DEBUG}の頭に@<tt>{//}を追加して、@<tt>{// #define DEBUG }に変更すると、
@<tt>{#ifdef〜#endif}間のスケッチをコンパイルしなくなりシリアルポートが使えなくなります。

7行目:@<tt>{Dccinit()}関数を呼んで初期設定を行なっています（後述）

10行目:@<tt>{gPreviousL5}という@<tt>{gloval long}変数に@<tt>{Arduino}が立ち上がってから経過した現在の時刻を代入しています。

※@<tt>{FunctionDecoderDoNothing.ino}内どこでも使用できる変数です。

=== loop()の実装
@<tt>{Arduino}で用意されている@<tt>{loop()}内のプログラムを永遠に繰り返す関数です。

本来は色々な処理を記述しますが、@<tt>{Dcc}受信関数のコール、@<tt>{10msec}毎に何もしない処理を行なっています。

//emlistnum{
void loop() {
  Dcc.process();
  if ( (millis() - gPreviousL5) >= 10){     // 10:10msec
    gPreviousL5 = millis();
  }
}
//}

2行目:@<tt>{NmraDcc}ライブラリの@<tt>{process}関数を呼んでいます。

3行目:現在の時刻から前回記録した@<tt>{gPreviousL5}の時間を減算して経過時間を求めています。
10ms以上空いていたら@<tt>{gPreviosL5}変数に現在の時刻を代入します。

この書き方は今後重要な記述方法となります。他の処理も行いつつ経過時間を求めることで@<tt>{delay()}関数を使わずに処理を待つというのに
使用しています。この場合@<tt>{loop()}を無限に動き続けますが、@<tt>{10ms}に別の仕事を行います。

=== dccinit()の実装

デコーダの初期化を行う関数です。
@<tt>{Dcc}クラスの初期設定、製造者IDの設定、自分の@<tt>{DCC}アドレスの設定を行います。

//emlistnum{
//DCCの応答用負荷ピン
#if defined(DCCACKPIN)
//Setup ACK Pin
pinMode(DccAckPin, OUTPUT);
digitalWrite(DccAckPin, 0);
#endif

#if !defined(DECODER_DONT_DEFAULT_CV_ON_POWERUP)
if ( Dcc.getCV(CV_MULTIFUNCTION_PRIMARY_ADDRESS) == 0xFF ) {
	notifyCVResetFactoryDefault();
} else {
	Serial.println("CV Not Defaulting");
}
#else
  Serial.println("CV Defaulting Always On Powerup");
  notifyCVResetFactoryDefault();
#endif

Dcc.pin(0, 2, 0); // Arduino Nano  D2(PD2)pinをDCC信号入力端子に設定

// Call the main DCC Init function to enable the DCC Receiver
Dcc.init( MAN_ID_DIY, 1, FLAGS_MY_ADDRESS_ONLY , 0 );

//Init CVs
gCV1_SAddr = Dcc.getCV( CV_MULTIFUNCTION_PRIMARY_ADDRESS ) ;
gCVx_LAddr = (Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB ) << 8) +
                            Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB );
//}

2行目@<tt>{:DCC}の@<tt>{ACK}ピンの定義 @<tt>{DCCACKPIN }が定義されていると@<tt>{#if〜#endif}間のスケッチが使用されます。
@<tt>{DCCACKPIN}が未定義のため、@<tt>{DCC ACK}機能は未使用の設定になっています。


8行目@<tt>{:DECODER_DONT_DEFAULT_CV_ON_POWERUP }が定義されていないと@<tt>{ CV1=255 }の時は@<tt>{notifyCVResetFactoryDefault()}関数を呼んで
@<tt>{CV}値をリセットします。@<tt>{DECODER_DONT_DEFAULT_CV_ON_POWERUP} 定義されていると電源@<tt>{ON}時に毎回@<tt>{CV}値をリセットします。


20行目@<tt>{:Dcc}信号の入力端子を定義します。
@<tt>{Arduino Nano }を使用する場合は、@<tt>{Dcc.pin(0, 2, 0); }が決り構文になりますので変更しないでください。

 引数1の0は、@<tt>{attachInterrupt()}関数の割り込み番号に0番を設定しています。

 引数2の2は、ポート番号@<tt>{ D2(PD2)pin}に設定しています。

 引数3の0は、@<tt>{LOW}の時に割り込みを発生検知に設定しています。

23行目@<tt>{:Dcc}ライブラリの初期化を定義します。
 引数1@<tt>{ CV8}のメーカー個別番号は個人で作成するための@<tt>{ MAN_ID_DIY }で定義している値@<tt>{ 14 }を使用します。

 引数2 メーカーバージョン番号 1

 引数3@<tt>{ FLAGS_MY_ADDRESS_ONLY }自分の@<tt>{DCC}アドレスだけ処理するように設定

 引数4@<tt>{ OpsMode}は未使用

26行目@<tt>{:gCV1_SAdder }に@<tt>{ CV1}で設定されている@<tt>{DCC}基本アドレス@<tt>{(1〜127)}を@<tt>{EEPROM}から読み込みます。
27行目@<tt>{:gCVx_LAddr }に@<tt>{ CV17}と@<tt>{CV18}で設定されているDCC拡張アドレス@<tt>{(1〜9999)}を@<tt>{EEPROM}から読み込みます。

=== MOTOR_Ack()の実装
@<tt>{Dcc}の@<tt>{ACK}を返す時に使用するポートを指定しますが、なにもしないデコーダーでは未使用です。

=== resetCVToDefault()の実装
@<tt>{CV}値をデフォルトに書き換える関数です。

=== notifyDccSpeed()の実装
@<tt>{DCC}速度信号の受信によるイベント発生時に呼び出す関数です。なにもしないデコーダーでは未使用です。

=== notifyDccFunc()の実装
ファンクション信号受信のイベント発生時に呼び出す関数です。なにもしないデコーダーでは未使用です

=== notifyCVResetFactoryDefault()の実装
@<tt>{CV8} によるリセットコマンド受信処理

=== notifyCVAck()の実装
@<tt>{CV Ack}の処理、デコーダ側で@<tt>{60mA}消費させる必要があるので、@<tt>{LED}ファンクションデコーダの場合は消費電流が少ないため、
@<tt>{ACK}を返すのが難しいです。
詳細は@<tt>{Desktop}ステーションさんのブログ参照
@<tt>{http://powerele.sblo.jp/article/184852358.html}

=== notifyCVChange()の実装
CV値が変化した時の処理

=== notifyDccReset()の実装
リセット処理、特に何もしません。


//embed[latex]{
\clearpage
//}

#@#-----------------------------------------------------------------------------
#@# LEDファンクションデコーダを作る
#@#-----------------------------------------------------------------------------

== LEDファンクションデコーダを作る
@<tt>{Arduino Nano  DCC Shield }を使った、両極性@<tt>{1FX 6FX}デコーダのスケッチを紹介します。
点灯効果は一切ない@<tt>{ON/OFF}だけのスケッチになります。
下記接続図は@<tt>{Arduino Nano ANO }の@<tt>{I/O D1〜D12}にLEDを取り付けました。
実際使用するのは、@<tt>{D3} フロント@<tt>{LED,D4} リア@<tt>{LED,D5〜D11 AUX1〜7　}までの@<tt>{ 9ch}分の@<tt>{LED}を
@<tt>{DCC}から制御ができます。

=== 完成写真
//image[ledfunction2][完成写真][scale=0.8]

=== 接続図
//image[ledfunction1][LEDファンクションデコーダの接続][scale=0.8]

オプションの抵抗基板に@<tt>{1kΩ}と@<tt>{LED}を実装しました。

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/FunctionDecoderLed,[ FunctionDecoderLedスケッチ ]}

@<tt>{https://github.com/DccShield/FunctionDecoderLed}

=== スケッチの構成

以下のソースファイルが @<tt>{FunctionDecoderLed}のスケッチに含まれています。

@<tt>{NmraDcc.cpp            :　NmraDcc}ライブラリ

@<tt>{NmraDcc.h　            :　NmraDcc}ライブラリ用ヘッダファイル

@<tt>{DccCV.h                :　}定義ファイルとグローバル変数宣言

@<tt>{FunctionDecoderLed.ino :　}両極性@<tt>{1FX 6FX}ファンクションデコーダスケッチ

=== FunctionDecoderLed.ino に含まれている関数
以下の関数が、@<tt>{FunctionDecoderLed.ino} ファイルに入っています。

@<tt>{setup()                ： }初期化関数

@<tt>{loop()                 ： }ループ関数

@<tt>{dccinit()              ： }デコーダの初期化を行う関数

@<tt>{MOTOR_Ack()            ： DCC}の@<tt>{ACK}を返答するときにモータを動かして@<tt>{ACK}を応答する関数

@<tt>{resetCVToDefault()     ： CV}テーブルを初期化する関数

@<tt>{notifyDccSpeed()       ： }ノードファイ（通知）@<tt>{DCC}の速度を受信した時に呼ばれる関数

@<tt>{notifyDccFunc()        ： DCC}のファンクション命令を受信した時に呼ばれる関数

@<tt>{FunctionProcess()}

@<tt>{notifyCVResetFactoryDefault()　： CV8}を受信した時に呼ばれる関数

@<tt>{notifyCVAck()          ：　CV ack}を通知する時に呼ばれる関数

@<tt>{notifyCVChange()       ：　CV}値が変化あった時に呼ばれる関数

@<tt>{notifyDccReset()       ：　DCC　RESET}が通知された時に呼ばれる関数

=== 各種設定、宣言

//emlistnum{
//各種設定、宣言

#define PIN_F0_F  3     // D3 PD3,PWM
#define PIN_F0_R  4     // D4 PD4
#define PIN_AUX1  5     // D5 PD5
#define PIN_AUX2  6     // D6 PD6
#define PIN_AUX3  7     // D7 PD7
#define PIN_AUX4  8     // D8 PB0
#define PIN_AUX5  9     // D9 PB1,DIGITAL TR,PWM
#define PIN_AUX6  10    // D10 PB2,DIGITAL TR,PWM
#define PIN_AUX7  11    // D11 PB3,DIGITAL TR,PWM
//}

両極性@<tt>{1FX 6FX}のポートを割り当てています。

//emlistnum{
CVPair FactoryDefaultCVs [] = {
  {CV_MULTIFUNCTION_PRIMARY_ADDRESS, DECODER_ADDRESS}, // CV01
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0},
  {CV_dummy,0},
};
//}

この@<tt>{FactoryDefaultCVs[]}配列は、工場出荷時に（@<tt>{CV8}でリセットした時）に@<tt>{CV}値を工場出荷時の値（デフォルト値）に
設定するパラメータです。
@<tt>{CV}値の機能を増やしたら、この@<tt>{FactoryDefaultCVs[]}配列に@<tt>{CV}値をデフォルトにしたい値を追記していきます。

=== setup()の実装
//emlistnum{
//ファンクションの割り当てピン初期化
pinMode(PIN_F0_F, OUTPUT);
digitalWrite(PIN_F0_F, OFF);
pinMode(PIN_F0_R, OUTPUT);
digitalWrite(PIN_F0_R, OFF);
pinMode(PIN_AUX1, OUTPUT);
digitalWrite(PIN_AUX1, OFF);
pinMode(PIN_AUX2, OUTPUT);
digitalWrite(PIN_AUX2, OFF);
pinMode(PIN_AUX3, OUTPUT);
digitalWrite(PIN_AUX3, OFF);
pinMode(PIN_AUX4, OUTPUT);
digitalWrite(PIN_AUX4, OFF);
pinMode(PIN_AUX5, OUTPUT);
digitalWrite(PIN_AUX5, OFF);
pinMode(PIN_AUX6, OUTPUT);
digitalWrite(PIN_AUX6, OFF);
pinMode(PIN_AUX7, OUTPUT);
digitalWrite(PIN_AUX7, OFF);

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Hello,Smile Function Decoder");
#endif

Dccinit();

//Reset task
gPreviousL5 = millis();
//}

2行目〜：@<tt>{Arduino Nano}で使用する出力ポートの設定と出力@<tt>{OFF}を行なっています。

21行目〜：デバック用のシリアルポートの初期化を行なっています。
スケッチの先頭で@<tt>{#define DEBUG}の一文を記述すると@<tt>{#ifdef DEBUG}が有効になり、初期化と起動メッセージが
シリアルポートに出力されます。

26行目：@<tt>{Dccinit()}関数を呼んで各種初期設定を行なっています（後述）

29行目：@<tt>{gPreviousL5}という@<tt>{gloval long}変数に@<tt>{Arduino}が立ち上がってから経過した
現在の時刻を代入しています。


=== loop()の実装
@<tt>{Arduino}で用意されている@<tt>{loop()}内のプログラムを永遠に繰り返す関数です。
ファンクションの具体的な処理は時間で細切れにして処理をさせて擬似的に並列動作をさせる構造とします。

//emlistnum{
Dcc.process();
if ( (millis() - gPreviousL5) >= 10){ // 100:100msec  10:10msec。
  FunctionProcess();
  gPreviousL5 = millis();
}
//}
1行目:@<tt>{NmraDcc}ライブラリの@<tt>{process}関数を呼んでいます。

2行目:現在の時刻から前回記録した@<tt>{gPreviousL5}の時間を減算して経過時間を求めて

3行目:@<tt>{10ms}周期に@<tt>{FunctionProcess()}関数を呼んでファンクションの実行を行なっています。

4行目:@<tt>{gPreviosL5}変数に現在の時刻を代入します。


=== notifyDccSpeed()の実装
@<tt>{DCC}通信で速度のコマンドを受信した時に呼ばれるイベントです。
コマンドステーションは常に定期的に速度のコマンドを送信を続けるので都度呼び出されています。
@<tt>{KATO}の@<tt>{DCS50K}の@<tt>{128}ステップモードの場合、@<tt>{SpeedSteps:127}でスロットが@<tt>{0}の停止で@<tt>{Speedは0}、スロット@<tt>{99}で@<tt>{126}が送られてきます。
@<tt>{28}ステップモードの場合は、@<tt>{SpeedSteps:29}でスロットが@<tt>{0}の停止で@<tt>{Speedは0}、スロット@<tt>{99}で@<tt>{29}が送られてきます。
なお@<tt>{Speed 0}で非常停止となっています。

//emlistnum{
extern void notifyDccSpeed( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed,
                                 DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps )
{
  uint16_t aSpeedRef = 0;
  //速度値の正規化(255を100%とする処理)
  if( Speed >= 1){
    aSpeedRef = ((Speed - 1) * 255) / SpeedSteps;
  } else {
    //緊急停止信号受信時の処理 //Nagoden comment 2016/06/11
    aSpeedRef = 0;
  }
}
//}

@<tt>{DSbluebox}の場合、以下のようになります。

@<tt>{Spd0時：Speed-ADR:3, AddrType:0, SPD:1, DIR:1, SpeedSteps:127, aSpeedRef:0}

@<tt>{Spd9時：Speed-ADR:3, AddrType:0, SPD:112, DIR:1, SpeedSteps:127, aSpeedRef:222}


=== notifyDccFunc()の実装
@<tt>{DCC}通信でファンクションのコマンドを受信した時に呼ばれるイベントです。
コマンドステーションは常に定期的にファンクションコマンドを送信を続けるので、都度呼び出されています。
@<tt>{notifyDccFunc}の引数はファンクションと変数は以下の表のようになっています。各ファンクションでグループに別れており@<tt>{FuncState}のビット割付された
状態になっています。
@<tt>{F0}だけマッピングが特殊です。

//blankline

@<tt>{extern void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)}

@<tt>{Addr}には自身の@<tt>{DCC}アドレス値が設定されています。次のページは@<tt>{DCC}アドレス@<tt>{3}の時にファンクションを受信した時の表となります。

//table[notifyDccFunc][notifyDccFunc引数]{
ファンクション	Addr	AddrType	FuncGrp	FuncState
F0	3	0	1	0x10
F1	3	0	1	0x01
F2	3	0	1	0x02
F3	3	0	1	0x04
F4	3	0	1	0x08
F5	3	0	2	0x01
F6	3	0	2	0x02
F7	3	0	2	0x04
F8	3	0	2	0x08
F9	3	0	3	0x01
F10	3	0	3	0x02
F11	3	0	3	0x04
F12	3	0	3	0x08
F13	3	0	4	0x01
F14	3	0	4	0x02
F15	3	0	4	0x04
F16	3	0	4	0x08
F17	3	0	4	0x10
F18	3	0	4	0x20
F19	3	0	4	0x40
F20	3	0	4	0x80
F21	3	0	5	0x01
F22	3	0	5	0x02
F23	3	0	5	0x04
F24	3	0	5	0x08
F25	3	0	5	0x10
F26	3	0	5	0x20
F27	3	0	5	0x40
F28	3	0	5	0x80
//}

よって、@<tt>{F0}のみ@<tt>{ON}の時は、@<tt>{FuncGrp: 1, FuncState: 10}になります。

@<tt>{F0}と@<tt>{F3}が@<tt>{ON}の時は@<tt>{0x10}と@<tt>{0x02}を加算して、@<tt>{FuncGrp: 1, FuncState: 12}になります。

コマンドステーションによりますが、@<tt>{DSbluebox}の場合、以下のように@<tt>{DCC}コマンドが常に送られてきます。
※すべてのファンクションが@<tt>{OFF}の時
//emlist[]{
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 1, FuncState: 0
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 2, FuncState: 0
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 3, FuncState: 0
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 4, FuncState: 0
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 5, FuncState: 0
//}

複数のファンクションを@<tt>{ON}にすると、以下のような@<tt>{FuncState}値として送られてきます。
//emlist[]{
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 1, FuncState: 1F　　　//1F 0001:1111
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 2, FuncState: D      //0D 0000:1101
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 3, FuncState: B      //0B 0000:1011
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 4, FuncState: 9E     //9E 1001:1110
notifyDccFunc()
Addr:3, AddrType:0, FuncGrp: 5, FuncState: 4F     //4F 0100:1111
//}
解析すると・・・

@<tt>{FuncGrp 1:FuncState 1F = 0001:1111 F0,F1,F2,F3,F4}

@<tt>{FuncGrp 2:FuncState 0D = 0000:1101 F5,F7,F8}

@<tt>{FuncGrp 3:FuncState 0B = 0000:1011 F9,F10,F12}

@<tt>{FuncGrp 4:FuncState 9E = 1001:1110 F14,F15,F16,F17,F20}

@<tt>{FuncGrp 5:FuncState 4F = 0100:1111 F21,F22,F23,F24,F27}

この仕組みより@<tt>{notifyDccFunc()}の処理は、@<tt>{FuncGrp}と@<tt>{FuncState}を確認しながら、受信したファンクションが
あると、ファンクションがセットされていると、@<tt>{gState_F0 〜 gState_F16}にそのビットがセットされます。

@<tt>{F0}が@<tt>{ON}の場合、@<tt>{gState_F0 = 0x10}がセットされます。ちょっとわかりづらいですよね。

ファンクション解析部分のプログラムは以下の様になっております。

//emlistnum{
extern void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType,
　　　　　　　　　　　　　　　　FN_GROUP FuncGrp, uint8_t FuncState)

if( FuncGrp == FN_0_4)  // F0〜F4の解析
{
  if( gState_F0 != (FuncState & FN_BIT_00))
  {
    //Get Function 0 (FL) state
    gState_F0 = (FuncState & FN_BIT_00);
  }
  if( gState_F1 != (FuncState & FN_BIT_01))
  {
    //Get Function 1 state
    gState_F1 = (FuncState & FN_BIT_01);
  }
  if( gState_F2 != (FuncState & FN_BIT_02))
  {
    gState_F2 = (FuncState & FN_BIT_02);
  }
  if( gState_F3 != (FuncState & FN_BIT_03))
  {
    gState_F3 = (FuncState & FN_BIT_03);
  }
  if( gState_F4 != (FuncState & FN_BIT_04))
  {
    gState_F4 = (FuncState & FN_BIT_04);
  }
}

if( FuncGrp == FN_5_8)  // F5〜F8の解析
{
  if( gState_F5 != (FuncState & FN_BIT_05))
  {
    //Get Function 0 (FL) state
    gState_F5 = (FuncState & FN_BIT_05);
  }
  if( gState_F6 != (FuncState & FN_BIT_06))
  {
    //Get Function 1 state
    gState_F6 = (FuncState & FN_BIT_06);
  }
  if( gState_F7 != (FuncState & FN_BIT_07))
  {
    gState_F7 = (FuncState & FN_BIT_07);
  }
  if( gState_F8 != (FuncState & FN_BIT_08))
  {
    gState_F8 = (FuncState & FN_BIT_08);
  }
}
if( FuncGrp == FN_9_12)  // F9〜F12の解析
{
  if( gState_F9 != (FuncState & FN_BIT_09))
  {
    gState_F9 = (FuncState & FN_BIT_09);
  }
  if( gState_F10 != (FuncState & FN_BIT_10))
  {
    gState_F10 = (FuncState & FN_BIT_10);
  }
  if( gState_F11 != (FuncState & FN_BIT_11))
  {
    gState_F11 = (FuncState & FN_BIT_11);
  }
  if( gState_F12 != (FuncState & FN_BIT_12))
  {
    gState_F12 = (FuncState & FN_BIT_12);
  }
}

if( FuncGrp == FN_13_20)   // F13〜F20の解析
{
  if( gState_F13 != (FuncState & FN_BIT_13))
  {
    gState_F13 = (FuncState & FN_BIT_13);
  }
  if( gState_F14 != (FuncState & FN_BIT_14))
  {
    gState_F14 = (FuncState & FN_BIT_14);
  }
  if( gState_F15 != (FuncState & FN_BIT_15))
  {
    gState_F15 = (FuncState & FN_BIT_15);
  }
  if( gState_F16 != (FuncState & FN_BIT_16))
  {
    gState_F16 = (FuncState & FN_BIT_16);
  }
}
//}



//emlistnum{
if( FuncGrp == FN_0_4)  // F0〜F4の解析
{
  if( gState_F0 != (FuncState & FN_BIT_00))
  {
    //Get Function 0 (FL) state
    gState_F0 = (FuncState & FN_BIT_00);
  }
  if( gState_F1 != (FuncState & FN_BIT_01))
//}

@<tt>{F0}のファンクションを受信した時の処理の説明ですが、@<tt>{FN_0_4}は@<tt>{NmraDcc.h}で定義されていますが@<tt>{1}が設定されています。

@<tt>{F0}を受信した時は@<tt>{FuncGrp = 1}になりますので、@<tt>{if( FuncGrp == FN_0_4)}が成立します。（1行目）

次に、@<tt>{NmraDcc.h}で定義されていますが@<tt>{#define FN_BIT_00	0x10}となっており、@<tt>{if( gState_F0 != (FuncState & FN_BIT_00))}
の構文で、@<tt>{F0}を受信した時は、@<tt>{FuncState = 0x10}になりますので、@<tt>{0x10 & 0x10}で@<tt>{0x10}になり@<tt>{gState_F0}に入っていた値と
違う値になった場合、条件が成立します。（３行目）

@<tt>{gState_F0 = (FuncState & FN_BIT_00);}より、@<tt>{gState_F0}には、@<tt>{OFF->ON}の時は@<tt>{0x10}。@<tt>{ON->OFF}の時は@<tt>{0x10}が代入されます。
（６行目）

この代入している数値がややこしくしていまして、@<tt>{F0} が@<tt>{ON}だからと言って、@<tt>{gState_F0}に@<tt>{1}が代入されず@<tt>{0x10}が代入します。

=== resetCVToDefault()の実装
@<tt>{FactoryDefaultCVs}で設定した各@<tt>{CV}デフォルトテーブルを元に@<tt>{EEPROM}内の@<tt>{CV}値を初期化します。

//emlistnum{
for (int j = 0; j < FactoryDefaultCVIndex; j++ ) {
  Dcc.setCV( FactoryDefaultCVs[j].CV, FactoryDefaultCVs[j].Value);
}
//}

=== notifyCVResetFactoryDefault()の実装
@<tt>{CV8}コマンドによるデコーダリセットが送られてきた時に呼ばれるイベントです。
@<tt>{CV}値をデフォルトに設定し、ファンクションデコーダをリセットして再起動します。

//emlistnum{
resetCVToDefault();
delay(1000);
resetFunc();
//}

=== notifyCVAck()の実装

@<tt>{LED}の電流値だけでは@<tt>{DCC}の@<tt>{ACK}を応答するには電流が足りないので、本スケッチは応答しない
作りにしています。

//emlistnum{
#if 0
  digitalWrite(O1,HIGH);
  digitalWrite(O2,HIGH);
  digitalWrite(O3,HIGH);
  digitalWrite(O4,HIGH);
  delay( 6 );
  digitalWrite(O4,LOW);
  digitalWrite(O3,LOW);
  digitalWrite(O2,LOW);
  digitalWrite(O1,LOW);
//MOTOR_Ack();
//}


=== Dccinit()の実装

デコーダの初期化を行います。

//emlistnum{
  #if defined(DCCACKPIN)
    //Setup ACK Pin
    pinMode(DccAckPin, OUTPUT);
    digitalWrite(DccAckPin, 0);
  #endif

  #if !defined(DECODER_DONT_DEFAULT_CV_ON_POWERUP)
    if ( Dcc.getCV(CV_MULTIFUNCTION_PRIMARY_ADDRESS) == 0xFF ) {
      notifyCVResetFactoryDefault();
    } else {
      Serial.println("CV Not Defaulting");
    }
  #else
    Serial.println("CV Defaulting Always On Powerup");
    notifyCVResetFactoryDefault();
  #endif

    Dcc.pin(0, 2, 0);   // Arduino Nano D2(PD2)pinをDCC信号入力端子に設定

    // Call the main DCC Init function to enable the DCC Receiver
    Dcc.init( MMAN_ID_DIY, 1,   FLAGS_MY_ADDRESS_ONLY , 0 );

    //Init CVs
    gCV1_SAddr = Dcc.getCV( CV_MULTIFUNCTION_PRIMARY_ADDRESS ) ;
    gCVx_LAddr = (Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB ) << 8) +
                             Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB );

    //Init CVs
    //E2P-ROMからCV値を読み込む
//}

=== notifyCVChange()の実装
@<tt>{CV}値が変換した時の処理です。

=== notifyDccReset()の実装
リセット処理です




//embed[latex]{
\clearpage
//}



#@#-----------------------------------------------------------------------------
#@# 発光ギミックを搭載したLEDファンクションデコーダを作る
#@#-----------------------------------------------------------------------------

== 発光ギミックを搭載したLEDファンクションデコーダを作る










#@#-----------------------------------------------------------------------------
#@# モーターファンクションデコーダを作る
#@#-----------------------------------------------------------------------------

== モーターファンクションデコーダを作る
@<tt>{Arduino Nano  DCC Shield }はレイアウト用のデコーダを想定しているためモータを動かす為の
モータドライバは用意されていません。また@<tt>{Arduino Nano DCC Shield}では基板の大きさから車両に実装できませんが、
@<img>{arduinoDecoder} のデコーダが売られていますので、これらを使用することで車両用デコーダを作れます。
モータデコーダはコマンドステーションからのスロット値からモータ駆動用@<tt>{PWM}を出力するまでに様々な処理を行うため数段階に分けて説明していきます。

=== 車両用デコーダのベース基板
//image[arduinoDecoder][arduinoベースのDCCデコーダ][scale=1]
DCC電子工作連合は左から、スマイルデコーダ、@<tt>{MP3}サウンドデコーダ@<tt>{V6　N18、SmileDecoder N18、SmileMotorDecoder、SmileFunctionDecoder}
の基板が用意されています。
※@<tt>{SmileMotorDecoder、SmileFunctionDecoder}は@<tt>{Attiny85}を使用したデコーダになります。

この章ではマイコンに@<tt>{Atmega328}を使用し@<tt>{Arduino}プラットフォームで動作するスマイルデコーダを使用した車両用デコーダーを紹介します。

=== スマイルデコーダのピンアサイン
@<tt>{Arduino Nano DCC Shield}とは違うので@<img>{arduinoport}に表にまとめました。スマイルデコーダとなごでんさんのスマイルデコーダとは@<tt>{PWM_A}と@<tt>{PWM_B}の
ポートが異なりますので注意してください。

//image[arduinoport][スマイルデコーダのピンアサイン][scale=1]

=== スロット値をPWM値に変換するだけのデコーダ

まず最初にコマンドステーションから送信されるスロット値を@<tt>{PWM}値に変換してモータを制御する簡単なデコーダを紹介します。
@<img>{motorDecoderL1}の様なブロック図になります。

//image[motorDecoderL1][スロット値をPWM値に変換し出力するデコーダ][scale=1]

コマンドステーションのステップ数の設定が@<tt>{128STEP}に設定されている場合、@<tt>{0-127}の値が@<tt>{DCC}コマンドとして送信されてきます。
この@<tt>{0-127}の数値を@<tt>{PWM}値@<tt>{0-254}に変換します。
簡単なイメージですが、@<tt>{PWM}値が小さいとモーターに電圧が掛かる時間が短いので低回転となり、@<tt>{PWM}値が大きいとモーターに電圧が
掛かる時間が長いので高回転となります。

//image[motorpwm][モーターに印加されるPWM波形][scale=1]

※なお@<tt>{PWM}幅が小さい時はモーターに加わる力が弱い為、モーター自身の磁石の力、ギアなどの機械的な負荷が強い為、
車両が動きませんので動き出すまでにはある程度の電圧幅が必要になります。

処理は単純ですがアナログコントローラと同等な運転ができますので十分に遊べます。

@<href>{https://desktopstation.net/wiki/doku.php/ds_smile_decoder_r4,[ スマイルデコーダー紹介のページ ]}

@<tt>{https://desktopstation.net/wiki/doku.php/ds_smile_decoder_r4}

シリアルデバックする為にスマイルデコーダから@<tt>{+5V,GND,TxD(31pin)}を取り出して、@<tt>{Isolation Smile Writer}の
絶縁型@<tt>{USB}シリアル変換器部に接続します。何らかの故障原因で@<tt>{DCC}電圧が@<tt>{Mac}の@<tt>{USB}ポートに印加されるとロジックボードが壊れてしまうので、
フォトカプラで絶縁しています。
//image[motor2][SmiLedecoder][scale=0.5]

=== 接続図
//image[motor1][SmiLedecoder][scale=0.5]

@<tt>{Smile Decoder Tester}に接続しています。

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/FunctionDecoderMotor_L1,[ FunctionDecoderMotor_L1スケッチ ]}

@<tt>{https://github.com/DccShield/FunctionDecoderMotor_L1}

=== スケッチの構成

以下のソースファイルが @<tt>{FunctionDecoderMotro}のスケッチに含まれています。

@<tt>{NmraDcc.cpp　                :　NmraDcc}ライブラリ

@<tt>{NmraDcc.h　                  :　NmraDcc}ライブラリ用ヘッダファイル

@<tt>{DccCV.h                     ：　}定義ファイルとグローバル変数宣言

@<tt>{motor_ctrl.h　               :　motor}ライブラリ用ヘッダファイル

@<tt>{motor_ctrl.h                ：　}定義ファイルとグローバル変数宣言

@<tt>{FunctionDecoderMotor_L1.ino　:　}モーターデコーダスケッチ

=== FunctionDecoderMotorL1.ino に含まれている関数
以下の関数が、@<tt>{FunctionDecoderMotor.ino} ファイルに含まれています。

@<tt>{setup()                      ： }初期化関数

@<tt>{loop()                       ： }ループ関数

@<tt>{resetCVToDefault()           ： CV}テーブルを初期化する関数

@<tt>{notifyDccSpeed()             ： }ノードファイ（通知）@<tt>{DCC}の速度を受信した時に呼ばれる関数

@<tt>{notifyDccFunc()              ： DCC}のファンクション命令を受信した時に呼ばれる関数

@<tt>{exec_function()              ： }ファンクションコマンドによってポート出力する関数

@<tt>{notifyCVResetFactoryDefault()： CV8}を受信した時に呼ばれる関数

@<tt>{notifyCVAck()                ：　CV ack}を通知する時に呼ばれる関数

@<tt>{notifyCVChange()             ：　CV}値が変化あった時に呼ばれる関数

=== setup()の実装
@<tt>{Arduino}で用意されている@<tt>{setup()}は、@<tt>{Arduino Nano }に電源が入った時に最初の一度だけ処理させる関数です。
この何もしないファクションデコーダでは、@<tt>{DEBUG}用のシリアルポートの初期化、@<tt>{NmraDcc}の初期化、現在時刻の初期化を行なっています。

//emlistnum{
//ファンクションの割り当てピン初期化
pinMode(FunctionPin0, OUTPUT);
digitalWrite(FunctionPin0, 0);

pinMode(FunctionPin1, OUTPUT);
digitalWrite(FunctionPin1, 0);

pinMode(FunctionPin2, OUTPUT);
digitalWrite(FunctionPin2, 0);

pinMode(FunctionPin3, OUTPUT);
digitalWrite(FunctionPin3, 0);

pinMode(FunctionPin4, OUTPUT);
digitalWrite(FunctionPin4, 0);

//DCCの応答用負荷ピン
#if defined(DCCACKPIN)
//Setup ACK Pin
//pinMode(DccAckPin,OUTPUT);
//digitalWrite(DccAckPin, 0);
#endif

#if !defined(DECODER_DONT_DEFAULT_CV_ON_POWERUP)
 if ( Dcc.getCV(CV_MULTIFUNCTION_PRIMARY_ADDRESS) == 0xFF ){
   notifyCVResetFactoryDefault();
 } else{
 Serial.println("CV Not Defaulting");
 }
#else
 Serial.println("CV Defaulting Always On Powerup");
 notifyCVResetFactoryDefault();
#endif

Dcc.pin(0, 2, 0);

// Call the main DCC Init function to enable the DCC Receiver
Dcc.init( MAN_ID_DIY, 100,   FLAGS_MY_ADDRESS_ONLY , 0 );

//Reset task
gPreviousL5 = millis();

//Init CVs
gCV1_SAddr = Dcc.getCV( CV_MULTIFUNCTION_PRIMARY_ADDRESS ) ;
gCV2_Vstart = Dcc.getCV( CV_VSTART ) ;
gCV3_AccRatio = Dcc.getCV( CV_ACCRATIO ) ;
gCV4_DecRatio = Dcc.getCV( CV_DECCRATIO ) ;
gCVx_LAddr = (Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB ) << 8) +
　　　　　　　　Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB );
//}

@<tt>{#ifdef〜#endif}間のスケッチは紙面の都合上削除しました。

2行目〜 ファンクションピンの割り当てを設定します。

17行目〜 @<tt>{DCC}応答用の負荷ピンを割り当てます。（未使用）

24行目〜 @<tt>{#if !defined(DECODER_DONT_DEFAULT_CV_ON_POWERUP)} は、@<tt>{#define DECODER_DONT_DEFAULT_CV_ON_POWERUP} の
定義が無いので通常は有効になります。
@<tt>{CV1}を@<tt>{EEPROM}から読み出して@<tt>{0xFF}だったら@<tt>{CV}値を工場出荷状態に設定します。
通常起動時はシリアルポートに@<tt>{"CV Not Defaulting"}を出力します。

25行目〜 @<tt>{#define DECODER_DONT_DEFAULT_CV_ON_POWERUP} が定義されているときは、起動時にかならず@<tt>{CV}値を工場出荷状態に変更します。

35行目@<tt>{:Dcc}信号の入力端子を定義します。
@<tt>{Arduino Nano }を使用する場合は、@<tt>{Dcc.pin(0, 2, 0); }が決り構文になりますので変更しないでください。

 引数1の0は、@<tt>{attachInterrupt()}関数の割り込み番号に0番を設定しています。

 引数2の2は、ポート番号@<tt>{ D2(PD2)pin}に設定しています。

 引数3の0は、@<tt>{LOW}の時に割り込みを発生検知に設定しています。

38行目@<tt>{:Dcc}ライブラリの初期化を定義します。
  引数1@<tt>{ CV8}のメーカー個別番号は個人で作成するための@<tt>{ MAN_ID_DIY }で定義している値@<tt>{ 14 }を使用します。

  引数2 メーカーバージョン番号 100

  引数3@<tt>{ FLAGS_MY_ADDRESS_ONLY }自分の@<tt>{DCC}アドレスだけ処理するように設定

  引数4@<tt>{ OpsMode}は未使用

41行目 @<tt>{gPreviousL5}に現在の時刻を代入します。

44行目 @<tt>{gCV1_SAddr}に@<tt>{EEPROM}から@<tt>{CV1}のアドレスを読み込んで代入

45行目 @<tt>{gCV2_VstartにEEPROM}から@<tt>{CV2}のスタート電圧を読み込んで代入

46行目 @<tt>{gCV3_AccRatio}に@<tt>{EEPROM}から@<tt>{CV3}の加速度を読み込んで代入

47行目 @<tt>{gCV4_DecRatio}に@<tt>{EEPROM}から@<tt>{CV4}の減速度を読み込んで代入

48行目 @<tt>{CVx_LAddr}に@<tt>{EEPROM}から@<tt>{CV17,C18}の拡張アドレスを読み込んで代入


=== loop()の実装
@<tt>{Arduino}で用意されている@<tt>{loop()}内のプログラムを永遠に繰り返す関数です。

@<tt>{Dcc}受信関数のコール、@<tt>{100msec}毎にモータを制御しています。

//emlistnum{
void loop(){
	Dcc.process();
	if( (millis() - gPreviousL5) >= 100){
		//Motor drive control
		Motor_Control();
		//Reset task
		gPreviousL5 = millis();
	}
}
//}

2行目 @<tt>{DCC}のコマンド処理を行います。

3行目〜 @<tt>{100ms}周期で@<tt>{Motor_Control()}を読み出して、モータの制御を行います。

7行目 次の@<tt>{100ms}後の処理のために、@<tt>{gPreviousL5}に現在時刻を代入します。


=== motor_ctrl.cpp に含まれている関数
以下の関数が、@<tt>{motor_ctrl.cpp} ファイルに含まれています。

@<tt>{MOTOR_Init() ： }モーター初期化関数

@<tt>{MOTOR_Main() ： }モーターメイン処理関数

@<tt>{MOTOR_Ack() ： }モーターACK関数

=== Motor_Init()の実装
モーターの初期化を行います。
//emlistnum{
void MOTOR_Init()
{
#ifdef SmDecN18
  //D3,D11 PWM キャリア周期:31kHz
  TCCR2B &= B11111000;
  TCCR2B |= B00000001;
  //PWM出力ピン D3,D11を出力にセット
  pinMode(MOTOR_PWM_B, OUTPUT);
  pinMode(MOTOR_PWM_A, OUTPUT);
#else
  //D9,D10 PWM キャリア周期:31kHz
  TCCR1B &= B11111000;
  TCCR1B |= B00000001;
  //PWM出力ピン D9,D10を出力にセット
  pinMode(MOTOR_PWM_B, OUTPUT);
  pinMode(MOTOR_PWM_A, OUTPUT);
#endif
}
//}
11行目〜 @<tt>{SmiLEDecoder}のモータは@<tt>{TIMER1のD9,D10}番ピンを使用しています。
なごでんさんの@<tt>{SmDecN18}の回路のモータドライバは@<tt>{D3,D11}に接続されていますので、@<tt>{#define SmDecN18}を定義
して使用します。
@<tt>{Arduino}のデフォルトは@<tt>{TCCR1B}の下位@<tt>{3bit}は3がセットされて@<tt>{64}分周しています。
周波数を換算すると、@<tt>{f = 16MHz / (64 * 255 * 2) = 490.2Hz}になります。
@<tt>{31kHz}のキャリア周波数にする為@<tt>{TCCRIBの}の下位@<tt>{3bit}に1をセットすると、
@<tt>{f = 16MHz / (1 * 255 * 2) = 31372Hz}になります。


=== Motor_Main()の実装
モータードライバICに@<tt>{PWM}を出力する処理を行います。
//emlistnum{
void MOTOR_Main(int inSpeedCmd, int inDirection)
{
  uint16_t aPWMRef = 0;

  aPWMRef = inSpeedCmd;
  //PWM出力
  if( aPWMRef == 0){
    #ifdef ONBRAKEPWM
      analogWrite(MOTOR_PWM_A, 255);
      analogWrite(MOTOR_PWM_B, 255);
    #else
      analogWrite(MOTOR_PWM_A, 0);
      analogWrite(MOTOR_PWM_B, 0);
    #endif
  } else {
    //進行方向でPWMのABを切り替える
    if( inDirection > 0){
      #ifdef ONBRAKEPWM
        analogWrite(MOTOR_PWM_B, 255);            //Change  by MP6513.
        analogWrite(MOTOR_PWM_A, 255 - aPWMRef);  //Change  by MP6513.
      #else
        analogWrite(MOTOR_PWM_B, 0);
        analogWrite(MOTOR_PWM_A, aPWMRef);
      #endif
    }
    else
    {
      #ifdef ONBRAKEPWM
        analogWrite(MOTOR_PWM_A, 255);            //Change  by MP6513.
        analogWrite(MOTOR_PWM_B, 255 - aPWMRef);  //Change  by MP6513.
      #else
        analogWrite(MOTOR_PWM_A, 0);
        analogWrite(MOTOR_PWM_B, aPWMRef);
      #endif
    }
  }
}
//}
@<tt>{ON}ブレーキタイプのモータドライバと、@<tt>{OFF}ブレーキタイプのモータドライバ両方に対応させています。
@<tt>{#define ONBRAKEPWM}の定義を入れておくと有効になります。

7行目〜 速度コマンドが@<tt>{0}の時は、@<tt>{PWM_A,PWM_B}に@<tt>{0}を出力します。

16行目〜 コマンドステーションからの方向コマンドを元に@<tt>{FWD}か@<tt>{REV}を判断します。

0の時は、@<tt>{PWM_A}に@<tt>{PWM}値を設定。

1の時は、@<tt>{PWM_B}に@<tt>{PWM}値を設定。

このように片方の@<tt>{PWM}に@<tt>{aPEMRef}を出力する事で、@<tt>{FWD/REV}の切り替えが可能となります。



#@#-----------------------------------------------------------------------------
#@# モーターファンクションデコーダを作る
#@# スロット値からモーター出力を市販品並みの処理を行うデコーダ
#@#-----------------------------------------------------------------------------

== BEMF処理に対応したモーターファンクションデコーダを作る

この章からはスロット値からスケール変換してPWM出力を行う簡易版から速度補償を組み込んだモータデコーダを紹介します。
@<img>{motorDecoderL4}の様なブロック図になります。

//image[motorDecoderL4][モーター処理の最終形][scale=1]

市販されているデコーダはモータに対して複数の制御を行うことができる様に作られています。今回は３種類のスピードテーブルを実装しました。

１、リニアスピードテーブル処理

２、@<tt>{CV02}スタート電圧、@<tt>{CV06}中間電圧、@<tt>{CV05}最大電圧の3点を使用したスピードテーブル処理

３、@<tt>{CV29,CV67-94}の@<tt>{28}点スピードテーブルを使用した処理

この3種類のスピードテーブル処理に@<tt>{CV03}の上昇率、@<tt>{CV04}の下降率の処理、

さらに@<tt>{BEMF}処理を実装しました。

※本スケッチでは@<tt>{CV02}スタート電圧、@<tt>{CV06}中間電圧、@<tt>{CV05}最大電圧のパラメータはリニアスピードテーブル、@<tt>{28}点スピードテーブルの処理には
関わらない作りにしました。


=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/FunctionDecoderMotor_L4,[ FunctionDecoderMotor_L4スケッチ ]}

@<tt>{https://github.com/DccShield/FunctionDecoderMotor_L4}

=== スケッチの構成

以下のソースファイルが @<tt>{FunctionDecoderMotro}のスケッチに含まれています。

@<tt>{NmraDcc.cpp　                :　NmraDcc}ライブラリ

@<tt>{NmraDcc.h　                  :　NmraDcc}ライブラリ用ヘッダファイル

@<tt>{DccCV.h                     ：　}定義ファイルとグローバル変数宣言

@<tt>{motor_ctrl.h　               :　motor}ライブラリ用ヘッダファイル

@<tt>{motor_ctrl.h                ：　}定義ファイルとグローバル変数宣言

@<tt>{FunctionDecoderMotor_L4.ino　:　}モーターデコーダスケッチ

=== FunctionDecoderMotor_L4.ino に含まれている関数
以下の関数が、@<tt>{FunctionDecoderMotor_L4.ino} ファイルに含まれています。

@<tt>{setup() ： }初期化関数

@<tt>{loop() ： }ループ関数

@<tt>{resetCVToDefault()          ： CV}テーブルを初期化する関数

@<tt>{notifyDccSpeed()            ： }ノードファイ（通知）@<tt>{DCC}の速度を受信した時に呼ばれる関数

@<tt>{notifyDccFunc()             ： DCC}のファンクション命令を受信した時に呼ばれる関数

@<tt>{exec_function()             ： }ファンクションコマンドによってポート出力する関数

@<tt>{notifyCVResetFactoryDefault()　： CV8}を受信した時に呼ばれる関数

@<tt>{notifyCVAck()               ：　CV ack}を通知する時に呼ばれる関数

@<tt>{notifyCVChange()            ：　CV}値が変化あった時に呼ばれる関数


=== setup()の実装

//emlistnum{
  //ファンクションの割り当てピン初期化
	pinMode(FunctionPin0, OUTPUT);
	digitalWrite(FunctionPin0, 0);

	pinMode(FunctionPin1, OUTPUT);
	digitalWrite(FunctionPin1, 0);

	pinMode(FunctionPin2, OUTPUT);
	digitalWrite(FunctionPin2, 0);

	pinMode(FunctionPin3, OUTPUT);
	digitalWrite(FunctionPin3, 0);

	pinMode(FunctionPin4, OUTPUT);
	digitalWrite(FunctionPin4, 0);
//}

ファンクション用の割り当てとして以下の様に@<tt>{Port3〜Port7}まで割り当てました。なごでんさんのデコーダを使用する場合は@<tt>{PWM}ポートと被っていますのでポート定義の
変更が必要です。
//emlistnum{
  /* Function assigned pins */
  const int FunctionPin0 = 3;
  const int FunctionPin1 = 4;
  const int FunctionPin2 = 5;
  const int FunctionPin3 = 6;
  const int FunctionPin4 = 7;
//}

//emlistnum{
  //Init CVs
  gCV1_SAddr = Dcc.getCV( CV_MULTIFUNCTION_PRIMARY_ADDRESS ) ;
  gCV2_Vstart = Dcc.getCV( CV_VSTART ) ;
  gCV3_AccRatio = Dcc.getCV( CV_ACCRATIO ) ;
  gCV4_DecRatio = Dcc.getCV( CV_DECCRATIO ) ;
  gCV5_VMAX = Dcc.getCV( CV_VMAX ) ;
  gCV6_VMIDDLE = Dcc.getCV( CV_VMIDDLE ) ;
  gCVx_LAddr = (Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB ) << 8) +
                Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB );
  gCV29_Vstart = Dcc.getCV( CV_29_CONFIG ) ;
  gCV10_BEMF_CutOff = Dcc.getCV( CV_BEMFCUTOFF );
  gCV54_BEMFcoefficient = Dcc.getCV( CV_BEMFcoefficient );  // 係数
  gCV55_KP = Dcc.getCV(CV_KP);            // 比例ゲイン(Pゲイン）
  gCV56_KI = Dcc.getCV(CV_KI);            // 積分ゲイン(Iゲイン)
  gCV58_KD = Dcc.getCV(CV_KD);            // 微分ゲイン(Dゲイン)

  //cv29 Direction Check
  if(gCV29_Vstart & 0x01) {
    gCV29Direction = 1; //REVをFWDにする
  } else {
    gCV29Direction = 0;//FWDをFWDにする
  }
  if(gCV29_Vstart & 0x10) {
    gCV29SpeedTable = 1; //スピードテーブル有効
  } else {
    gCV29SpeedTable = 0; //スピードテーブル無効
  }
//}

@<tt>{EEPROM}に格納されている@<tt>{CV}値を読み込み変数にセットするスケッチで以下の@<tt>{CV}値を使用しています。

//table[cvtable][CV一覧]{
CV	機能	グローバル変数	CV記号定数
1	2桁アドレス	gCV1_SAddr	CV_MU.._PR.._AD..
2	スタート電圧	gCV2_Vstart	CV_VSTART
3	加速率	gCV3_AccRatio	CV_ACCRATIO
4	減速率	gCV4_DecRatio	CV_DECCRATIO
5	最大電圧	gCV5_VMAX	CV_VMAX
6	中間電圧	gCV6_VMIDDLE	CV_VMIDDLE
17,18	4桁アドレス	gCVx_LAddr	CV_MU.._EX.._ADDRESS_MSBとLSB
29	設定データ	CV29_Vstart	CV_BEMFCUTOFF
10	BEMF停止スロット値	gCV10_BEMF_CutOff	CV_VMIDDLE
54	BEMF A/D補正係数	gCV54_BEMFcoefficient	CV_BEMFcoefficient
55	BEMF 比例ゲイン	gCV55_KP	CV_KP
56	BEMF 積分ゲイン	gCV56_KI	CV_KI
58	BEMF 微分ゲイン	gCV58_KD	CV_KD
//}

@<tt>{CV29}は1ビット目と2ビット目を使用しています。他のビットは未定義なので設定しても何も変わりません。
//table[cv29][CV29詳細]{
ON/OFF	8bit	7bit	6bit	5bit	4bit	3bit	2bit	1bit
0	-	-	-	-	-	-	スピードテーブル無効	進行方向正
1	-	-	-	-	-	-	スピードテーブル有効	進行方向
//}

//emlistnum{
  //cv29 Direction Check
  if(gCV29_Vstart & 0x01) {
    gCV29Direction = 1; //REVをFWDにする
  } else {
    gCV29Direction = 0; //FWDをFWDにする
  }
  if(gCV29_Vstart & 0x10) {
    gCV29SpeedTable = 1; //スピードテーブル有効
  } else {
    gCV29SpeedTable = 0; //スピードテーブル無効
  }
//}
2行目〜 @<tt>{1bit}目を確認し進行方向を決めています。

7行目〜 スピードテーブルを有効にするかを決めています。
本デコーダはスピードテーブルを無効にするとリニアスピードテーブルか3点スピードテーブルを使用します。

//emlistnum{
  MOTOR_Init();
  MOTOR_SetCV(2, gCV2_Vstart);
  MOTOR_SetCV(3, gCV3_AccRatio);
  MOTOR_SetCV(4, gCV4_DecRatio);
  MOTOR_SetCV(5, gCV5_VMAX);
  MOTOR_SetCV(6, gCV6_VMIDDLE);
  MOTOR_SetCV(10, gCV10_BEMF_CutOff);
  MOTOR_SetCV(29, gCV29_Vstart);
  MOTOR_SetCV(54, gCV54_BEMFcoefficient); // 係数
  MOTOR_SetCV(55, gCV55_KP);            // 比例制御（P制御）
  MOTOR_SetCV(56, gCV56_KI);            // 積分ゲイン(Iゲイン)
  MOTOR_SetCV(58, gCV58_KD);            // 微分ゲイン(Dゲイン)

  for( int i = 67 ; i <= 94 ; i++ ){      // SpeedTable EEPROMから読み込み
    if(gCV29SpeedTable == 1){
      MOTOR_SetCV( i, Dcc.getCV( i ));            // ユーザー設定テーブル
    } else {
      MOTOR_SetCV( i, LinerSpeedTable[ i-67 ] );  // メーカー設定テーブル
    }
  }

  if(gCV29SpeedTable == 1){                       // ユーザースピードテーブルが有効？
    MOTOR_SpeedMode(UserSpeedTableMode);          // ユーザースピードテーブルモード
  } else if(gCV2_Vstart < gCV6_VMIDDLE && gCV6_VMIDDLE < gCV5_VMAX){ // CV2,5,6 の関係が成立？
    MOTOR_SpeedMode(ThreePpointsTableMode);       // 三点スピードテーブルモード
  } else {
    MOTOR_SpeedMode(LinerSpeedTableMode);         // リニアスピードテーブルモード
  }
//}
1行目 モーターのキャリア周波数と@<tt>{PWM}出力ピンを設定します。

2行目〜 モータードライバ側の@<tt>{gParam}構造体に@<tt>{CV}値をセットします。

22行目〜　スピードテーブルのモードを判断します。ユーザースピードテーブルは、3点スピードテーブルか、リニアスピードテーブルかを
判断しています。

モードの優先順位はユーザースピードテーブルが一番高く、次に3点スピードテーブル、最後にリニアスピードテーブルになる様に作られています。

//emlistnum{
void loop(){
	Dcc.process();

  if( (millis() - gPreviousL1) >= 10){
    if(gCV10_BEMF_CutOff != 0){             // CV10 = 0 以外だったらBEMF処理のための誘起電圧測定を実行
      MOTOR_Sensor();
    }
    gPreviousL1 = millis();
  }

	if( (millis() - gPreviousL5) >= 100){
		//Motor drive control
		MOTOR_Main(gSpeedCmd, gPwmDir);
  	//Reset task
		gPreviousL5 = millis();
	}
}
//}
4行目〜　@<tt>{loop()}では、@<tt>{10ms}周期で@<tt>{BEMF}処理を行なっています。@<tt>{CV10 = 0}の時は@<tt>{BEMF}処理は行いません。

11行目〜　@<tt>{100ms}周期でモーター制御を行なっております。


//emlistnum{
extern void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp,
            uint8_t FuncState)
{
	switch(FuncGrp)
	{
		case FN_0_4: //Function Group 1 F0 F4 F3 F2 F1
			exec_function( 0, FunctionPin0, (FuncState & FN_BIT_00)>>4 );
			exec_function( 1, FunctionPin1, (FuncState & FN_BIT_01));
			exec_function( 2, FunctionPin2, (FuncState & FN_BIT_02)>>1);
  		exec_function( 3, FunctionPin3, (FuncState & FN_BIT_03)>>2 );
  		exec_function( 4, FunctionPin4, (FuncState & FN_BIT_04)>>3 );
    	break;

  	case FN_5_8: //Function Group 1 S FFFF == 1 F8 F7 F6 F5 & == 0 F12 F11 F10 F9 F8
  		exec_function( 5, FunctionPin5, (FuncState & FN_BIT_05));
  		exec_function( 6, FunctionPin6, (FuncState & FN_BIT_06)>>1 );
  		exec_function( 7, FunctionPin7, (FuncState & FN_BIT_07)>>2 );
  		exec_function( 8, FunctionPin8, (FuncState & FN_BIT_08)>>3 );
  		break;
//}

@<tt>{notifyDccFunc()}はファンクションコマンドを受信した時に呼ばれれるイベントです。@<tt>{LED}ファンクションデコーダーとはちょっと違った
構文解析になっています。
@<tt>{FnucGrp}に受信されたファンクションのグループが定義されますので、@<tt>{switch case}文で振り分けて、@<tt>{FuncState}に格納されているビット
テストして点灯、消灯させています。


=== motor_ctrl.cpp に含まれている関数
以下の関数が、@<tt>{motor_ctrl.cpp} ファイルに含まれています。

@<tt>{MOTOR_SetCV()     ： CV}値から@<tt>{gParam}構造体にセットする関数

@<tt>{MOTOR_Init()      ： }キャリア周波数、PWMポートを初期化関数

@<tt>{leerp()           ： }ラープ関数、線形補間を行います

@<tt>{pid()             ： pid}演算用関数

@<tt>{MOTOR_Main()      ： }モータ制御関数

@<tt>{MOTOR_Ack()       ： }デコーダー応答用関数

@<tt>{MOTOR_SpeedMode() ： }スピードテーブル設定関数

@<tt>{MOTOR_Sensor()    ： BEMF}処理関数

@<tt>{MOTOR_GetBEMF()   ： }誘起電圧計測関数


=== MOTOR_SetCV()の実装
@<tt>{motoro_ctrl.cpp}では、モータパラメータを@<tt>{gParam}のメンバで管理されているので、デコーダ起動時に@<tt>{CV}値を
@<tt>{EEPROM}から読みだし@<tt>{gParam}構造体へ書き込みます。@<tt>{gParam}構造体は以下の様に定義されています。

//emlist{
Typedef struct _MOTOR_PARAM
{
  byte mSpeedMode;        スピードモードを格納
  byte mStartVoltage;　　　CV2の初期電圧を格納
  byte mMaxVoltage;       CV5の最大電圧を格納
  byte mMiddleVoltage;    CV6の中間電圧を格納
  byte mCV29;             CV29のデコーダ設定値を格納
  byte mAccRatio;         CV3の上昇率を格納
  byte mDecRatio;         CV4の減少率を格納
  byte mBEMFcutoff;       CV10のBEMF有効・無効を格納
  float mBEMFcoefficient; CV54でA/D値とPWMの差を補正する
  float mKp;              CV55の比例制御用のゲイン
  float mKi;              CV56の積分制御用のゲイン
  float mKd;              CV57の微分制御用のゲイン
  unsigned int mSpeedStep[33]; CV67-94のユーザースピードテーブル
} MOTOR_PARAM;
//}

//emlistnum{
void MOTOR_SetCV(byte iNo, byte inData)
{
  switch(iNo){
    case 2:
            gParam.mStartVoltage = inData;
            break;
    case 3:
            gParam.mAccRatio = inData;
            break;
    case 4:
            gParam.mDecRatio = inData;
            break;
//}
メインスケッチ@<tt>{(FunctionDecoderMotor_L4.ino)}からは、@<tt>{MOTOR_Setcv()}関数を使用して、@<tt>{motor_ctrl.cpp}内で使用できる@<tt>{gPramam}構造体に値をセットします。

3行目 @<tt>{iNo}に@<tt>{CV}値が入っていますので、@<tt>{switch case}で分岐処理します。

4行目〜 @<tt>{CV2}の処理、開始電圧を@<tt>{gParam.mStartVoltage}に代入します。

以降、各@<tt>{CV}値を@<tt>{gParam}構造体に代入しています。


=== MOTOR_Init()の実装
キャリア周波数、@<tt>{PWM}ポートを設定します。


=== lerp()の実装
@<tt>{lerp()}線形補完関数ですが、サーボモータファンクションデコーダの説明で@<tt>{map}関数という線形補完関数を使用していますが同じものです。
ちなみに、@<tt>{lerp}（読み方はラープ）は線形補間@<tt>{ (linear interpolate)}の略字となっております。
//emlistnum{
byte lerp(int x0, int y0, int x1, int y1, int x) {
  return y0 + ((y1 - y0) * (x - x0)) / (x1 - x0);
}
//}

=== map関数
@<tt>{Arduino}では数値をある範囲から別の範囲に変換する@<tt>{map()}という関数が用意されています。@<tt>{Arduino}の@<tt>{map}関数は以下にの様に
@<tt>{long}型で計算されています。
@<tt>{lerp()}と若干違う様に見えますが計算の順番が違うだけで処理は同じになります。

//emlistnum{
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//}


=== pid()の実装
スロットル値をそのまま@<tt>{PWM}出力を行いモーターを駆動させると、線路の勾配や牽引する車両の増減によってスロットル値に対する速度が変わってきます。
スロットル値が速度になる様に制御を行うのが@<tt>{PID}制御とういうフィードバック制御を行います。
@<tt>{PID}制御なしで平坦を走らせると@<img>{pid1}の様にスロットル５で、速度５がでます。
//image[pid1][平坦を走らせると][scale=1]

@<img>{pid2}の様に上り坂を走らせると速度低下、下り坂を走らせると速度が早まります。
//image[pid2][坂を走らせると][scale=1]

@<img>{pid3}の様に車両の台数が少ないと速度が早まり、台数が多くなると速度が低下します。S
//image[pid3][牽引数をかえると][scale=1]

@<tt>{pid}制御を行い速度一定の制御を行うと@<img>{pid4}の様に上り坂、下り坂でもスロットル値に対して同じ速度で運転制御できる様になります。
//image[pid4][速度一定制御][scale=1]

@<tt>{PID}制御の説明は他紙、@<tt>{Web}の情報を参考にしてください。

//emlistnum{
byte pid( int spd )
{
  float aP = 0;
  static float aI = 0;
  static float aD = 0;
  static float preP = 0;
  static int pwn = 0;

  aP = spd - gSpeed_calculated; // スロット値 - 誘起電圧（スケーリング済）
  aI = aI + (aP * 0.1);
  aD = ( aP - preP ) / 0.1;
  preP = aP;

  pwn = pwn + (gParam.mKp * aP + gParam.mKi * aI + gParam.mKd * aD);

  if(pwn < 0){
    aP = 0;
    aI = 0;
    aD = 0;
    preP = 0;
    pwn = 0;
  }
  return pwn;
}
//}
3行目 比例制御(Pゲイン)格納用変数です。

4行目 積分制御(Iゲイン)格納用変数です。

5行目 微分制御(Dゲイン)格納用変数です。

6行目 比例制御の前値格納用変数です。

7行目 PWM値格納用変数です。

@<tt>{aI,aD,preP,pwm}の変数は前値を計算で使用するため@<tt>{static}で宣言しています。

9行目 比例制御の計算です。スロット値（目標値）から誘起電圧（現状値）を減算し偏差を求めています。
偏差は最初に図で表しましたが勾配、牽引数等で発生します。この偏差を0にする事で一定速度の制御を行います。

10行目 偏差を時間係数と合わせて加算します。

11行目 比例制御の前回と今回の微分値を算出します。

12行目 前値を現在値に書き換えます。

14行目 比例制御、積分制御、微分制御にそれぞれ係数を乗算しPWM値を算出します。

16行目〜 @<tt>{PWM}計算結果が0以下になったら、比例生業、積分制御、微分制御の変数を初期化します。


=== MOTOR_Main()の実装

モータードライバ@<tt>{IC}に@<tt>{PWM}を出力する処理を行います。
ここでは３種類の、スロープ処理、@<tt>{BEMF}処理を行なっています。

スピードテーブルの処理
//emlistnum{
void MOTOR_Main(byte inSpeedCmd, byte inDirection)
{
  byte aPWMRef = 0;
  byte aPidPWMRef = 0;
  static int aNowPWMRef = 0;
  byte aSubcal = inSpeedCmd >> 3;  // inSpeedCmd/8
  static int aDelay = 0;

  if(gParam.mSpeedMode == LinerSpeedTableMode ||
                   gParam.mSpeedMode == UserSpeedTableMode){ // SpeedTable有効
    aPWMRef = lerp(aSubcal*8 , gParam.mSpeedStep[aSubcal] , (aSubcal+1)*8 ,
                                   gParam.mSpeedStep[aSubcal+1] , inSpeedCmd);
  } else {                         // 3点SpeedTable有効
    // スタート電圧〜中間電圧間を補間
    if(gParam.mStartVoltage <= inSpeedCmd && gParam.mMiddleVoltage >= inSpeedCmd){
      aPWMRef = lerp(1,gParam.mStartVoltage,127,gParam.mMiddleVoltage,inSpeedCmd);

    // 中間電圧間〜最大電圧間を補間
    } else if(gParam.mMiddleVoltage < inSpeedCmd && gParam.mMaxVoltage >= inSpeedCmd){
      aPWMRef = lerp(127,gParam.mMiddleVoltage,255,gParam.mMaxVoltage,inSpeedCmd);
    }
 }
}
//}
6行目 スピードテーブルは@<tt>{28}段階なので@<tt>{aSubcal}変数に@<tt>{0-255}にスケーリングされた速度コマンドを@<tt>{8}で除算した値を代入しています。
8で除算するというのがマジックナンバーですが、@<tt>{255}時に@<tt>{255/8=31.875≒32}となりスピードテーブルの@<tt>{28}を超えていますが、本スケッチでは
@<tt>{MOTOR_Init()}内で残り@<tt>{32}までを@<tt>{255}として定義しています。@<tt>{floatMap()}を使用して速度コマンド@<tt>{0-255}をスピードテーブル@<tt>{1-28}に
変換してもよいです（ただし処理速度が大幅に増えます）

9行目〜リニアテーブルまたはユーザーテーブル設定時の処理です。@<tt>{lerp()}関数を使用してスピードテーブルから@<tt>{PWM}値を線形補完した値を取得します。
//image[pid5][テーブルからPWM値を線形補完][scale=1]
横軸を速度、縦軸はスピードテーブルから取得した@<tt>{PWM}値として速度から線形補完し@<tt>{PMW}値を取得します。

12行目〜 3点スピードテーブル時はスタート電圧〜中間電圧、中間電圧〜最高電圧と区間を分け、@<tt>{lerp()}関数を使用してPWM値を線形補完した値を取得します。

=== スイープ処理

@<tt>{CV3}と@<tt>{CV4}で設定する加速率と減速率を処理します。

//emlistnum{
    if(aNowPWMRef < aPWMRef){         // 増加率計算の場合
      if(gParam.mAccRatio != 0 ){
        aDelay++;
        if(aDelay < gParam.mAccRatio){  // Delay時間に達していなかったら抜ける
          return;
        } else {
          aDelay = 0;                   // Delay時間に達していたら、クリアする。
          aNowPWMRef++;
          if(aNowPWMRef >= aPWMRef)
            aNowPWMRef = aPWMRef;
        }
      } else {
      aNowPWMRef = aPWMRef;
      }
    }

    if(aNowPWMRef > aPWMRef) {                          // 減少率計算の場合
      if(gParam.mDecRatio != 0 ){
        aDelay++;
        if(aDelay < gParam.mDecRatio){  // Delay時間に達していなかったら抜ける
          return;
        } else {
          aDelay = 0;                   // Delay時間に達していたら、クリアする。
          aNowPWMRef--;
          if(aNowPWMRef <= aPWMRef)
            aNowPWMRef = aPWMRef;
        }
      } else {
      aNowPWMRef = aPWMRef;
      }
    }
//}

@<tt>{MOTOR_Main()}は@<tt>{100ms}周期に実行されますので@<tt>{aDelay++}の実行も@<tt>{100ms}周期でカウントアップします。
このスロープ処理は@<tt>{100ms * CV}値の設定までカウントアップし@<tt>{PWM}値を@<tt>{1}加算または@<tt>{1}減算し最終値まで増加または減算します。
@<tt>{CV3}または@<tt>{CV4}が@<tt>{0}ですと最速、@<tt>{1}が設定されていると@<tt>{100ms}間隔で@<tt>{PWM}値を変化、@<tt>{10}が設定されていると@<tt>{1s}間隔で@<tt>{PWM}を変化
できます。

1行目〜増加計算

17行目〜減算処理

=== BEMF処理

実計算は@<tt>{pid()}関数内で行われます。

//emlistnum{
    aPidPWMRef = pid(aNowPWMRef);
//}


速度が@<tt>{0}司令の時は@<tt>{PWM}値を@<tt>{0}に設定します。

//emlistnum{
    if(inSpeedCmd == 0){  // 速度0の時はPWMも0(ofF)
      aNowPWMRef  = 0;
    }
//}

=== PWM出力部

方向コマンドによる@<tt>{PWM}値の処理、モータドライバーの種類による@<tt>{PWM}波形の処理を行います。

//emlistnum{
      if( inDirection > 0){
        #ifdef ONBRAKEPWM
          analogWrite(MOTOR_PWM_B, 255);            //Change  by MP6513.
          analogWrite(MOTOR_PWM_A, 255 - aPidPWMRef);  //Change  by MP6513.
        #else
          analogWrite(MOTOR_PWM_B, 0);
          analogWrite(MOTOR_PWM_A, aPidPWMRef);
        #endif
      } else {
        #ifdef ONBRAKEPWM
          analogWrite(MOTOR_PWM_A, 255);            //Change  by MP6513.
          analogWrite(MOTOR_PWM_B, 255 - aPidPWMRef);  //Change  by MP6513.
        #else
          analogWrite(MOTOR_PWM_A, 0);
          analogWrite(MOTOR_PWM_B, aPidPWMRef);
        #endif
      }

    gDirection = inDirection;
    gPWMRef = aPidPWMRef;
  }
//}
1行目〜　前進処理。@<tt>{#ifdef ONBRAKEPWM}で分けていますが、モータドライバ@<tt>{IC}の種類によりオンブレーキタイプかそれ以外かを切り分けています。

9行目〜　後進処理

=== モーターの誘起電圧を計測した値からPWM値にスケーリング

//emlistnum{
void MOTOR_Sensor()
{
  int aBEMF_result = 0;

  aBEMF_result = MOTOR_GetBEMF();

  // 計測したした誘起電圧をCV54の1/10の定数でPWM値に合わせる。
  gSpeed_calculated = (float)aBEMF_result / gParam.mBEMFcoefficient ;

  if(gSpeed_calculated > 255){
    gSpeed_calculated = 255;
  }
}
//}

5行目　モータの誘起電圧を計測します。

6行目  測したした誘起電圧を@<tt>{CV54}の@<tt>{1/10}の定数で@<tt>{PWM}値のスケーリングを行います。

@<tt>{Tomix M-9（タイプ7）}モータを使用した時の速度対誘起電圧データをご紹介します。
@<img>{ad1}速度を@<tt>{10step}ずつ上昇させ同時に誘起電圧データを測定しました。
@<tt>{4}回測定した結果、速度@<tt>{40}からモータが回り出します。以降は直線的（リニア）な比例関係になっています。このデータからわかることは@<tt>{PWM}幅を@<tt>{40}以上上げないとモータが回りません。
デフォルトで@<tt>{CV54 = 19}に設定していますので、誘起電圧値から@<tt>{1.9}で除算して、速度値に近づけています。

//image[ad1][速度対誘起電圧][scale=1]

//image[ad2][速度対誘起電圧グラフ][scale=1]

=== 誘起電圧を計測
誘起電圧の計測は@<tt>{PWM}出力を@<tt>{OFF}にして、@<tt>{300ms}後に@<tt>{8}回サンプリングし平均値を算出しています。

//emlistnum{
  int MOTOR_GetBEMF()
  {
    uint32_t aAve = 0;
    int i;

    //Stop PWM
    #ifdef ONBRAKEPWM
      analogWrite(MOTOR_PWM_B, 255);  //Change  by MP6513.
      analogWrite(MOTOR_PWM_A, 255);  //Change  by MP6513.
    #else
      analogWrite(MOTOR_PWM_B, 0);
      analogWrite(MOTOR_PWM_A, 0);
    #endif

    delayMicroseconds(300);

    for( i = 0; i < 8; i++){
     aAve = aAve + analogRead(A0);
     delayMicroseconds(30);
    }

    //MaxValueCheck
    if( gPWMRef >= 255){
        gPWMRef = 255;
    }

    //進行方向でPWMのABを切り替える
    if( gDirection > 0){
    #ifdef ONBRAKEPWM
      analogWrite(MOTOR_PWM_B, 255);            //Change  by MP6513.
      analogWrite(MOTOR_PWM_A, 255 - gPWMRef);  //Change  by MP6513.
    #else
      analogWrite(MOTOR_PWM_B, 0);
      analogWrite(MOTOR_PWM_A, gPWMRef);
    #endif
    } else {
    #ifdef ONBRAKEPWM
      analogWrite(MOTOR_PWM_A, 255);            //Change  by MP6513.
      analogWrite(MOTOR_PWM_B, 255 - gPWMRef);  //Change  by MP6513.
    #else
      analogWrite(MOTOR_PWM_A, 0);
      analogWrite(MOTOR_PWM_B, gPWMRef);
    #endif
    }
    return (aAve >> 3);
  }
//}
7行目〜 @<tt>{PWM}出力を@<tt>{OFF}にします。

15行目　@<tt>{300ms}待ちます。
※@<tt>{loop()}関数から、@<tt>{10ms}周期に@<tt>{MOTOR_Sensor()}関数を呼んでいますが、デコーダーはタイムスライス処理で多岐の機能を短時間で実行し、
他の処理が滞らないようにする作りをしているのですが、ここで@<tt>{300ms}という長めのウエイトを入れるのはあまり良い作りではありません。

17行目〜　@<tt>{30us}の間隔で@<tt>{8}回サンプリングします。

23行目〜 @<tt>{PWM}幅リミッタ処理

28行目〜　@<tt>{PWM}出力を@<tt>{OFF}にする前の@<tt>{PWM}出力開始

45行目〜　@<tt>{8}で除算して@<tt>{8}回サンプリングを積算した値から平均値を算出します。
@<tt>{>>}はビットシフト演算子で、@<tt>{>>1}で@<tt>{2}で除算、@<tt>{>>2}で@<tt>{4}で除算、@<tt>{>>3}で@<tt>{8}で除算するのと等価になります。
※マイコンなので@<tt>{/8}の除算処理は時間がかかるので割る基数は固定になりますがビットシフト演算は数ステップで処理ができます。
※一方他のスケッチで@<tt>{float}型や@<tt>{long}型を多用しているので本末転倒ですが原理を説明するために多用しています。
なごでんさんのスケッチを見ていただくと@<tt>{float}型や@<tt>{long}を使わずに工夫して少ないビット数で処理する作りになっており@<tt>{Flash}の容量を節約できるのと
処理速度が早くなります。


//embed[latex]{
\clearpage
//}

#@#-----------------------------------------------------------------------------
#@# サーボモータファァンクションデコーダを作る
#@#-----------------------------------------------------------------------------

== サーボモータファァンクションデコーダを作る
@<tt>{Arduino Nano DCC Shield}に@<tt>{2}台のサーボモータを取り付けたファンクションデコーダを作ります。
@<tt>{CV}値で各パラメータが変更できるようにしております。
@<tt>{OFF}時の角度、@<tt>{ON}時の角度、@<tt>{OFF->ON}の移動時間、@<tt>{ON->OFF}の移動時間を@<tt>{CV}値のパラメータとして設定ができます。
//image[servoFunc1][パラメータ][scale=1]
//table[][CV一覧表]{
CV	初期値	機能
47	180	ServoA ON時の角度
48	0	ServoA OFF時の角度
49	0	ServoA 電源切る前の角度
50	5	ServoA OFF->ONのスピード sec
51	1	ServoA ON->OFFのスピード sec
52	0	ServoA gDirの最新値保存用 STR/DIV
53	0	ServoA のファンクション番号 F0
54	180	ServoB ON時の角度
55	0	ServoB OFF時の角度
56	0	ServoB 電源切る前の角度
57	5	ServoB OFF->ONのスピード sec
58	1	ServoB ON->OFFのスピード sec
59	0	ServoB gDirの最新値保存用 STR/DIV
60	0	ServoB のファンクション番号 F0
//}

※@<tt>{CV47}〜割り当てているのは、@<tt>{NMRA}の規格書で、
@<href>{https://www.nmra.org/sites/default/files/standards/sandrp/pdf/s-9.2.2_decoder_cvs_2012.07.pdf,[Configuration Variables For Digital Command Control,All Scales ]}
@<tt>{CV47〜CV64}は@<tt>{Manufacturer Unique}とユーザが自由に使えるように定義されているため、使用しています。

=== 完成写真
//image[servofunctiondecoder1][サーボモータファンクションデコーダ][scale=0.8]

=== 接続図
//image[handa26][サーボモータの接続][scale=1]

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/FunctionDecoderServo,[ サーボモータファンクションデコーダスケッチ ]}

@<tt>{https://github.com/DccShield/FunctionDecoderServo}

=== スケッチの構成

以下のソースファイルが サーボモータファンクションデコーダに含まれています。

@<tt>{FunctionDecoderServo.ino : }サーボモータファンクションデコーダ本体のスケッチ

@<tt>{NmraDcc.cpp              : NmraDcc}ライブラリ

@<tt>{NmraDcc.h                : NmraDcc}ライブラリ用ヘッダファイル

@<tt>{DccCV.h                  : }定義ファイルとグローバル変数宣言

@<tt>{ServoDrv.cpp             : }サーボモータ用ライブラリ（ファンクションデコーダ用）

@<tt>{ServoDrv.h               : }サーボモータ用ライブラリのヘッダファイル

=== FunctionDecoderServo.ino に含まれている関数
以下の関数が、@<tt>{FunctionDecoderServo.ino}ファイルに入っています。

@<tt>{ServoInit()}

@<tt>{setup()}

@<tt>{loop()}

@<tt>{FunctionProcess()}

@<tt>{notifyDccSpeed()}

@<tt>{notifyDccFunc()}

@<tt>{resetCVToDefault()}

@<tt>{notifyCVResetFactoryDefault()}

@<tt>{notifyCVAck()}

@<tt>{MOTOR_Ack()}

@<tt>{Dccinit()}

@<tt>{notifyCVChange()}

@<tt>{notifyDccReset()}

=== 各種設定、宣言

@<tt>{FunctionDecoderLED.ino}とかなり似てきますがサーボ用を使用する設定が追加になっています。

//emlistnum{
  #define PIN_F0_F  3     // D3 PD3,PWM
  #define PIN_F0_R  4     // D4 PD4
  #define PIN_AUX1  5     // D5 PD5
  #define PIN_AUX2  6     // D6 PD6
  #define PIN_AUX3  7     // D7 PD7
  #define PIN_AUX4  8     // D8 PB0
  #define PIN_AUX5  9     // D9 PB1,DIGITAL TR,PWM
  #define PIN_AUX6  10    // D10 PB2,DIGITAL TR,PWM
  #define PIN_AUX7  11    // D11 PB3,DIGITAL TR,PWM

  #define PIN_SERVO1 5    // D5 PD5
  #define PIN_SERVO2 4    // D4 PD4
  #define PIN_LED_DIV1 6  // D6 PD6 SERVO1用DIV(分岐) LED
  #define PIN_LED_STR1 7  // D7 PD7 SERVO1用STR(直進) LED
  #define PIN_LED_DIV2 8  // D8 PB0 SERVO2用DIV(分岐) LED
  #define PIN_LED_STR2 9  // D9 PB1 SERVO2用STR(直進) LED

  void Dccinit(void);

  //使用クラスの宣言
  NmraDcc   Dcc;
  DCC_MSG  Packet;
  ServoDriver ServoCH0;
  ServoDriver ServoCH1;

  struct CVPair {
    uint16_t  CV;
    uint8_t Value;
  };

  CVPair FactoryDefaultCVs [] =
  {
    {CV_MULTIFUNCTION_PRIMARY_ADDRESS, DECODER_ADDRESS},
    {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
    {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0},
    {CV_29_CONFIG, 128 },
    {47, 180}, // ServoA ON時の角度
    {48, 0},   // ServoA OFF時の角度
    {49, 0},   // ServoA 電源切る前の角度
    {50, 5},   // ServoA OFF->ONのスピード sec
    {51, 1},   // ServoA ON->OFFのスピード sec
    {52, 0},   // ServoA gDirの最新値保存用 STR/DIV
    {53, 0},   // ServoA のファンクション番号 F0

    {54, 180}, // ServoB ON時の角度
    {55, 0},   // ServoB OFF時の角度
    {56, 0},   // ServoB 電源切る前の角度
    {57, 1},   // ServoB OFF->ONのスピード sec
    {58, 1},   // ServoB ON->OFFのスピード sec
    {59, 0},   // ServoB gDirの最新値保存用 STR/DIV
    {60, 1},   // ServoB のファンクション番号 F1
  };

  void(* resetFunc) (void) = 0;  //declare reset function at address 0

  uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);

  void notifyDccReset(uint8_t hardReset );
//}
11行目〜　上の@<tt>{PIN_AUX}と一部被りますが、サーボ用の@<tt>{I/O、DIV.STR}を表現する@<tt>{LED}のポートを
宣言しています。

23行目〜　今回は@<tt>{2ch}に対応するため、@<tt>{ServoDriver}クラスから@<tt>{ServoCH0}と@<tt>{ServoCH1}を実態化しています。

31行目〜　サーボモータ用@<tt>{CV}値を格納するテーブルを作成。

=== loop()の実装
@<tt>{Arduino}で用意されている@<tt>{loop()}内のプログラムを永遠に繰り返す関数です。

初回@<tt>{500ms}間はサーボの処理を行わないようにしています。

起動後@<tt>{10msec}毎にサーボモータの状態遷移を動かしています。

//emlistnum{
  void loop() {

    Dcc.process();

    if(ServoEnable == 0 && ( millis() - gPreviousL5) >=500){
      ServoEnable = 1;
    }

    if(ServoEnable == 1){
      if ( (millis() - gPreviousL5) >= 10)
      {
        ServoCH0.stateCheck();
        ServoCH1.stateCheck();
        gPreviousL5 = millis();

      }
    }
  }
//}
5行目　@<tt>{DCS50K}の場合、初回通信で0を送るため、デコーダで保持している状態と不一致になってしまうので、
@<tt>{500ms}待ってからサーボの状態遷移を動かすようにしています。

9行目　サーボモータの処理。@<tt>{10ms}単位で動作させています。

=== ServoInit()の実装
//emlistnum{
  ServoCH0.ch = 0;
  ServoCH0.port = PIN_SERVO1;
  ServoCH0.onDeg = Dcc.getCV( 47 );    // ON時の角度
  ServoCH0.offDeg = Dcc.getCV( 48 );   // OFF時の角度
  ServoCH0.initDeg = Dcc.getCV( 49 );  // 電源切る前の角度
  ServoCH0.onSpeed = Dcc.getCV( 50 );  // OFF->ONのスピード
  ServoCH0.offSpeed = Dcc.getCV( 51 ); // ON->OFFのスピード
  ServoCH0.sdir = Dcc.getCV( 52 );     // gDirの最新値保存用 STR/DIV
  ServoCH0.cv = 52;
  ServoCH0.servoAddress = Dcc.getCV( 53 );// サーボモータをON/OFFするファンクション番号
  ServoCH0.STR = PIN_LED_STR1;
  ServoCH0.DIV = PIN_LED_DIV1;
  ServoCH0.MinAngle = 670;
  ServoCH0.MaxAngle = 2600;
  ServoCH0.Set();

  ServoCH1.ch = 1;
  ServoCH1.port = PIN_SERVO2;
  ServoCH1.onDeg = Dcc.getCV( 54 );    // ON時の角度
  ServoCH1.offDeg = Dcc.getCV( 55 );   // OFF時の角度
  ServoCH1.initDeg = Dcc.getCV( 56 );  // 電源切る前の角度
  ServoCH1.onSpeed = Dcc.getCV( 57 );  // OFF->ONのスピード
  ServoCH1.offSpeed = Dcc.getCV( 58 ); // ON->OFFのスピード
  ServoCH1.sdir = Dcc.getCV( 59 );     // gDirの最新値保存用 STR/DIV
  ServoCH1.cv = 59;
  ServoCH1.servoAddress = Dcc.getCV( 60 );// サーボモータをON/OFFするファンクション番号
  ServoCH1.STR = PIN_LED_STR2;
  ServoCH1.DIV = PIN_LED_DIV2;
  ServoCH1.MinAngle = 400;
  ServoCH1.MaxAngle = 2100;
  ServoCH1.Set();
//}
サーボモータパラメータの初期設定を行なっています。

=== FunctionProcess()の実装
今回のサーボモータスケッチでは使用していません。

=== notifyDccSpeed()の実装
今回のサーボモータスケッチでは使用していません。

=== notifyDccFunc()の実装
ファンクションコマンド受信した時の処理です。

詳細は「@<tt>{LED}ファンクションデコーダを作る」のページを参照してください。

=== resetCVToDefault()の実装
詳細は「@<tt>{LED}ファンクションデコーダを作る」のページを参照してください。

=== notifyCVResetFactoryDefault()の実装
詳細は「@<tt>{LED}ファンクションデコーダを作る」のページを参照してください。

=== notifyCVAck()の実装
デコーダの@<tt>{ACK}は未対応

=== MOTOR_Ack()の実装
デコーダの@<tt>{ACK}は未対応

=== Dccinit()の実装
デコーダの初期化処理

=== notifyCVChange()の実装
特に何もしません。

=== notifyDccReset()の実装
特に何もしません。


=== ServoDrv.cpp に含まれている関数
以下の関数が、@<tt>{ServoDrv.cpp} ファイルに入っています。

@<tt>{mapfloat()                 : map()}関数の@<tt>{flaot}版です。

@<tt>{ServoDriver::ServoDriver() : }コンストラクタ

@<tt>{ServoDriver::nowState()    ： }ステートマシンのステートを確認する関数

@<tt>{ServoDriver::writeCV()     ： CV}値を@<tt>{EEPROM}に書き込む関数

@<tt>{ServoDriver::SVattach()    ： Servo}を有効にします。

@<tt>{ServoDriver::servoABwrite()： Servo}に@<tt>{PWM}を設定します。

@<tt>{ServoDriver::SVdetach()    ： Servo}を無効にします。

@<tt>{ServoDriver::LED()         ： div/str}の@<tt>{LED}を制御します。

@<tt>{ServoDriver::stateCheck()  ： }ステートマシン

@<tt>{ServoDriver::gState()      ： }

=== mapfloat()の実装
//emlistnum{
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//}
@<tt>{Arduino}の関数として用意されている@<tt>{map()}関数の@<tt>{float}版で、角度から@<tt>{PWM}幅へのスケール変換関数として
使用しています。

//image[servoFunc2][２点を通る直線の方程式][scale=1]

原理は２点を通る直線の方程式を使用してスケール変換しています。

//image[servoFunc3][スケール変換][scale=1]

=== ServoDriver()の実装
//emlistnum{
  ServoDriver::Set()
  {
    pinMode(port, OUTPUT);
    digitalWrite(port, HIGH);

    lPW.onDeg = map((float)onDeg,offDeg,onDeg,MinAngle,MaxAngle);  // ON時の角度

    lPW.offDeg = map((float)offDeg,offDeg,onDeg,MinAngle,MaxAngle);// OFF時の角度

    lPW.onDelta = mapfloat(((float)(onDeg - offDeg) / onSpeed / 100),
      (float)offDeg,(float)onDeg,0,(float)(MaxAngle-MinAngle));
      // offDegからonDegまでの10ms時の移動量を算出

    lPW.offDelta = mapfloat(((float)(offDeg - onDeg) / offSpeed / 100),
      (float)offDeg,(float)onDeg,0,(float)(MaxAngle-MinAngle));
      // offDegからonDegまでの10ms時の移動量を算出

    lPW.nowDeg = 0;

    SERVO.attach(port, MinAngle, MaxAngle);
    delay(20);
    SERVO.detach();

    state = ST_STANDABY;
  }

//}
1行目〜　@<tt>{ServoDriver}クラスのコンストラクタです。※本当は@<tt>{ServoDriver::ServoDriver()}ですが
オブジェクト生成時にはサーボモータのパラメータ初期化ができないので、実態は@<tt>{Set()}で行なっています。

3、4行目　@<tt>{Arduino Nano}のポート番号から@<tt>{I/O}を初期化します。

6行目〜 @<tt>{ServoDriver}クラスでは、サーボモータの制御は角度では無く@<tt>{PWM}幅（時間）で管理しているため、
角度から時間へ@<tt>{map}関数を使用して変換しています。またあらかじめ@<tt>{OFF->ON}と@<tt>{ON->OFF}の@<tt>{10ms}毎に変化させる@<tt>{Δt}（サーボモータの@<tt>{PWM}変化量)
を算出しています。

20行目〜 @<tt>{attach}してサーボモータを使えるようにしています。@<tt>{detach()}しているのは、サーボモータへの@<tt>{PWM}を停止させて、
トルクを０にしています。

※サーボが角度を維持する力が０になりますので、サーボホーンに力が掛かっていると角度の保持ができません。


=== writeCV()の実装
//emlistnum{
void ServoDriver::writeCV(void)
{
  Dcc.setCV(cv,gState_Function);        // 最終値のアクセサリ番号をCV_sdirに書き込み
}
//}
サーボモータの位置を都度@<tt>{EEPROM}に書き込みます。デコーダが再起動した時にリアルの位置（角度）を取り込み、
プログラムが管理している角度とサーボモータの角度が不一致になる事を防止しています。
起動時に不一致になると最速で@<tt>{OFF->ON}または@<tt>{ON->OFF}に動く可能性があるので、レイアウトを壊しかねません。

=== LED()の実装
//emlistnum{
void ServoDriver::LED( void )
{
  if(gState_Function == 0){ //0:div/ 1:str|
    digitalWrite(STR,OFF);
    digitalWrite(DIV,ON);
  } else {
    digitalWrite(STR,ON);
    digitalWrite(DIV,OFF);
  }
}
//}
@<tt>{gState_Function}の値で@<tt>{DIV}の@<tt>{LED}を点灯、@<tt>{STR}の@<tt>{LED}を点灯させています。

=== stateCheck()の実装
//emlistnum{
void ServoDriver::stateCheck()
{
  gState();

  switch(state){
    case ST_STANDABY:               // 起動時一回だけの処理
        LED();
        if(gState_Function == sdir ){ // 前回最後のSTR/DIVが同じ？
          if(gState_Function == 0){   // OFF?
            SVattach();
            lPW.nowDeg = lPW.offDeg;
            ServoWite((int)lPW.nowDeg);
          } else {                    // ON?
            SVattach();
            lPW.nowDeg = lPW.onDeg;
            ServoWite((int)lPW.nowDeg);
          }
          SVdetach();
          state = ST_IDLE;
          break;
        } else { // EEPROMの状態とコマンドステーションが異なっていた時の初回処理
          if(sdir != 0 and gState_Function == 0){
              nextDeg = lPW.offDeg;
              nextDelta = lPW.offDelta;
              state = ST_RUN;
          } else {
              nextDeg = lPW.onDeg;
              nextDelta = lPW.onDelta;
              state = ST_RUN;
          }
        }
        break;

    case ST_IDLE:
        if(gState_Function == 0 ){           // ServoB:OFF
          if(lPW.nowDeg == lPW.offDeg){   // 最終値まで行っていたら抜ける
            state = ST_IDLE;
            return;
          }
        SVattach();
        nextDeg = lPW.offDeg;
        nextDelta = lPW.offDelta;
        } else if(gState_Function != 0 ){    // ServoB:ON
          if(lPW.nowDeg == lPW.onDeg){    // 最終値まで行っていたら抜ける
              state = ST_IDLE;
              return;
          }
          SVattach();
          nextDeg = lPW.onDeg;
          nextDelta = lPW.onDelta;
        }
        if(lPW.nowDeg - nextDeg < 0){
          updownFlg = UP;
        } else {
          updownFlg = DOWN;
        }
        state = ST_RUN;
        break;

    case ST_RUN:
        ServoWite((int)lPW.nowDeg);
        lPW.nowDeg = lPW.nowDeg + nextDelta;
        // 下りONまで行った？ or 上りONまで行った？
        if( ((updownFlg == DOWN) && (lPW.nowDeg <= nextDeg)) ||
        　　　　　　　　　　　((updownFlg == UP) && (lPW.nowDeg >= nextDeg)) ) {
          lPW.nowDeg = nextDeg;
          ServoWite((int)nextDeg);
          writeCV();
          SVdetach();
          LED();
          state = ST_IDLE;
        }
        break;
      default:
          break;
    }
  }
//}

3行目　サーボモータに割り当てられたファンクション@<tt>{Fn}から、@<tt>{NmraDcc}ライブラリで管理している@<tt>{gState＿Fn}変数に
格納されている状態を取得する関数。@<tt>{gState_Function}変数に@<tt>{STR/DIV}の情報を取り寄せます。

5行目〜　サーボモータステートマシン処理

6行目〜　起動時一度だけ処理させるプログラムです。@<tt>{EEPROM}に格納してあった@<tt>{STR/DIV}の情報と@<tt>{DCC}で受信した情報を
比較して同じだったらステータス類を更新して、@<tt>{ST_IDLE}に遷移
異なっていた場合は@<tt>{DCC}で受信した状態でサーボモータを動かすため、@<tt>{ST_RUN}に遷移します。

34行目〜　@<tt>{DCC}からコマンドを待つステートです。@<tt>{STR/DIV}に変化があったら、次のパルス幅、サーボモータのΔTをセットして
@<tt>{ST_RUN}に遷移します。

60行目〜　サーボモータを動作させるステート。@<tt>{10ms}毎に呼ばれるので、あらかじめ計算しておいた@<tt>{ΔV}のパルス幅を
都度設定していき、最終地まで達したら、@<tt>{ST_IDLE}に遷移します。

//embed[latex]{
\clearpage
//}

#@#-----------------------------------------------------------------------------
#@# サーボモータアクセサリデコーダを作る
#@#-----------------------------------------------------------------------------

== サーボモータアクセサリデコーダを作る
@<tt>{Arduino Nano  DCC Shield}に2台のサーボモータを取り付けたアクセサリデコーダを作ります。
サーボモータファンクションデコーダと似ていますが、ファンクションで受信する部分をアクセサリで受信できる
ように一部修正しています。よって、プログラムの説明を一部割愛しています。

//table[][CV一覧表]{
CV	初期値	機能
47	180	ServoA ON時の角度
48	0	ServoA OFF時の角度
49	0	ServoA 電源切る前の角度
50	5	ServoA OFF->ONのスピード sec
51	1	ServoA ON->OFFのスピード sec
52	0	ServoA gDirの最新値保存用 STR/DIV
53	0	ServoA のファンクション番号 F0
54	180	ServoB ON時の角度
55	0	ServoB OFF時の角度
56	0	ServoB 電源切る前の角度
57	5	ServoB OFF->ONのスピード sec
58	1	ServoB ON->OFFのスピード sec
59	0	ServoB gDirの最新値保存用 STR/DIV
60	0	ServoB のファンクション番号 F0
//}

※@<tt>{CV47}〜割り当てているのは、@<tt>{NMRA}の規格書で、
@<href>{https://www.nmra.org/sites/default/files/standards/sandrp/pdf/s-9.2.2_decoder_cvs_2012.07.pdf,[Configuration Variables For Digital Command Control,All Scales ]}
@<tt>{CV47〜CV64} は @<tt>{Manufacturer Unique} とユーザが自由に使えるように定義されているため、使用しています。

=== 完成写真
//image[servofunctiondecoder1][サーボモータアクセサリデコーダ][scale=0.8]

=== 接続図
//image[handa26][サーボモータの接続][scale=1]

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/AccDecoderServo,[ サーボモータアクセサリデコーダスケッチ ]}

@<tt>{https://github.com/DccShield/AccDecoderServo}

=== スケッチの構成

以下のソースファイルが サーボモータアクセサリデコーダに含まれています。

@<tt>{AccDecoderServo.ino　:}　サーボモータアクセサリデコーダ本体のスケッチ

@<tt>{NmraDcc.cpp　　　　　　:　NmraDcc}ライブラリ

@<tt>{NmraDcc.h　　　　　　　:　NmraDcc}ライブラリ用ヘッダファイル

@<tt>{DccCV.h　　　　　　　　:}　定義ファイルとグローバル変数宣言

@<tt>{ServoDrv.cpp　　　　　:}　サーボモータ用ライブラリ（ファンクションデコーダ用）

@<tt>{ServoDrv.h　　　　　　:}　サーボモータ用ライブラリのヘッダファイル

=== FunctionDecoderServo.ino に含まれている関数
以下の関数が、@<tt>{AccDecoderServo.ino} ファイルに入っています。

@<tt>{ServoInit()}

@<tt>{setup()}

@<tt>{loop()}

@<tt>{FunctionProcess()}

@<tt>{notifyDccSpeed()}

@<tt>{notifyDccFunc()}

@<tt>{resetCVToDefault()}

@<tt>{notifyCVResetFactoryDefault()}

@<tt>{notifyCVAck()}

@<tt>{MOTOR_Ack()}

@<tt>{Dccinit()}

@<tt>{notifyCVChange()}

@<tt>{notifyDccReset()}

=== 各種設定、宣言

@<tt>{FunctionDecoderServo.ino}とほぼ同じですが、@<tt>{CV29}の設定がアクセサリでコーダに変更しています。

//emlistnum{
  CVPair FactoryDefaultCVs [] =
  {
    {CV_ACCESSORY_DECODER_ADDRESS_LSB, 1},
    {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
    {CV_29_CONFIG, 0b10000000},  // CV29 Software sw CV29＝128 アクセサリデコーダ
    {47, 180}, // ServoA ON時の角度
    {48, 0},   // ServoA OFF時の角度
    {49, 0},   // ServoA 電源切る前の角度
    {50, 5},   // ServoA OFF->ONのスピード sec
    {51, 1},   // ServoA ON->OFFのスピード sec
    {52, 0},   // ServoA gDirの最新値保存用 STR/DIV
    {53, 0},   // ServoA のファンクション番号 F0

    {54, 180}, // ServoB ON時の角度
    {55, 0},   // ServoB OFF時の角度
    {56, 0},   // ServoB 電源切る前の角度
    {57, 1},   // ServoB OFF->ONのスピード sec
    {58, 1},   // ServoB ON->OFFのスピード sec
    {59, 0},   // ServoB gDirの最新値保存用 STR/DIV
    {60, 1},   // ServoB のファンクション番号 F1
  };
//}

=== loop()の実装
//emlistnum{
void loop(){
  Dcc.process();

  if( FactoryDefaultCVIndex && Dcc.isSetCVReady()){
    FactoryDefaultCVIndex--;
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV,
      FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
  }

  if( gCTevent == 1 ){
    gCTevent = 0;
    if( gAccessoryAddress == gAccAdr){
        ServoCH0.change(gDir);
　  }
    if( gAccessoryAddress2 == gAccAdr){
        ServoCH1.change(gDir);
    }
  }

  if(ServoEnable == 0 && ( millis() - gPreviousL5) >=500){
    ServoEnable = 1;
  }

  if(ServoEnable == 1){
    if ( (millis() - gPreviousL5) >= 10){
      ServoCH0.stateCheck();
      ServoCH1.stateCheck();
      gPreviousL5 = millis();
    }
  }
}
//}

1行目〜　@<tt>{loop()}関数

4行目〜 @<tt>{CV8}等でリセットされた時に、デフォルトの@<tt>{CV}テーブルを元に、再セットする処理

10行目〜　ポイント切り替えのイベントを受信したかを確認して@<tt>{ServoDriver}の状態を更新する。

20行目〜　@<tt>{DCS50k}で線路電源を@<tt>{ON}にした初回0を送られてくるので@<tt>{500ms}間の@<tt>{DCC}信号を無視する処理。

24行目〜　@<tt>{10ms}周期にサーボモータのステートマシンを動かす。


=== getMyAddr_Acc()の実装
アクセサリデコーダのアドレスを@<tt>{EEPROM}から取得します。

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

=== notifyDccAccState()の実装
@<tt>{DCC}信号からアクセサリ命令の@<tt>{ON/OFF}を受信して、@<tt>{gDir}に格納します。

//emlistnum{
void notifyDccAccState( uint16_t Addr, uint16_t BoardAddr,
  　　　　　　　　　　　　　　　　　　　　　　　uint8_t OutputAddr, uint8_t State)
{
  uint16_t aAccAdress = Addr;
  uint8_t aDir = (OutputAddr & 0b001);

  gCTevent = 1;
  gAccAdr = aAccAdress;
  gDir = (OutputAddr & 0b001);
}
//}

=== Dccinit()の実装

//emlistnum{
void Dccinit(void)
{
  #if defined(DCCACKPIN)
    //Setup ACK Pin
    pinMode(DccAckPin, OUTPUT);
    digitalWrite(DccAckPin, 0);
  #endif

  #if !defined(DECODER_DONT_DEFAULT_CV_ON_POWERUP)
    if ( Dcc.getCV(CV_MULTIFUNCTION_PRIMARY_ADDRESS) == 0xFF ) {
      notifyCVResetFactoryDefault();
    } else {
      Serial.println("CV Not Defaulting");
    }
  #else
    Serial.println("CV Defaulting Always On Powerup");
    notifyCVResetFactoryDefault();
  #endif

  Dcc.pin(0, 2, 0);   // Arduino Nano D2(PD2)pinをDCC信号入力端子に設定

  Dcc.init( MAN_ID_NUMBER, MAN_VER_NUMBER, FLAGS_OUTPUT_ADDRESS_MODE |
    FLAGS_DCC_ACCESSORY_DECODER, 0 );  // アクセサリデコーダ

  gAccessoryAddress = getMyAddr_Acc();          // ch1
  gAccessoryAddress2 = gAccessoryAddress + 1;   // ch2
}
//}

3行目〜　@<tt>{DCC ACK pin}の初期化、本デコーダでは未使用。

9行目〜　@<tt>{DECODER_DONT_DEFAULT_CV_ON_POWERUP}を@<tt>{#define}で定義していないため、10行目以降の
スケッチが有効となっています。@<tt>{DCC}アドレスが@<tt>{255}の場合、@<tt>{CV}値をデフォルトにセットします。

20行目  @<tt>{D2(PD2)pin}を@<tt>{DCC}信号入力端子に設定

22行目　@<tt>{DCC}ドライバをアクセサリデコーダで初期化します。

25行目〜　アクセサリアドレスを取得して、@<tt>{ch2}のアドレスは@<tt>{ch1}のアドレスに１を加算した値にします。


//embed[latex]{
\clearpage
//}


#@#-----------------------------------------------------------------------------
#@# MP3サウンドデコーダを作る
#@#-----------------------------------------------------------------------------


== MP3サウンドデコーダを作る
@<tt>{DFRobottoDFPlayerMini}を使用した@<tt>{MP3}サウンドデコーダです。主にホームの構内放送や建物の効果音等に使用できます。

=== 完成写真
//image[mp3decoder2][MP3 Decoder][scale=1]

=== 仕様を考える

@<tt>{DFRobotDFPlayerMini}のドライバは@<tt>{DFRobotDFPlayerMin}を使用します。
@<href>{http://www.dfrobot.com,[ Github DFRobotDFPlayerMiniライブラリ ]}

@<tt>{SD CARD}の@<tt>{mp3}フォルダに、@<tt>{0001.mp3 0002.mp3}の様に4桁の数字ファイル名で格納しておくと、ファンクション

ファンクション @<tt>{F0:0001.mp3} 〜 ファンクション @<tt>{F10:0011.mp3}に対応したMP3を再生します。

接続は、@<tt>{2.10.2 MP3 Sound }基板のページを参照してください。

=== 接続図

//image[mp3decoder1][MP3 Decoder][scale=1]
@<tt>{Arduino Nano  DCC Shield} に@<tt>{ DFPlayer mini Board AYA049-2}を取り付けます。

@<tt>{VIN}に接続されない様に1ピンずらして実装してください。間違って@<tt>{VIN}に接続すると@<tt>{DFPlayer mini Board}に@<tt>{DC12V}が印加されて即故障します。

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/FunctionDecoderMP3,[ MP3ファンクションデコーダスケッチ ]}

@<tt>{https://github.com/DccShield/FunctionDecoderMP3}

=== スケッチの構成

@<tt>{FunctionDecoderMP3.ino   ：} メインプログラムです。

@<tt>{DFRobotDFPlayerMini.h    ： DFRobotDFPlayerMini MP3}ドライバ用ヘッダファイル

@<tt>{DFRobotDFPlayerMini.cpp　： DFRobotDFPlayerMini MP3}ドライバ

@<tt>{NmraDcc.cpp              : NmraDcc}ライブラリ

@<tt>{NmraDcc.h                : NmraDcc}ライブラリ用ヘッダファイル

@<tt>{DccCV.h                  : }定義ファイルとグローバル変数宣言

//blankline

@<tt>{FunctionDecoderMP3.ino}を参照してください。
//emlistnum{
#include <arduino.h>
#include "DccCV.h"
#include "NmraDcc.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

#define DEBUG      //リリースのときはコメントアウトすること

//各種設定、宣言
unsigned char cvtblePrev[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//使用クラスの宣言
NmraDcc   Dcc;
DCC_MSG  Packet;

SoftwareSerial mySoftwareSerial(A4, A5); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

struct CVPair {
  uint16_t  CV;
  uint8_t Value;
};

CVPair FactoryDefaultCVs [] = {
  {CV_MULTIFUNCTION_PRIMARY_ADDRESS, DECODER_ADDRESS},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0},
  {CV_MP3_Vol,20},
  {CV_dummy,0},
};

void(* resetFunc) (void) = 0;  //declare reset function at address 0
uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
void notifyDccReset(uint8_t hardReset );
void Dccinit(void);

//------------------------------------------------------------------
// Arduino固有の関数 setup() :初期設定
//------------------------------------------------------------------
void setup()
{
  // ソフトウエアシリアル通信レートセット:
  mySoftwareSerial.begin(9600);

  pinMode(A3, INPUT);  //Mp3 Busy pin 設定
  digitalWrite(A3, HIGH); //Internal pull up enabLED

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(gCV58_MP3_Vol);  //Set volume value. From 0 to 30

  //ファンクションの割り当てピン初期化
  Dccinit();

  //Reset task
  gPreviousL5 = millis();
}
//}

10行目 @<tt>{unsigned char cvtblePrev[20] DCC}コマンドステーションから送られてくる@<tt>{F0〜F10}を格納しておく変数です。
@<tt>{F0 ON}を受信した時には、@<tt>{cvtblePrev[0] = 1}をセットし、@<tt>{F0 OFF}を受信した時は@<tt>{cvtblePrev[0] = 0}をセットします。
//image[FuncMP3_1][cvtblPrev配列][scale=1]

@<tt>{DCC}パケットは同じコマンドが連続で送られてくるため、２重処理を防止するための変数になります。

41行目〜 @<tt>{setup() DFRobot DFPlayer Mini}の初期化スケッチがそのまま、コピペされています。

@<tt>{myDFPlayer.begin(mySoftwareSerial)}で、@<tt>{DFPlayer}のポートを開いた時に、応答がなかった時に
エラーメッセージを出力しています。（応答が無い事例は@<tt>{MicroSD}が実装されていない場合です）

64行目　@<tt>{DFPlayerMini}の音量をセットしています。

//emlistnum{
//---------------------------------------------------------------------
// Arduino main loop
//---------------------------------------------------------------------
void loop() {
  Dcc.process();

  if ( (millis() - gPreviousL5) >= 100)
  {
    FunctionProcess();
    gPreviousL5 = millis();
  }
}

//---------------------------------------------------------------------
//ファンクション受信によるイベント
//---------------------------------------------------------------------
void FunctionProcess(void){

#ifdef DEBUG
  gStateView();     // gState変数を表示
#endif

// F0 受信時の処理
    if(gState_F0 == 0 && cvtblePrev[0] == ON){
      myDFPlayer.stop();
      cvtblePrev[0] = OFF;
    } else if(gState_F0 == 1 && cvtblePrev[0] == OFF){
      myDFPlayer.play(1);
      cvtblePrev[0] = ON;
    }

// F1 受信時の処理
    if(gState_F1 == 0 && cvtblePrev[1] == ON){
      myDFPlayer.stop();
      cvtblePrev[1] = OFF;
    } else if(gState_F1 == 1 && cvtblePrev[1] == OFF){
      myDFPlayer.play(2);
      cvtblePrev[1] = ON;
    }

// F2 受信時の処理 gState_F2 に 2が入る
    if(gState_F2 == 0 && cvtblePrev[2] == ON){
      myDFPlayer.stop();
      cvtblePrev[2] = OFF;
    } else if(gState_F2 == 1 && cvtblePrev[2]== OFF){
      myDFPlayer.play(3);
      cvtblePrev[2] = ON;
    }

// F3 受信時の処理
    if(gState_F3 == 0 && cvtblePrev[3] == ON){
      myDFPlayer.stop();
      cvtblePrev[3] = OFF;
    } else if(gState_F3 == 1 && cvtblePrev[3]== OFF){
      myDFPlayer.play(4);
      cvtblePrev[3] = ON;
    }

// F4 受信時の処理
    if(gState_F4 == 0 && cvtblePrev[4] == ON){
      myDFPlayer.stop();
      cvtblePrev[4] = OFF;
    } else if(gState_F4 == 1 && cvtblePrev[4]== OFF){
      myDFPlayer.play(5);
      cvtblePrev[4] = ON;
    }

// F5 受信時の処理
    if(gState_F5 == 0 && cvtblePrev[5] == ON){
      myDFPlayer.stop();
      cvtblePrev[5] = OFF;
    } else if(gState_F5 == 1 && cvtblePrev[5]== OFF){
      myDFPlayer.play(6);
      cvtblePrev[5] = ON;
    }

// F6 受信時の処理
    if(gState_F6 == 0 && cvtblePrev[6] == ON){
      myDFPlayer.stop();
      cvtblePrev[6] = OFF;
    } else if(gState_F6 == 1 && cvtblePrev[6]== OFF){
      myDFPlayer.play(7);
      cvtblePrev[6] = ON;
    }

// F7 受信時の処理
    if(gState_F7 == 0 && cvtblePrev[7] == ON){
      myDFPlayer.stop();
      cvtblePrev[7] = OFF;
    } else if(gState_F7 == 1 && cvtblePrev[7]== OFF){
      myDFPlayer.play(8);
      cvtblePrev[7] = ON;
    }

// F8 受信時の処理
    if(gState_F8 == 0 && cvtblePrev[8] == ON){
      myDFPlayer.stop();
      cvtblePrev[8] = OFF;
    } else if(gState_F8 == 1 && cvtblePrev[8]== OFF){
      myDFPlayer.play(9);
      cvtblePrev[8] = ON;
    }

// F9 受信時の処理
    if(gState_F9 == 0 && cvtblePrev[9] == ON){
      myDFPlayer.stop();
      cvtblePrev[9] = OFF;
    } else if(gState_F9 == 1 && cvtblePrev[9]== OFF){
      myDFPlayer.play(10);
      cvtblePrev[9] = ON;
    }

// F10 受信時の処理
    if(gState_F10 == 0 && cvtblePrev[10] == ON){
      myDFPlayer.stop();
      cvtblePrev[10] = OFF;
    } else if(gState_F10 == 1 && cvtblePrev[10]== OFF){
      myDFPlayer.play(11);
      cvtblePrev[10] = ON;
    }
}
//}
4行目〜 @<tt>{loop()}関数@<tt>{100ms}周期に@<tt>{DCC}コマンドの処理を行なっています。

17行目〜　@<tt>{FunctionProcess()}　ファンクション受信し@<tt>{mp3}を再生と停止処理を行なっています。

//emlist[]{
  // F0 受信時の処理
      if(gState_F0 == 0 && cvtblePrev[0] == ON){
        myDFPlayer.stop();
        cvtblePrev[0] = OFF;
      } else if(gState_F0 == 1 && cvtblePrev[0] == OFF){
        myDFPlayer.play(1);
        cvtblePrev[0] = ON;
      }
//}

@<tt>{DCC}信号からファンクション信号を受信する、@<tt>{gState_F0}の変数には、@<tt>{ON:(1) OFF:(0)} の値が代入されていきます。
それとあらかじめファンクションを受信した設定値を比較して、再生処理と停止処理を行います。

@<tt>{if(gState_F0 == 0 && cvtblePrev[0] == ON)}

@<tt>{F0 OFF} を受信、前回受信したファンクションが@<tt>{ON}の状態だったら、再生を停止。

@<tt>{if(gState_F0 == 1 && cvtblePrev[0] == OFF)}

@<tt>{F0 ON}を受信、前回受信したファンクションが@<tt>{OFF}の状態だったら、再生を開始。

なお、@<tt>{DCC}パケットは連続して送られてくるため、@<tt>{gState_F0 == 1、cvtblePrev[0] == ON}の条件が
成立しないため、同じ処理が実行されないようにしています。

//emlist[]{
  // F0 受信時の処理
      if(gState_F0 == 0 ){
        myDFPlayer.stop();
        cvtblePrev[0] = OFF;
      } else if(gState_F0 == 1){
        myDFPlayer.play(1);
        cvtblePrev[0] = ON;
      }
//}

上記の様にスケッチを作ると、@<tt>{F0 ON}を連続で受信するので、@<tt>{myDFPlayer.play(1)}の関数を
毎回実行する事になり、イントロだけ短時間@<tt>{（100ms）}再生します。


//emlistnum{
extern void notifyDccFunc(uint16_t Addr,DCC_ADDR_TYPE AddrType,FN_GROUP FuncGrp,
                                                              uint8_t FuncState)
{
  if( FuncGrp == FN_0_4){  // F0〜F4の解析
    gState_F0 = (FuncState & FN_BIT_00) ? 1 : 0;
    gState_F1 = (FuncState & FN_BIT_01) ? 1 : 0;
    gState_F2 = (FuncState & FN_BIT_02) ? 1 : 0;
    gState_F3 = (FuncState & FN_BIT_03) ? 1 : 0;
    gState_F4 = (FuncState & FN_BIT_04) ? 1 : 0;
  }

  if( FuncGrp == FN_5_8){  // F5〜F8の解析
    gState_F5 = (FuncState & FN_BIT_05) ? 1 : 0;
    gState_F6 = (FuncState & FN_BIT_06) ? 1 : 0;
    gState_F7 = (FuncState & FN_BIT_07) ? 1 : 0;
    gState_F8 = (FuncState & FN_BIT_08) ? 1 : 0;
  }

  if( FuncGrp == FN_9_12) { // F9〜F12の解析
    gState_F9 = (FuncState & FN_BIT_09) ? 1 : 0;
    gState_F10 = (FuncState & FN_BIT_10) ? 1 : 0;
    gState_F11 = (FuncState & FN_BIT_11) ? 1 : 0;
    gState_F12 = (FuncState & FN_BIT_12) ? 1 : 0;
  }

  if( FuncGrp == FN_13_20){   // F13〜F20の解析
    gState_F13 = (FuncState & FN_BIT_13) ? 1 : 0;
    gState_F14 = (FuncState & FN_BIT_14) ? 1 : 0;
    gState_F15 = (FuncState & FN_BIT_15) ? 1 : 0;
    gState_F16 = (FuncState & FN_BIT_16) ? 1 : 0;
    gState_F17 = (FuncState & FN_BIT_17) ? 1 : 0;
    gState_F18 = (FuncState & FN_BIT_18) ? 1 : 0;
    gState_F19 = (FuncState & FN_BIT_19) ? 1 : 0;
    gState_F20 = (FuncState & FN_BIT_20) ? 1 : 0;
  }
}
//}
ファンクション信号受信のイベントです。@<tt>{FN_0_4}と@<tt>{FN_5_8}は常時イベント発生@<tt>{（DCS50K}は@<tt>{F8}まで）
@<tt>{FN_9_12}以降は@<tt>{FUNCTION}ボタンが押されたときにイベント発生。前値と比較して変化あったら処理するような作り。

//embed[latex]{
\clearpage
//}

====[column] notifyDccFuncの処理方法
このページのスケッチでは受信したファンクションの処理方法に三項演算子を使用して簡潔化して作っています。
@<tt>{gState_F0 = (FuncState & FN_BIT_00) ? 1 : 0;} を@<tt>{ if} 分で書き直すと以下のようになります。
//emlist[]{
if( FuncGrp == FN_0_4){
  if(FuncState & FN_BIT_00){
    gState_F0 = 1;
    } else {
    gState_F0 = 0;
  }
}
//}
長めになりがちな @<tt>{notifyDccFunc} 内の処理は三項演算子を使用すると良いです。

====[/column]


//embed[latex]{
\clearpage
//}



== DCC通信モニタ（DCCスニファ）を作る
@<tt>{Arduino Nano DCC Shield} を使った、@<tt>{DCC}通信モニタです。
元は@<tt>{MynaBay Inc. Building a DCC Monitor with an Arduino}をベースにしています。
ちょっと改造してありまして、ファンクションが何番が来ているかなどわかりやすい表示にしています。
なお、@<tt>{DCC}通信モニタのスケッチの説明はありません。

=== 完成写真
//image[dccmonitor1][DCC通信モニタ][scale=1]

=== 接続図
//image[dccmonitor2][接続図][scale=1]

@<tt>{Arduino Nano DCC Shield}とパソコンを直接接続すると、@<tt>{DCC}受信回路の故障等発生した時にパソコン側に
@<tt>{DCC}電圧がパソコンに印加されて故障につながります。
@<tt>{USB}アイソレーターを使用するとDCC受信回路が故障してもパソコン側は保護されます。

=== スケッチのダウンロード

@<tt>{github}からダウンロードしてください。

@<href>{https://github.com/DccShield/DccMonitor,[ DCC通信モニタスケッチ ]}

@<tt>{https://github.com/DccShield/DccMonitor}

=== スケッチの構成

@<href>{http://www.mynabay.com/dcc_monitor/,Building a DCC Monitor with an Arduino}

以下のソースファイルが@<tt>{DCC_Monitor}のスケッチに含まれています。

@<tt>{DCC_Monitor.ino}

@<tt>{DCC_Decoder.cpp}

@<tt>{DCC_Decoder.h}

=== DCC通信モニタ使用時のArduino Nano DCC Shieldの接続
レールの@<tt>{DCC}信号を@<tt>{Arduino Nano DCC Shield}の@<tt>{DCC}コネクタに接続します。

@<tt>{Arduino Nano }の@<tt>{USB}端子とパソコンを@<tt>{USB}ケーブルで接続します。

//image[dccSniffer1][危ない接続][scale=1]
ここで、なぜ危ない接続かと言いますと、万が一@<tt>{DCC}信号のが@<tt>{USB}コネクタの信号に短絡していた時に、パソコン側のハードウエアを
故障させる原因になります。（最悪はパソコンが起動不可になります）
筆者は壊れても痛くない安物のノートパソコンに接続して実験しています。
@<tt>{USB}アイソレータを使用した安全な接続を行なってください。

//image[dccSniffer2][安全な接続][scale=1]

=== シリアルモニタを開いてみましょう
@<tt>{115200bps}に設定すると、以下のメッセージが約2秒周期で表示されます。まだ何も@<tt>{DCC}通信を受信していない状態です。
//emlist[]{
  ============================================
  Total Packet Count: 0
  Idle Packet Count:  0
  Longest Preamble:  0
  Count    Packet_Data
  ============================================
//}

@<tt>{DSbluebox}の@<tt>{[Loc Control]}を選択すると何やらメッセージが出てきます。
あまりにも大量の@<tt>{DCC}コマンドを受信するため、同じコマンドは@<tt>{Count}を@<tt>{UP}させて、まとめて表示しています。
基本的には、@<tt>{idle}パケットが送信し続けています。
//emlist[]{
  ============================================
  Total Packet Count: 25
  Idle Packet Count:  24
  Longest Preamble:  16
  Count    Packet_Data
  1        00000000 00000000 00000000                   Adr:   0, 000:
  24       11111111 00000000 11111111                   idle
  ============================================
  Total Packet Count: 282
  Idle Packet Count:  282
  Longest Preamble:  16
  Count    Packet_Data
  282      11111111 00000000 11111111                   idle
//}

ファンクションボタンをぽちぽち押すと、ファンクション系の@<tt>{DCC}通信が追加されます。

出現多い順に並びますので、最新の情報は@<tt>{Count}の少ないパケットになります。

@<tt>{F1}と@<tt>{F2}を@<tt>{ON}にしました。

//emlist[]{
  ============================================
  Total Packet Count: 253
  Idle Packet Count:  0
  Longest Preamble:  16
  Count    Packet_Data
  128  00000011 01100000 01100011
  -> Adr:3, 011:
  15   00000011 10000000 10000011
  -> Adr:3, F 0 OFF:F 1 OFF:F 2 OFF:F 3 OFF:F 4 OFF
  25   00000011 10110000 10110011
  -> Adr:3, F 5 OFF:F 6 OFF:F 7 OFF:F 8 OFF
  26   00000011 10100000 10100011
  -> Adr:3, F 9 OFF:F10 OFF:F11 OFF:F12 OFF
  23   00000011 11011110 00000000 11011101
  -> Adr:3, F13 OFF:F14 OFF:F15 OFF:F16 OFF:F17 OFF:F18 OFF:F19 OFF:F20 OFF
  25   00000011 11011111 00000000 11011100
  -> Adr:3, F21 OFF:F22 OFF:F23 OFF:F24 OFF:F25 OFF:F26 OFF:F27 OFF:F28 OFF
  11   00000011 10000001 10000010
  -> Adr:3, F 0 OFF:F 1 ON :F 2 OFF:F 3 OFF:F 4 OFF
  ============================================
//}

次に、スピードを入れていくと以下のような感じになります。@<tt>{Speed}の@<tt>{DCC}パケットが送信されます。

//emlist[]{
  ============================================
  Total Packet Count: 233
  Idle Packet Count:  0
  Longest Preamble:  16
  Count    Packet_Data
  6    00000011 00111111 10001110 10110010
  -> Adr:3, 128 Speed step,FWD,14
  24   00000011 10100000 10100011
  -> Adr:3, F 9 OFF:F10 OFF:F11 OFF:F12 OFF
  21   00000011 11011110 00000000 11011101
  -> Adr:3, F13 OFF:F14 OFF:F15 OFF:F16 OFF:F17 OFF:F18 OFF:F19 OFF:F20 OFF
  22   00000011 11011111 00000000 11011100
  -> Adr:3, F21 OFF:F22 OFF:F23 OFF:F24 OFF:F25 OFF:F26 OFF:F27 OFF:F28 OFF
  24   00000011 10000011 10000000
  -> Adr:3, F 0 OFF:F 1 ON :F 2 ON :F 3 OFF:F 4 OFF
  24   00000011 10110000 10110011
  -> Adr:3, F 5 OFF:F 6 OFF:F 7 OFF:F 8 OFF
  18   00000011 00111111 10011010 10100110
  -> Adr:3, 128 Speed step,FWD,26
  25   00000011 00111111 10100110 10011010
  -> Adr:3, 128 Speed step,FWD,38
  69   00000011 00111111 10110011 10001111
  -> Adr:3, 128 Speed step,FWD,51
  ============================================
//}

@<tt>{CV 8}に1を書き込んだ時の@<tt>{DCC}パケットは以下のようになります。

//emlist[]{
  ============================================
  Total Packet Count: 187
  Idle Packet Count:  0
  Longest Preamble:  24
  Count    Packet_Data
  177      00000000 00000000 00000000
  -> Adr:   0, 000:
  10       01111100 00000111 00000001 01111010
  -> CV:  8,  1
  ============================================
//}

アクセサリ１アドレスにCを送信

//emlist[]{
  ============================================
  Total Packet Count: 290
  Idle Packet Count:  284
  Longest Preamble:  16
  Count    Packet_Data
  284      11111111 00000000 11111111
  -> idle
  1        10000001 11111001 10000000 11111000
  -> Adr:   1, C
  4        10000001 11111001 01111000
  -> Adr:   1, C
  1        10000001 11111001 10110000 11001000
  -> Adr:   1, C
  ============================================
//}

アクセサリ１アドレスにTを送信

//emlist[]{
  ============================================
  Total Packet Count: 297
  Idle Packet Count:  292
  Longest Preamble:  16
  Count    Packet_Data
  292      11111111 00000000 11111111
  -> idle
  1        10000001 11111000 10000000 11111001
  -> Adr:   1, T
  4        10000001 11111000 01111001
  -> Adr:   1, T
  ============================================
//}

@<tt>{OPS}モードで@<tt>{CV2 4}を送信@<tt>{Count6}がその@<tt>{DCC}コマンドに該当します。

@<tt>{CV}番号は@<tt>{-1}されます。@<tt>{CV2}なので、@<tt>{00000001}が送信されています。

//emlist[]{
  ============================================
  Total Packet Count: 232
  Idle Packet Count:  0
  Longest Preamble:  18
  Count    Packet_Data
  169      00000011 00111111 10000000 10111100
  -> Adr:   3, 128 Speed step,FWD,0
  28       00000011 10000000 10000011
  -> Adr:   3, F 0 OFF:F 1 OFF:F 2 OFF:F 3 OFF:F 4 OFF
  29       00000011 10110000 10110011
  -> Adr:   3, F 5 OFF:F 6 OFF:F 7 OFF:F 8 OFF
  6        00000011 11101100 00000001 00000100 11101010
  -> Adr:   3, ***
  ============================================
//}
