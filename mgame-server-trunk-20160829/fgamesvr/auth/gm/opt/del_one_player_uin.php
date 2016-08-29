<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1 color='red'>删除一个玩家数据</h1>
<form name="input" action="opt/del_one_player_uin_submit.php" method="get" target="blank" align="right">
	<div>
	服务器ip:
	<input type="text" name="server_ip" value="10.10.1.134"/>
	</div>
	<div>
	服务器端口:
	<input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
	</div>
    <div>
    玩家UIN:
    <input type="text" name="player_uin" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
    </div>
    <div>
    平台:
    <?php
    include "platform.php";
    ?>
    </div>
    <input type="submit" value="删除"/>
</form>
</div>
