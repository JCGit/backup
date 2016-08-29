
<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1 color='red'>生成激活码</h1>
<form name="input" action="opt/create_act_code_submit.php" method="get" target="blank" align="right">
	<div>
	服务器ip:
	<input type="text" name="server_ip" value="10.10.1.134"/>
	</div>
	<div>
	服务器端口:
	<input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
	</div>
	<div>
	激活码名字:
	<input type="text" name="name" value=""/>
	</div>
	<div>
	激活码类型:
	<input type="text" name="type" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
	</div>
	<div>
	生成数量:
	<input type="text" name="count" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
	</div>
	<div>
	激活码长度:
	<input type="text" name="length" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
	</div>
	<div>
	激活码奖励:
	<input type="text" name="reward" value="{20001:100,1002:2}"/>
	</div>
	<div>
	激活码奖励类型(0:礼包id,1:邮件):
	<input type="text" name="reward_type" value="1"/>
	</div>
	<input type="submit" value="生成"/>
</form>
<div align="left">
	<b>
		激活码生成规则：
		<br>1：激活码名字对应批次，同一名字生成的激活码会冲掉上一批次的数据
		<br>2：类型说明：
		<br>类型1：同一类型的激活码，玩家只能领一个，并且领过后激活码失效
		<br>类型2：同一类型的激活码，玩家可领多个，并且领过后激活码失效
		<br>类型3：同一类型的激活码，玩家可令多个，并且令过后激活码只对自己失效，其他玩家还可领当前激活码
	</b>
</div>
</div>
