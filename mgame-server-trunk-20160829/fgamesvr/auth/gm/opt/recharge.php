<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1>充值</h1>
<form name="input" action="opt/recharge_submit.php" method="get" target="blank" align="right">
	<div>
	服务器ip:
	<input type="text" name="server_ip" value="10.10.1.134"/>
	</div>
	<div>
	服务器端口:
	<input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
	</div>
	<div>
	平台名字:
	<input name="platform_name" value="memoriki"/>
	</div>
	<div>
	open_id:
	<input type="text" name="open_id"/>
	</div>
	<div>
	模拟订单号:
	<input type="text" name="order_id"/>
	</div>
	<div>
	商品id:
	<input name="shop_id" value="1" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
	</div>
    <div>
    平台:
    <?php
    include "platform.php";
    ?>
    </div>
	<input type="submit" value="添加"/>
</form>
<div align="left">
<b>open_id 需要完整的数据库储存的内容</b><br><br>
<b>平台接口名字匹配:</b><br>
微信,qq => midas<br>
m+ => memoriki
</div>
</div>
