#!/usr/bin/env python
#-*- coding: utf-8 -*-
'''

Copyright © 1998 - 2013 Tencent. All Rights Reserved. 腾讯公司 版权所有

'''

import sys
import xinge


mess = xinge.Message() #mess = MessageIOS()
mess.type = xinge.Message.TYPE_NOTIFICATION
mess.title = sys.argv[2]
mess.content = sys.argv[3]
mess.multiPkg = 1

#含义：样式编号0，响铃，震动，不可从通知栏清除
#mess.style = Style(0,1,1,0)
#action = ClickAction()
#action.actionType = ClickAction::TYPE_URL
#action.url = "http://xg.qq.com"
#mess.action = $action
#mess.custom = {'key1':'value1', 'key2':'value2'}
#t1 = TimeInterval(12, 0, 13, 59)
#t2 = TimeInterval(19, 0, 20, 59)
#mess.acceptTime = (t1, t2)

push = xinge.XingeApp(2100145241, '37c1e7cb4edf243e2a5279315310dd04')

#完善Message 消息
#PushSingleDevice(deviceToken, message, environment=0)
ret = push.PushSingleDevice(sys.argv[1], mess)

#ret = push.PushSingleDevice("56dbb72c6ac5bb7540d59ff1337d65d415652bb6", mess)
print ret

