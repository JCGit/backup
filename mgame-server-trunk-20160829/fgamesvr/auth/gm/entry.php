<html>
<head>
	<title>fgame gm</title>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<body>
	<script src="javascript/jquery-2.2.2.min.js"></script>
<div>
<h1> 此gm工具只用于内部人员测试用，绝不可用于线上或正式环境</h1>
</div>
<?php
include "style.css";
include "menu.php"
?>

<div id="content">
<?php
	if(array_key_exists("opt_page", $_GET))
	{
		include "opt/".$_GET["opt_page"];
	}
	else
	{
		include "opt/add_diamond.php";
	}
?>
</div>

</body>
</html>

<?php

?>
