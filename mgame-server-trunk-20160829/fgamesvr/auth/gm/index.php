<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title></title>
    <script src="javascript/jquery-2.2.2.min.js"></script>
    <script src="javascript/jquery-md5.js"></script>
    <!-- // <script src="script/cryptico.js"></script> -->

    <script type="text/javascript">
        function Login(){
            var name = $('#name').val();
            var pass = $.md5($('#pass').val());

            if (name.length == 0 ){
                alert('用户名不能为空!');
                return
            }

            if (pass.length == 0 ){
                alert('你忘记输密码啦！！');
                return
            }

            $.ajax({
                type:"GET",
                url:"/login.php",
                data:{user_name:name,user_pass:pass},
                dataType:'json',
                success: function(data) {
                    if (data.ret == 0){
                        window.location.href = '/entry.php';
                    }else {
                        alert('登录失败: '+ data.msg);
                    }
                },
                error: function(data) {
                    alert('http failed' + data);
                }
            }
            );
        }
    </script>

    <style type="text/css">
        body {
            margin-left: 226px;
            margin-top: 80px;
        }
    </style>

</head>

<body>
    <?php
        include "config.php";
        if (!DEV){
            session_start();
            if(empty($_SESSION['name'])) {
                include 'login.html';
            }else{
                include 'entry.php';
            }
        }else{
            include 'entry.php';
        }
    ?>
</body>

<html>