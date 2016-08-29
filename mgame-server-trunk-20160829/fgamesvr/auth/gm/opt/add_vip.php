<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1>指定vip</h1>
<form name="input" action="opt/add_vip_submit.php" method="get" target="blank" align="right">
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
    会员类型:
    <div>
        <select name="level">
            <option value="1" selected="selected">白银会员</option>
            <option value="2">黄金会员</option>
            <option value="3">钻石会员</option>
        </select>
    </dir>
    <div>
    天数:
    <input type="text" name="day" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>   
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
