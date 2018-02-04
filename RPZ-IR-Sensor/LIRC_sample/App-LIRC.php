<!DOCTYPE html>


<html lang="ja">

<head>
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
	<title>LIRCでRPiTPH Monitor/RPZ-IR-Sensorの赤外線制御</title>

	<!-- Bootstrap -->
	<!-- <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous"> -->

	<!-- Bootswatch flatly -->
	<link href="https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/flatly/bootstrap.min.css" rel="stylesheet" integrity="sha384-+ENW/yibaokMnme+vBLnHMphUYxHs34h9lpdbSLuAwGkOKFRl4C34WkjazBtb7eT" crossorigin="anonymous">

	<!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
	<!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
	<!--[if lt IE 9]>
	  <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
	  <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
	<![endif]-->

	<!-- Google Code Prettify -->
	<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>
</head>


<body style="padding-top:70px">

<nav class="navbar navbar-default navbar-fixed-top">
	<div class="container-fluid">
		<div class="navbar-header">
			<button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
				<span class="sr-only">Toggle navigation</span>
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
			</button>
			<a class="navbar-brand" href="#">Indoor Corgi Elec.</a>
		</div>

		<div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
			<ul class="nav navbar-nav">
				<li class="dropdown">
					<a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">製品一覧<span class="caret"></span></a>
					<ul class="dropdown-menu" role="menu">
						<li><a href="RPiTPHMonitor.php">Raspberry Pi用 温湿度 + 赤外線拡張ボード「RPi TPH Monitor」</a></li>
<li><a href="RPiTPHMonitorRev2.0.php">Raspberry Pi用 温湿度 + 赤外線拡張ボード「RPi TPH Monitor Rev2.x」</a></li>
<li><a href="RPZ-IR-Sensor.php">Raspberry Pi Zero用 温湿度 + 光センサ + 赤外線拡張ボード「RPZ-IR-Sensor」</a></li>
<li><a href="ESP-IR+TPHMonitor.php">WiFi + 赤外線 + 温湿度気圧センサ ホームIoT基板「ESP-IR+TPH Monitor」</a></li>
<li><a href="ESP-SensorCam.php">WiFi + カメラ + 人感、照度センサ ネットワークカメラ基板「ESP-SensorCam」</a></li>
<li><a href="ESP-PowerMonitor.php">WiFi + INA219搭載 プログラマブル電力モニター基板「ESP-PowerMonitor」</a></li>
<li><a href="E32-BreadPlus.php">WiFi + BLE ESP-WROOM-32 ブレッドボード開発基板「E32-BreadPlus」</a></li>
<li><a href="E32-SolarCharger.php">WiFi + 鉛バッテリーソーラー充電、コントローラー基板「E32-SolarCharger」</a></li>
<li><a href="NC64-BaseShield.php">Nucleo-64用 WiFi + microSD + 電源 拡張ボード「NC64-BaseShield」</a></li>
					</ul>
				</li>

				<li class="dropdown active">
					<a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">応用例<span class="caret"></span></a>
					<ul class="dropdown-menu" role="menu">
						<li><a href="App-ESP-IR+TPH-Server.php">ESP-IR+TPH Monitorをブラウザから制御</a></li><li><a href="App-ESP-SensorCam-Server.php">ESP-SensorCamをブラウザから制御</a></li><li><a href="App-ESP-PowerMonitor.php">ESP-PowerMonitorで電流測定</a></li><li class="active"><a href="#">LIRCでRPiTPH Monitor/RPZ-IR-Sensorの赤外線制御</a></li>
					</ul>
				</li>

				
				<li><a href="contact.php">連絡先</a></li>
			</ul>
		</div>
	</div>
</nav>


<div class="container">
	<h1>LIRCでRPiTPH Monitor/RPZ-IR-Sensorの赤外線制御</h1>

	<h2>概要</h2>

	<div class="row"> <div class="col-md-9">
		<p><a href="RPiTPHMonitorRev2.0.php">RPiTPH Monitor</a>及び<a href="RPZ-IR-Sensor.php">RPZ-IR-Sensor</a>用の、
		<a href="http://www.lirc.org/" target="_blank">LIRC</a>を使って赤外線の送受信を行う応用例です。
		GPIOを直接制御することでも赤外線送受信は可能ですが、
		他のタスクが入るとタイミングがずれてうまくいかないケースがあります。LIRCを使うことで安定した通信が可能になります。
		CPUリソースが限られているRaspberry Pi ZeroではLIRCを使うことを推奨しております。</p>

		<p>設定方法はKernelとLIRCバージョンによって異なることがあります。 
		本ページの方法で動作確認済みのOSとLIRCバージョンの一覧は以下の通りです。
		Kernelバージョンはapt-get upgradeで更新されることがあります。更新したくない場合は
		raspberrypi-bootloaderおよびraspberrypi-kernelパッケージのバージョンを固定することをおすすめします。</p> 

		<p class="text-warning">
		本ページは2017年8月にリリースされたRaspbianの最新版Stretch用の設定方法です。
        それ以前のバージョンであるJessieをお使いの場合は設定方法が異なりますので、
        <a href="App-LIRC-Jessie.php">こちら</a>をご覧ください。</p>

		<table class="table table-striped table-hover table-bordered">
			<thead>
				<tr class="success">
					<th>OS</th>
					<th>Kernelバージョン</th>
					<th>LIRCバージョン</th>
					<th>動作確認</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td><a href="https://downloads.raspberrypi.org/raspbian/images/raspbian-2017-09-08/2017-09-07-raspbian-stretch.zip">Raspbian Stretch 2017-09-07</a></td>
					<td>4.9</td>
					<td>0.9.4</td>
					<td>確認済</td>
				</tr>
			</tbody>
		</table>
	</div></div><!-- row, col -->


	<h2>LIRCのインストール</h2>
	<div class="row"><div class="col-md-9">
		<p>以下のコマンドでlircをインストールしてください。</p>
		<pre class="prettyprint">
sudo apt-get install lirc</pre>
	</div></div><!-- row, col -->

	<h2>LIRCの設定</h2>
	<div class="row"><div class="col-md-9">
		<p>/boot/config.txtをスーパーユーザーで開き、lirc-rpiに関する設定を以下のように編集してください。</p>
		<pre class="prettyprint">
# Uncomment this to enable the lirc-rpi module
dtoverlay=lirc-rpi
dtparam=gpio_out_pin=13
dtparam=gpio_in_pin=4</pre>

		<p>/etc/lirc/lirc_options.confをスーパーユーザーで開き、一部設定を以下のように編集してください。</p>
		<pre class="prettyprint">
driver = default
device = "/dev/lirc0"</pre>
		<p>設定が終了したら再起動して、以下を実行してlircdを一度終了してください。</p>
		<pre class="prettyprint">
sudo service lircd stop</pre>
	</div></div><!-- row, col -->

	<h2>赤外線受信</h2>
	<div class="row"><div class="col-md-9">
		<p>以下のコマンドを実行した後、基板の赤外線受信ユニットに向けて、お使いのリモコンから赤外線を送信してください。
		送信が終わったらCtrl+Cで取り込みを終了します。</p>
		<pre class="prettyprint">
mode2 -d /dev/lirc0 > rec.txt</pre>

		<p>受信に成功すると、赤外線情報がrec.txtに記録されます。</p>
		<pre class="prettyprint">
space 1141941
pulse 3506
space 1700
pulse 466
space 401
pulse 467
space 1269
...</pre>
	</div></div><!-- row, col -->

	<h2>赤外線送信</h2>
	<div class="row"><div class="col-md-9">
		<p>先ほど記録したデータを赤外線送信データとして登録します。
		/etc/lirc/lircd.conf.dディレクトリに好きな名前でconfファイルを作成します。(今回はaircond.confとします)
        さらに、confファイルを以下のように編集してください。nameの行は好きな名前を付けられます。
		今回はエアコンのオフボタンの例としてname aircond, name offとしました。
		begin raw_codesのname offに続く行は赤外線データを示しています。
        rec.txtに記録されたデータのうち、先頭のspaceを抜いたpulseから始まる数値をスペースで区切って入力していきます。
		 1行のデータが非常に多い場合エラーとなる事があります。適宜改行を入れてください。
		</p>
		<pre class="prettyprint">
begin remote
	name aircond
	flags RAW_CODES
	eps	30
	aeps	100
	gap	200000
	toggle_bit_mask	0x0

	begin raw_codes
	name off
	3506 1700 466 401 467 1269 ...
	end raw_codes
end remote</pre>

		<p>送信準備として以下を実行します。</p>
		<pre class="prettyprint">
sudo service lircd stop
sudo service lircd start</pre>

		<p>登録した赤外線データを送信するには以下のコマンドを実行します。正常に送信されればお使いの機器が反応します。また、赤外線送信中を示す赤色LEDが一瞬点灯します。</p>
		<pre class="prettyprint">
irsend SEND_ONCE aircond off</pre>
	</div></div><!-- row, col -->
	<br>
</div>

<!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
<!-- Include all compiled plugins (below), or include individual files as needed -->
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script>

</body>
</html>
