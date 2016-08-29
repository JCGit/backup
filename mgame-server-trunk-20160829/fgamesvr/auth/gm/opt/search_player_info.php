<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1 color='red'>查询玩家信息</h1>
<h2>添加活动</h2>
<form name="input" action="opt/search_player_info_submit.php" method="get" target="blank" align="right">
    <div>
    服务器ip:
    <input type="text" name="server_ip" value="10.10.1.134"/>
    </div>
    <div>
    服务器端口:
    <input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
    </div>
    <div>
    openid:
    <input type="text" name="openid"/>
    </div>
    <div>
    平台id:
    <input type="text" name="platformid" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/> 
    </div>
    <div>
    平台:
    <?php
    include "platform.php";
    ?>
    </div>
    <input type="submit" value="查询"/>
</form>
</div>
