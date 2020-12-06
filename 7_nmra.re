= NmraDcc
== NmraDccとは
@<tt>{Arduino}言語で作られた、@<tt>{NMRA}デジタルコマンドコントロールライブラリです。このライブラリと@<tt>{DCC}受信回路を使用する事で@<tt>{NMRA DCC}トラック信号の受信ができます。
よって簡単にファンクションデコーダ、アクセサリデコーダが作れます。
@<tt>{CPU}も@<tt>{AVR ATTiny84/85、ATMega88/168/328P/32u4/1280/2560、ESP32/ESP8266、ARM MK20DX128/MK20DX256/STM32F1、Teensy 3.x }と幅広く対応していので
簡易的なデコーダから高機能なデコーダまで様々なデコーダを作ることができます。
@<tt>{DCC}ライブラリは@<tt>{NMRA.org}で規定されている@<tt>{NMRA STANDARD}に準拠しているため各社のコマンドステーションのDCCトラック信号を安定した受信ができます。

== NmraDccライブラリ
@<tt>{NmraDcc}ライブラリはgithub @<href>{https://github.com/mrrwa/NmraDcc}で管理されています。
//image[nmra1][NmraDccのダウンロードサイト][scale=0.8]
最新版のダウンロードは@<tt>{「Clone or download」}をクリックすると、@<tt>{ZIP}ファイルとなってダウンロードできます。

@<tt>{2020/9/20}時点のバージョンは@<tt>{ Ver2.0.5 }になります。

== NmraDccの構成
@<tt>{NmraDcc}ライブラリは以下の2つのファイルで構成されています。
//blankline
@<tt>{NmraDcc.cpp　　NmraDcc}ライブラリ

@<tt>{NmraDcc.h　　　NmraDcc}ヘッダファイル

//embed[latex]{
\clearpage
//}

=== NmraDcc.cppに含まれている関数
@<tt>{NmraDCC}クラスで定義されており、各メンバ関数にアクセスしています。

//table[table][]{
関数名	機能
pin()	DCCで使用するポート番号の定義
init()	NmraDccオブジェクトの初期化
initAccessoryDecoder()	アクセサリデコーダとしての初期化
process()	Dcc信号処理関数
getCV()	EEPROMからCV値を読み込み
setCV()	EEPROMにCV値を書き込み
setAccDecDCCAddrNextReceived()
isSetCVReady()	EEPROMが書き込み可能かチェック
getAddr()	デコーダアドレスを返します
notifyDccReset()	DCCリセットコマンドのコールバック関数
notifyDccIdle()	DCCアイドルコマンドのコールバック関数
notifyDccSpeed()	速度コマンドのコールバック関数
notifyDccSpeedRaw()	速度コマンドのコールバック関数（14STEP,28STEP用)
notifyDccFunc()	ファンクションコマンドのコールバック関数
notifyDccAccTurnoutBoard()	ターンアウトのコールバック関数
notifyDccAccTurnoutOutput()	ターンアウトのコールバック関数
notifyDccAccBoardAddrSet()	ターンアウトのコールバック関数
notifyDccAccOutputAddrSet()	ターンアウトのコールバック関数
notifyDccSigOutputState()	信号機アクセサリデコーダのコールバック関数
notifyDccMsg()	DCCパケットのコールバック関数
notifyCVValid()	CV値が有効がチェックするコールバック関数
notifyCVRead()	CV値を読み込む為のコールバック関数
notifyCVWrite()	CV値を書き込むためのコールバック関数
notifyIsSetCVReady()	CV値が書き込めるか判断する為のコールバック関数
notifyCVChange()	CV値が変更になった時に呼び出されます
notifyDccCVChange()
notifyCVResetFactoryDefault()	CV値を工場出荷状態にします
notifyCVAck()
notifyAdvancedCVAck()
notifyServiceMode()	サービスモードの入ると読み出されます。
//}

これらの関数は制御系の関数と、通知系関数で構成されています。関数名に@<tt>{notify}と名付けられているのが
通知系関数@<tt>{（call back function）}となっています。

//embed[latex]{
\clearpage
//}

=== pin()
[ユーザープログラムで使用します]

@<tt>{Dcc}信号に使用する割り込み番号の設定、@<tt>{DCC}信号の入力端子ポート、入力端子ポートのプルアップを有効・無効の設定を行います。

@<tt>{Arduino}の@<tt>{setup()}関数内で定義します。

==== 構文
@<tt>{pin( ExtIntNum , ExitIntPinNum , EnablePullup );}

==== パラメーター
@<tt>{ExtIntNum     : }割り込み番号。@<tt>{Arduino Nano}の場合、@<tt>{0:int0(PIN2) 1:int1(PIN3)} の設定できます。

@<tt>{ExitIntPinNum : DCC}信号を入力ピンを設定します。@<tt>{ExtIntNum}で設定した入力ピンになります。

@<tt>{EnablePullup  : DCC}入力ピンのプルアップを有効・無効の設定。1で有効です。

==== 戻り値
なし

==== pin()の実装
//emlistnum{
void NmraDcc::pin(uint8_t ExtIntNum,uint8_t ExtIntPinNum,uint8_t EnablePullup)
{
  DccProcState.ExtIntNum = ExtIntNum;
  DccProcState.ExtIntPinNum = ExtIntPinNum;
  pinMode( ExtIntPinNum, INPUT );
  if( EnablePullup )
    digitalWrite(ExtIntPinNum, HIGH);
}
//}
@<tt>{DCC}入力ポートの設定、割り込み番号の設定、入力ピンのプルアップの有効・無効の設定を行います。

==== 使用例
//emlist[]{
  Dcc.pin(0, 2, 0);
//}
@<tt>{DCC}電子工作連合のデコーダは上記設定で初期化します。

引数１：割り込み @<tt>{int0}、引数２：@<tt>{DCC}信号入力ポート 2、引数３：プルアップ無効



//embed[latex]{
\clearpage
//}


=== init()
[ユーザープログラムで使用します]

@<tt>{NmDcc}オブジェクトを初期化し、@<tt>{DCC}信号を処理できるようにします。

マニファクチャ@<tt>{ID}の設定と、デコーダ種別の設定を行います。

@<tt>{Arduino}の@<tt>{setup()}関数内で定義します。

==== 構文
@<tt>{init( ManufacturerId, VersionId, Flags, OpsModeAddressBaseCV );}

==== パラメーター
@<tt>{ManufacturerId : CV8}で応答するメーカー個別番号（マニュファクチャ@<tt>{ID}）を指定します。

@<tt>{VersionId : CV7}で応答するメーカーバージョン番号(@<tt>{Version ID})を指定します。

@<tt>{Flags : FLAGS_MY_ADDRESS_ONLY,FLAGS_DCC_ACCESSORY_DECODER,}

@<tt>{        FLAGS_OUTPUT_ADDRESS_MODE} のいずれかを設定します。
//blankline
@<tt>{        FLAGS_MY_ADDRESS_ONLY:0x01}

        ファンクションデコーダとして設定します。
//blankline
@<tt>{        FLAGS_DCC_ACCESSORY_DECODER:0x80}

        アクセサリデコーダとして設定します。
//blankline
@<tt>{        FLAGS_OUTPUT_ADDRESS_MODE:0x40}

        アクセサリデコーダと組み合わせて使用します。

//image[nmra2][CV29 ファンクションデコーダ、アクセサリデコーダの違い][scale=1]

@<tt>{OpsModeAddressBaseCV : Ops Mode base address. Set it to 0?}

アクセサリデコーダ時の@<tt>{OPS}書き換えアドレスを格納している@<tt>{CV}番号指定。
@<tt>{（CV33 low Address , CV34 High Address )}

==== 戻り値
なし

==== init()の実装
//emlistnum{
  // Clear all the static member variables
  memset( &DccRx, 0, sizeof( DccRx) );

  MODE_TP1; // only for debugging and timing measurement
  MODE_TP2;
  MODE_TP3;
  MODE_TP4;
  bitMax = MAX_ONEBITFULL;
  bitMin = MIN_ONEBITFULL;

  DccProcState.Flags = Flags ;
  DccProcState.OpsModeAddressBaseCV = OpsModeAddressBaseCV ;
  DccProcState.myDccAddress = -1;
  DccProcState.inAccDecDCCAddrNextReceivedMode = 0;

  ISREdge = RISING;
  attachInterrupt( DccProcState.ExtIntNum, ExternalInterruptHandler, RISING);

  // Set the Bits that control Multifunction or Accessory behaviour
  // and if the Accessory decoder optionally handles Output Addressing
  // we need to peal off the top two bits
  DccProcState.cv29Value = writeCV( CV_29_CONFIG, ( readCV( CV_29_CONFIG ) & _
	~FLAGS_CV29_BITS ) | (Flags & FLAGS_CV29_BITS) ) ;

  uint8_t doAutoFactoryDefault = 0;
  if((Flags & FLAGS_AUTO_FACTORY_DEFAULT) && (readCV(CV_VERSION_ID) == 255)&&_
	(readCV(CV_MANUFACTURER_ID) == 255))
  	  doAutoFactoryDefault = 1;

  writeCV( CV_VERSION_ID, VersionId ) ;
  writeCV( CV_MANUFACTURER_ID, ManufacturerId ) ;

  clearDccProcState( 0 );

  if(notifyCVResetFactoryDefault && doAutoFactoryDefault)
  	notifyCVResetFactoryDefault();
//}
2行目 @<tt>{DccRx}構造体一式を初期化します。

4行目〜 @<tt>{MODE_TP1,TP2,TP3,TP4} は、@<tt>{DDRC |= (1<<1),DDRC |= (1<<2),DDRC |= (1<<3)}
@<tt>{DDRC |= (1<<4)} という処理ですが、デバック用の信号出力用ポートでしょうか。
出力ポートとして@<tt>{A1,A2,A3,A4}を初期化しています。

17行目 @<tt>{DCC}信号割り込みの条件を @<tt>{attachInterrupt()} で設定。

22行目 @<tt>{CV29}の値を読み込み、@<tt>{FLAGS_DCC_ACCESSORY_DECODER、FLAGS_OUTPUT_ADDRESS_MODE}
の条件を再書き込み。

25行目 毎回@<tt>{CV}値をデフォルトに再設定する条件、@<tt>{FLAGS_AUTO_FACTORY_DEFAULT、CV7=255}
@<tt>{CV8＝255} の時に、@<tt>{doAutoFactoryDefault=1}にセットして@<tt>{CV}値を工場出荷状態にします。

28行目 @<tt>{CV7} メーカーバージョン番号値セット

29行目 @<tt>{CV8} メーカー個別番号（マニュファクチャ@<tt>{ID}）値再セット

33行目 @<tt>{notifyCVResetFactoryDefault()}関数が存在して@<tt>{doAutoFactoryDefault=1}の時、
@<tt>{CV}値を工場出荷状態にセットします。

==== 使用例
//emlist[]{
1.　Dcc.init( MAN_ID_NUMBER, MAN_VER_NUMBER, FLAGS_MY_ADDRESS_ONLY , 0 );
    // ファンクションデコーダ

2. Dcc.init( MAN_ID_NUMBER, MAN_VER_NUMBER, FLAGS_OUTPUT_ADDRESS_MODE |
    FLAGS_DCC_ACCESSORY_DECODER, 0 );  // アクセサリデコーダ

3. Dcc.init( MAN_ID_NUMBER, MAN_VER_NUMBER, FLAGS_OUTPUT_ADDRESS_MODE |
      FLAGS_DCC_ACCESSORY_DECODER, 33 );  // アクセサリデコーダ

   CVPair FactoryDefaultCVs [] ={
      {33, 13}, //　CV書き換え用DCCアドレス : address 13
      {34, 0},
//}
1.ファンクションデコーダの@<tt>{init()}例です。

2.アクセサリデコーダの@<tt>{init()}例です。

3.アクセサリデコーダの@<tt>{init()}例です。@<tt>{ops}書き換え時の@<tt>{DCC}アドレスを@<tt>{CV33}にあることを
設定しています。


====[column] メーカー個別番号について

デコーダから@<tt>{CV8}を読み出すとメーカー個別番号（@<tt>{NMRA}で認証された番号）を読み出すことができます。
このメーカー個別番号@<tt>{(DCC Manufacturer ID Numbers)}は@<tt>{NMRA}のサイトの、
@<tt>{https://www.nmra.org/manufacturer-id-numbers}から番号に紐づけられているデコーダのメーカを調べられます。
デコーダを自作する場合は、@<tt>{MAN_ID_DIY}（自作デコーダ用）の@<tt>{ID}は@<tt>{0x0D} を設定してください。

//blankline

余談ですが、国内では@<tt>{KATO(0x28:40)}、永末@<tt>{(0x67:103)}、@<tt>{DCC}電子工作連合では@<tt>{DektopStation(0x8c:140)}、
@<tt>{Nucky(0x9C:156)}、@<tt>{Nagoden(0x6c:108)}、@<tt>{Maison de DCC(0xA6:166)}の@<tt>{6}メーカーが登録されています。

//blankline

@<tt>{NmraDCC}ライブラリを使用した@<tt>{DCC}デコーダを作られると比較的簡単に取得できるかと思います。
私の場合作成したデコーダを@<tt>{４}回ほどアメリカに送って@<tt>{2}年ちょっとで取得できました。
@<tt>{NMRA}のサイトの@<tt>{https://www.nmra.org/dcc-working-group}の
@<tt>{DCC Section}を読んでもらい応募してみてください。（大変申し訳ございませんが、@<tt>{DCC}電子工作連合ではメーカー個別番号の取得に関してアドバイスはいたしません）

====[/column]




//embed[latex]{
\clearpage
//}


=== initAccessoryDecoder()
アクセサリデコーダとしてメンバ関数@<tt>{init()}をコールするための関数である。

==== 構文
@<tt>{initAccessoryDecoder( ManufacturerId, VersionId, Flags, OpsModeAddressBaseCV );}
//blankline
@<tt>{NmDcc} オブジェクトを初期化し、@<tt>{DCC}信号を処理できるようにします。
メンバ関数@<tt>{init()}の第３引数に@<tt>{FLAGS_DCC_ACCESSORY_DECODER}のフラグを立てて関数をコールしています。

@<tt>{init()}と機能が同じため、以降の説明は割愛します。

==== 戻り値
なし

==== 使用例
//emlistnum{
initAccessoryDecoder(ManufacturerId, VersionId, Flags, OpsModeAddressBaseCV);
//}


//embed[latex]{
\clearpage
//}


=== process()
[ユーザープログラムで使用します]

@<tt>{process()} は @<tt>{loop()} から呼び出され、@<tt>{DCC} のパケットを処理します。
パケットの処理が滞ることなく処理するには、@<tt>{process()}を高頻度で呼び出す必要があります。

==== 構文
@<tt>{process();}

==== 戻り値
Dataを処理すると１、処理しないと0を返します。

==== process()の実装
//emlistnum{
uint8_t NmraDcc::process()
{
  if( DccProcState.inServiceMode )
  {
    if( (millis() - DccProcState.LastServiceModeMillis ) > 20L )
    {
      clearDccProcState( 0 ) ;
    }
  }

  if( DccRx.DataReady )
  {
    // We need to do this check with interrupts disabled
    //SET_TP4;
    noInterrupts();
    Msg = DccRx.PacketCopy ;
    DccRx.DataReady = 0 ;
    interrupts();
      #ifdef DCC_DBGVAR
      countOf.Tel++;
      #endif

    uint8_t xorValue = 0 ;

    for(uint8_t i = 0; i < DccRx.PacketCopy.Size; i++)
      xorValue ^= DccRx.PacketCopy.Data[i];
    if(xorValue) {
      #ifdef DCC_DBGVAR
      countOf.Err++;
      #endif
      return 0 ;
    } else {
		if( notifyDccMsg ) 	notifyDccMsg( &Msg );

        execDccProcessor( &Msg );
    }
    return 1 ;
  }

  return 0 ;
};
//}

3行目〜：サービスモードからある時間(20ms)経過したら自動的にサービスモードを抜ける処理？

11行目〜：@<tt>{DCC}信号を最後まで受信し終わったら、@<tt>{Msg}に受信した@<tt>{DCC}パケットをコピー。

33行目〜： @<tt>{notifyDccMsg()}コールバック関数が用意されていたら、@<tt>{notifyDccMsg()}をコールします。

35行目：@<tt>{DCC}パケットを引数としてセットして、@<tt>{execDccProcessor()}をコールします。

==== 使用例

//emlist[]{
void loop() {
    Dcc.process();
//}


//embed[latex]{
\clearpage
//}

=== getCV()
[ユーザープログラムで使用します]

@<tt>{getCV()}は@<tt>{EEPROM}に記録されている@<tt>{CV}値に対する設定値を読み取りその結果を返します。
デコーダ起動時に@<tt>{EEPROM}から読み出されます。

@<tt>{Arduino}の@<tt>{setup()}関数内で定義します。

==== 構文
@<tt>{getCV( CV );}

==== パラメーター
@<tt>{cv : CV}値をセットします。

==== 戻り値
CVの値を返します。

==== getCV()の実装

//emlistnum{
uint8_t NmraDcc::getCV( uint16_t CV )
{
  return readCV(CV);
}
//}

readCV()をコールしています。

==== 使用例
//emlist[]{
ServoCH0.onDeg = Dcc.getCV( 47 );    // ON時の角度
//}




//embed[latex]{
\clearpage
//}

=== setCV()
@<tt>{setCV()}は@<tt>{CV}番号と値を引数として、@<tt>{EEPROM}に@<tt>{CV}値を書き込みます。
コマンドステーションからCV値を書き込む時に使用されます。

==== 構文
@<tt>{setCV( CV, Value)}

==== パラメーター
@<tt>{CV : CV}番号をセットします。

@<tt>{Value : CV}番号に書き込む値をセットします。

==== 戻り値
書き込んだCVの値を返します。

==== setCV()の実装

//emlistnum{
uint8_t NmraDcc::setCV( uint16_t CV, uint8_t Value)
{
  return writeCV(CV,Value);
}
//}


==== 使用例
//emlist[]{
Dcc.setCV(cv,ctState);        // 最終値のアクセサリ番号をCV_sdirに書き込み
//}




//embed[latex]{
\clearpage
//}


=== setAccDecDCCAddrNextReceived()
次の受信したターンアウトコマンドからの基板アドレスの設定を有効／無効にします。

==== 構文
setAccDecDCCAddrNextReceived( enable )

==== パラメーター
@<tt>{enable : 0 or 1}

==== 戻り値
なし

//embed[latex]{
\clearpage
//}


=== isSetCVReady()
@<tt>{EEPROM}が書き込み可能かチェックします。

==== 構文
@<tt>{isSetCVReady()}

==== 戻り値
書き込み準備ができていれば1、そうでなければ0を返します。
@<tt>{atmegaのEECR EEPROM }コントロール・レジスタをチェックして@<tt>{EEPROM}に書き込み可能か確認します

==== isSetCVReady()の実装
//emlistnum{
uint8_t NmraDcc::isSetCVReady(void)
{
  if(notifyIsSetCVReady)
	  return notifyIsSetCVReady();
  return readyEEPROM();
}
//}



//embed[latex]{
\clearpage
//}

=== getAddr()
デコーダアドレスを返します。

==== 構文
@<tt>{getAddr()}

==== 戻り値
デコーダアドレスを返します。

==== getAddrの実装
//emlistnum{
uint16_t NmraDcc::getAddr(void)
{
  return getMyAddr();
}
//}






//embed[latex]{
\clearpage
//}


=== notifyDccReset()
[ユーザープログラムで使用します]

@<tt>{DCC}リセットコマンドのコールバック関数です。

@<tt>{Decoder and Consist Control Instruction (000) }に関する処理です。

@<tt>{DCC}パケット（@<tt>{Cmd}変数)で、@<tt>{00000000} を受信した時はソフトウエアリセットを行います。

デジタルデコーダリセット

//emlist[]{
デジタルデコーダリセットは、すべての揮発性メモリ115（および速度と方向データを含む）を消去し、
@<tt>{NMRA}規格 S9.2.4セクションAで定義されているように初期のパワーアップ状態に戻るものとする。
//}

@<tt>{DCC}パケットで、@<tt>{00000001 }を受信した時はハードウエアリセット（工場出荷状態にします）

//emlist[]{
ハード・リセットはコンフィグレーション変数 29,31,32 は工場出荷時の状態にリセットされ、
CV#19 は "000000 "に設定され、デジタルデコーダのリセット(上記の指示)が行われる。
//}

==== 構文
@<tt>{notifyDccReset( hardReset )}

==== パラメーター
引数　@<tt>{hardReset : 0 normal reset command , 1 hard reset command.}

==== 戻り値
なし

==== notifyDccReset()の実装

//emlistnum{
//------------------------------------------------------------------
// Reset処理（特に何もしない）
//------------------------------------------------------------------
void notifyDccReset(uint8_t hardReset )
{
}
//}

==== 使用例
//emlistnum{
void notifyDccReset(uint8_t hardReset )
{
  digitalWrite(O1,LOW);
  digitalWrite(O2,LOW);
  digitalWrite(O3,LOW);
  digitalWrite(O4,LOW);

  gState_F0 = 0; // ADD
  gState_F1 = 0;
  gState_F2 = 0;
  gState_F3 = 0;
  gState_F4 = 0;
  gState_F5 = 0;
}
//}

@<tt>{ATtiny85}版のファンクションデコーダの@<tt>{NMRA}の規格テストを通すために、リセットコマンドを受信した

時にファンクションの@<tt>{LED}消灯、影響する@<tt>{gState_Fn} のフラグをクリアさせました。



//embed[latex]{
\clearpage
//}


=== notifyDccIdle()
@<tt>{DCC}アイドルコマンドのコールバック関数です。

※用途不明、未定義？

==== 構文
@<tt>{notifyDccIdle()}

==== パラメーター
@<tt>{Address 11111111 (255): Idle Packe}を受信したときに使用します。

==== 戻り値
なし

//embed[latex]{
\clearpage
//}




=== notifyDccSpeed()
[ユーザープログラムで使用します]

ファンクションデコーダの速度コマンドのコールバック関数です。

コマンドステーションから送信された速度コマンドの受信処理を行います。

==== 構文
@<tt>{notifyDccSpeed( Addr, AddrType, Speed, Dir, SpeedSteps )}

==== パラメーター
@<tt>{Addr : }デコーダアドレス

@<tt>{AddrType : DCC_ADDR_SHORT(0) or DCC_ADDR_LONG(1)}

@<tt>{Speed : Decoder speed. 0 = Emergency stop}

@<tt>{                       1 = Regular stop}

@<tt>{                       2 to SpeedSteps = Speed step 1 to max.}

@<tt>{Dir : DCC_DIR_REV(0) or DCC_DIR_FWD(1)}

@<tt>{SpeedSteps : SPEED_STEP_14   =  15}

@<tt>{             SPEED_STEP_28   =  29}

@<tt>{             SPEED_STEP_128  = 127}

==== 使用例
//emlistnum{
extern void notifyDccSpeed( Addr, AddrType, Speed, Dir, SpeedSteps )
{
    Serial.print("Speed - ADR: ");
    Serial.print(Addr);
    Serial.print(", AddrType: ");
    Serial.print(AddrType);
    Serial.print(", SPD: ");
    Serial.print(Speed);
    Serial.print(", DIR: ");
    Serial.print(Dir);
    Serial.print(", SpeedSteps: ");
    Serial.println(SpeedSteps);
}
//}

引数@<tt>{Addr}は、受信した自身の@<tt>{DCC}アドレスが格納されます。他の@<tt>{DCC}アドレスは受信されません。

引数@<tt>{AddrType}は、@<tt>{8bit}ショート@<tt>{DCC}アドレス@<tt>{[0]}か、@<tt>{16bit}ロング@<tt>{DCC}アドレス@<tt>{[1]}が使われているかのフラグが格納されます。

引数@<tt>{Speed}は、コマンドステーションから送信された速度値が格納されます。

引数@<tt>{Dir}は、前進@<tt>{[1]}、後進@<tt>{[0]}が格納されます。

引数@<tt>{SpeedSteps}は、コマンドステーションが設定された@<tt>{STEP}情報、
スピードステップ@<tt>{14[15]}、スピードステップ@<tt>{28[29]}、スピードステップ@<tt>{128[127]}が格納されます。
※実際のステップ数から異なる数値が格納されます。

====[column] コマンドステーションから受信例
ステップモードを変えた時のシリアルデバックポートに出力される事例を紹介します。

@<tt>{128STEP}モード

@<tt>{ Speed - ADR: 3, AddrType: 0, SPD: 2, DIR: 1, SpeedSteps: 127}

//blankline

@<tt>{ 28STEP}モード

@<tt>{ Speed - ADR: 3, AddrType: 0, SPD: 1, DIR: 1, SpeedSteps: 29}

//blankline

@<tt>{ 14STEP}モード(@<tt>{ DSC50Kは28STEP}モードで@<tt>{ Speed}が間引かれて送信されます)

@<tt>{ Speed - ADR: 3, AddrType: 0, SPD: 1, DIR: 1, SpeedSteps: 29}


====[/column]


このサンプルは、受信した速度コマンドをそのままgSpeedRef変数に代入指定います。この方法では
ステップ数によって数値の上限が変わってしまいます。

//emlistnum{
extern void notifyDccSpeed( Addr, AddrType, Speed, Dir, SpeedSteps )
{
  if ( gDirection != Dir )
  {
    gDirection = Dir;
  }
  gSpeedRef = Speed;
}
//}

//embed[latex]{
\clearpage
//}

速度の受信はステップ数を加味して以下のスケッチの様に@<tt>{ SpeedStep} の値で @<tt>{ aSpeedRef が 255 でMax}になるようにスケーリングの
処理を行います。

//emlistnum{
extern void notifyDccSpeed( Addr, AddrType, Speed, Dir, SpeedSteps )
{
	uint16_t aSpeedRef = 0;
	//速度値の正規化(255を100%とする処理)
	if( Speed >= 1)
	{
		aSpeedRef = ((Speed - 1) * 255) / SpeedSteps;
	}
	else
	{
		//緊急停止信号受信時の処理
		aSpeedRef = 0;
  }
  gSpeedCmd = aSpeedRef;

  gPwmDirv = Dir;
  if ( gCV29Direction > 0)
  {
     if ( Dir == DCC_DIR_FWD) {
          gPwmDirv = 128;
     }
    else  {
          gPwmDirv = 0;
    }
  }
  else  {
     if ( Dir == DCC_DIR_FWD) {
          gPwmDirv = 0;
     }
    else {
         gPwmDirv =128;
    }
  }
}
//}





//embed[latex]{
\clearpage
//}

=== notifyDccSpeedRaw()
ファンクションデコーダの速度コマンドのコールバック関数です。

@<tt>{14STEPと28STEP}のみ機能します。

@<tt>{Raw}の値は、@<tt>{Speed and Direction Instructions (010 and 011)}のコマンドがそのまま
送られます。@<tt>{RevとFor} のビットも@<tt>{ON}になっています。

@<tt>{Forward で speed 1}のときは、@<tt>{01100001 0x61 97d} が格納されます。

@<tt>{DCC}電子工作連合ではこのコールバック関数は使用していません。

==== 構文
@<tt>{notifyDccSpeedRaw( Addr, AddrType, Raw)}

==== パラメーター
 @<tt>{Addr : }デコーダアドレス

 @<tt>{AddrType : DCC_ADDR_SHORT(0) or DCC_ADDR_LONG(1)}

 @<tt>{Raw :  Raw decoder speed command.}

==== 戻り値
なし

==== 使用例
//emlistnum{
void notifyDccSpeedRaw( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Raw)
{
    Serial.print("Speed - ADR: ");
    Serial.print(Addr);
    Serial.print(", AddrType: ");
    Serial.print(AddrType);
    Serial.print(", Raw: ");
    Serial.println((int)Raw);
}
//}
 @<tt>{1:128STEP}

コールされません。

 @<tt>{2:28STEP}

 @<tt>{Speed - ADR: 3, AddrType: 0, Raw: 96 (DCS50K スロットル最小)}

 @<tt>{Speed - ADR: 3, AddrType: 0, Raw: 127 (DCS50K スロットル最大)}

 @<tt>{3:14STEP}

 @<tt>{Speed - ADR: 3, AddrType: 0, Raw: 97 (DCS50K スロットル最小)}

 @<tt>{Speed - ADR: 3, AddrType: 0, Raw: 111 (DCS50K スロットル最大)}






//embed[latex]{
\clearpage
//}


=== notifyDccFunc()
[ユーザープログラムで使用します]

ファンクションデコーダのファンクションコマンドのコールバック関数です。

ユーザープログラムにて。ファンクションコマンドを受信した時の動きを定義します。

F1-F28のファンクションの受信が可能です。

==== 構文
@<tt>{notifyDccFunc( Addr, AddrType, FuncGrp, FuncState)}

==== パラメーター
@<tt>{Addr : }デコーダアドレス

@<tt>{AddrType : DCC_ADDR_SHORT(0) or DCC_ADDR_LONG(1)}

@<tt>{FuncGrp : FN_0      - 14 speed step headlight function Mask FN_BIT_00.}

@<tt>{          FN_0_4    - Functions  0 to  4. Mask FN_BIT_00 - FN_BIT_04}

@<tt>{          FN_5_8    - Functions  5 to  8. Mask FN_BIT_05 - FN_BIT_08}

@<tt>{          FN_9_12   - Functions  9 to 12. Mask FN_BIT_09 - FN_BIT_12}

@<tt>{          FN_13_20  - Functions 13 to 20. Mask FN_BIT_13 - FN_BIT_20}

@<tt>{          FN_21_28  - Functions 21 to 28. Mask FN_BIT_21 - FN_BIT_28}

@<tt>{FuncState : }アクティブな関数がそのビットに 1 を持つビットマスク。
指定されたビットを分離するには、適切な@<tt>{FN_BIT_nn}値を@<tt>{FuncState}に指定する必要があります。

==== 戻り値
なし

==== 受信したファンクションに対応する引数

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


==== 使用例
//emlistnum{
extern void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType,
    　　　　　　　　　　　　　　　　　　　　　　FN_GROUP FuncGrp, uint8_t FuncState)
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

//embed[latex]{
\clearpage
//}



=== notifyDccAccTurnoutBoard()
ターンアウトアクセサリデコーダのボード指向コールバック関数です。

@<tt>{CV29_OUTPUT_ADDRESS_MODE }が設定されていない場合に最も有用です。

このタイプのデコーダはボードごとに4つのペアのターンアウト出力を持ちます。

@<tt>{OutputPower} は電源が入っていれば@<tt>{ 1，}そうでなければ@<tt>{ 0} です。

@<tt>{Dcc.init( MAN_ID_NUMBER, MAN_VER_NUMBER, FLAGS_DCC_ACCESSORY_DECODER, 0 );}の
様に @<tt>{FLAGS_OUTPUT_ADDRESS_MODE} のフラグが@<tt>{OFF}の場合に使用できます。

@<tt>{DCC}電子工作連合ではこのコールバック関数は使用していません。


==== 構文

@<tt>{notifyDccAccTurnoutBoard( BoardAddr, OutputPair, Direction, OutputPower )}

==== パラメーター

@<tt>{BoardAddr : }ボードアドレスごとに指定します．@<tt>{CV 1 LSB }および@<tt>{ CV 9 MSB }に相当します。

@<tt>{OutputPair :} 出力ペア番号．@<tt>{0～3 }の範囲です。
アクセサリーパケットの@<tt>{3}つの@<tt>{ DDD} ビットの上位@<tt>{2}ビットに相当します。

@<tt>{Direction :} ターンアウト方向。@<tt>{0}または@<tt>{1}の値を持ちます。
@<tt>{0}または@<tt>{1}の値を持ちます。 アクセサリーパケットの @<tt>{DDD3} ビットのビット@<tt>{0}に相当します。

@<tt>{OutputPower :} 出力オン@<tt>{/}オフ。パケット@<tt>{C}ビットに相当します。以下の値を持ちます。
@<tt>{0 - }出力ペアがオフ。
@<tt>{1 - }出力ペアがオンです。

//table[TurnoutBoard][BoardAddrとOutputPairの関係]{
t/c	BoardAddr	OutputPair	Direction
1t	1	0	0
1c	1	0	1
2t	1	1	0
2c	1	1	1
3t	1	2	0
3c	1	2	1
4t	1	3	0
4c	1	3	1
5t	2	0	0
5c	2	0	1
6t	2	1	0
6c	2	1	1
7t	2	2	0
7c	2	2	1
8t	2	3	0
8c	2	3	1
//}

==== 戻り値
なし

==== 使用例
//emlistnum{
void notifyDccAccTurnoutBoard( uint16_t BoardAddr, uint8_t OutputPair,
        uint8_t Direction, uint8_t OutputPower )
{
    Serial.print("BoardAddr:");
    Serial.print(BoardAddr);
    Serial.print(" OutputPair:");
    Serial.print(OutputPair);
    Serial.print(" Direction:");
    Serial.print(Direction);
    Serial.print(" OutputPower:");
    Serial.println( OutputPower);
}
//}

@<tt>{BoardAddr:1 OutputPair:0 Direction:1 OutputPower:1}




//embed[latex]{
\clearpage
//}


=== notifyDccAccTurnoutOutput()
ターンアウトアクセサリデコーダの出力指向コールバック関数です。

@<tt>{CV29_OUTPUT_ADDRESS_MODE} が設定されていない場合に最も有用です．

このタイプのデコーダは，1枚のボードにつき4つのペアのターンアウト出力を持ちます．

@<tt>{OutputPower} は，電源が入っていれば 1，そうでなければ 0 です．

@<tt>{Dcc.init( MAN_ID_NUMBER, MAN_VER_NUMBER, FLAGS_DCC_ACCESSORY_DECODER |}
@<tt>{  LAGS_OUTPUT_ADDRESS_MODE, 0 );}の様に@<tt>{ FLAGS_OUTPUT_ADDRESS_MODE} のフラグが@<tt>{ON}の場合に使用できます。

@<tt>{DCC}電子工作連合ではこのコールバック関数は使用していません。


==== 構文

@<tt>{notifyDccAccTurnoutOutput(Addr, Direction, OutputPower )}

==== パラメーター

@<tt>{Addr : }出力アドレスごとに 4つの出力ペアを持つ標準的なアクセサリーデコーダの場合、1ボードあたり4つのアドレスがあります。

@<tt>{Direction : }ターンアウト方向。0または1の値を持ちます。アクセサリーパケットの@<tt>{DDD3}ビットのビット0に相当します。

@<tt>{OutputPower :} 出力オン/オフ。パケット@<tt>{C}ビットに相当します。以下の値を持ちます。

0 - 出力はオフです。

1 - 出力はオンです。

//table[TurnoutOutput][ AddrとDirectionとOutputPowerの関係]{
t/c	Addr	Direction	OutputPower
1t	1	0	1
1c	1	1	1
2t	2	0	1
2c	2	1	1
3t	3	0	1
3c	3	1	1
4t	4	0	1
4c	4	1	1
5t	5	0	1
5c	5	1	1
//}

==== 戻り値
なし

==== 使用例
//emlistnum{
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction,
       uint8_t OutputPower )
{
    Serial.print("Addr:");
    Serial.print(Addr);
    Serial.print(" Direction:");
    Serial.print(Direction);
    Serial.print(" OutputPower:");
    Serial.println( OutputPower);
}
//}



//embed[latex]{
\clearpage
//}
=== notifyDccAccBoardAddrSet()
ターンアウトアクセサリーデコーダのボード指向のコールバック関数です。

この通知は新しいボードアドレスが受信した次の@<tt>{DCC}ターンアウトパケットのアドレスに設定されている場合に通知されます。

これは上記の@<tt>{ setAccDecDCCAddrNextReceived()} メソッドで有効になります。

==== 構文
@<tt>{notifyDccAccBoardAddrSet( BoardAddr )}

==== パラメーター
@<tt>{BoardAddr :} ボードアドレスごと。@<tt>{CV1 LSBとCV9 MSB}に相当します。
4つの出力ペアを持つ標準的なアクセサリデコーダのためのボードごとのアドレス。

==== 戻り値
なし

//embed[latex]{
\clearpage
//}

=== notifyDccAccOutputAddrSet()

ターンアウトアクセサリーデコーダの出力指向コールバック関数です。

この通知は新しい出力アドレスが、受信した次の@<tt>{DCC}ターンアウトパケットのアドレスに設定されている場合です。

これは、上記の @<tt>{setAccDecDCCAddrNextReceived() }メソッドで有効になります。


==== 構文

@<tt>{notifyDccAccOutputAddrSet( uint16_t Addr)}

==== パラメーター

@<tt>{Addr : }出力アドレスごとに4つの出力ペアを持つ標準的なアクセサリーデコーダの場合1ボードあたり4つのアドレスがあります。

==== 戻り値
なし

//embed[latex]{
\clearpage
//}



=== notifyDccSigOutputState()
シグナルアスペクトのアクセサリーデコーダのコールバック関数です。

@<tt>{S-9.2.1} で拡張アクセサリーデコーダコントロールパケットとして定義されています。

==== 構文
@<tt>{notifyDccSigOutputState( Addr, State )}

==== パラメーター
@<tt>{Addr :} デコーダアドレス

@<tt>{State : 6 bit command equivalent to S-9.2.1 00XXXXXX.}

==== 戻り値
なし

//embed[latex]{
\clearpage
//}



=== notifyDccMsg()
@<tt>{DCC} パケットのコールバック。@<tt>{DCC} パケットバイトで呼び出されます。

==== 構文

@<tt>{notifyDccMsg( Msg )}

==== パラメーター

@<tt>{Msg : DCC_MSG }構造体へのポインタ。値は以下の通りです。

@<tt>{Msg->Size -} パケット内のデータバイト数。

@<tt>{Msg->PreambleBits - }パケット内のプリアンブルビット数。

@<tt>{Msg->Data[] - }パケット内のデータバイトの配列。

==== 戻り値
なし



=== notifyCVValid()
与えられたCVが有効かどうかを判断するためのコールバック関数です。
これは，ライブラリがCVが有効かどうかを判断する必要がある場合に呼び出されます．
注意:このコールバックが定義されている場合，このコールバックは，与えられたCVが有効かどうかを判断し，適切な値を返さなければなりません（MUST）．
このコールバックが定義されていない場合は，ライブラリが有効かどうかを判断します．

==== 構文
@<tt>{notifyCVValid( CV, Writable )}

==== パラメーター
@<tt>{notify:}通知 @<tt>{Valid:}有効

@<tt>{CV : CV number.}
@<tt>{Writable : 1 for CV writes. 0 for CV reads.}

==== 戻り値
なし

//embed[latex]{
\clearpage
//}


=== notifyCVRead()
CVに値を書き込むためのコールバック関数です。
これは，ライブラリがCVを書き込む必要がある場合に呼び出されます．
注: 定義されている場合，このコールバック

CVに値を書き込み，その値を返さなければなりません．

このコールバックが定義されていない場合，ライブラリはEEPROMからCVを読み込みます．

==== 構文
@<tt>{notifyCVRead( CV　)}

==== パラメーター
@<tt>{cv : CV number.}

==== 戻り値
なし

//embed[latex]{
\clearpage
//}


=== notifyCVWrite()
CVに値を書き込むためのコールバック関数です。

これは，ライブラリがCVを書き込む必要がある場合に呼び出されます．
注: 定義されている場合，このコールバック
CVに値を書き込み，その値を返さなければなりません．

このコールバックが定義されていない場合，ライブラリはEEPROMからCVを読み込みます．

==== 構文
@<tt>{notifyCVWrite( CV, Value)}

==== パラメーター
@<tt>{CV    - CV number. It must point to a valid CV.}
@<tt>{Value - CV value.}

==== 戻り値
なし




//embed[latex]{
\clearpage
//}


=== notifyIsSetCVReady()
CVが書けるかどうかを判断するためのコールバック関数です。

このコールバックは，ライブラリがブロックや失敗をせずに書き込み可能かどうかを判断する必要がある場合に呼び出されます．
注意: このコールバックが定義されている場合、このコールバックは CV への書き込みがブロックするか失敗するかを判断しなければなりません(MUST)。
このコールバックが定義されていない場合は，EEPROMへの書き込みがブロックするかどうかを判断します．


==== 構文
@<tt>{notifyIsSetCVReady(void)}

==== 戻り値
@<tt>{1         - CV is ready to be written.}
@<tt>{0         - CV is not ready to be written.}


//embed[latex]{
\clearpage
//}

=== notifyCVChange()
[ユーザープログラムで使用します]

@<tt>{CV}の値が変更されたときに呼び出されます。
注意：@<tt>{notifyCVWrite()}が定義されている場合や，@<tt>{EEPROM}内の値が書き込みコマンドの値と同じである場合には呼び出されません．

==== 構文
@<tt>{notifyCVChange( CV, Value)}

==== パラメーター
@<tt>{CV        - CV number.}
@<tt>{Value     - Value of the CV.}

==== 使用例

//emlistnum{
extern void	   notifyCVChange( uint16_t CV, uint8_t Value)
{
   //CVが変更されたときのメッセージ
#ifdef DEBUG
   Serial.print("CV ");
   Serial.print(CV);
   Serial.print(" Changed to ");
   Serial.println(Value, DEC);
#endif
}
//}

==== 戻り値
なし

//embed[latex]{
\clearpage
//}


=== notifyCVResetFactoryDefault()
[ユーザープログラムで使用します]

CV8が受信された時に呼び出されます。

このコールバックは，関連する CV の工場出荷時デフォルト値を @<tt>{setCV()}メソッドに書き出します．

@<tt>{setCV()} は，このメソッドが呼ばれた際にブロックしてはいけません．

@<tt>{setCV()} を呼び出す前に，@<tt>{isSetCVReady() }でテストしてください．

==== 構文
@<tt>{notifyCVResetFactoryDefault(void)}


==== 使用例

//emlistnum{
void notifyCVResetFactoryDefault()
{
  resetCVToDefault();
  delay(1000);
  resetFunc();
}
//}
3行目 : @<tt>{resetCVToDefault()} の関数を呼び、@<tt>{CV}値を工場出荷状態に設定します。

4行目 : @<tt>{CV8}コマンドが複数受信しても無視するためのウエイト

5行目 : デコーダをリセットスタートさせます。

//embed[latex]{
\clearpage
//}



=== notifyCVAck()

[ユーザープログラムで使用します]

コマンドステーションからデコーダの@<tt>{CV}を読み取る命令が届いた時に応答する時に呼び出されます．
このコールバック関数は、デコーダが消費する電流を @<tt>{6ms ±1ms} の間、@<tt>{60mA}以上増やす必要があります。
@<tt>{notifyServiceMode()}の値で、@<tt>{DIRECT MODE }時のみ@<tt>{Ack}を返す様に作ると、運転中支障なく@<tt>{DCV}値の変更が
可能になります。

@<tt>{NmraDcc.cppのackCV()}から呼び出されます。

モーターデコーダの場合は、モータ@<tt>{ON}、@<tt>{6ms}ウエイト、モータOFFさせます。

ファンクションデコーダの場合は、@<tt>{LED ON、6ms}ウエイト、@<tt>{LED OFF}させます。しかし消費電流が小さい為コマンドステーション側で
認識されないことが多いです。

==== 構文

@<tt>{notifyCVAck(void)}

==== 使用例
//emlistnum{
void notifyCVAck(void)
{
#ifdef DEBUG
  Serial.println("notifyCVAck");
#endif
  if(gServiceMode == 1 )  // OPS モードの時はACKを返さない
    MOTOR_Ack();
}
//}

//embed[latex]{
\clearpage
//}

====[column] CV ACK信号の作り方

@<tt>{MP3}デコーダやワンコインデコーダではモータを繋げて動かすことが前提ですのでモータを動かすことで代用しています。
もしモータが無いデコーダではどうすればいいか？トランジスタ出力回路と抵抗を付けて電流をダミー消費する回路を作るしかありません。
@<tt>{PIN_CVACK}には、ピン番号を入れて下さい。
//emlist[]{
void notifyCVAck(void)
{
  analogWrite( PIN_CVACK, 255 );
  delay( 8 );
  analogWrite( PIN_CVACK, 0 );
}
//}

//image[dccack][電流をダミー消費する回路][scale=0.8]

なお@<tt>{ATtiny85}版のスマイルファンクションデコーダは空いているピンが無いのと電流を取るのがむつかしいので最初から
@<tt>{CV}値がコマンドステーションから読み取れない事を明記しています。


====[/column]






//embed[latex]{
\clearpage
//}


=== notifyServiceMode()
サービスモード入ると呼び出されるコールバック関数です。

==== 構文
@<tt>{notifyServiceMode(bool)}

@<tt>{0 : normal(OPS MODE etc)}
　　　
@<tt>{1 : ServiceMode(Direct MODE)}

==== 使用例
//emlistnum{
extern void notifyServiceMode(bool svc)
{
  gServiceMode = svc;
  #ifdef _DSFOX
   Serial.print("notifyServiceMode ");
   Serial.println(gServiceMode);
  #endif
}
//}
