# RPZ-IR-SensorのLIRC赤外線サンプル
-----

## LIRCでRPiTPH Monitor/RPZ-IR-Sensorの赤外線制御
* URL
  - http://indoor.lolipop.jp/IndoorCorgiElec/App-LIRC.php
* リビングライトのリモコンOFFを記録して、以下のコマンドで数値化
```
grep -v '^#' living_light_off.txt | sed -e 's/pulse//g' | sed -e 's/space//g' | perl -pe 's/\n//g' > living_light_off_numbers.txt
```
* 以下を入力すると、ライトが消えます。
```
irsend SEND_ONCE light off
```
* 一緒のconfに書けなかったけど、こんな感じでonにできた
```
irsend SEND_ONCE light2 on
```
