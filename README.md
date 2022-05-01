# GP2040-PicoAnn
Raspberry Pi PicoなどのRP2040マイコンを搭載したボード向けのゲームパッドファームウェアである[GP2040](https://github.com/FeralAI/GP2040)に連射機能を追加したファームウェアです。
GP2040は、PC、MiSTer、Android、Raspberry Pi、Nintendo Switch、PS3、PS4（レガシーコントローラ対応）に対応しています。
TURBOボタン以外の基本的な機能は　<https://gp2040.info>　を参照してください。

## 機能

* TURBOボタンの機能を追加しました。
* 連射はB1,B2,B3,B4,L1,R1,L2,R2の8ボタンに設定可能
* 連射速度は秒間1～60連射の範囲で調整可能(ボタン単位での速度調整は未対応)
* OLEDを接続すれば、現在の連射速度およびTURBO設定されているボタン数が確認できます。
* 設定した連射速度は電源を切っても前回の状態が保存されますが、連射ボタンの状態はクリアされます。
* 連射機能が動作中はRaspberry Pi Pico本体のLEDが点滅します。

## 操作方法

■連射機能のON/OFF

* TURBOボタンを押しながら連射を設定したいボタンを押してください。
* 設定後、同じ操作で連射を解除することができます。

■連射速度の調整

* TURBOボタンを押しながらUPキーを押すと連射速度がUPします。
* TURBOボタンを押しながらDOWNキーを押すと連射速度がDOWNします。

## PINマッピング

基本的なピンアサインはPico Fighting Boardと同じです。TURBOボタンはGP28とGNDの端子に接続してください。
![](PinMapping_PicoAnn.png)

## Development

ファームウェアを独自にビルドしたい人向けの情報です。

TURBOボタン用のConfigとしてBoard Configuration (BoardConfig.h)に以下のオプションを追加しています。

| Name | 説明 | 必須？ |
| - | - | - |
| HAS_TURBO | TURBOボタンを有効にするフラグ | No,省略時はTURBO機能が無効 |
| PIN_BUTTON_TURBO | TURBOボタン用のGPIO番号を設定 | If HAS_TURBO is enabled |
| DEFAULT_SHOT_PER_SEC | 連射速度のデフォルト値を設定 | If HAS_TURBO is enabled |

###### 定義例
```
// TURBO Setting (PicoAnn Additional function)
#define HAS_TURBO 1                 // TURBO 1:Enable
#define PIN_BUTTON_TURBO 28         // TURBO Button GPIO No.
#define DEFAULT_SHOT_PER_SEC 20     // DEFAULT Shot Speed (Shot/Sec)
```

