<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1 color='red'>发送邮件</h1>
<form name="input" action="opt/send_mail_submit.php" method="get" target="blank" align="right">
	<div>
	server-ip:
	<input type="text" name="server_ip" value="10.10.1.134"/>
	</div>
	<div>
	server-port:
	<input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
	</div>
	<div>
	玩家UIN:
	<input type="text" name="player_uin" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
	</div>
	<div>
	邮件title:
	<input type="text" name="title" value=""/>
	</div>
	<div>
	邮件内容:
	<input type="text" name="content" value=""/>
	</div>
	<div>
	附件:
	<input type="text" name="plugin" value=""/>
	</div>
    <div>
    平台:
    <?php
    include "platform.php";
    ?>
    </div>
	<div>
	<input type="submit" value="send"/>
</form>
<div align="left">
	附件写法：
	<br>标准的json格式，如需要 物品1002*2 和 物品1003*1 这两个附件,写法如下。
	<br><b>{1002:2,1003:1}</b>
	<br>  *注，格式错误会导致前端打开邮件时崩溃
	<br><br>
	<br>特殊物品id：
	<br>20001 ：钻石
	<br>20002 : power
	<br>20004 : 积分
</div>
</div>
