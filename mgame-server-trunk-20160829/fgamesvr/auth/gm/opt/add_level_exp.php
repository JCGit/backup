<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1>加等级经验</h1>
<form name="input" action="opt/add_level_exp_submit.php" method="get" target="blank" align="right">
	<div>
	服务器ip:
	<input type="text" name="server_ip" value="10.10.1.134"/>
	</div>
	<div>
	服务器端口:
	<input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
	</div>
	<div>
	UIN:
	<input type="text" name="uin" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
	</div>
	<div>
	等级:
	<input type="text" name="level" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
	</div>
	<div>
	经验:
	<input type="text" name="exp" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
	</div>
    <div>
    平台:
    <?php
    include "platform.php";
    ?>
    </div>
	<input type="submit" value="添加"/>
</form>
</div>
