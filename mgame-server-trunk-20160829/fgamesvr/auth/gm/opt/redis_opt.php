<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1>redis</h1>
<form name="input" action="opt/redis_opt_submit.php" method="post" target="blank" align="right">
	<div>
	服务器ip:
	<input type="text" name="server_ip" value="10.10.1.134"/>
	</div>
	<div>
	服务器端口:
	<input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
	</div>
	<div>
	opt:
	<input type="text" name="opt"/>
	</div>
	<div>
	param1:
	<input type="text" name="param1"/>
	</div>
	<div>
	param2:
    <input type="text" name="param2"/>
    </div>
	<div>
	param3:
	<input type="text" name="param3"/> 
	</div>
    <div>
    平台:
    <?php
    include "platform.php";
    ?>
    </div>
	<input type="submit" value="执行"/>
</form>

<div align="left">
<h1>操作命令 opt选项说明</h1><br>
    查询表内容: get_obj<br>
    查询hash表内容: hget_obj<br>
    设置表内容: set_obj<br>
    设置hash表内容: hset_obj<br>
</div>
</div>
