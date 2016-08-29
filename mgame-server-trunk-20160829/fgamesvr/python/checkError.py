#coding:utf-8
# 用来检测log中的TraceBack并发送邮件

import  os
import  smtplib
from    email.mime.text import MIMEText
from    email.header import Header
import  re
import  time
import thread

#配置
ip_addr     = "10.10.2.58"          #ip地址 或者 服务器名称
ProjectName = "美味战争"            #项目名
logdir      = os.getcwd() + "/log"  #路径
interval    = 60 * 60               #执行间隔时间

#邮件列表(接收者)
mailto_list = [
    "andy@61games.hk",      #andy
    "168550228@qq.com",     #ron
    "denny@61games.hk",    #denny
    "owen@61games.hk",     #owen
]

#发送者
mail_host       = "smtp.qq.com"
mail_user       = "419637330"
mail_pass       = "wqnecpxkaiobbjha" #ssl认证
mail_postfix    = "qq.com"

#文件当前读取记录 发送后要重置 隔天要重置
line_tmp  = {
    'gamed'             : 0, #gamed-20160406.log
    'center'            : 0,
    'gslb'              : 0,
    'login'             : 0,
    'comrank'           : 0,
    'logserver'         : 0,
    'rank'              : 0,
    'callbacklogger'    : 0, #callbacklogger.20160406-1.log
}

'''
to_list :邮件地址列表
title   :标题
content :内容
send_mail(["xxx@qq.com"],"title","content")
'''
def send_mail(to_list, title, content):
    me = "%s<"%(Header('《' + ProjectName + '》服务器警告邮件','utf-8')) + mail_user + "@" + mail_postfix+">"
    msg = MIMEText(content.encode('utf-8'), 'plain', 'utf-8')
    msg['Subject'] = title
    msg['From'] = me
    msg['To'] = ";".join(to_list)
    msg["Accept-Language"]="zh-CN"
    msg["Accept-Charset"]="ISO-8859-1,utf-8"
    try:
        s = smtplib.SMTP()
        s.connect(mail_host)
        s.starttls()
        s.login(mail_user,mail_pass)
        s.sendmail(me, to_list, msg.as_string())
        s.close()
        return True
    except Exception, e:
        print str(e)
        return False
    thread.exit_thread() #发送邮件结束后线程退出


##定时检测 【正常情况下：一个进程一天生成一个文件】
def check_Error(now):
    date = time.strftime('%Y%m%d', now)
    print("exec check_Error " +  time.strftime('%Y%m%d %H:%M:%S', now))
    print line_tmp

    for k in line_tmp:
        process_name = k
        last_file_line = line_tmp[k]
        log_file_name = ""
        if process_name != 'callbacklogger':
            log_file_name = process_name + "-" + date + ".log" # 默认生成一个文件 后缀不带-n
        else:
            log_file_name = process_name + "." + date + "-1.log" # 默认生成一个后缀-1的文件

        is_exist_file = os.path.exists(logdir + "/" + log_file_name)

        if is_exist_file:
            print log_file_name + " is exist"
            line_list = [] #发送一个小时内的邮件
            last_line = ""

            file_object = open(logdir + "/" + log_file_name)
            line_it = 0
            for line in file_object:
                line_it += 1
                if line_it > last_file_line:
                    line_tmp[k] += + 1
                    if re.match("stack traceback", line):
                        line_list.extend(["\n", last_line])
                    if not re.match("\[", line):
                        line_list.append(line)
                last_line = line
            file_object.close()
        else:
            print log_file_name + " is not exist"

        if len(line_list):
            title       = ip_addr + " || " + log_file_name
            content     = "\n".join(line_list)
            thread.start_new_thread(send_mail, (mailto_list, title, content))

last_check_time = time.localtime()
def is_same_day(beginDate,endDate):
    format="%Y-%m-%d";
    bd = time.strftime(format, beginDate)
    ed = time.strftime(format, endDate)
    if bd == ed:
        return True
    else:
        return False

while True:
    now  = time.localtime()
    #隔天处理
    if not is_same_day(last_check_time, now):
        for k in line_tmp:
            line_tmp[k] = 0
        last_check_time = now
    check_Error(now)
    time.sleep(interval)
