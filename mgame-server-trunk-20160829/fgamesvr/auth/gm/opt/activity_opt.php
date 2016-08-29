
<?php
require_once(dirname(__FILE__)."/../title.php");
?>
<div align="right">
<h1>活动控制</h1>
<div>
    <h2>添加活动</h2>
    <form name="input" action="opt/activity_opt_add_submit.php" method="get" target="blank" align="right">
        <div>
        服务器ip:
        <input type="text" name="server_ip" value="10.10.1.134"/>
        </div>
        <div>
        服务器端口:
        <input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
        </div>
        <div>
        活动id:
        <input type="text" name="id" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
        </div>
        <div>
        开始时间:
        <input type="text" name="start_time"/>
        </div>
        <div>
        结束时间:
        <input type="text" name="end_time"/>
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
<div>
    <h2>关闭活动</h2>
    <form name="input" action="opt/activity_opt_close_submit.php" method="get" target="blank" align="right">
        <div>
        服务器ip:
        <input type="text" name="server_ip" value="10.10.1.134"/>
        </div>
        <div>
        服务器端口:
        <input name="server_port" value="7602" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}" type="text"/>
        </div>
        <div>
        活动id:
        <input type="text" name="id" onkeyup="if(/\D/.test(this.value)){alert('只能输入数字');this.value='';}"/>
        平台:
        <?php
        include "platform.php";
        ?>
        </div>
        <input type="submit" value="关闭"/>
    </form>
</div>

时间格式例子：<br>
    <b>2016-01-01 2:01:01</b>
</div>
