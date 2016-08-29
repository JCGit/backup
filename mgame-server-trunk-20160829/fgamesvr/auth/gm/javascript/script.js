$(function(){
    $('.right table').eq(0).show();
    $('.left li').on('click', function(){
        var nowCurrent = $(this).index();
        $(this).addClass('current').siblings().removeClass('current');
        $('.right table').eq(nowCurrent).show().siblings().hide();
    });


    /*
        1. 实例化列表方法，需要接受两个值，target为目标div, title为列表的标题
        2. 构建dom是根据后台返回的json的key==data来判断，形式为key==openid的json成列表式排版，可进行添加删除操作;
           形式为json数组，值包含key==content的可进行编辑操作，但是不可进行添加删除操作
    */
    var list = new ListAct({'target':$('.right'), 'title':['qweqweqw', 'b']});
    list.titleLi();
    $('.left li').eq(0).trigger('click');
})


function ListAct(obj){
    if (!obj.target || obj.target == '') {
        alert('需要指定目标div');
        return false;
    };

    if (!obj.title) {
       obj.title = [];
    };

    this.target = obj.target;
    this.title = obj.title;
}

ListAct.prototype = {
    titleLi: function(){
        var _this = this;
        $('.left li').on('click', function(){
           var type = $(this).attr('data-type');
           var platform = $(".platform").val()
           $('.right').html('');

           if (type == 5){
                $('.right').append('<h3 style="color:red">重新加载策划配置文件!</h3>')
                $('.right').append('</br></br><button class="modify" style=float:right>重新加载</button')
                _this.modify()
                return
           }
           $.ajax({
               url: '/status/status.php',
               data: {'type':type, platform : platform},
               type: 'get',
               dataType: 'json',
               success: function(data){

                    if (data.ret != 0){
                        alert("请求数据失败 : " + data.msg);
                        return;
                    }

                    if ( type == 2 || type == 3 || type == 4) {
                        $('.right').append('<textarea class=text1 rows="20" cols="60"></textarea>')
                        $('.right .text1').html(JSON.stringify(data.data, null, 4))
                        if (type == 2){
                            $('.right').append('<br><textarea class=text2 rows="7" cols="60"></textarea>')
                            $('.right .text2').html(JSON.stringify(data.ext, null, 4))
                        }
                    }else if (type == 6) {
                        $('.right').append('当前版本号: <input type="text" class="goal_version" value="0"/><br><br>' +
                            '是否已经提审: <select class="release_flag" >' +
                            '<option value=0>正式环境</option>' +
                            '<option value=1>处理提审状态</option>' +
                            '</select>')
                        console.log(data)
                        $('.right .goal_version').attr('value', data.data.goal_version);

                        if (data.data.release_flag) {
                            $('.right .release_flag option:eq(1)').attr("selected", "selected")
                        }else{
                            $('.right .release_flag option:eq(0)').attr("selected", "selected")
                        }
                    }
                    $('.right').append('</br></br><button class="modify" style=float:right>修改</button')
                    _this.modify()
               }
           });
       });
    },
    modify: function(){
        $('.modify').click(function(){
            if(confirm("^_^你真的确定要修改吗？！！！")){

            }else{
                return
            }
            var text = $('.right .text1').val()
            var type = $('.left li.current').attr('data-type')
            var platform = $('.platform').val()
            var param = {
                type : type,
                text : text,
                platform : platform,
            }

            if (platform == 0) {
                param.ip = $("input[name='ip']").val()
            }

            if (type == 2){
                param.ext = $(".right .text2").val()
            }

            if (type == 6){
                param.goal_version = $(".goal_version").val()
                param.release_flag = $(".release_flag").val()
            }

            $.ajax({
                    url: '/status/status.php',
                    data: param,
                    type: 'post',
                    dataType: 'json',
                    success: function(data){
                        if (data.ret == 0) {
                            alert('修改成功' + data.msg);
                        }else{
                            console.error(data);
                            alert('修改失败' + data.msg);
                        }
                    },
                    error: function(data){
                        console.error(data);
                        alert('请求失败');
                    }
                });
        })
    },
    add: function(){
        var _this = this;
        $('button.add').on('click', function(){
            $(this).parent().before('<li><input type="text" class="addCont" /><button class="on">确定</button><button class="off">取消</button></li>');
            _this.on();
            _this.off();
        });
    },
    del: function(){
        var _this = this;
        $('button.del').on('click', function(){
            if ($(this).parents('ul').find('li.current').length == 0) {
                alert('至少选择一项');
            }else{
                var con = confirm('确定需要删除吗？');
                if (con) {
                    var item = {};
                    item.type = 2;
                    item.action = 2;
                    item.openid = $('.right li.current').text();
                    item.index = $(this).parents('.cont-box').index();
                    $.ajax({
                        url: '/status/status.php',
                        data: item,
                        type: 'post',
                        dataType: 'json',
                        success: function(data){
                            if (data.ret == 0) {
                                $('.right li.current').remove();
                                alert('已删除');
                            }else{
                                alert('失败');
                            }
                        },
                        error: function(data){
                            console.error(data);
                            alert('请求失败');
                        }
                    });
                };
            }
        });
    },
    on: function(){
        var _this = this;
        $('.on').on('click', function(){
            var _this_ = $(this);
            if (_this_.parent().find('.change').length > 0) {
                var check = true;
                _this_.parent().find('span').each(function(){
                    if ($.trim($(this).find('input[type="text"]').val()) == '') {
                        check = false;
                    };
                });

                if (!check) {
                    alert('请输入值');
                    return false;
                };

                var item = {};
                item.type = 1;
                item.status = _this_.parent().find('span').find('input.data-status').val();
                item.content = _this_.parent().find('span').find('input.data-cont').val();
                item.index = _this_.parents('li').index();
            }else{
                var addVal = $.trim(_this_.prev('input').val());
                if (addVal == '') {
                    alert('请输入值');
                    _this_.prev('input').focus();
                    return false;
                };
                var item = {};
                item.type = 2;
                item.action = 1;
                item.index = _this_.parents('.cont-box').index();
                item.openid = addVal;
            }

            $.ajax({
                url: '/status/status.php',
                data: item,
                type: 'post',
                dataType: 'json',
                success: function(rst){
                    if (rst.ret == 0) {
                        if (_this_.parent().find('.change').length > 0) {
                            _this_.parent().find('.off').trigger('click');
                            alert('修改成功');
                        }else{
                          _this_.parent().html(addVal);
                          _this.select();
                          alert('数据添加成功');
                        }
                    }else{
                        console.error(rst);
                        alert('失败');
                    }
                },
                error: function(rst){
                    console.error(rst);
                    alert('请求失败');
                }
            });
        });
    },
    off: function(){
        var _this = this;
        $('.off').on('click', function(){
            if ($(this).parent().find('.change').length > 0) {
                $(this).parent().find('span').each(function(){
                    var cont = $(this).find('input').val();
                    $(this).append('<em>'+cont+'</em>');
                    $(this).find('input').remove();
                });
                $(this).parent().find('.change').show();
                $(this).parent().find('.on').remove();
                $(this).parent().find('.off').remove();
            }else{
                $(this).parent().remove();
            }
        });
    },
    select: function(){
        $('.right ul').each(function(){
            $(this).find('li').not('li:last()').on('click', function(){
                $(this).addClass('current').siblings().removeClass('current');
            });
        });
    },
    change: function(){
        var _this = this;
        $('.change').on('click', function(){
            $(this).parent().find('span').each(function(){
                var cont = $(this).find('em').text();
                var cClass = $(this).attr('class');
                $(this).find('em').remove();
                $(this).append('<input type="text" class="'+cClass+'" value="'+cont+'"/>');
            });
            $(this).hide();
            $(this).after('<button class="on">确定</button><button class="off">取消</button>');
            _this.on();
            _this.off();
        });
    }
}
